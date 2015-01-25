#include <hidef.h>     
#include "can.h"
#include "derivative.h"
#include "defines.h"
#include "CANDrive.h"
#include "relays.h"


// xCAN library specific headers
#include "per_XDx512_L15Y.h"
#include "xgCAN_drv.h"
#include "xgate_vectors.h"
#include "xgate_init.h"


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
// Returns the ID of the slave or 0 if message is not a slave node ID (not from a slave)
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
// ******************************************************************************
// Extracts the receiver bit from the CAN message ID
// ******************************************************************************
uint8 CAN_getReceiverBit(uint16 msgID)
{
	// Receiver bit bitmask is 0b00000100000 = 0x20
	return (uint8)((msgID & 0x20) >> 5);
}


// ******************************************************************************
//	Send a firmware revision request to the slave's.
// ******************************************************************************
uint8 CAN_SendFirmwareRevisionRequest()
{
	uint8 result = 0;
	uint8 msgLength = 0;	// number of data bytes
	
	result += WriteCANMsg(SLAVE_CHANNEL, TBOX_FIRMWARE_REQUEST, &msgLength, NULL);	// Write the message to mailbox
	result += SendCANMsg(SLAVE_CHANNEL, TBOX_FIRMWARE_REQUEST);						// Queue the message to be send
	
	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
}


// ******************************************************************************
//	Send a BMS status message
// ******************************************************************************
uint8 CAN_SendStatus(void)
{
	uint8 result = 0;
	
	result += CAN_SendState();	// Send BMS and relay state
	result += CAN_SendError();	// Send BMS error message
	result += CAN_SendInfo();	// Send BMS information message
	result += CAN_SendMiscInfo();	// Send BMS misc info message
	result += CAN_SendPackInfo();	// Send Total pack voltage

	timeRef_lastBMSstatusMessage = gTime_ms;	// Update the time reference
	
	return result;
}

uint8 CAN_SendState(void)
{
	uint8 result = 0;
	uint8 msgLength = 3;	// number of data bytes
	uint8 data[3];
	
	// General state
	// 0 = OK, 1 = Warning, 2,3 = Error
	data[0] = gFlags.errorState << 1 | (gFlags.cellLowVolt | gFlags.cellHighVolt | gFlags.cellLowTemp | gFlags.cellHighTemp);
	
	data[1] = gMode;
	
	data[2] = gRelayState;
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_BMS_STATE, &msgLength, data);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_BMS_STATE);						// Queue the message to be send
	
	return result;
}

uint8 CAN_SendInfo(void)
{
	uint8 result = 0;
	uint8 msgLength = 8;	// number of data bytes
	uint8 data[8];
	
	// Highest cell voltage value (signed, big endian)
	data[0] = ((*gHighestCellVoltage) & 0xFF00) >> 8;
	data[1] = (*gHighestCellVoltage) & 0xFF;
	
	// Lowest cell voltage value (signed, big endian)
	data[2] = ((*gLowestCellVoltage) & 0xFF00) >> 8;
	data[3] = (*gLowestCellVoltage) & 0xFF;
	
	// Highest celle temperature (signed, big endian)
	data[4] = ((*gHighestCellTemp) & 0xFF00) >> 8;
	data[5] = (*gHighestCellTemp) & 0xFF;
	
	// Lowest cell temperature (signed, big endian)
	data[6] = ((*gLowestCellTemp) & 0xFF00) >> 8;
	data[7] = (*gLowestCellTemp) & 0xFF;
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_BMS_INFO, &msgLength, data);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_BMS_INFO);						// Queue the message to be send
	
	return result;
}


