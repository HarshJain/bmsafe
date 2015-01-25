
#include "charger.h"
#include "can.h"

// Charging control parameters
ChargingParameters gChargingParameters = {	1,						// Enable CAN
											1,						// Enable Ventilation
											MAINS_CURRENT_MAXIMUM,	// Maximum input current
											0,						// Output voltage
											0						// Output current
											};

											
uint32 timeRef_chargerSendTimeout = 0;
Charging_State gCharging_state = NOT_CHARGING;		// Holds the charging state
Charger_State NLG5_state;	// Holds NLG5 charger parameters and values


// Enable or disable ventilation (1 = enable, 0 = disable)
void charger_enableVentilation(uint8 enable)
{
	if(enable == 0)
		gChargingParameters.ventilation = 0;	// No ventilation
	else
		gChargingParameters.ventilation = 1;	// Activate ventilation
}

// Change the charging output parameters (voltage and current)
void charger_setOutputParameters(uint16 output_voltage, uint16 output_current)
{
	gChargingParameters.output_voltage = output_voltage;
	gChargingParameters.output_current = output_current;
}

void charger_clearErrorLatch()
{
	gChargingParameters.clear_error = 1;	// Clear error latch on next CAN message send
}


void manage_charging()
{
	if(gMode != CHARGE_MODE)
	{
		gCharging_state = NOT_CHARGING;
		return;
	}
	
	if(gCharging_state == STEP1) // Step 1 : Current charging
	{
		// Go to next step if condition is verified
		if(*gHighestCellVoltage >= STOP_VOLTAGE_STEP1)
			gCharging_state++;
		else
			charger_setOutputParameters(MAXIMUM_OUTPUT_VOLTAGE, CHARGE_CURRENT_STEP1);
	}
	
	else if(gCharging_state == STEP2) // Step 2 : Voltage charging
	{
		// Go to next step if condition is verified
		if(NLG5_state.actual_1.output_current <= STOP_CURRENT_STEP2)
			gCharging_state++;
		else
			charger_setOutputParameters(CHARGE_VOLTAGE_STEP2, MAXIMUM_OUTPUT_CURRENT);
	}
	
	else if(gCharging_state == STOP_CHARGING)
	{
		// We need to stop the charger before opening the relays
		charger_setOutputParameters(MAXIMUM_OUTPUT_VOLTAGE, 0); // Cut off the charging current current
		CAN_sendChargingControlParameters(&gChargingParameters); // Send the parameters to the charger
		timeRef_chargerSendTimeout = gTime_ms;
		
		gCharging_state++;
	}
	
	else if(gCharging_state == CHARGING_DONE)
	{
		// Nothing to do here, the relays will close when gCharging_state == CHARGING_DONE (see relays.c)
	}
	
	// Send the command each 100 ms or depending on the configuration (CHARGER_SEND_MSG_PERIOD_MS)
	if(gTime_ms - timeRef_chargerSendTimeout > CHARGER_SEND_MSG_PERIOD_MS)
	{
		CAN_sendChargingControlParameters(&gChargingParameters);
		timeRef_chargerSendTimeout = gTime_ms;
	}

}


void charger_updateStatus(uint8 *data, uint8 length)
{
	uint8 *status;

	if(length < 3) // Data length should be of 4 bytes
		return;
	
	status = (uint8*)&(NLG5_state.status); // Points to the first byte of the structure
	
	// Retreive data (and parsing by casting into a structure)
	*status = data[0];
	*(status + 1) = data[1];
	*(status + 2) = data[2];
}

void charger_updateErrors(uint8 *data, uint8 length)
{
	if(length < 5) // Data length should be of 4 bytes
		return;
	
	// Parse data
	// TODO : get interesting data
}

void charger_updateAcutals(uint8 *data, uint8 length)
{
	if(length < 8) // Data length should be of 8 bytes
		return;
	
	// Retreive data (big endian)
	NLG5_state.actual_1.mains_current = data[0] << 8 | data[1];
	NLG5_state.actual_1.mains_voltage = data[2] << 8 | data[3];
	NLG5_state.actual_1.output_voltage = data[4] << 8 | data[5];
	NLG5_state.actual_1.output_current = data[6] << 8 | data[7];
}

// Function which verify that charging is going well
void AssertSafeCharging(void)
{
}

