#include "timers.h"
#include "derivative.h"

//*****************************************************************************
// initMicroTimers
//
// Description:   Initialization of the micro-timers load register values.
//
// Inputs:  mt0: micro-timer 0 load value (0 to 255)
//          mt1: micro-timer 1 load value (0 to 255)
//
//*****************************************************************************
void initMicroTimers(unsigned char mt0, unsigned char mt1)
{
   PITCFLMT = 0;        //PITE=0, PITSWAI=0, PITFRZ=0
   PITMTLD0 = mt0;      //Micro-timer 0 load value
   PITMTLD1 = mt1;      //Micro-timer 1 load value
}


//*****************************************************************************
// initDelayTimer
//
// Description:   Initialization of PIT0 for the Tx CAN timeout.
//                Using micro-counter 0 with value of 0 and timer 0 with value
//                of 31220 will give a 1 s timeout with a clock bus of 8 MHz. The
//                micro timers values are set with the funciton initMicroTimers.
//                The PIT0 timer is not enabled here. 
//
//*****************************************************************************
void initCanTxTimer(unsigned int pit0)
{

  PITMUX_PMUX0 = 0;         //on sélectionne la microbase de temps 0 comme horloge pour le timer 0              
  PITLD0 = pit0;            //valeur à charger dans le compteur 0
  PITINTE_PINTE0 = 1;       //enable interrupt on channel 0
  
}


//*****************************************************************************
// initADCTimer
//
// Description:   Initialization of PIT1 for the use of the ADC timeout.
//                Using micro-counter01 with value of 0 and timer 1 with value
//                of 62499 will give a 2 s timeout with a clock bus of 8 MHz. The
//                micro timers values are set with the funciton initMicroTimers.
//                The PIT1 timer is not enabled here.
//
//*****************************************************************************
void initADCtimer(unsigned int pit1)
{
            
  PITMUX_PMUX1 = 0;              //on sélectionne la microbase de temps 0 comme horloge pour le timer 1
  PITLD1 = pit1;                 //valeur à charger dans le compteur 1
  PITINTE_PINTE1 = 1;            //enable interrupt on channel 1
 
}


