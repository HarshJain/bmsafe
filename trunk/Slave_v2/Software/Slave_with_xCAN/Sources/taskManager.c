#include "derivative.h"      /* derivative-specific definitions */

#include "defines.h"
#include "ltc6803.h"
#include "taskManager.h"
#include "temp.h"


void manage_voltages()
{
	static uint32 startTime;
	static uint8 voltMeasureCount = 0;
	uint8 i;
	uint8 txError = 0;
	uint8 openWireCell = 0;
	int16 voltages[NB_CELL];	// Receive voltage values
	int32 val;					// Used for manipulations
	

	// -------------- Mesure des tensions -----------------
	// voltTimeout = 1 : le délai entre deux mesures est passé
	// On vérifie que le WDT n'est pas dépassé (LTC toujours en mode "measure")
	//
	// -> si gVoltState = MEASURE_START (début du cycle) 
	// On vérifie la communication avec la LTC (chaque 10 mesures)
	// On lance la mesure des tensions
	//
	// -> si gVoltState = MEASURE_WAIT et les conversions sont terminées
	// On récupère les données
	// On fait la moyenne et on envoie tout ça au master
	// On vérifie si il y a une connexion ouverte
	// On met voltTimetout et gVoltState à V_STANDBY
	
    if (gVoltState != V_STANDBY) {	 
	
		#ifdef SPI_ENABLE
		if(ltcConfigurationOK(&g_ltcConfig))	 // Verify the configurations
		{
			if(gVoltState == MEASURE_START) // Début du cycle de mesure de tensions
			{
				turnOffLEDs();		// Turn off LED to decrease input current and prevent measure errors
				voltMeasureCount++; // Nombre de mesures prises depuis le dernier envoi

				// Each 10 measures, clear the LTC measure register
				if(voltMeasureCount % 10 == 0)
					// Fast Self Check resets the ltc register to 0xFF so we make sure not to measure the registers are well updated
					ltcClearADC();

				// ------------------ On lance la mesure des tensions (conversions) -------------------
				ltcStartVoltageMeasure();
			
				gVoltState = MEASURE_WAIT; // Maintenant, on doit attendre que les conversions soient terminées
			
				startTime = gElapsedTime; // Time reference
			}
			else if(gVoltState == SELF_TEST_START)
			{
				ltcSelfCheck(); // Start the LTC Self Test procedure
			}
			 else if(gVoltState == DIAGNOSTIC_START)
			{
				ltcDiagnostic();
			} 
			else if(gVoltState == MEASURE_WAIT || gVoltState == SELF_TEST_WAIT || gVoltState == DIAGNOSTIC_WAIT) // Attente de la fin des conversions terminées
			{
				// TODO : vérifier si on est mieux d'attendre dans l'interruption de 16 kHz ce
				//			qui nous éviterais de faire les checks sur les régistres à chaque fois

				if(ltcConversionFinished()) // Si les conversions sont terminées
				{
					if(gVoltState == SELF_TEST_WAIT)	// Currently performing a self check
					{
						// Finish the self check by recalling the function and storing it's return value (1 = error)
						gFlags.ltcConversionFail = ltcSelfCheck();
					}
					 else if(gVoltState == DIAGNOSTIC_WAIT)
					{
						// Finish the diagnostic by recalling the function and storing it's return value (1 = error)
						gFlags.ltcConversionFail = ltcDiagnostic();
					} 
					else if(gVoltState == MEASURE_WAIT)
					{
						// -------------------- On récupère les données -----------------------
						txError = ltcReadCellVoltage(voltages); // On récupère les données
						
						// Arithmetic average on the data acquired since the last transmission of the voltage to the master
						// V = (V1 + V2 + V3 + ... + Vn)/n		n = number of measure taken since the last voltage message to the master
						for(i = 0; i < NB_CELL; i++) {
							val = (int32)voltages[i] + ((voltMeasureCount - 1) * (int32)gVoltages[i]);
							gVoltages[i] = val / voltMeasureCount;
							//gVoltages[i] = voltages[i];
						}
						
						#ifdef CAN_ENABLE
						// ------------------- Envoie des tensions vers le maitre si recues sans erreur du LTC6803 ---------------
						if(!txError && voltMeasureCount >= NB_VOLT_SAMPLES)
						{
							gFlags.canTxError = CAN_SendVoltages(gSlaveID, gVoltages);  //TODO: on ne fait rien avec ca...
							voltMeasureCount = 0;
						}
						#endif //CAN_ENABLE
					
					
						if(voltMeasureCount % 10 == 0) // Chaque 10 mesures, on démarre la vérification de connexions ouvertes
						{
							// Detection des connexions ouvertes
							 openWireCell = ltcVerifyOpenWire();
							
							if(openWireCell != 0){
								gFlags.openWire = 1;
								CAN_SendDTC(11, 1, gSlaveID);			// Sends a DTC message with possible additionnal data
							}
						}

						gVoltState = V_STANDBY;		// On retourne à la première étape
						//gFlags.voltTimeout = 0;		// On attent le prochain délai
						txError = 0;
						gFlags.ltcConversionFail = 0;
					}
					else
						gVoltState = V_STANDBY;
				}
				else
				{
					// If the conversions are not finished after 1 second, the LTC chip is probably not plugged to the cells
					if(gElapsedTime - startTime > 10)
					{
						gFlags.ltcConversionFail = 1;
						gVoltState = V_STANDBY;
					}
				}
			}
		 } // if(ltcConfigurationOK(&ltcConfig))

		#endif // SPI_ENABLE
    }
}

