// Freescale Headers
#include <hidef.h>      		/* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// C STD Headers
#include <string.h>
#include <stdio.h>

// BMS specific Headers
#include "defines.h"
#include "ltc6802.h"
#include "CANSlave.h"
#include "temp.h"


#define CAN_ENABLE
#define SPI_ENABLE


/*********************
   Global variables
*********************/

//---------------
//  Flags
//---------------

uint8 gBalanceFlag = 0;	       // 0 = Normal mode ; 1 = Discharge Cell mode
uint8 gEquiStatusChange = 0;   // 1 = les registres de config. du mon. de batteries ont changes et doivent etre réenvoyés
uint16 gBalanceVector = 0;     // The n-th bit indicates the balancing status of the n-th cell. 1 = discharge enable.
uint8 gVoltTimeout = 0;        // 1 = les mesures de voltages doivent etre prises par le moniteur de batterie
uint8 gADC0done = 0;           // 1 = les mesures de l'ADC0 sont pretes a etre recuperees
uint8 gADC1done = 0;           // 1 = les mesures de l'ADC1 sont pretes a etre recuperees
errors_t gErrorFlags = {0, 0, 0};   //TODO MAT: tester nouvelle structure + init    
uint8 gSPItimeout = 0;

//-----------
// Variables
//-----------

uint32  gElapsedTime = 0;	      // le temps en secondes ecoule depuis l'activation du timer PIT0				
uint8   gSlaveID = 0;            // le numéro d'identification du module, lu sur le port E.
uint16  gVoltages[NB_CELL];	   // table containing the Cell Voltages in mV
int     gTemp[NB_CELL];         	// table contenant les temperatures des cellules (300 = 30.0 oC)
uint16  gBalThres = 0;          	// Balancing target voltage

 
/*************************
   Functions prototypes
*************************/

void MCU_init(void);
void checkForBalancedCells(uint16*, uint16[], uint16, uint16);


void main(void) 
{
   uint8 i, txError=0, openWireCell=0;
   uint8 rcvConfig[6];

   //LTC6802 configuration register initialisation
   //---------------------------------------------
   //GPIO pins pull down off
   //Level polling mode           (after polling command, SDO is low when LTC6802 is busy)
   //Ignore cells 11 and 12
   //Comparator duty cycle = 1    (comparator off, value used to verify config integrity, do not change)
   //Discharge cells off
   //No cell interrupts           (not used)
   //Undervoltage comparaison Voltage = 2.688V  (not used)
   //Overvoltage comparaison Voltage = 4.2V     (not used)
   
   Config_Reg_t ltcConfig = {0b11111001,0,0,0,112,175}; //CFG0 default read = 0b01000000


   //Initialization of the uC peripherals
   //--------------------------------------------------------
   //Timers initiazed for
   //    cell temperature measurements every 2 seconds (with uC ADC)
   //    cell voltage measurements every 1 second (with external battery monitor)
   //    we have a precision of 10 bits on the temperature measurements
   //    we have a precision of 12 bits on the voltage measurements
   
	MCU_init();

   //Slave ID reading on port E
   gSlaveID = PORTE & 0x0F;   //Only the 4 lower bits are meaningful
   if(gSlaveID == 0 || (gSlaveID > 10)) {
      gErrorFlags.badSlaveId = 1;     //TODO: we do nothing with this so far
   }
   
   
   //CAN synchronization
   #ifdef CAN_ENABLE
   while(!CAN0CTL0_SYNCH);   // Wait for Synchronization 
   #endif
    
    
	//Timers activation
   PITCE_PCE0 = 1;           //Activation of the voltage measurement timer and
                             //TX CAN timeout timer (PIT0)
   PITCE_PCE1 = 1;           //Activation of the temperature measurement timer (PIT1)
   PITCFLMT_PITE = 1;        //Activation of the timer module
   
   //Program the LTC6802-2 configuration registers   
   ltcWriteConfig(&ltcConfig);
      
	while(1) {
  
      //Temp measurements are ready to be sent to the master (every 2 seconds)
      if(gADC0done && gADC1done){

         //Conversion of temperatures to a readable form (ex: 295 = 29.5 oC)
         for(i=0; i<NB_CELL; i++)
            gTemp[i] = convertTemp(gTemp[i]);         

         #ifdef CAN_ENABLE
         //send temp to master via CAN
         gErrorFlags.canTxError = CAN0SendTemp(gTemp, gSlaveID);    //TODO: we do nothing with this flag so far...
         #endif

         gADC0done = 0;
         gADC1done = 0;
      }
      
      
      //Voltages measurements need to be acquired and sent (every second)
      //Open-wire connection detection is done
      //And configuration register verification too
      if(gVoltTimeout){
      
         #ifdef SPI_ENABLE  
         //Send the configuration register if needed.
         while(ltcReadConfig(rcvConfig) != 0);  //Read the current config, without errors.

         if((rcvConfig[0] & 0x01) == 0x00) {//This means that the LTC6802 config register
            gSPItimeout = 0;                //was reset after its watchdog timed out
            ltcWriteConfig(&ltcConfig);     //We do not use the WTD bit (doesnt work). We use
         }                                  //the CDC bits instead because the default value (0)
                                            //is different than the one we use (1). When the watch dog
                                            //times out, the config registers are reset. The chip is then
                                            //in standby mode, no new measures are taken. A read voltages
                                            //command seems to return the last measured voltages, so no erroneous
                                            //measures are sent to the master.   
         ltcStartVoltageMeasure();
	      txError = ltcReadCellVoltage(gVoltages);
         
         //open-wire connection detection
         openWireCell = ltcVerifyOpenWire();    //TODO: we do nothing with this so far..
         #endif                              
         
         
         #ifdef CAN_ENABLE
         //send voltages to the master via CAN, if received without error from LTC6802
         if(!txError)
            gErrorFlags.canTxError = CAN0SendVoltages(gVoltages, gSlaveID);  //TODO: we do nothing with this flag so far...
         #endif

         gVoltTimeout = 0;
         txError = 0;
      }
      
      //If the config registers have been modified by the equilibration routine,
      //send it to the LTC6802
      if(gEquiStatusChange){
         #ifdef SPI_ENABLE  
         ltcMakeConfigRegister(&ltcConfig, gBalanceVector);
         ltcWriteConfig(&ltcConfig);
         #endif SPI_ENABLE  
         gEquiStatusChange = 0; 
      }
      

      if(gBalanceFlag) {          //if in balancing mode
        checkForBalancedCells(&gBalanceVector, gVoltages, gBalThres, BAL_DELTA_VOLT);
        if(!gBalanceVector) {      //all cells are balanced, send the status change to the master
          #ifdef CAN_ENABLE
          gErrorFlags.canTxError = CAN0SendEquiStatus(gBalanceVector, gBalThres, gSlaveID);
          #endif
          gBalanceFlag = 0;           
        }
      }
      
	} //end while(1)
}

// Functions implementation.
void checkForBalancedCells(uint16* curBalanceArray, uint16 voltages[],
  uint16 targetVoltage, uint16 fuzzFactor) {
  uint8 i = 0;
  for (i = 0; i < NB_CELL; i++) {
    if ((*curBalanceArray & (1 << i)) &&  // If this cell is currently discharging
        (voltages[i] < (targetVoltage + fuzzFactor))) {  // And it is equilibrated
      *curBalanceArray &= ~(1 << i);  // Remove it's equilibration bit.
      gEquiStatusChange = 1;  // Ask to update register.
    }
  }
}