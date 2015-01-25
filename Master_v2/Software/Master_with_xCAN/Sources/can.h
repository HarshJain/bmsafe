
/*! \file can.h
 * 	This file contains all CAN (Controller Area Network) related code.
 */

#ifndef CAN_H
#define CAN_H

#include "defines.h"
#include "common.h"
#include "TimedFunction.h"
#include "charger.h"

// xCAN library specific headers
struct XGCANstruct;

#define MAIN_CHANNEL 		&ChannelCAN1	// Main network channel
#define MAIN_CHANNEL_INIT	&CAN_Init_CAN1	// Main network channel initialisation values
#define SLAVE_CHANNEL 		&ChannelCAN0	// Slave network channel
#define SLAVE_CHANNEL_INIT	&CAN_Init_CAN0	// Slave network channel initialisation values

#define CAN_ERROR		1
#define CAN_NO_ERROR	0

// Node IDs (NID)
#define MASTER_NID		0x01		// BMS master node ID
#define TCS_NID			0x02		// Traction Control System node ID
#define DRIVE_NID		0x03		// Drive node ID
#define CHARGER_NID		0x04		// Battery Charger node ID
#define DAQ_NID			0x05		// Data Acquisition System node ID

// Commands IDs from BMS slave
#define CAN_VOLT_1TO4_ID        	1
#define CAN_VOLT_5TO8_ID        	2
#define CAN_VOLT_9TO12_ID       	3
#define CAN_TEMP_1TO4_ID            4
#define CAN_TEMP_5TO8_ID            5
#define CAN_TEMP_9TO12_ID           6
#define CAN_EQUI_REPORT_ID          7
#define CAN_INIT_REPORT_ID          8
#define CAN_FIRMWARE_REVISION_ID    9

// Command IDs from BMS master
#define CAN_EQUI_COMMAND_ID		    	1	// Emitted to slave network : equilibration command
#define CAN_REQUEST_FIRMWARE_REV_ID     2	// Emitted to slave network : request firmware revision
#define CAN_BMS_STATE					3	// Emitted to main network : BMS state message
#define CAN_VOLTAGE_MODULE_1TO4			4	// Emitted to main network : Module voltage
#define CAN_VOLTAGE_MODULE_5TO6			5	// Emitted to main network : Module voltage
#define CAN_PACK_INFO					6	// Emitted to main network : Battery pack info
#define CAN_BMS_ERROR					7	// Emitted to main network : BMS error message
#define CAN_BMS_INFO					8 	// Emitted to main network : BMS info message
#define CAN_BMS_MISC					9	// Emitted to main network : BMS misc info message
#define CAN_PING_MESSAGE				10	// Emitted to slave network : ping message

// Error code command (to DAQ)
#define CAN_DTC							1

// Command IDs from TCS
#define CAN_CURRENT_VALUE	0x01

// Command IDs from Drive
#define CAN_DRIVE_ID_Rx   		0x0C0  	// Tx ID for drive's point of view
// Useful drive's register value
#define DRIVE_BUS_VOLTAGE_REG_ID	0x0EB	// Drive's register containing DC bus voltage

// Command IDs from Charger (NLG5 documentation reference)
#define CHARGER_CTL			1 		// Charger command (NLG5_CTL)
#define CHARGER_ERROR		2		// Error and Warning message (NLG5_ERR)
#define CHARGER_STATUS		3		// Status message (NLG5_ST)
#define CHARGER_ACTUAL_1	4		// Actual values 1 (NLG5_ACT_1)
#define CHARGER_ACTUAL_2	5		// Actual values 2 (NLG5_ACT_2)
#define	CHARGER_TEMP		6		// Temperature feedback (NLG5_TEMP)

// DTC codes
#define DTC_FIRMWARE		1001
#define DTC_INTERLOCK_OPEN	1000
#define DTC_SLAVE_RESET		1002
#define DTC_SLAVE_DOUBLE_ID	1003



// Receiver bit state
#define EMITTER		0	// Node ID is the ID of the emitter node
#define RECEIVER	1	// Node ID is the ID of the destination node


