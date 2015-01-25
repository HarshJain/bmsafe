
#include <MC9S12XDP512.h>
#include "ltc6803.h"
#include "InterruptHandlers.h"
#include "taskManager.h"

uint32 timeRef_lastMasterMessage = 0;


void HandleTimer_10Hz()
{	
    static uint8 voltCount = 0;			// Voltage measure counter
    static uint8 tempCount = 0;			// Temperature measure counter
	static uint8 selfCheckCount = 0;	// LTC Self Check procedure counter
    static uint8 ltcDiagnosticCount = 3;// LTC diagnostic procedure counter (start with an offset)
	
    gElapsedTime++;      //On compte le nombre de dixiemes de secondes ecoules depuis le dernier init,
    voltCount++;         //la dernière mesure de tensions,
    tempCount++;         //et la dernière mesure de température
	selfCheckCount++;
	ltcDiagnosticCount++;
    
	
	if(selfCheckCount >= SELF_CHECK_PERIOD && gVoltState == V_STANDBY){
		gVoltState = SELF_TEST_START;
		selfCheckCount = 0;
	}
	
	 if(ltcDiagnosticCount >= LTC_DIAGNOSTIC_PERIOD && gVoltState == V_STANDBY)
	{
		gVoltState = DIAGNOSTIC_START;
		ltcDiagnosticCount = 0;
	}
	
    if(voltCount >= VOLT_MEASURE_PERIOD && gVoltState == V_STANDBY) {
		//gFlags.voltTimeout = 1;        // TODO : enlever => redondant avec gVoltState = MEASURE_START
		gVoltState = MEASURE_START;
		voltCount = 0;
    }
	

	// On est à l'étape 0 && Le temps entre 2 mesures de température est passé
	// Si c'est le cas, on commence la séquence de mesure de température
    if(tempCount >= TEMP_MEASURE_PERIOD && gTempState == T_STANDBY) {
		turn_ON_ADC();
		
		gTempState = WAIT; // On passe à l'étape 1 : attente de 20 us pour laisser le temps à l'ADC de se réveiller
		tempCount = 0;  // On remet le compteur à 0
		
		PITCE_PCE1 = 1;	// Activation du timer PIT1 pour attendre au moins 20 us
		PITFLT_PFLT1 = 1; // Reset the PIT1 counter to ensure the interrupt is called 64 us after
    }
}


void HandleTimer_1kHz(void)
{
	gElapsedTime_ms++;
}

void UpdateTimeDependentFlags(void)
{
	if(gElapsedTime_ms - timeRef_lastMasterMessage > MASTER_COM_TIMEOUT_MS)
	gFlags.master_online = 0;	// Communication timeout
	else
	gFlags.master_online = 1;
}