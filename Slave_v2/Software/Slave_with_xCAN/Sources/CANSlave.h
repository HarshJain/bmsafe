#ifndef CANSLAVE_H
#define CANSLAVE_H

#include "defines.h"


#define CAN_ERROR		1
#define CAN_NO_ERROR	0
#define CAN_CHANNEL		&ChannelCAN0
#define MASTER_NID		0x01		// BMS master node ID
#define DAQ_NID			0x05		// Data Acquisition System node ID

// DTC codes
#define DTC_SLAVE_DOUBLE_ID	1003

// Error code command (to DAQ)
#define CAN_DTC						1

// Commands IDs from BMS slave
#define CAN_VOLT_1TO4_ID        	1
#define CAN_VOLT_5TO8_ID        	2
#define CAN_VOLT_9TO12_ID       	3
#define CAN_TEMP_1TO4_ID            4
#define CAN_TEMP_5TO8_ID            5
#define CAN_TEMP_9TO12_ID           6
//#define CAN_EQUI_REPORT_ID          7		// Not used
#define CAN_INIT_REPORT_ID          8
#define CAN_FIRMWARE_REVISION_ID    9

// Command IDs from BMS master
#define CAN_EQUI_COMMAND_ID		     1
#define CAN_REQUEST_FIRMWARE_REV_ID     2


#define EMITTER		0	// Node ID is the ID of the emitter node
#define RECEIVER	1	// Node ID is the ID of the destination node

/************************************************************
CAN Message ID bit definition :
	[10..6] : node ID, for BMS slave, 1XXXX where the XXXX correspond to the slave ID (slave ID != slave's node ID)
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


// Functions

uint8 CAN_SendInitReport(uint8 slaveid);
uint8 CAN_SendFirmwareRevision(uint8 slaveID, uint8 revision);
uint8 CAN_SendTemperatures(uint8 slaveID, int16 *temperatures);
uint8 CAN_SendVoltages(uint8 slaveID, int16 *voltages);
uint8 CAN_SendDTC(uint16 DTCode, uint8 length, uint8 *data);			// Sends a DTC message with possible additionnal data
uint8 CAN_ProcessMessage(uint8 slaveID);

uint8 CAN_getNodeID(uint16 msgID);
uint8 CAN_getSlaveID(uint16 msgID);
uint8 CAN_getCommandID(uint16 msgID);
uint8 CAN_getReceiverBit(uint16 msgID);

void CAN_handle_rcv_interrupt(void);



// uint8 CAN_CheckStatus(uint8 mailbox);

#endif