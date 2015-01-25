#include "TimedFunction.h"

#include "relays.h"
#include "common.h"
#include "TimedFunction.h"
#include "Display.h"

#include "defines.h"
#include "derivative.h"      /* derivative-specific definitions */

uint32 gTime_ms = 0;		// Elapsed Time

// Time reference variables
uint32 timeRef_lastDriveVoltageMessage = 0;
uint32 timeRef_lastDriveVoltageRequest = 0;
uint32 timeRef_lastBMSstatusMessage = 0;
uint32 timeRef_lastChargerMessage = 0;
uint32 timeRef_lastPackVoltageCalculation = 0;
uint32 timeRef_lastCurrentMeasurementMessage = 0;
uint32 timeRef_lastTCS_Message = 0;
uint32 timeRef_lastPING_Message = 0;
uint32 timeRef_lastBalanceCommand = 0;


void HandleErrorCode(void) 
{
  uint16 anyError = 0;

   // Possible errors :
   // 1 - Temperature or Voltage of a cell is too low or too high (gError / anyError)
   // 2 - We lost communication with a slave (gError.slaveTimeout / anyError)
   // 3 - There is a ground fault (gFlags.ImdError)
   // 4 - The TCS is offline and the charger is offline (not in charging mode means we need the TCS online)
   // 5 - The drive and the charger are both offline (we need one of both online to do something)
   // 6 - The drive and the charger are both online (this is not normal, we only want one online)
   
   anyError = *((uint16*) &gError); 								// 1 and 2
   anyError += gFlags.ImdError; 									// 3
   anyError += (gFlags.drive_online && gFlags.charger_online); 		// 4
   anyError += (!gFlags.drive_online && !gFlags.charger_online);	// 5
   anyError += !gFlags.TCS_online; 									// 6
   
   if(anyError != 0){
		//Erreurs qui requiert le signal errorReset pour être remises à zéro
		gFlags.errorState = 1;
   }
   else{
      if(gFlags.errorState && gFlags.errorReset)
         gFlags.errorState = 0;
   }
}

                               
// Section pour calculer la resistance avec MHS 
const uint16 countTrigger = 1000; //TODO; envoyer vers define

void ComputeImpedance(void) 
{
  static uint16 highCount = 0;
  static uint16 lowCount = 0;
  float dutyCycle;
  
  uint8 reading = IMD_MHS; // read input port pin PS1
  
  if(reading == 1)
  {
    highCount ++;
  } 
  else
  {
    lowCount ++;
  }
  
  if(lowCount + highCount >= countTrigger)
  {
    // Équation: Rf = (90% * 1200K-ohms) / (dutyCycle - 5%) - 1200k-ohms
    dutyCycle = highCount / (highCount + lowCount);
    gImpedance = (0.90 * 1200) / (dutyCycle - 0.05) - 1200;
    highCount = 0;
    lowCount = 0;
  }
}

void UpdateSlaveActivity(void)
{
	// Timer pour mesurer l'activité des esclaves
	static uint8 idleTimerCount = 0;
	uint8 i;
	
	idleTimerCount++;
	if(idleTimerCount >= IDLE_TIMER_COUNT_PERIOD) {
		for(i=0; i < N_MOD; ++i) {
			if(idleCount[i] < COM_IDLE_COUNT_MAX)
				idleCount[i]++;
		}
	
	idleTimerCount = 0;
	}
}


void UpdateInterlockState(void)
{
	static uint16 interlockStateHistory = 0;
	uint16 curInterlockState;

	//On détermine l'état de l'entrée INTERLOCK STATE. On attend d'avoir reçu 5 valeurs (50 ms pour changer)
    //identiques avant de changer le drapeau.
    curInterlockState = (uint16) INTERLOCK_STATE;
    interlockStateHistory = interlockStateHistory << 1;
    interlockStateHistory = interlockStateHistory | (curInterlockState & 0x1);
    
    if((interlockStateHistory & 0x1F) == 0x1F)    //logique normale, i.e. 1 = interlock fermé
        gFlags.interlockClosed = 1;
    else if((interlockStateHistory & 0x1F) == 0) 
        gFlags.interlockClosed = 0;
}