uint8 CAN_SendError(void)
{
	uint8 result = 0;
	uint8 msgLength = 5;	// number of data bytes
	uint8 data[5];
	
	// General state
	// 0 = OK, 1 = Warning, 2,3 = Error
	data[0] = gFlags.errorState << 1 | (gFlags.cellLowVolt | gFlags.cellHighVolt | gFlags.cellLowTemp | gFlags.cellHighTemp);
	
	// Voltage state (Error / Warning / Voltages OK)
	// data[0] = 0|0|0|0| cellMinVolt | cellMaxVolt | cellHighVolt | cellLowVolt
	data[1] = gFlags.cellLowVolt |
						gFlags.cellHighVolt << 1 |
						gError.cellMaxVolt << 2 |
						gError.cellMinVolt << 3;
	
	// Temperature state (Error / Warning / Voltages OK)
	// data[0] = 0|0|0|0| cellMinTemp | cellMaxTemp | cellHighTemp | cellLowTemp
	data[2] = gFlags.cellLowTemp |
						gFlags.cellHighTemp << 1 |
						gError.cellMaxTemp << 2 |
						gError.cellMinTemp << 3;
	
	// Slave timeout error
	// 1 = slave timeout error, 0 = no error
	data[3] = gError.slaveTimeout;
	
	// IMD Error
	// 1 = IMD error, 0 = no error
	data[4] = gFlags.ImdError;
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_BMS_ERROR, &msgLength, data);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_BMS_ERROR);						// Queue the message to be send
	
	return result;
}

uint8 CAN_SendMiscInfo(void)
{
	uint8 result = 0;
	uint8 msgLength = 4;	// number of data bytes
	uint8 data[4];
	
	// Interlock state : 0 = interlock closed
	data[0] = !gFlags.interlockClosed;
	
	// Equilibrating : 1 = equilibrating
	data[1] = gFlags.equilibrating;
	
	// IMD value (unsigend, big endian)
	data[2] = (gImpedance & 0xFF00) >> 8;
	data[3] = gImpedance & 0xFF;
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_BMS_MISC, &msgLength, data);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_BMS_MISC);						// Queue the message to be send
	
	return result;
}

uint8 CAN_SendPackInfo(void)
{
	uint8 result = 0;
	uint8 msgLength = 4; // number of data bytes
	uint8 data[4];
	
	// Total pack voltage (unsigned, big endian)
	data[0] = (gTotalPackVoltage & 0xFF000000) >> 24;
	data[1] = (gTotalPackVoltage & 0xFF0000) >> 16;
	data[2] = (gTotalPackVoltage & 0xFF00) >> 8;
	data[3] = (gTotalPackVoltage & 0xFF);
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_PACK_INFO, &msgLength, data);		// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_PACK_INFO);						// Queue the message to be send
	
	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;	
}


