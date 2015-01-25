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
#include "defines.h"
#include "charger.h"

// xCAN specific Headers
#include "per_XDx512_L15Y.h"
#include "xgCAN_drv.h"
#include "xgate_vectors.h"
#include "xgate_init.h"

  
/*********************
   Global variables
*********************/

//-------
// Flags
//-------
                              
errors_t gError = {1,1,1,1,1,1,1};            //Les drapeaux d'erreurs sont 'activés' au début, et se rétabliront
                                              //au fur et à mesure que les données requises seront recuillies.

flags_t gFlags; // Initialized in code
                                    
uint16 gSlaveComState = 0;        //Si le i-ème bit est à 1, le (i+1)-ème module est en erreur de communication                               

//-----------
// Variables
//-----------

int16 gCellVolt[N_MOD][N_CELL];                    	// [mV] Cell voltages
int16 *gLowestCellVoltage = &gCellVolt[0][0];      	// [mV] Points to the lowest cell voltage
int16 *gHighestCellVoltage = &gCellVolt[0][0];     	// [mV] Points to the highest cell voltage
int32 gCurrentFlow_mA = 0;                          // [mA] Current flow measured by the TCS

uint8 gLowestVoltageCellNum = 0;
uint8 gLowestVoltageCellSlaveId = 1;
uint8 gHighestVoltageCellNum = 0;
uint8 gHighestVoltageCellSlaveId = 1;

uint32 gTotalPackVoltage = 0;                       // [mV]
uint32 gDriveVoltage = 0;							// [?]

int16 gCellTemp[N_MOD][N_CELL];                     // [dixieme oC] Cell temperatures  
int16 *gLowestCellTemp = &gCellTemp[0][0];          // [dixieme oC]
int16 *gHighestCellTemp = &gCellTemp[0][0];         // [dixieme oC];
int16 gCellIgnoreTemp[N_MAX_IGNORE_TEMP][2];		// Liste des sondes de températures ignorées
uint8 gNumberOfIgnoredTemp;							// Nombre de sondes de températures ignorées

uint8 gLowestTempCellNum = 0;
uint8 gLowestTempCellSlaveId = 1;
uint8 gHighestTempCellNum = 0;
uint8 gHighestTempCellSlaveId = 1;

uint16 gImpedance = 0;								// IMD impedance value

uint8 gMode = STAND_BY_MODE;                       // Operating mode

uint8 idleCount[N_MOD];                             //Un tableau qui compte le nombre de secondes d'inactivité des esclaves
uint8 gSlaveReset[N_MOD];                           //Le nombre d'initialisations des modules esclaves
uint8 gSlaveRev[N_MOD];                             //Le numéro de révision du firmware de chacun des modules esclave
uint8 gSlaveLastRevision = 0;						// Highest revision in the slave nodes
uint8 idTable[N_MOD];								// Table de correspondance des ID des esclaves et index dans les tableaux

// To toggle to 1 if we want to start a opening or closing sequence for manual requests
uint16 gCloseRelayRequest = 0;
uint16 gOpenRelayRequest = 0;

params_t gParams =   {
						MIN_DISCH_CELL_TEMP,
						MAX_DISCH_CELL_TEMP,
						LOW_DISCH_CELL_TEMP,
						HIGH_DISCH_CELL_TEMP,
						MIN_CHARGE_CELL_TEMP,
						MAX_CHARGE_CELL_TEMP,
						LOW_CHARGE_CELL_TEMP,
						HIGH_CHARGE_CELL_TEMP,
						MIN_CELL_VOLTAGE,
						MAX_CELL_VOLTAGE,
						LOW_CELL_VOLTAGE,
						HIGH_CELL_VOLTAGE,
						MAX_MEAN_CHARGE_CURRENT,
						MAX_MEAN_DISCH_CURRENT,
						HIGH_PEAK_DISCH_CURRENT,
						MAX_PEAK_DISCH_CURRENT,
						0,  //manualMode
                        0,  //ignoreBMSerrors
                        0   //ignoreInterlockState
                    };


void MCU_init(void);
void modeSelection(void);
void deviceInit(void);

/***********************
 *  Main program start
 **********************/