void manage_temperatures()
{
	static uint8 tempMeasureCount = 0;
	uint16 *result; // Va servir à récupérer les données des ADCs en pointant vers le régistre correspondant
	uint8 i;
	int32 val;

	// Timeout reached, we start measure procedure
	if(gTempState == START)
	{
		
	}
	
	// Les mesures de temperatures sont pretes a etre récupérées et utilisées
	else if (gFlags.ADC0done && gFlags.ADC1done)  // gTempState = 2, but no need to test the value
	{
		tempMeasureCount++;

		// ------------ Récupération des mesures des ADC ---------------
		
		// ####### ADC0 ######
		result = (uint16*) &ATD0DR0H; // &ATD0DR0H Pointe sur le début du registre de l'ADC0
		
		// On récupère les données dans le premier registre et les registres subséquents
		for(i=0; i < 8; i++){
			gTempRaw[gTempIndex[i]] = *result; // On récupère les données en les mettant en ordre (T0, T1, ...) dans gTempRaw[]
			result++;
		}
		
		// ####### ADC1 #######
		result = (uint16*) &ATD1DR0H; // Pointe sur le registre de l'ADC1
		
		// On récupère les données dans le premier registre et les registres subséquents
		for(i=0; i < 4; i++){
			gTempRaw[gTempIndex[i + 8]] = *result; // On récupère les données en les mettant en ordre (T0, T1, ...) dans gTempRaw[]
			result++;
		}
		
		
		// Arithmetic average on the values measured since the last temperature message sent to the master
		for(i = 0; i < NB_CELL; i++) {
			val = (int32)convertTemp(gTempRaw[i]) + ((int32)gTemp[i] * (tempMeasureCount - 1));
			gTemp[i] = val / tempMeasureCount;
		}

	    #ifdef CAN_ENABLE
	    // Send the temperatures to master via CAN
	    if(tempMeasureCount >= NB_TEMP_SAMPLES) {
	      // TODO: we do nothing with this flag so far...
	      gFlags.canTxError = CAN_SendTemperatures(gSlaveID, gTemp);
	      tempMeasureCount = 0;
	    }
	    #endif

	    gFlags.ADC0done = 0;
	    gFlags.ADC1done = 0;
		gTempState = T_STANDBY;
	}
}