// ******************************************************************************
// Process all the CAN messages received on a specific channel
// ******************************************************************************
uint8 CAN_ProcessAllMessages(struct XGCANstruct const *channel)
{
	uint16 msgID;
	uint8 msgSlaveID = 0, msgNodeID = 0, dataLength, box, receiverBit, commandID, i;
	int8 index;
	uint8 data[8];
	uint16 var;
	
	box = FindNewCANMsg(channel); // Find which mailbox has a new message
	
	while(box != NONEWCANMSG) // We intend to process all messages
	{
	
		// SLAVE_CHANNEL message processing
		if(channel == SLAVE_CHANNEL)
		{
		
			// This box that catches all unknown ID messages
			if(box == RBOX_OTHER)
			{
				// Quickly filter the message ID to make sure it is not destinated to another node
				ReadCANMsgId(channel, box, &msgID); 		// Get the CAN message ID
				msgSlaveID = CAN_getSlaveID(msgID); 		// Get the slave ID from the message ID
				msgNodeID = CAN_getNodeID(msgID);			// Get the node ID
				receiverBit = CAN_getReceiverBit(msgID);	// receiverBit = 1 means the node ID is the receiver
			
				if(msgSlaveID != 0 && receiverBit == EMITTER) // If message is from a slave (then we process it)
				{
					index = indexOf(msgSlaveID); 	// Get the slave's index (used for arrays)
					
					if(idleCount[index] > 0)
						idleCount[index] = 0;			// Slave is alive
					
					if(index == -1) 				// If no index is attribuated to this slave yet
						index = addId(msgSlaveID); 	// Create an index for this slave ID and store it
			
					commandID = CAN_getCommandID(msgID); 			// Get the command ID
					
					ReadCANMsg(channel, box, &dataLength, data); 	// Get the CAN message data and clear the mailbox flag
					
					 switch(commandID)
					{
						case CAN_VOLT_1TO4_ID:											// Cell 1 to 4 voltage data
							CAN_updateVoltageData(msgSlaveID, 0, dataLength, data);		// Update voltage data (gCellVolt array)
							break;
							
						case CAN_VOLT_5TO8_ID:											// Cell 5 to 8 voltage data
							CAN_updateVoltageData(msgSlaveID, 4, dataLength, data);		// Update voltage data (gCellVolt array)
							break;
						
						case CAN_VOLT_9TO12_ID:											// Celle 9 to 12 voltage data
							CAN_updateVoltageData(msgSlaveID, 8, dataLength, data);		// Update voltage data (gCellVolt array)
							break;
						
						case CAN_TEMP_1TO4_ID:											// Cell 1 to 4 temperature data
							CAN_updateTemperatureData(msgSlaveID, 0, dataLength, data);	// Update temperature data (gCellTemp array)
							break;
						
						case CAN_TEMP_5TO8_ID:											// Cell 5 to 8 temperature data
							CAN_updateTemperatureData(msgSlaveID, 4, dataLength, data);	// Update temperature data (gCellTemp array)
							break;
						
						case CAN_TEMP_9TO12_ID:											// Cell 9 to 12 temperature data
							CAN_updateTemperatureData(msgSlaveID, 8, dataLength, data);	// Update temperature data (gCellTemp array)
							break;
					
						case CAN_EQUI_REPORT_ID:
							// Not used
							break;
					
						case CAN_INIT_REPORT_ID:
							gSlaveReset[index]++;
							if(gSlaveReset[index] > 10)
							{
								// TODO : send warning by DTC
								// gSlaveReset[index] = 0;
							}
							break;
					
						case CAN_FIRMWARE_REVISION_ID:
							gSlaveRev[index] = data[0];		// Store the revision of this slave
							
							
						#ifdef DTC_ENABLE
							// Check if revision is lower than the last
							if(gSlaveRev[index] < gSlaveLastRevision)
							{
								CAN_SendDTC(DTC_FIRMWARE, 1, &msgSlaveID); // Send Diagnostic Trouble Code
							}
							else if(gSlaveRev[index] > gSlaveLastRevision) // Check if the revision is higher than the latest
							{
								gSlaveLastRevision = gSlaveRev[index]; // set new last revision
								
								for(i = 0; i < N_MOD; i++)	// Check which slave has a lower revision
								{
									if(gSlaveRev[i] != 0 && gSlaveRev[i] < gSlaveLastRevision)
										CAN_SendDTC(DTC_FIRMWARE, 1, &msgSlaveID); // Send Diagnostic Trouble Code
								}
							}
						#endif
						
							break;
					} 
				}
				else if(msgNodeID == MASTER_NID && receiverBit == RECEIVER) // Message is destinated to master
				{
					// Message is for the master, but we do not know what to do with it
					ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
					return CAN_ERROR;
				}
				else // This message is not for the BMS master
					ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
	
			}
			
			// Data Trouble Code Messages
			else if(box == RBOX_DTC)
			{
			#ifdef DTC_ENABLE
				ReadCANMsg(channel, box, &dataLength, data);
				
				if(dataLength >= 2)
				{
					var = (data[0] << 8) | data[1]; // var = DTC of the received message
					
					CAN_SendDTC(var, dataLength - 2, &(data[2]));	// Send the DTC on the main network
				}
			#endif
			}
			
			
			// If the message is place in an unknown mailbox
			else
			{
				// Message in another mailbox ? Not supported yet (see "xgCAN_ini.h" for mailbox configurations)
				ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
				return CAN_ERROR;
			}
		}
		

		// MAIN_CHANNEL messages processing
		else if(channel == MAIN_CHANNEL)
		{		
			// Get the node ID (NID) to see who sent the message
			ReadCANMsgId(channel, box, &msgID); 		// Get the CAN message ID
			msgNodeID = CAN_getNodeID(msgID);			// Get the node ID
			receiverBit = CAN_getReceiverBit(msgID);	// receiverBit = 1 means the node ID is the receiver
		
			if(receiverBit == EMITTER)
			{
				// Message was sent by the TCS
				if(msgNodeID == TCS_NID)
					timeRef_lastTCS_Message = getTime();
				
				// Message was sent by the Charger
				if(msgNodeID == CHARGER_NID)
					timeRef_lastChargerMessage = getTime();
					
				if(msgNodeID == DRIVE_NID)
					timeRef_lastDriveMessage = getTime();
			}
		
		
				
			// ID specific mailbox : Current flow info message
			if(box == RBOX_CURRENT)	
			{
				// Command ID = CAN_CURRENT_VALUE
				// Node ID = TCS_NID
				// Receiver bit = EMITTER
				
				ReadCANMsg(channel, box, &dataLength, data);
				
				if(dataLength < 4)
					return CAN_ERROR;
				else
				{
					// Retreive the current
					gCurrentFlow_mA = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
					
					timeRef_lastCurrentMeasurementMessage = getTime();
				}
				
			}
			
			
			// ID specific mailbox : Drive voltage info message
			else if(box == RBOX_DRIVE_VOLT)
			{
				// Command ID = CAN_DRIVE_ID_Rx
				// Node ID = MASTER_NID
				// Receiver bit = EMITTER
				
				ReadCANMsg(channel, box, &dataLength, data);				// Read message and get data
				
				if(dataLength > 0) {
					if(data[0] == 0xEB) // This message contains the drive's bus voltage information
					{
						if(dataLength > 2) {
							// Retrieve voltage information
							gDriveVoltage = (data[2] << 1) | data[1];
							timeRef_lastDriveVoltageMessage = getTime();	// Update the last receive time
							timeRef_lastDriveVoltageRequest = getTime();
						}
					}
				}
			}
			
			// ID specific mailbox : Charger status message
			else if (box == RBOX_CHARGER_STATUS)
			{
				// Command ID = CHARGER_CTL
				// Node ID = CHARGER_NID
				// Receiver bit = EMITTER
				
				ReadCANMsg(channel, box, &dataLength, data);				// Read message and get data

				// Retreive data
				charger_updateStatus(data, dataLength);
			}
			
			
			// ID specific mailbox : Charger error message
			else if (box == RBOX_CHARGER_ERROR)
			{
				// Command ID = CHARGER_ERROR
				// Node ID = CHARGER_NID
				// Receiver bit = EMITTER
				
				ReadCANMsg(channel, box, &dataLength, data);				// Read message and get data

				// Retreive data
				charger_updateErrors(data, dataLength);
			}


			// ID specific mailbox : Charger actual values 1 message
			else if (box == RBOX_CHARGER_ACTUAL_1)
			{
				// Command ID = CHARGER_ACTUAL_1
				// Node ID = CHARGER_NID
				// Receiver bit = EMITTER
				
				ReadCANMsg(channel, box, &dataLength, data);				// Read message and get data

				// Retreive data
				charger_updateAcutals(data, dataLength);
			}
			
			
			// This box that catches all unknown ID messages
			else if(box == RBOX_OTHER) 
			{
				
				if(msgNodeID == MASTER_NID && receiverBit == RECEIVER) // Message destinated to master
				{
					// Message is for the master, but we do not know what to do with it
					ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
					
					CAN_SendDTC(1004, 2, (uint8*)&msgID);
					
					return CAN_NO_ERROR;
				}
				else
					ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)

			}
			
			
			// If the message is placed in an unknown mailbox
			else
			{
				// Message in another mailbox ? Not supported yet (see xgCAN_ini.h if other mailbox exist)
				ReadCANMsg(channel, box, &dataLength, data); 	// Dummy read of the CAN message to clear the RxStatus bit (release the mailbox)
				return CAN_ERROR;
			}
		
		}
	
		box = FindNewCANMsg(channel); // Find which mailbox has a new message
	}
	
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
	#ifdef DTC_ENABLE

	uint8 result = 0;
	uint8 msgLength, i;
	uint8 msgData[8];
	
	if(length > 6) // No more than 6 bytes of additionnal data ( bytes of data in a CAN message - 2 used for the DTC code)
		return CAN_ERROR;
	
	msgLength = 2 + length;
	
	msgData[0] = DTCode & 0xFF;
	(msgData[1] = DTCode & 0xFF00) >> 8;
	
	for(i = 0; i < length; i++)
		msgData[2+i] = data[i]; // Copying data bytes to the message data bytes
	
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_DTC, &msgLength, msgData);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_DTC);						// Queue the message to be send
	
	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
	
	#else
	
		return CAN_NO_ERROR;
		
	#endif
	
}