void main(void)
{    
    deviceInit();
    CAN_SendFirmwareRevisionRequest();
		
    while(1) 
    {
	
		if(!gParams.manualMode)
		{
			//Automatic mode
			modeSelection();
		}    
	
		manage_relays(gMode);
		manage_charging();

		CAN_ProcessAllMessages(SLAVE_CHANNEL);
		CAN_ProcessAllMessages(MAIN_CHANNEL);
		
		// Calculate the total pack voltage periodically each PACK_VOLT_CALC_PERIOD_MS miliseconds
		if(getTime() - timeRef_lastPackVoltageCalculation  > PACK_VOLT_CALC_PERIOD_MS)
		{
			gTotalPackVoltage = computePackVoltage(gCellVolt);
			timeRef_lastPackVoltageCalculation = getTime();
		}
		
		//On traite les bytes reçus du lien avec l'usager
		if(gGuiReadIndex != gGuiWriteIndex || gGuiBufferFull) {
			sciByteReception(gGuiBuffer[gGuiReadIndex]);
			gGuiBufferFull = 0;
			gGuiReadIndex++;
			if(gGuiReadIndex > GUI_RX_BUFFER_SIZE)
				gGuiReadIndex = 0;
		}

	  	UpdateErrorDisplay();	// Update the 7 segments display		
		
		UpdateTimeDependentFlags();
		
		#ifdef USE_DRIVE_VOLTAGE
		UpdateDriveVoltageTimeout();	// Update the drive voltage timeout
	
		if(gFlags.driveSendRequestVoltage && gFlags.driveVoltageOutdated && gRelayState != RELAY_STANDBY) {
			CAN_RequestDriveVoltage();
		}
		#endif
		
		
		// Send BMS status periodically each BMS_STATUS_MSG_DELAY_MS miliseconds
		if(getTime() - timeRef_lastBMSstatusMessage > BMS_STATUS_MSG_DELAY_MS)
		{
			CAN_SendStatus();
			timeRef_lastBMSstatusMessage = getTime();
		}
		
		if(getTime() - timeRef_lastPING_Message > BMS_PING_MSG_DELAY_MS)
		{
			CAN_SendPing();
			timeRef_lastPING_Message = getTime();
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
       
   //Initialisation des périphériques du uC
   //--------------------------------------------------------
   //Timers initialisés:
   //    - Cell measurements refresh every 2 seconds using PIT3 (in user interface continuous mode)
   //    - Mesure du courant chaque 100 ms. On compte 10 interruptions du timer. (PIT2)
   //    - On détermine l'état interlock state, error reset et imd state chaque 10 ms (PIT2).
   //    - Communication with the slave modules timeout tick every 1 second using PIT1
   //    - Warning led flashes with help of PIT4 at 4Hz
   //
   //Relais
   //    - Les relais du circuit haute tension sont initialisés ouverts.
   //

    MCU_init();
    Int_Init();
    XGATE_Init();
    InitCAN(SLAVE_CHANNEL,SLAVE_CHANNEL_INIT);
	InitCAN(MAIN_CHANNEL,MAIN_CHANNEL_INIT);


	initialiseMainVariables(); // gCellTemp, gCellVolt, idleCount, gSlaveReset, gSlaveRev, idTable, gCurrentFlow_mA

	resetIgnoreTempTable(); //Initialisation de la table de températures ignorées    

   //User interface initialization
   SCI5CR2_RIE = 1;                   //Receiver Full Interrupt Enable Bit activation
   SCI5BD = USER_INT_SPEED;           //On assigne la bonne vitesse de transmission

   //Timers activation
   PITCE_PCE0 = 1;
   PITCE_PCE1 = 1;                   //Activation of the 1000 Hz timer (PIT1) : iPIT1_safety_check()
   PITCE_PCE2 = 1;                   //Activation of the ADC timer (PIT2) : iPIT2_100Hz()
   // Lest timer 0 et 3 sont activés ailleurs selon le besoin

   PITCFLMT_PITE = 1;    //Activation of the timer module
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
	// If there is errors (BMS error or IMD error) and we don't ignore the errors
	if(gFlags.errorState && !gParams.ignoreErrors) { 
		gMode = ERROR_MODE;
	}
	
	// If the interlock is open and we dont ignore its state
	else if(!gFlags.interlockClosed && !gParams.ignoreInterlockState)
	{
		gMode = STAND_BY_MODE;
	}
	
	// If the drive is offline and we use its voltage for precharge and relays are opened 
	else if(USE_DRIVE_VOLTAGE == 1 && gFlags.driveVoltageOutdated && gRelayState == OPENED){
		gMode = STAND_BY_MODE;
	}

	// If the interlock is closed or we want to ignore the interlock state (in case the interlock input to the BMS is not plugged)
	else if( (gFlags.interlockClosed || gParams.ignoreInterlockState) && !gFlags.charger_online) {
		gMode = NORMAL_MODE;
	}
	
	// If we are in charging mode and the interlock is closed (or we ignore its state)
	else if (gFlags.charger_online && (gFlags.interlockClosed || gParams.ignoreInterlockState)) {
		
		// We want to avoid passing from NORMAL_MODE (relays cloed) to CHARGE_MODE (relays closed)
		// directly wouthout opening the relays
		if(gMode == CHARGE_MODE || gRelayState == OPENED)
			gMode = CHARGE_MODE;
		else
			gMode = STAND_BY_MODE;
			
		// TODO : trigger a warning or an error if the motor drive and the charger are connected to the network
		// simultaniously
	}
	
	else {
		gMode = STAND_BY_MODE;
	}
}

