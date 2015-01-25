
#include <hidef.h>
#include "derivative.h"

#include "defines.h"
#include "CANSlave.h"
#include "InterruptHandlers.h"

// xCAN library specific headers
#include "per_XDx512_L15Y.h"
#include "xgCAN_drv.h"
#include "xgate_vectors.h"
#include "xgate_init.h"


// Send an inititialisation report message to the BMS master to indicate the slave has reset
uint8 CAN_SendInitReport(uint8 slaveID)
{
	uint8 result;
	uint8 msgLength = 0;
	//slaveID = gSlaveID;  // TODO : Find oug why slaveID parameter is always 0...

	
	// Build the CAN message ID
	tU16 msgId = MAKE_CAN_ID( SLAVE_NODE_ID(slaveID), 	// Node ID of the current slave (depents on slave ID)
							  EMITTER,					// Node ID is the emitter ID
							  CAN_INIT_REPORT_ID);		// Command ID

	// Write the message to mailbox
	// PARAMETERS :
	// 		Channel 0 (MSCAN0)
	// 		Transmit mailbox corresponding to the command
	// 		Data lenght = 0
	// 		No data
	WriteCANMsg(CAN_CHANNEL, TBOX_FIRMWARE_REVISION, &msgLength, NULL);
	
	// Write the message ID to mailbox
	WriteCANMsgId(CAN_CHANNEL, TBOX_FIRMWARE_REVISION, &msgId);
	
	// Queue the message to be send
	// return : 1 = error (invalid mailbox), 0 = OK
	result = SendCANMsg(CAN_CHANNEL, TBOX_FIRMWARE_REVISION);
	
	return result;
}

// ******************************************************************************
// Send the firmware revision to the BMS Master
// ******************************************************************************
uint8 CAN_SendFirmwareRevision(uint8 slaveID, uint8 revision)
{
	uint8 result;
	uint8 msgLength = 1; // Number of bytes
	//slaveID = gSlaveID;  // TODO : Find oug why slaveID parameter is always 0...

	
	// Build the CAN message ID
	tU16 msgId = MAKE_CAN_ID( SLAVE_NODE_ID(slaveID), 		// Node ID of the current slave (depents on slave ID)
							  EMITTER,						// Node ID is the emitter ID
							  CAN_FIRMWARE_REVISION_ID);	// Command ID

	// Write the message to mailbox
	// PARAMETERS :
	// 		Channel 0 (MSCAN0)
	// 		Transmit mailbox corresponding to the command
	// 		Data lenght = 2 bytes
	// 		Data : revision ID (16 bits)
	WriteCANMsg(CAN_CHANNEL, TBOX_INIT_REPORT, &msgLength, (uint8*)&revision);
	
	// Write the message ID to mailbox
	WriteCANMsgId(CAN_CHANNEL, TBOX_INIT_REPORT, &msgId);
	
	// Queue the message to be send
	// return : 1 = error (invalid mailbox), 0 = OK
	result = SendCANMsg(CAN_CHANNEL, TBOX_INIT_REPORT);
	
	return result;
}

// ******************************************************************************
// Sends the temperatures value to the BMS Master
// Three messages are needed to transfer all the values
// ******************************************************************************
uint8 CAN_SendTemperatures(uint8 slaveID, int16 *temperatures)
{
	uint8 result = 0;
	tU16 msgId;
	uint8 msgLength = 8;
	slaveID = gSlaveID;  // TODO : Find oug why slaveID parameter is always 0...
	
	// ------------- Cell 1 to 4 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_TEMP_1TO4_ID); 					// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_TEMP_1TO4, &msgLength, (uint8*)&temperatures[0]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_TEMP_1TO4, &msgId);								// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_TEMP_1TO4);											// Queue the message to be send
	
	// ------------- Cell 5 to 8 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_TEMP_5TO8_ID); 					// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_TEMP_5TO8,&msgLength, (uint8*)&temperatures[4]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_TEMP_5TO8, &msgId);								// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_TEMP_5TO8);											// Queue the message to be send
	
	// ------------- Cell 9 to 12 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_TEMP_9TO12_ID);					// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_TEMP_9TO12, &msgLength, (uint8*)&temperatures[8]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_TEMP_9TO12, &msgId);								// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_TEMP_9TO12);										// Queue the message to be send

	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
}