// ******************************************************************************
// Update the cell's voltage array gCellVolt with updated data from the slave (received by CAN)
// Parameters
//		slaveID : slave ID corresponding to the slave from whom is this message
// 		offset : index offset for these voltage data (depending on which voltage message we received)
//		legth : length of the data array (in bytes)
//		data : data of the CAN message
// ******************************************************************************
void CAN_updateVoltageData(uint8 slaveID, uint8 offset, uint8 length, uint8 *data)
{
	uint8 maxIncrement = 4;		// maximum number of voltage value in a message
	uint8 i;
	int16 rcvVolt;
	int8 index;
	
	index = indexOf(slaveID);
	
	if((maxIncrement + offset) > N_CELL)
		maxIncrement = N_CELL - offset;	// Ensure not to overflow the gCellVolt array in case of less than 12 cells
		
	if(maxIncrement * 2 > length)
		maxIncrement = length / 2;	// If there is less data than there should be... (not suppose if slave's are well configured)

	for(i = 0; i < maxIncrement; i++)
	{
		rcvVolt = (data[2*i] << 8) | data[2*i+1];	// Get the voltage value (Browse "data" as a uint16 array)
		
		// Check if we have a new extreme voltages and update corresponding pointers
		if(rcvVolt < *gLowestCellVoltage)
		{
			gLowestCellVoltage = &gCellVolt[index][i+offset];
			gLowestVoltageCellSlaveId = slaveID;
			gLowestVoltageCellNum = i + offset;
		}
		else if (rcvVolt > *gHighestCellVoltage)
		{
			gHighestCellVoltage = &gCellVolt[index][i+offset];
			gHighestVoltageCellSlaveId = slaveID;
			gHighestVoltageCellNum = i + offset;
		}
		else if(gLowestCellVoltage == &gCellVolt[index][i+offset] || gHighestCellVoltage == &gCellVolt[index][i+offset])
		{
			// This voltage was a maximum or a minimum but may not be anymore since it is updated now
			updateMinMaxVoltage();
		}
		gCellVolt[index][i+offset] = rcvVolt; // Update the voltage array
	}
}

