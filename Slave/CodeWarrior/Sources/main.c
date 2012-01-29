// Freescale Headers
#include <hidef.h>      		/* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// C STD Headers
#include <string.h>
#include <stdio.h>

// BMS specific Headers
#include "defines.h"
#include "ConfigSPI.h"
#include "CANSlave.h"
#include "timers.h"
#include "termio.h"


#define CAN_ENABLE

/*********************
   Global variables
*********************/

//---------------
//  Flag
//---------------

unsigned char gBalanceFlag = 0;	      // 0 = Normal mode ; 1 = Discharge Cell mode
unsigned char gEquiStatusChange = 0;   // 1 = the config registers have been modified and need to be sent
unsigned int  gBalanceVector = 0;      // The n-th bit indicates the balancing status of the n-th cell. 1 = discharge enable.
unsigned char gVoltTimeout = 0;        // 1 = ready to take voltages measurements
unsigned char gADC0done = 0;
unsigned char gADC1done = 0;



//-----------
// Variables
//-----------

unsigned int 	gInt_Voltage_Table[NB_CELL];			    // Int table containing the Cell Voltages in mV
int            gTemp[NB_CELL];
unsigned int   gBalThres = 0;                   // Balancing threshold voltage

 
/*************************
   Functions prototypes
*************************/

void MCU_init(void);


/**************
   Functions
**************/


void main(void) 
{

	unsigned char i;
	unsigned int tmp;	
	unsigned char rcvData[6] = {0,0,0,0,0,0};

	MCU_init();

	//Parametrage du CAN
   CAN0CTL0 = 0;                // Initialize MSCAN12 Module 
   while(!(CAN0CTL0&0x10));  // Wait for Synchronization 
   

	//Timers initialization
	initMicroTimers(255,0);
	initCanTxTimer(31220);        //1 s timeout
	initADCtimer(62499);          //2 s timeout
	PITCE_PCE0 = 1;               //Activation of the TX CAN timeout timer (PIT0)   // Hugues modif
	PITCE_PCE1 = 1;               //Activation of the ADC timer (PIT1)
   PITCFLMT_PITE = 1;            //Activation of the timer module
   
	
	//Comm SPI
	Write_Config();
	
   
	for(;;) {

     
      //Temp measurements ready to be sent to the master
      if(gADC0done && gADC1done){

#ifdef CAN_ENABLE
         //send temp to master via CAN
         CAN0SendTemp();
#endif

         gADC0done = 0;
         gADC1done = 0;
      }
      
      //Voltages measurements need to be acquired and sent
      if(gVoltTimeout){
      
         Start_Voltage_AD_Conversion();
	      Read_Cell_Voltage();
	      
#ifdef CAN_ENABLE
         //send volt to master via CAN
         CAN0SendVoltages();
#endif

         gVoltTimeout = 0;
      }
      
      //If the config registers have been modified by the equilibration routine, write it to the linear
      if(gEquiStatusChange){
         Define_Config();
         Write_Config();
         gEquiStatusChange = 0; 
      }
      
      
      if(gBalanceFlag){

         //Redefine the discharge vector
         for(i=0; i<NB_CELL; i++){
         
            tmp = 1;
            tmp = tmp << i;
         
            if((gInt_Voltage_Table[i] < (gBalThres + BAL_DELTA_VOLT)) && (gBalanceVector & tmp)) {
               tmp = ~tmp;
               gBalanceVector = gBalanceVector & tmp;
               gEquiStatusChange = 1;
            } 
         }
                 
         //If the balance vector is null, then the equilition is over
         if(gBalanceVector == 0){
         
#ifdef CAN_ENABLE
         //send to the master the balancing status (equi done)
         CAN0SendCommand(COMMAND_BAL_DONE,0,0,0);
#endif   
      
            gBalanceFlag = 0;
         }
      }
   
	}
	
}
 