// ******************************************************************************
// Sends the voltages value to the BMS Master
// Three messages are needed to transfer all the values
// ******************************************************************************
uint8 CAN_SendVoltages(uint8 slaveID, int16 *voltages)
{
	uint8 result = 0;
	tU16 msgId;
	uint8 msgLength = 8;
	slaveID = gSlaveID;  // TODO : Find oug why slaveID parameter is always 0...
	
	
	// ------------- Cell 1 to 4 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_VOLT_1TO4_ID); 				// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_VOLT_1TO4, &msgLength, (uint8*)&voltages[0]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_VOLT_1TO4, &msgId);							// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_VOLT_1TO4);										// Queue the message to be send
	
	// ------------- Cell 5 to 8 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_VOLT_5TO8_ID); 				// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_VOLT_5TO8,&msgLength, (uint8*)&voltages[4]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_VOLT_5TO8, &msgId);							// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_VOLT_5TO8);										// Queue the message to be send
	
	// ------------- Cell 9 to 12 ------------------
	msgId = MAKE_CAN_ID(SLAVE_NODE_ID(slaveID), EMITTER, CAN_VOLT_9TO12_ID);				// Build the CAN message ID
	result += WriteCANMsg(CAN_CHANNEL, TBOX_VOLT_9TO12, &msgLength, (uint8*)&voltages[8]);	// Write the message to mailbox
	result += WriteCANMsgId(CAN_CHANNEL, TBOX_VOLT_9TO12, &msgId);							// Write the message ID to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_VOLT_9TO12);										// Queue the message to be send
	
	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
}

// ******************************************************************************
// Send a DTC code message to the DAQ (Main network)
// Parameters
//		DTCode : Diganostic Trouble Code
//		length : length of the data array (in bytes), maximum 6 bytes
//		data : facultative useful data related to the DTC
// ******************************************************************************
uint8 CAN_SendDTC(uint16 DTCode, uint8 length, uint8 *data)
{
	uint8 result = 0;
	uint8 msgLength, i;
	uint8 msgData[8];
	
	if(length > 6) // No more than 6 bytes of additionnal data ( bytes of data in a CAN message - 2 used for the DTC code)
		return CAN_ERROR;
	
	msgLength = 2 + length;
		
	// Big endian
	msgData[0] = (DTCode & 0xFF00) >> 8;
	msgData[1] = (DTCode & 0xFF);
	
	for(i = 0; i < length; i++)
		msgData[2+i] = data[i]; // Copying data bytes to the message data bytes
	
	result += WriteCANMsg(CAN_CHANNEL, TBOX_DTC, &msgLength, msgData);	// Write the message to mailbox
	result += SendCANMsg(CAN_CHANNEL, TBOX_DTC);						// Queue the message to be send
	
	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
}