// ******************************************************************************
// Update the cell's temperature table gCellVolt with updated data received from the slave
// Parameters
//		slaveID : slave ID corresponding to the slave from whom is this message
// 		offset : index offset for these temperature data (depending on which temperature message we received)
//		legth : length of the data array (in bytes)
//		data : data of the CAN message
// ******************************************************************************
void CAN_updateTemperatureData(uint8 slaveID, uint8 offset, uint8 length, uint8 *data)
{
	uint8 maxIncrement = 4;		// maximum temperature values in a message
	uint8 i;
	uint16 rcvTemp;
	int8 index;
	uint8 ignoreTemp;
	
	index = indexOf(slaveID);
	
	if((maxIncrement + offset) > N_CELL)
		maxIncrement = N_CELL - offset;	// Ensure not to overflow the gCellTemp array in case of less than 12 cells
		
	if(maxIncrement * 2 > length)
		maxIncrement = length / 2;	// If there is less data than there should be (if slave's are not well configured for instance)

	for(i = 0; i < maxIncrement; i++)
	{
		rcvTemp = (data[2*i] << 8) | data[2*i+1]; 		// Get the temperature value (Browse "data" as a uint16 array)
		
		// Check if we have a new extreme temperature and update corresponding pointers
		if(rcvTemp < *gLowestCellTemp)
		{
			if(!ignoreThisTemperatureSensor(slaveID,index))
			{		
				gLowestCellTemp = &gCellTemp[index][i+offset];
				gLowestTempCellSlaveId = slaveID;
				gLowestTempCellNum = i + offset;
			}
		}
		else if (rcvTemp > *gHighestCellTemp)
		{
			if(!ignoreThisTemperatureSensor(slaveID,index))
			{
				gHighestCellTemp = &gCellTemp[index][i+offset];
				gHighestTempCellSlaveId = slaveID;
				gHighestTempCellNum = i + offset;
			}
		}
		else if(gLowestCellTemp == &gCellTemp[index][i+offset] || gHighestCellTemp == &gCellTemp[index][i+offset] && rcvTemp != gCellTemp[index][i+offset])
		{
			// This temperature was a maximum or a minimum but may not be anymore since it is updated now
			updateMinMaxTemperature();
		}
		gCellTemp[index][i+offset] = rcvTemp; // Update the temperature array
	}
}

