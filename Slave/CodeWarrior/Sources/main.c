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


//-----------
// Variables
//-----------

uint32  gElapsedTime = 0;	             // le temps en dixièmes de secondes ecoule depuis l'activation du timer PIT0				
uint8   gSlaveID = 0;                   // le numéro d'identification du module, lu sur le port E.
uint16  gVoltages[NB_CELL];	          // table containing the Cell Voltages in mV
int     gTemp[NB_CELL];         	       // table contenant les temperatures des cellules (300 = 30.0 oC)
int     gTempRaw[NB_CELL];              // table contenant les lectures de l'ADC
uint16  gBalThres = 0;          	       // Balancing target voltage
uint16  gBalanceVector = 0;             // The n-th bit indicates the balancing status of the n-th cell. 1 = discharge enable.
flags_t gFlags = {0,0,0,0,0,0,0,0,0};   // Les drapeaux globaux utilisés

 
/*************************
   Functions prototypes
*************************/

void MCU_init(void);
void checkForBalancedCells(uint16*, uint16[], uint16, uint16);


void main(void) 
{
   uint8 i, txError=0, openWireCell=0;
   uint8 rcvConfig[6];
   static uint8 voltMeasureCount = 0, tempMeasureCount = 0;

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
   //    cell temperature measurements every 1 seconds (with uC ADC)
   //    cell voltage measurements every 0.1 second (with external battery monitor)
   //    we have a precision of 10 bits on the temperature measurements
   //    we have a precision of 12 bits on the voltage measurements
   
	MCU_init();

    
    //Initialisation des tableaux de mesures
    for(i=0; i<NB_CELL; i++) {
        gTemp[i] = 210;
        gVoltages[i] = 3700;
    }
    
   //Lecture du slave ID sur le port E
   gSlaveID = PORTE & 0x0F;   //Seuls les 4 bits les moins significatifs sont importants 
   if(gSlaveID == 0 || (gSlaveID > 10)) {
      gFlags.badSlaveId = 1;     //TODO: on ne fait rien avec ca...
   }
   
   
   #ifdef CAN_ENABLE
   while(!CAN0CTL0_SYNCH);   //Attente de la synchro du bus CAN
   
   //Envoyer un message au maître pour indiquer que notre initialisation est complétée
   //Envoyer le numéro de révision également
   gFlags.canTxError = CAN0SendInitStatus(gSlaveID); 
   gFlags.canTxError = CAN0SendFirmwareRevision(gSlaveID);  
   #endif
    
    
	//Avtivation du timer
   PITCE_PCE0 = 1;           //Activation du timer PIT0. Ce timer a une fréquence de 10 Hz.
                             //Dans l'interruption, on gère la prise de mesures.
   PITCFLMT_PITE = 1;        //Activation of the timer module
   
   //Programmation du registre de configuration du LTC6802-2
   ltcWriteConfig(&ltcConfig);
      
	while(1) {
  
      #ifdef CAN_ENABLE
      if(gFlags.firmwareRequest) { 
          gFlags.canTxError = CAN0SendFirmwareRevision(gSlaveID);
          gFlags.firmwareRequest = 0;
      }
      #endif
  
      //Les mesures de temperatures sont pretes a etre utilisees
      if(gFlags.ADC0done && gFlags.ADC1done){

         tempMeasureCount++;
      
         //On ferme le ADC pour l'économie d'énergie
         ATD0CTL2_ADPU = 0;
         ATD1CTL2_ADPU = 0;
      
         //Conversion of temperatures to a readable form (ex: 295 = 29.5 oC)
         //Moyenne mobile exponentielle avec alpha = 1/16, donc moyenne
         //sur N=31 dernieres valeurs. alpha = 2/(N+1)
         for(i=0; i<NB_CELL; i++) {
            gTemp[i] = convertTemp(gTempRaw[i]) + (16-1)*gTemp[i];     
            gTemp[i] = gTemp[i] >> 4;
         }

         #ifdef CAN_ENABLE
         //send temp to master via CAN
         if(tempMeasureCount == TEMP_SEND_PERIOD) {
            gFlags.canTxError = CAN0SendTemp(gTemp, gSlaveID);    //TODO: we do nothing with this flag so far...
            tempMeasureCount = 0;
         }
         #endif

         gFlags.ADC0done = 0;
         gFlags.ADC1done = 0;
      }
      
      
      //Les mesures de tensions sont pretes a etre prises.
      //On verifie les connections ouvertes.
      //On verifie l'integrite du registre de configuration du LTC6802-2
      if(gFlags.voltTimeout){
      
         voltMeasureCount++;
      
         #ifdef SPI_ENABLE  
         //On envoie le registre de configuration si necessaire
         while(ltcReadConfig(rcvConfig) != 0);  //Lecture de la configuration actuelle, sans erreur.

         if((rcvConfig[0] & 0x01) == 0x00) {//Une condition vrai signifie que le registre de configuration du LTC6802
            gFlags.spiTimeout = 0;          //a ete reinitialise par le watchdog timer. On n'utilise pas le bit WTD pour
            ltcWriteConfig(&ltcConfig);     //detecter une expiration du WTD (ne fonctionne pas). On utilise les bits CDC parce
         }                                  //que la valeur utilisee (1) est differente de la valeur par defaut (0).
                                            //Quand le WTD expire, les registres de configuration sont reinitialises.
                                            //La puce est alors en mode standby, et aucune nouvelle mesure ne peut etre prise. 
                                            //Une commande de lecture des tensions semble retourner la derniere mesure de tensions,
                                            //donc aucune valeur erronnee n'est transmise au maitre. 
         ltcStartVoltageMeasure();
	      txError = ltcReadCellVoltage(gVoltages);
         
         //Detection des connexions ouvertes
         //openWireCell = ltcVerifyOpenWire();    //TODO: on ne fait rien avec ca...
         #endif                              
         
         
         #ifdef CAN_ENABLE
         //Envoie des tensions vers le maitre si recues sans erreur du LTC6802
         if(!txError && voltMeasureCount == VOLT_SEND_PERIOD) {
            gFlags.canTxError = CAN0SendVoltages(gVoltages, gSlaveID);  //TODO: on ne fait rien avec ca...
            voltMeasureCount = 0;
         }
         #endif

         gFlags.voltTimeout = 0;
         txError = 0;
      }
      
      //If the config registers have been modified by the equilibration routine,
      //send it to the LTC6802
      if(gFlags.equiStatusChange){
         #ifdef SPI_ENABLE  
         ltcMakeConfigRegister(&ltcConfig, gBalanceVector);
         ltcWriteConfig(&ltcConfig);
         #endif SPI_ENABLE  
         gFlags.equiStatusChange = 0; 
      }
      

      if(gFlags.balancingActive) {          //if in balancing mode
        checkForBalancedCells(&gBalanceVector, gVoltages, gBalThres, BAL_DELTA_VOLT);
        if(!gBalanceVector) {      //all cells are balanced, send the status change to the master
          #ifdef CAN_ENABLE
          gFlags.canTxError = CAN0SendEquiStatus(gBalanceVector, gBalThres, gSlaveID);
          #endif
          gFlags.balancingActive = 0;           
        }
      }
      
      //On entre en mode veille, en attendant le prochain interrupt.
      //Ce sera un timer qui indique le moment de prendre des mesures ou la
      //réception d'un message sur le port CAN.
      
      //Le module SPI entrera lui aussi en mode veille puisque SPI1CR2_SPISWAI = 1
      //On envoie l'ADC en stop mode quand il a terminé de prendre ses mesures.
      //Le périphérique CAN n'entre pas en mode wait car dans ce cas on perd le msg
      //qui a réveillé l'interface CAN.
      
      //TODO: mettre le module CAN en wait mode et tjrs envoyer un msg bidon à partir du maître
      //quand il envoie une commande. Cependant, les mesg des autres esclaves vont constamment
      //réveiller l'interface, sauf si on utilise le Twup...
      
      asm {
        WAI
      };    
      
	} //end while(1)
    
} //end main

// Functions implementation.
void checkForBalancedCells(uint16* curBalanceArray, uint16 voltages[],
  uint16 targetVoltage, uint16 fuzzFactor) {
  uint8 i = 0;
  for (i = 0; i < NB_CELL; i++) {
    if ((*curBalanceArray & (1 << i)) &&  // If this cell is currently discharging
        (voltages[i] < (targetVoltage + fuzzFactor))) {  // And it is equilibrated
      *curBalanceArray &= ~(1 << i);  // Remove it's equilibration bit.
      gFlags.equiStatusChange = 1;  // Ask to update register.
    }
  }
}