// ******************************************************************************
// Process the CAN messages received
// ******************************************************************************
uint8 CAN_ProcessMessage(uint8 slaveID)
{
	uint16 msgID;
	uint8 msgSlaveID = 0, dataLength, box, receiverBit, commandID;
	uint8 data[8];
	slaveID = gSlaveID;  // TODO : Find oug why slaveID parameter is always 0...

	
	g_canMsgCount--;
	
	box = FindNewCANMsg(CAN_CHANNEL); // Find which mailbox has a new message
	
	if(box == NONEWCANMSG)
	{
		// No new CAN Msg
		
		return CAN_NO_ERROR; 
	}
	
	receiverBit = CAN_getReceiverBit(msgID); // receiverBit = 1 means the node ID is the receiver

	if(CAN_getNodeID(msgID) == MASTER_NID && receiverBit == EMITTER)
		timeRef_lastMasterMessage = gElapsedTime_ms;	// Update the time reference
	
	
	if(box == RBOX_OTHER) // This box that catches all unknown ID messages
	{
		// Quickly filter the message ID to make sure it is not destinated to another node
		
		ReadCANMsgId(CAN_CHANNEL, box, &msgID); 	// Get the CAN message ID
		msgSlaveID = CAN_getSlaveID(msgID); 		// Get the slave ID from the message ID
		
		if(msgSlaveID != slaveID)
		{
			// The message is not for this slave
			
			ReadCANMsg(CAN_CHANNEL, box, &dataLength, data); // Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
			return CAN_NO_ERROR;
		}
		
		if(receiverBit == RECEIVER)	// The message is destinated to this slave
		{
			// Get the command ID
			commandID = CAN_getCommandID(msgID);
			
			// Process the command
			switch(commandID) // TODO : peut-être déporter le traitement (utiliser des flags et des fonctions)
			{
				case CAN_EQUI_COMMAND_ID:
					ReadCANMsg(CAN_CHANNEL, box, &dataLength, data); // Read the CAN message and retrieve data
					
					if(dataLength != 4)		// This message should have 4 bytes length
						return CAN_ERROR;
					
					// First two bytes are the balancing vector (big endian)
					gBalanceVector = (data[0] << 8) | data[1];
					// Following two bytes are the balance threshold value (big endian)
					gBalThres = (data[2] << 8) | data[3];
					// Indicate that the equilibration status has changed
					gFlags.equiStatusChange = 1;
					break;
				
				case CAN_REQUEST_FIRMWARE_REV_ID:
					ReadCANMsg(CAN_CHANNEL, box, &dataLength, data); // Dummy read of the CAN message to clear the RxStatus (release the mailbox)
					CAN_SendFirmwareRevision(slaveID, FIRMWARE_REVISION);
					break;
				
				default:	// Unknown command ID
					return CAN_ERROR;
			}
		}
		else
		{
			// The message is from this slave, this is not normal... (except in loopback testing mode)
			// In normal mode, it means there is another slave with the same slave ID => trigger error
			
			ReadCANMsg(CAN_CHANNEL, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
			
			if(gFlags.slaveDoubleID == 0)
			{
				gFlags.slaveDoubleID = 1;	// 1 = Double slave ID
			
				#ifdef DTC_ENABLE
				CAN_SendDTC(10, 1, &slaveID);				// Send a DTC
				#endif
			}
			
			return CAN_ERROR; 							// Trigger an error
		}
		
		
	}
	else if(box == RBOX_FIRMWARE_REVISION)
	{
		// Command ID = CAN_REQUEST_FIRMWARE_REV_ID
		
		// Node ID = MASTER_NID
		// Receiver bit = RECEIVER
		
		ReadCANMsg(CAN_CHANNEL, box, &dataLength, data); // Dummy read of the CAN message to clear the RxStatus (release the mailbox)
		CAN_SendFirmwareRevision(slaveID, FIRMWARE_REVISION);
		return CAN_NO_ERROR;
	}
	
	return CAN_NO_ERROR;
	
	
}

// ******************************************************************************
// Extracts the node ID from the CAN message ID, returns the node ID
// ******************************************************************************
uint8 CAN_getNodeID(uint16 msgID)
{
	// Node ID mask is 0b11111000000 = 0x7C0
	return (uint8)((msgID & 0x7C0) >> 6);
}


// ******************************************************************************
// Extracts the slave ID from the CAN message ID
// Returns the ID of the slave or 0 if message is not a slave node ID
// ******************************************************************************
uint8 CAN_getSlaveID(uint16 msgID)
{
	// It is a slave ID if the first bit is set to 1 (mask = 0b100000000000 = 0x400)
	if((msgID & 0x400) == 0)
	{
		return 0; // The node ID is not a BMS slave node ID
	}

	//Slave ID mask is 0b01111000000 = 0x3C0
	return (uint8)((msgID & 0x3C0 )>>6);
}

// ******************************************************************************
// Extract the command ID from the CAN message ID, returns the command ID
// ******************************************************************************
uint8 CAN_getCommandID(uint16 msgID)
{
	// Command ID bit mask is 0b00000011111 = 0x1F
	return (uint8)(msgID & 0x1F);

}

// Extracts the receiver bit from the CAN message ID
uint8 CAN_getReceiverBit(uint16 msgID)
{
	// Receiver bit bitmask is 0b00000100000 = 0x20
	return (uint8)((msgID & 0x20) >> 5);
}

// Called by an interrupt when a CAN message is received
void CAN_handle_rcv_interrupt(void)
{
	g_canMsgCount++;
}
