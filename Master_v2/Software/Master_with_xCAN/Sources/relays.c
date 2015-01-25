#include "relays.h"
#include "derivative.h"
#include "common.h"
#include "defines.h"
#include "terminal_code.h"
#include "TimedFunction.h"
#include "charger.h"


CloseRelayState_t gCloseRelayState = RELAY_STANDBY;	// State of the closing relay sequence
RelayState_t gRelayState = OPENED;
static uint32 relayTimeRef = 0;




// Is call at each main loop
void manage_relays(uint16 gMode)
{
	if(gOpenRelayRequest)
	{
		HandleOpenRequest();
		gOpenRelayRequest = 0;	// Request has been processed
		gCloseRelayRequest = 0; // We cancel any closing request
	}
	else if(gCloseRelayRequest)
	{
		HandleCloseRequest();
		gCloseRelayRequest = 0;
	}
	else
	{		
		// Action depending of the BMS mode
		switch(gMode) 
		{
			case NORMAL_MODE:
				HandleCloseRequest(); 
			break;

			case STAND_BY_MODE:
				HandleOpenRequest();
			break;

			case ERROR_MODE: 
				HandleOpenRequest();
			break;
			
			case CHARGE_MODE:
				if(gCharging_state != CHARGING_DONE)
					HandleCloseRequest();
				else
					HandleOpenRequest();

			default:
				HandleOpenRequest();
			break;
        }
	}
}

// Open relay -> poweroff the car
void HandleOpenRequest()
{
	RelayState_t currentState = gRelayState;

	switch(currentState)
	{
		case CLOSING:
			gCloseRelayState = ABORTED;	// Stop the closing relay sequence
			SCIprintString("RelayManager: Interrupting closing relays sequence for opening sequence...\n\r");
			OpenRelays();
		break;
		
		case OPENING:
			//SCIprintString("An opening relay sequence is already running...\n\r");
		break;
		
		case CLOSED:
			SCIprintString("RelayManager: Starting relays opening sequence...\n\r");
			OpenRelays();
		break;
		
		case OPENED:
			if(gCloseRelayState == ABORTED)
				gCloseRelayState = RELAY_STANDBY;
			//SCIprintString("The relay are already opened...\n\r");
		break;
	}
}

// Close relay -> power on the car
void HandleCloseRequest()
{
	RelayState_t currentState = gRelayState;
	
	switch(currentState)
	{
		case CLOSING:
			//SSCIprintString("A closing relay sequence is already running...\n\r");
			CloseRelaysSequence();
		break;
		
		case OPENING:
			//SCIprintString("RelayManager: Closing relay request is void: because an opening sequence is running...\n\r");
		break;
		
		case CLOSED:
			//SCIprintString("Closing relay request is void: the relays are already closed...\n\r");
		break;
		
		case OPENED:
			SCIprintString("RelayManager: Starting relays closing sequence...\n\r");
			CloseRelays();
		break;
	}
}

// **************************************************************************************
// Open the relays
// **************************************************************************************
void OpenRelays()
{	
	gRelayState = OPENING;

	// Close the discharge relay
	DISCH_RLY = DISCH_RLY_CLOSED;
	// Open all the relays
	AIR_N = RLY_OPEN;
	AIR_P = RLY_OPEN;
	PRE_RLY = RLY_OPEN;
    
	gRelayState = OPENED;
	SCIprintString("RelayManager: Relays are now open.\n\n\n\r");
}


//*****************************************************************************
// Start a closing relay sequence if the relays are not already closed
// and no sequence is already started
//*****************************************************************************
void CloseRelays()
{
	gRelayState = CLOSING;
	
	if(gCloseRelayState == RELAY_STANDBY)
	{
		SCIprintString("RelayManager: Starting the closing relays sequence...\n\r");
		gCloseRelayState = PRECHARGE_PREPARATION; 	// Set the beginning of the sequence
		CloseRelaysSequence();						// Start executing the sequence	
	}
}

