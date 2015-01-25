

#include <stdio.h>
#include <hidef.h>            /* common defines and macros */

#include "derivative.h"      /* derivative-specific definitions */

#include "InterruptHandler.h"
#include "TimedFunction.h"
#include "Assert.h"
#include "Display.h"
#include "terminal_code.h"
#include "defines.h"
#include "termio.h"
#include "relays.h"


void HandleSafetyCheck_1kHz(void)
{   
   AssertSafeVoltage();
   AssertSafeTemperature();
   AssertCommunicationWithSlave();
   AssertSafeCurrent();
   ComputeImpedance();
   HandleErrorCode();
}


void HandleTimer_100Hz(void)
{
	// Uptade inputs states
	UpdateInterlockState();			// Update interlock input
	UpdateIMD_State();				// Update IMD input
	UpdateErrorResetButtonState();	// Update Error Reset button input
	
	// Other updates
    UpdateDashLed();
  	UpdateSlaveActivity();
	CAN_SendStatus();
}

void HandleTimer_1kHz(void)
{
	gTime_ms++;	// Increase elapsed time of 1 ms
}