// Freescale Headers
#include <hidef.h>
#include "derivative.h"

// C STD Headers
#include <string.h>
#include <stdio.h>

// BMS specific Headers
#include "MCUinit.h"
#include "termio.h" 
#include "terminal_code.h"
#include "can.h"
#include "relays.h"
#include "common.h"


//#define DEBUG

  
/*********************
   Global variables
*********************/

//-------
// Flags
//-------
                              
errors_t gError = {1,1,1,1,1,1,1};            //Les drapeaux d'erreurs sont 'activés' au début, et se rétabliront
                                                //au fur et à mesure que les données requises seront recuillies.
flags_t gFlags = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};                                    
                                    
unsigned int gSlaveEquiStatus = 0;      //Si le i-ème bit est à 1, le (i+1)-ème module est en équilibration
unsigned int gSlaveComState = 0;        //Si le i-ème bit est à 1, le (i+1)-ème module est en erreur de communication                               

//-----------
// Variables
//-----------

uint16 gCellVolt[N_MOD][N_CELL];                    // [mV] Cell voltages
uint16 *gLowestCellVoltage = &gCellVolt[0][0];      // [mV] 
uint16 *gHighestCellVoltage = &gCellVolt[0][0];     // [mV] 
long int gMeanCurrent = 0;                          // [mA]

uint8 gLowestVoltageCellNum = 0;
uint8 gLowestVoltageCellSlaveId = 1;
uint8 gHighestVoltageCellNum = 0;
uint8 gHighestVoltageCellSlaveId = 1;
uint32 gTotalPackVoltage = 0;                       // [mV]

int gCellTemp[N_MOD][N_CELL];                       // [dixieme oC] Cell temperatures  
int *gLowestCellTemp = &gCellTemp[0][0];            // [dixieme oC]
int *gHighestCellTemp = &gCellTemp[0][0];           // [dixieme oC];

uint8 gLowestTempCellNum = 0;
uint8 gLowestTempCellSlaveId = 1;
uint8 gHighestTempCellNum = 0;
uint8 gHighestTempCellSlaveId = 1;

uint8 gMode = STAND_BY_MODE;                       // Operating mode

uint8 idleCount[N_MOD];                             //Un tableau qui compte le nombre de secondes d'inactivité des esclaves
uint8 gSlaveReset[N_MOD];                           //Le nombre d'initialisations des modules esclaves
uint8 gSlaveRev[N_MOD];                             //Le numéro de révision du firmware de chacun des modules esclave


params_t gParams =   {
                       -300,  //minDischargeCellTemp (oC / 10)
                        700,  //maxDischargeCellTemp
                       -200,  //lowDischargeCellTemp
                        600,  //highDischargeCellTemp
                       -100,  //minChargeCellTemp
                        800,  //maxChargeCellTemp
                          0,  //lowChargeCellTemp
                        450,  //highChargeCellTemp
                       2600,  //minCellVoltage  (mV)
                       4300,  //maxCellVoltage  (mV)
                       3000,  //lowCellVoltage  (mV)
                       4250,  //highCellVoltage  (mV)
                      93000,  //maxMeanChargeCurrent  (mA)
                     175000,  //maxMeanDischargeCurrent  (mA)
                     175000,  //highPeakDischargeCurrent (mA)
                     175000,  //maxPeakDischargeCurrent  (mA) (moins de 10 secondes)
                          0,  //manualMode
                          0,  //ignoreBMSerrors
                          0   //ignoreIntState
                    };

                    
void MCU_init(void);
void errorMode(void);
void standbyMode(void);
void normalMode(void);
void modeSelection(void);
void deviceInit(void);
uint32 computePackVoltage(uint16 cellVolt[][N_CELL]);


/***********************
   Main program start
***********************/

void main(void)
{          
   deviceInit();
   CAN0RequestSlaveFirmware(CAN_BROADCAST_ID);

   while(1) {

      if(!gParams.manualMode) {        //Automatic mode

         modeSelection();

         switch(gMode) 
         {
            case NORMAL_MODE:    normalMode(); break;
            case STAND_BY_MODE:  standbyMode(); break;
            case ERROR_MODE:     errorMode(); break;
            default:             errorMode();
         } 
      }

      //On calcule la tension totale du pack
      if(gFlags.totalPackTime) {
         gTotalPackVoltage = computePackVoltage(gCellVolt);
         gFlags.totalPackTime = 0;
      }
      
      //On traite les bytes reçus du lien avec l'usager
      if(gGuiReadIndex != gGuiWriteIndex || gGuiBufferFull) {
         sciByteReception(gGuiBuffer[gGuiReadIndex]);
         gGuiBufferFull = 0;
         gGuiReadIndex++;
         if(gGuiReadIndex > GUI_RX_BUFFER_SIZE)
            gGuiReadIndex = 0;
      }
      
   }
}


