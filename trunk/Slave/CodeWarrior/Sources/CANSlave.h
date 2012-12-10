#ifndef CANSLAVE_H
#define CANSLAVE_H

#include "defines.h"
#include "type.h"

// Messages from slaves IDs
#define CAN_VOLTAGES_1TO4_ID        0
#define CAN_VOLTAGES_5TO8_ID        1
#define CAN_VOLTAGES_9TO10_ID       2
#define CAN_TEMP_1TO4_ID            3
#define CAN_TEMP_5TO8_ID            4
#define CAN_TEMP_9TO10_ID           5
#define CAN_EQUI_REPORT_ID          6
#define CAN_INIT_REPORT_ID          7
#define CAN_FIRMWARE_REVISION_ID    8
#define CAN_ERRORS_ID               9 

// Master messages IDs
#define CAN_EQUI_COMMAND_ID		        0
#define CAN_REQUEST_FIRMWARE_REV_ID     1
#define CAN_BROADCAST_ID                15

// ID construction macro
#define MAKE_CAN_ID(SLAVE_ID,MSG_ID)  (0x400 | (((uint16) (SLAVE_ID & 0xF))<<6) | (uint16) (MSG_ID & 0x3F))

// Messages internal priorities
#define CAN_INIT_STATUS_PRIORITY        0x01
#define CAN_VOLTAGES_PRIORITY           0x02
#define CAN_TEMP_PRIORITY               0x03
#define CAN_EQUI_STATUS_PRIORITY        0x04
#define CAN_FIRMWARE_REVISION_PRIORITY  0x05

// Error Flags Definition
#define CAN_NO_ERR            0x00
#define CAN_ERR_MAX_LEN       0x01
#define CAN_ERR_BUFFER_FULL   0x02
#define CAN_ERR_UNKNOWN_CMD   0x03

uint8 CAN0SendChar(uint16 id, uint8 priority, uint8 length, uint8 *txdata);
uint8 CAN0SendEquiStatus(uint16 gBalanceVector, uint16 gBalThres, uint8 slaveId);
uint8 CAN0SendVoltages(uint16 *voltages, uint8 slaveId);
uint8 CAN0SendTemp(int16 *temperatures, uint8 slaveId);
uint8 CAN0SendInitStatus(uint8 slaveId);
uint8 CAN0SendFirmwareRevision(uint8 slaveId); 

#endif