void manage_LED()
{
	if(gVoltState != V_STANDBY)
	{
		turnOffLEDs();
		return;
	}
	else
		LED2 = LED_ON;

	// LED control
	if(gFlags.spiTimeout || gFlags.canTxError || gFlags.canRxError || gFlags.ltcConversionFail || gFlags.openWire || !gFlags.master_online)
		LED4 = LED_ON;
	else
		LED4 = LED_OFF;
	
	if(gFlags.slaveDoubleID || gFlags.badSlaveId || (gFlags.balancingActive && !gFlags.disch_circuit_plugged))
		LED3 = LED_ON;
	else
		LED3 = LED_OFF;
}

void manage_CAN(void)
{
	#ifdef CAN_ENABLE
	// If the master has request the slave revision ID, we send it (he he, quite logic)
	if (gFlags.firmwareRequest)
	{
		gFlags.canTxError = CAN_SendFirmwareRevision(gSlaveID, FIRMWARE_REVISION);	// Send firmware revision to the master
		gFlags.firmwareRequest = 0;
	}
	
	// Process all received CAN messages
	while(g_canMsgCount > 0)
		CAN_ProcessMessage(gSlaveID);
	#endif
}

void manage_equilibration(void)
{
	if(!gFlags.balancingActive && gBalanceVector != 0 && gFlags.master_online) {
		gFlags.balancingActive = 1;
		gFlags.equiStatusChange = 1;
	}
	else if(gFlags.balancingActive && (gBalanceVector == 0 || !gFlags.master_online)) {
		gFlags.balancingActive = 0;
		gFlags.equiStatusChange = 1;
	}

	// We reconfigure the LTC registers if status has changed
    if(gFlags.equiStatusChange)
	{
		#ifdef SPI_ENABLE
		
		if(gFlags.balancingActive && gFlags.disch_circuit_plugged)
			ltcMakeConfigRegister(&g_ltcConfig, gBalanceVector);
		else // disch circuit not plugged OR balancing inactive
			ltcMakeConfigRegister(&g_ltcConfig, 0); // Null balancing vector
		
		ltcWriteConfig(&g_ltcConfig);
		
		#endif
		
		gFlags.equiStatusChange = 0; // Clear flag, changes have been made
    }

    if(gFlags.balancingActive)
	{
		checkForBalancedCells(&gBalanceVector, gVoltages, gBalThres, BAL_DELTA_VOLT);

		if(gBalanceVector == 0) // all cells are balanced, send the status change to the master
		{
			gFlags.balancingActive = 0;
			gFlags.equiStatusChange = 1;
		}
    }
}


// Functions implementation.
void checkForBalancedCells(uint16* curBalanceArray, uint16 voltages[], uint16 targetVoltage, uint16 fuzzFactor)
{
	uint8 i = 0;
	
	for (i = 0; i < NB_CELL; i++)
	{
		if ((*curBalanceArray & (1 << i)) &&  // If this cell is currently discharging
			(voltages[i] < (targetVoltage + fuzzFactor))) // And it is equilibrated
		{
	        *curBalanceArray &= ~(1 << i);  // Remove it's equilibration bit
	        gFlags.equiStatusChange = 1;  // Ask to update register
	    }
	  }
}

void turnOffLEDs()
{
	LED2 = LED_OFF;
	LED3 = LED_OFF;
	LED4 = LED_OFF;
}


void turn_ON_ADC()
{
	ATD0CTL2_ADPU = 1;  //On démarre l'ADC (channel 0)
    ATD1CTL2_ADPU = 1;  //On démarre l'ADC (channel 1)
}

void turn_OFF_ADC()
{
	// TODO : implémenter
}

void start_ADC_conversion()
{
	ATD0CTL5_CA = 0; 	//start ADC0 conversion (from AN0 to AN7)
	ATD1CTL5_CA = 0;  	//start ADC1 conversion (from AN8 to AN11)
}

void updateInputs()
{
	if(DISCH_MONITOR == 0 && !gFlags.disch_circuit_plugged) {
		gFlags.disch_circuit_plugged = 1;
		gFlags.equiStatusChange = 1;
	}
	else if(DISCH_MONITOR == 1 && gFlags.disch_circuit_plugged) {
		gFlags.disch_circuit_plugged = 0;
		gFlags.equiStatusChange = 1;
	}
}