//*****************************************************************************
// deviceInit
// Description:   This function initializes the BMS. It uses the MCU_init
//                function generated by the Device Initialization tool. 
//
//*****************************************************************************
void deviceInit()
{
    int i,j;
       
   //Initialisation des périphériques du uC
   //--------------------------------------------------------
   //Timers initialisés:
   //    - Cell measurements refresh every 2 seconds using PIT3 (in user interface continuous mode)
   //    - Mesure du courant chaque 100 ms. On compte 10 interruptions du timer. (PIT2)
   //    - On détermine l'état de ignition, interlock state, error reset et imd state chaque 10 ms (PIT2).
   //    - Communication with the slave modules timeout tick every 1 second using PIT1
   //    - Warning led flashes with help of PIT4 at 4Hz
   //
   //Relais
   //    - Les relais du circuit haute tension sont initialisés ouverts.
   //
   //Alimentation 24V
   //    - Le MOSFET de la barre d'alimentation 24V est initialisé ouvert aussi.
   //
    MCU_init();
    
    //These initialization values assure us that
    //the system stays in the error state until all the
    //real values are received. gSlaveReset and gSlaveRev
    //are not critical.
    for(i=0; i<N_MOD; i++){
        for(j=0; j<N_CELL; j++){
            gCellVolt[i][j] = 0;
            gCellTemp[i][j] = 0x8000;
        } 
        idleCount[i] = COM_IDLE_COUNT_MAX;      
        gSlaveReset[i] = 0;
        gSlaveRev[i] = 0;
    }
    gMeanCurrent = gParams.maxPeakDischargeCurrent + 1;    //Valeur pas trop grande parce que moyenne
                                                           //mobile sur 10s quand même c'est long à réagir
   
   //Sélection du channel AN7 comme channel de départ de l'ADC
   ATD0CTL5 = ATD0CTL5 | 0x07;
    
    
   //User interface initialization
   SCI5CR2_RIE = 1;                   //Receiver Full Interrupt Enable Bit activation
   SCI5BD = USER_INT_SPEED;           //On assigne la bonne vitesse de transmission
   
   //Timers activation
   PITCE_PCE1 = 1;                   //Activation of the 100 Hz timer (PIT1)
   PITCE_PCE2 = 1;                   //Activation of the ADC timer (PIT2)
   //PITCE_PCE4 = 1;                   //Activation of the warning led flash timer (PIT4)
   PITCFLMT_PITE = 1;                //Activation of the timer module
}


//*****************************************************************************
// modeSelection
//
// Description:   This function will determine in which state (mode) the BMS
//                is. 
//
//*****************************************************************************
void modeSelection()
{
   if(gFlags.errorState) {
      gMode = ERROR_MODE;
      
   } else if(gFlags.ignition && gFlags.interlockClosed) {
      gMode = NORMAL_MODE;

   } else {
      gMode = STAND_BY_MODE;
   }
}


//*****************************************************************************
// normalMode
//
// Description:   This function implements the actions undertaken when the 
//                BMS is in normal mode i.e. ignition is on and there is no
//                charging process. 
//
//*****************************************************************************
void normalMode()
{  
   if(!gFlags.relaysClosed && !gClosingRelays){
      //Fermeture des relais du circuit principal avec précharge.
      //Ouverture du circuit de décharge.
      CloseRelays(START_SEQUENCE);
   }
}


//*****************************************************************************
// standbyMode
//
// Description:   This function implements the actions undertaken when the 
//                BMS is in stand-by mode i.e. ignition is off and no charging
//                process is going on.
//
//*****************************************************************************
void standbyMode()
{
    if(gFlags.relaysClosed || gClosingRelays)
        OpenRelays();
}


//*****************************************************************************
// errorMode
//
// Description:   This function implements the actions undertaken when the 
//                BMS is in error mode i.e. any error was detected.
//
//*****************************************************************************
void errorMode()
{
    if(gFlags.relaysClosed && !gParams.ignoreErrors)
        OpenRelays();        
}


uint32 computePackVoltage(uint16 cellVolt[][N_CELL])
{
    uint8 i, j;
    uint32 packVoltage = 0;
    
    for(i=0; i<N_MOD; i++){
        for(j=0; j<N_CELL; j++) {
            packVoltage = packVoltage + (uint32) cellVolt[i][j];
        }
    }
    
    return packVoltage;
}