void CAN_RequestDriveVoltage(void)
{
	rqstDrivePermanentTransmit(DRIVE_BUS_VOLTAGE_REG_ID, 100);	// Request the drive voltage
	timeRef_lastDriveVoltageRequest = gTime_ms;	// Update the time reference
}


//------------------------------------------------------------------------------
// Sends the control parameter to the charger.
// Must be called each 100 ms (message loss time is 300 ms for the charger)
//------------------------------------------------------------------------------
uint8 CAN_sendChargingControlParameters(ChargingParameters* charging_parameters)
{
	uint8 dataLength = 7;
	uint8 data[7]; // Message data (7 bytes)
	uint8 result = 0;
	
	// Bolean parameters
	data[0] = 	charging_parameters->can_enable << 7 |
				charging_parameters->clear_error << 6 |
				charging_parameters->ventilation << 5;
				
	// Mains current maximum
	data[1] = (charging_parameters->mains_max_current & 0xFF00) >> 8;
	data[2] = (charging_parameters->mains_max_current & 0xFF);
	
	// Output voltage
	data[3] = (charging_parameters->output_voltage & 0xFF00) >> 8;
	data[4] = (charging_parameters->output_voltage & 0xFF);
	
	// Output current
	data[5] = (charging_parameters->output_current & 0xFF00) >> 8;
	data[6] = (charging_parameters->output_current & 0xFF);
	
	// Send the message
	result += WriteCANMsg(MAIN_CHANNEL, TBOX_CHARGER, &dataLength, data);	// Write the message to mailbox
	result += SendCANMsg(MAIN_CHANNEL, TBOX_CHARGER);						// Queue the message to be send
	
	// We just want to clear the error once, so we put clear_error flag back to 0
	charging_parameters->clear_error = 0;

	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;	
}


uint8 CAN_SendPing(void)
{
	uint8 dataLength = 0;
	uint8 result = 0;

	// Send the message
	result += WriteCANMsg(SLAVE_CHANNEL, TBOX_PING, &dataLength, NULL);	// Write the message to mailbox
	result += SendCANMsg(SLAVE_CHANNEL, TBOX_PING);						// Queue the message to be send

	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;
}



uint8 CAN_SendBalancingCommand(uint8 slaveID, int16 voltageThreshold, uint16 balanceVector)
{
	uint8 dataLength = 4;
	uint8 data[4]; // Message data (4 bytes)
	uint8 result = 0;
	tU16 msgID;
	
	// balance vector (big endian)
	data[0] = (0xFF00 & balanceVector) >> 8;
	data[1] = (0xFF & balanceVector);
	
	// voltage threshold (big endian)
	data[2] = (0xFF00 & voltageThreshold) >> 8;
	data[3] = (0xFF & voltageThreshold);
	
	// Build the CAN message ID
	msgID = MAKE_CAN_ID(	SLAVE_NODE_ID(slaveID), 	// Node ID of the current slave (depents on slave ID)
							RECEIVER,					// Node ID is the emitter ID
							CAN_INIT_REPORT_ID);		// Command ID

	// Send the message
	result += WriteCANMsg(SLAVE_CHANNEL, TBOX_EQUILIBRATION_CMD, &dataLength, data);	// Write the message to mailbox
	WriteCANMsgId(SLAVE_CHANNEL, TBOX_EQUILIBRATION_CMD, &msgID);						// Write the message ID to mailbox
	result += SendCANMsg(SLAVE_CHANNEL, TBOX_EQUILIBRATION_CMD);						// Queue the message to be send

	if(result > 0)
		return CAN_ERROR;
	else
		return CAN_NO_ERROR;	
}