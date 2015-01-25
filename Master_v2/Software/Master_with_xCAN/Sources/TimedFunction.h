#ifndef TIMEDFUNCTION_H
#define TIMEDFUNCTION_H

#include "defines.h"

extern uint32 gTime_ms;		// Elapsed time in ms

extern uint32 timeRef_lastDriveVoltageRequest; 		//!< Holds the time when last voltage request was sent
extern uint32 timeRef_lastBMSstatusMessage;			//!< Holds the time when the last BMS status message was sent
extern uint32 timeRef_lastPackVoltageCalculation;	//!< Holds the time when the last total voltage calculation was done
extern uint32 timeRef_lastCurrentMeasurementMessage; //!< Holds the time when the last current measurement message was received

extern uint32 timeRef_lastDriveVoltageMessage; 	//!< Holds the time when last drive voltage message was received
extern uint32 timeRef_lastChargerMessage; 		//!< Holds the time when the last charger message was received
extern uint32 timeRef_lastTCS_Message;			//!< Holds the time when the last TCS message was received
extern uint32 timeRef_lastDriveMessage;			
extern uint32 timeRef_lastPING_Message;			//!< Holds the time when the last master PING message was sent
extern uint32 timeRef_lastBalanceCommand;		//!< Holds the time when the last balancing command was sent

void HandleErrorCode(void);

void ComputeImpedance(void);	// Calculer l'impedance entre les systemes HV et le ground

void UpdateSlaveActivity(void);

// Input level updates
void UpdateInterlockState(void);
void UpdateIMD_State(void);
void UpdateErrorResetButtonState(void);

void UpdateDriveVoltageTimeout(void);
void UpdateTimeDependentFlags(void);

uint32 getTime(void);

#endif