// ***ICI*** Est ce nécéssaire ou on peut se basé directement sur le mode du BMS
uint16 CanCloseRelay()
{
	// Interlock state OK
	if(gFlags.interlockClosed || gParams.ignoreInterlockState)
	{
		// BMS error state OK (including IMD error)
		if(!gFlags.errorState || gParams.ignoreErrors)
				return 1;	// We can close the relays
		else
			SCIprintString("RelayManager: Relay Closing Sequence abort: ERROR.\n\n\n\r");
	} 
	else
		SCIprintString("RelayManager: Relay Closing Sequence abort: Interlock is opened.\n\n\n\r");

	return 0;
}

void CloseRelaysSequence()
{
	uint8 precharge = 0;

	switch((CloseRelayState_t) gCloseRelayState)
	{
		case ABORTED:
			// if (can close relays)
			gCloseRelayState = RELAY_STANDBY;
			return;

		// Standby state
		case RELAY_STANDBY:
			return; // Nothing to do
		
		// Step 1
		case PRECHARGE_PREPARATION:
			DISCH_RLY = DISCH_RLY_OPEN;	// Open discharge circuit
			AIR_N = RLY_CLOSED;			// Close the AIR-
			
			gCloseRelayState = PRECHARGING;			// Go to next step
			break;

		// Step 2
		case PRECHARGING:
			PRE_RLY = RLY_CLOSED;		// Closing discharge relay
			
			gCloseRelayState = PRECHARGING_WAIT;			// Go to next step
			relayTimeRef = gTime_ms;	// Time reference for precharging process
			break;
			
		case PRECHARGING_WAIT:
			precharge = preChargeDone(); // 1 = precharge done, 2 = timeout, 0 = precharge pending
			
			// Precharge is done
			if(precharge == 1)
				gCloseRelayState = CLOSING_AIRP;
			
			// Timeout for precharge (too long, there is probably an issue)
			else if(precharge == 2)
			{
				gCloseRelayState = ABORTED;
				HandleOpenRequest();
			}
			
			break;

		// Step 3
		case CLOSING_AIRP:
			AIR_P = RLY_CLOSED;        	// Closign AIR+
			
			gCloseRelayState = PRECHARGING_DONE;			// Go to next step
			break;

		// Step 4
		case PRECHARGING_DONE:
			PRE_RLY = RLY_OPEN;		// Opening precharge relay

			gCloseRelayState = RELAY_STANDBY;		// Go to next step
			gRelayState = CLOSED; // Relays are closed
			SCIprintString("RelayManager: Precharge successful Relays are  now closed.\n\n\n\r");
			break;
		
		// Unknown state
		default:
			OpenRelays();	// Since we don't know which relay are closed and open, we open (for safety)
			return;
   }
   
}






uint8 preChargeDone(void)
{
	// If we are not charging, the preCharge is done using the drive's voltage
	// if USE_DRIVE_VOLTAGE == 1, it is done with a delay otherwise
	if(gMode != CHARGE_MODE)
	{
		if(USE_DRIVE_VOLTAGE == 1)
		{
			// When the drive voltage is sufficiently high (>90% of pack voltage)
			if(gDriveVoltage > 0.91*gTotalPackVoltage)
				return 1;	// Precharge done
		}
		else
		{
			if(gTime_ms - relayTimeRef > PRECHARGE_DELAY_MS)
				return 1;	// Precharge done
		}
		
		if(gTime_ms - relayTimeRef > PRECHARGE_TIMEOUT_MS)
		{
			SCIprintString("RelayManager: Precharging timeout.\n\n\n\r");
			return 2;	// Timeout
		}
	}
	
	// If we are charging, we use a time interval
	else if(gMode == CHARGE_MODE)
	{
		if(gTime_ms - relayTimeRef > PRECHARGE_CHARGE_MODE_DELAY_MS)
			return 2;	// Timeout
	}
	
	return 0;	// Precharge not finished
}


