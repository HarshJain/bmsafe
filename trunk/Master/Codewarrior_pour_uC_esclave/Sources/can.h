#ifndef CAN_H
#define CAN_H

//#include <hidef.h>
//#include "defines.h"

//Messages from slaves IDs
#define CAN_VOLTAGES_1TO4_ID  	    0
#define CAN_VOLTAGES_5TO8_ID  	    1
#define CAN_VOLTAGES_9TO10_ID 	    2
#define CAN_TEMP_1TO4_ID      	    3
#define CAN_TEMP_5TO8_ID      	    4
#define CAN_TEMP_9TO10_ID     	    5
#define CAN_EQUI_REPORT_ID    	    6
#define CAN_INIT_REPORT_ID          7
#define CAN_FIRMWARE_REVISION_ID    8

//Master messages IDs
#define CAN_EQUI_COMMAND_ID		        0
#define CAN_REQUEST_FIRMWARE_REV_ID     1
#define CAN_BROADCAST_ID                15

//ID construction macro
#define MAKE_CAN_ID(SLAVE_ID,MSG_ID)  ((((unsigned int) (SLAVE_ID & 0xF))<<6) | (unsigned int) (MSG_ID & 0x3F))

//Messages internal priorities
#define CAN_EQUI_COMMAND_PRIORITY           0x01   
#define CAN_REQUEST_FIRMWARE_REV_PRIORITY   0x02


//Error Flags Definition 
#define CAN_NO_ERR            0x00
#define CAN_ERR_MAX_LEN       0x01
#define CAN_ERR_BUFFER_FULL   0x02
#define CAN_ERR_UNKNOWN_CMD   0x03


//************************
//  Functions prototypes
//************************ 


unsigned char CAN0SendChar(unsigned int id, unsigned char priority, unsigned char length, unsigned char *txdata);
unsigned char CAN0SendEquiCommand(unsigned int balVector, unsigned int balThres, unsigned char slaveId);
unsigned char CAN0RequestSlaveFirmware(unsigned char slaveId);
unsigned char CAN1SendChar(unsigned int id, unsigned char priority, unsigned char length, unsigned char *txdata);


#endif