/************************************************************
CAN Message ID bit definition :
	[10..6] : node ID (for BMS slave : 1XXXX where the XXXX correspond to the slave ID (slave ID != slave's node ID))
	[5] 	   : 0 = node ID is ID of the emitter, 1 = node ID is ID of the receiver (when specific receiver)
	[4..0]  : command ID
	
	ID construction macro (MAKE_CAN_ID) :
		NOTE_ID : ID of the node
 		TYPE : EMITTER or RECEIVER
 		MSG_ID : ID of the command
*************************************************************/
#define MAKE_CAN_ID(NODE_ID,ID_TYPE,CMD_ID)  ((((uint16) (NODE_ID & 0x1F))<<6) | (uint16)(ID_TYPE << 5) | (uint16) (CMD_ID & 0x1F))

// Slave ID construction macro (slave ID != node ID)
// Purpose : Gives the slave node ID from the slave ID
#define SLAVE_NODE_ID(SLAVE_ID)	(0x10 | (uint8) (SLAVE_ID & 0xF))

// CAN common fonctions
uint8 CAN_getNodeID(uint16 msgID);
uint8 CAN_getSlaveID(uint16 msgID);
uint8 CAN_getCommandID(uint16 msgID);
uint8 CAN_getReceiverBit(uint16 msgID);

// BMS Master specific CAN related functions
uint8 CAN_SendFirmwareRevisionRequest(void);	// Sends a broadcast firmware request (to slave network)
uint8 CAN_SendStatus(void);						// Sends BMS status to main network (state, error, info, ...)
uint8 CAN_SendState(void);						// Sends BMS state message to main network
uint8 CAN_SendInfo(void);						// Sends BMS info message (min temp, max temp, min voltage, max voltage, ...)
uint8 CAN_SendError(void);						// Send an BMS Error message
uint8 CAN_SendMiscInfo(void);					// Send an BMS misc info message
uint8 CAN_SendPackInfo(void);					// Sends Battery Pack Informations : Total voltage, State of Charge (SOC)
uint8 CAN_SendModuleVoltage(void);				// Sends each module voltage
uint8 CAN_SendPing(void);						// Sends a ping message to the Slave network
uint8 CAN_SendBalancingCommand(uint8 slaveID, int16 voltageThreshold, uint16 balanceVector);

//uint8 CAN_SendEquilibrationCmd(uint8 slaveID);

/*!	\brief	Process all received CAN Messages.
 *
 *			Process all received CAN messages from the specified
 *			channel.
 *
 * 	\param[in] channel Specifies the address of the channel we want to process.
 *
 * 	\return #CAN_ERROR if an error occured or #CAN_NO_ERROR if no error occured.
 */
uint8 CAN_ProcessAllMessages(struct XGCANstruct const *channel);


/*!	\brief	Sends a Data Trouble Code (DTC) on the main network.
 *
 * 	\param[in] DTCode The DTC code of the error.
 *	\param[in] length The length of bytes of data (maximum 6).
 *	\param[in] data A pointer to the data.
 *
 * 	\return #CAN_ERROR if an error occured or #CAN_NO_ERROR if no error occured.
 */
uint8 CAN_SendDTC(uint16 DTCode, uint8 length, uint8 *data);			// Sends a DTC message with possible additionnal data

/*!	\brief	Sends a request for voltage value to the drive.
 *
 *	The drive is able to return the voltage on its input. This function
 *	sends a request for that information to the drive. 
 *
 */
void CAN_RequestDriveVoltage(void);			// Request voltage from drive

uint8 CAN_sendChargingControlParameters(ChargingParameters* charging_parameters);	// Send a command to the charger



// Following functions are not intended to be used in other file than can.c : static
static void CAN_updateVoltageData(uint8 slaveID, uint8 offset, uint8 length, uint8 *data);	// Update cell's voltage array with data
static void CAN_updateTemperatureData(uint8 slaveID, uint8 offset, uint8 length, uint8 *data);	// Update cell's voltage array with data

#endif