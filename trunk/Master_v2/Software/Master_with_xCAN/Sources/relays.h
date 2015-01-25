
/*! \file relays.h
 * 	This file contains all relays management functions, structures and types.
 */

#ifndef RELAYS_H
#define RELAYS_H

#include "defines.h"

// Relay name
#define AIR_N        PORTA_PA0 // AIR-, MRC0
#define AIR_P        PORTA_PA1 // AIR+, MCR1
#define PRE_RLY      PORTA_PA2 // Precharge-relay, MCR2
#define DISCH_RLY    PORTA_PA3 // Discharge-relay, MCR3

// Relay state definition
#define RLY_OPEN    0
#define RLY_CLOSED  1
#define DISCH_RLY_OPEN    1
#define DISCH_RLY_CLOSED  0

// Delay definition
#define RELAY_DELAY         100            //[ms] Time between the opening or closing of 2 relays
#define PRECHARGE_DELAY     8000           //[ms] Time of precharge
#define DISCHARGE_DELAY     15000          //[ms] Time of discharge. Le circuit de décharge ne devrait pas être actif

// Tension importante
// TODO: Ajuster valeur
#define PRECHARGE_VOLTAGE   1000

#define START_SEQUENCE        0
#define CONTINUE_SEQUENCE     1

// Enumerations
typedef enum
{
	ABORTED = -1,
	PRECHARGE_PREPARATION = 0,	// First step for closing relays
	PRECHARGING,
	PRECHARGING_WAIT,
	CLOSING_AIRP,
	PRECHARGING_DONE,
	RELAY_STANDBY				// No closing relay sequence is going on
} CloseRelayState_t;


typedef enum
{
	OPENED = 0,			// Relays are opened (car is shut down)
	CLOSED = 1,			// Relays are closed (ready to drive !)
	CLOSING = 2,		// Relays are in a closing sequence
	OPENING = 3			// Relays are in an openning sequence
} RelayState_t;

/*!
\brief Manage relay state 

To be called at each frame, this function check the BMS state and the
current relay state and execute any necessary action

\param[in] gMode Enum which describe the BMS state
\return void
*/
void manage_relays(uint16 gMode);

/*!
\brief Execute necessary action to be in the relay open state (car shutdown)

Different action depending of the current relay state:
	- Already open -> Do nothing
	- Opening in process -> Do nothing
	- Closed -> Call openning process
	- Closing in progress -> Interupt closing request and call openning process

*/
void HandleOpenRequest(void);

/*!
\brief Execute necessary action to be in the relay close state (powered car)

Different action depending of the current relay state:
	- Opened -> Start the beginning of the closing sequence
	- Opening in progress -> Raised an error for the anormal condition
	- Closed -> Do nothing
	- Closing in progress -> Continue closing progress

*/
void HandleCloseRequest(void);

/*!
\brief Opening Process

Closed the following relay in this order (in the same function call):
 	- Close discharge relay
	- Open AIR- relay
	- Open AIR+ relay 
	- Open precharge relay

*/
void OpenRelays(void);

/*!
\brief Set-up before calling the closing sequence

Check if their is no error flag raised:
 -> Initialize data for closing sequence
 -> Start closing sequence
*/
void CloseRelays(void);

/*!
\brief Closing process

Close the following relays in order (done in multiple function calls) 
 	1. Precharge-preparation 
 		- Open discharge relay
 		- Open AIR- relay
 	2. Precharging
 		- Close precharge relay
 	-> the car is now precharging
 	3. Precharging_wait
 		- Wait until the precharge is done (check current voltage or timeout)
	4. Closing-AIR+
		- Close AIR+ relay
	5. Precharge-done
		- Open precharge relay
		-> the car is now done

*/
void CloseRelaysSequence(void);

/*!	\brief	Indicate the motor drive's pre-charge state.
 *
 *			Indicate if the motor drive's pre-charge is completed, incompleted or timed out.
 *			The pre-charge is completed when the drive's voltage reaches 90% of the battery
 *			pack voltage (total voltage) or a time laps if #USE_DRIVE_VOLTAGE = 0.
 *
 *
 * 	\return 0 if the pre-charge is not completed, 1 if the pre-charge is done, 2 if the precharge has timed out
 *	\sa #USE_DRIVE_VOLTAGE, #PRECHARGE_CHARGE_MODE_DELAY_MS, #PRECHARGE_DELAY_MS,PRECHARGE_TIMEOUT_MS
 */
uint8 preChargeDone(void);



uint16 CanCloseRelay(void);


// Variables
extern CloseRelayState_t gRelayState;

#endif RELAYS_H