void UpdateIMD_State(void)
{
	static uint16 imdStateHistory = 0;
	uint16 curImdState;

	 //On détermine l'état de l'entrée IMD STATE. On attend d'avoir reçu 5 valeurs (50 ms pour changer)
    //identiques avant de changer le drapeau.
    curImdState = (uint16) IMD_STATE;
    imdStateHistory = imdStateHistory << 1;
    imdStateHistory = imdStateHistory | (curImdState & 0x1);
    
    if((imdStateHistory & 0x1F) == 0x1F)    //logique inverse, i.e. 0 = imd error
        gFlags.ImdError = 0;
    else if((imdStateHistory & 0x1F) == 0) 
        gFlags.ImdError = 1;
}

void UpdateErrorResetButtonState(void)
{
	static uint16 errRstHistory = 0;
   	static uint16 errorRstDashHistory = 0;
   	
    uint16 curErrRst;
	uint16 curErrorRstDash;

	//On détermine l'état de l'entrée ERROR_RST_DASH. On attend d'avoir reçu 5 valeurs (50 ms pour changer)
    //identiques avant de changer le drapeau.
    curErrorRstDash = !((uint16) ERROR_RST_DASH); // ERROR_RST_DASH est à 0 s'il y a un problème
    errorRstDashHistory = errorRstDashHistory << 1;
    errorRstDashHistory = errorRstDashHistory | (curErrorRstDash & 0x1);
	
    
    //On détermine l'état de l'entrée ERROR RESET. On attend d'avoir reçu 5 valeurs (50 ms pour changer)
    //identiques avant de changer le drapeau.
    curErrRst = (uint16) ERROR_RESET;
    errRstHistory = errRstHistory << 1;
    errRstHistory = errRstHistory | (curErrRst & 0x1);
    
    if((errRstHistory & 0x1F) == 0x1F)    //logique inverse, i.e. 0 = bouton pressé
        gFlags.errorReset = 0;
    else if((errRstHistory & 0x1F) == 0)
	{
		resetIgnoreTempTable(); // On réinitialise le tableau des températures ignorées
		resetIDtable(); // On réinitialise la table de correspondance
		gFlags.errorReset = 1;
	}
}


void UpdateDriveVoltageTimeout(void)
{	
	// Time before gDriveVoltage (voltage value) is obsolete
	if(gTime_ms - timeRef_lastDriveVoltageMessage > DRIVE_VOLTAGE_RCV_TIMEOUT_MS)
		gFlags.driveVoltageOutdated = 1;
	else
		gFlags.driveVoltageOutdated = 0;
		
		
	// Time between each request
	if(gTime_ms - timeRef_lastDriveVoltageRequest > DRIVE_REQUEST_DELAY_MS)
		gFlags.driveSendRequestVoltage = 1;
	else
		gFlags.driveSendRequestVoltage = 0;
}


void UpdateTimeDependentFlags(void)
{
	// gFlags.charger_online
	// We consider the NLG Charger is offline when no message for more than a certain period
	if(getTime() - timeRef_lastChargerMessage > 400)
		gFlags.charger_online = 0;	// Charger offline
	else
		gFlags.charger_online = 1;	// Charger online

	
	// gFlags.currentMeasureOutdated
	// We consider the measure outated when we didn't receive a measure within a certain period
	if(getTime() - timeRef_lastCurrentMeasurementMessage > CURRENT_RCV_TIMEOUT_MS)
		gFlags.currentMeasureOutdated = 1;
	else
		gFlags.currentMeasureOutdated = 0;
		
		
	// gFlags.TCS_online
	// We consider the TCS offline if we didn't receive a message within a certain period
	if(getTime() - timeRef_lastTCS_Message > TCS_CONNECTION_TIMEOUT_MS)
		gFlags.TCS_online = 0;
	else
		gFlags.TCS_online = 1;
		
		
	// gFlags.drive_online
	if(getTime() - timeRef_lastDriveMessage > 300)
		gFlags.drive_online = 0;
	else
		gFlags.drive_online = 1;
}

uint32 getTime(void)
{
	return gTime_ms;
}
