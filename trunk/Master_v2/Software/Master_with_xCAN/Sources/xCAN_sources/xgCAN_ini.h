/******************************************************************************
	Copyright (c) Freescale Semiconductor 2005
File Name		 :	$RCSfile: xgCAN_ini.h,v $

Engineer		 :	$Author: ttz778 $

Location		 :	EKB

Date Created	 :	01/04/2004

Current Revision :	$Revision: 1.3 $

Notes            :  

******************************************************************************/
#ifndef XGCAN_INI
#define XGCAN_INI

#include "per_XDx512_L15Y.h"
#include "can.h"				// needed for some defines

#define CAN_RECEIVE_NOTIFICATION  /* to enable CAN RX interrupt for notification */

/* define CANBENCH to measure XGATE execution time */

/*****************************************************************************
 CAN0 - Slave Network Channel
******************************************************************************/
/* Define if this channel is used */
#define USE_CAN0						

/* Hardware initialisation */
#define CANCTL0_CAN0  0x00		    /* clocked in Wait mode, timer disabled, wake-up disabled */
#define CANCTL1_CAN0  CANE|CLKSRC	/* MSCAN enabled, clock = BUS_clock, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN0  0x43		    /* SJW = 1,  prescaler = 4 */
#define CANBTR1_CAN0  0x3A		    /* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN0  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN0  0			    /* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN0  IDAM0		    /* 4 x 16-bit filters */
#define CANIDAR0_CAN0 0
#define CANIDAR1_CAN0 0
#define CANIDMR0_CAN0 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN0 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN0  2			  /* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN0  3	         /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN0ID1			MAKE_CAN_ID(DAQ_NID,RECEIVER,CAN_DTC)															// Data Trouble Code from slaves			
/* define TXBOXSIZE_CAN0 transmit identifiers */
#define CAN0ID2 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_REQUEST_FIRMWARE_REV_ID)		// Firmware request ID (broadcasted to slave)
#define CAN0ID3 		0x555															// Random : will change for each slave depending on slave ID								 
#define CAN0ID4			MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_PING_MESSAGE)				// Ping message

/* optionally, define a handle for each mailbox as well */
#define RBOX_OTHER				0		// Mailbox that cathes all accepted messages with ID that is not assigned to another receiver mailbox (all slave's messages)
#define RBOX_DTC				1
// Transmit mailbox handle
#define TBOX_FIRMWARE_REQUEST	2		// Firmware request sent to slaves
#define TBOX_EQUILIBRATION_CMD	3		// Equilibration commands
#define TBOX_PING				4		// Ping message

/*****************************************************************************
 CAN1 - Main Network Channel
******************************************************************************/
/* Define if this channel is used */
#define USE_CAN1							

/* Hardware initialisation */
#define CANCTL0_CAN1  0x00		    /* clocked in Wait mode, timer disabled, wake-up disabled */
#define CANCTL1_CAN1  CANE|CLKSRC      /* MSCAN enabled, clock = BUS_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN1  0x40		    /* SJW = 1,  prescaler = 4 */
#define CANBTR1_CAN1  0x3A		    /* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN1  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN1  0			      /* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN1  IDAM0		    /* 4 x 16-bit filters */
#define CANIDAR0_CAN1 0
#define CANIDAR1_CAN1 0
#define CANIDMR0_CAN1 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN1 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN1  6			/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN1  10           /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN1ID1 		MAKE_CAN_ID(TCS_NID,EMITTER,CAN_CURRENT_VALUE)					// Current read from TCS
#define CAN1ID2			MAKE_CAN_ID(DRIVE_NID,EMITTER,CAN_DRIVE_ID_Rx)					// Voltage read from drive
#define CAN1ID3			MAKE_CAN_ID(CHARGER_NID,EMITTER,CHARGER_STATUS)					// Status message from charger
#define CAN1ID4			MAKE_CAN_ID(CHARGER_NID,EMITTER,CHARGER_ERROR)					// Error message from charger
#define CAN1ID5			MAKE_CAN_ID(CHARGER_NID,EMITTER,CHARGER_ACTUAL_1)				// Actual values message from charger

/* define TXBOXSIZE_CAN0 transmit identifiers */
#define CAN1ID6 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_BMS_STATE)					// BMS status message ID
#define CAN1ID7 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_VOLTAGE_MODULE_1TO4)			// BMS module 1 to 4 voltage message ID
#define CAN1ID8 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_VOLTAGE_MODULE_5TO6)			// BMS module 5 to 6 voltage message ID
#define CAN1ID9 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_PACK_INFO)					// Total pack voltage, SOC message ID
#define CAN1ID10			MAKE_CAN_ID(DAQ_NID,RECEIVER,CAN_DTC)							// Diagnostic Trouble Code message ID
#define CAN1ID11		MAKE_CAN_ID(DRIVE_NID,RECEIVER,1)								// Drive command message ID
#define CAN1ID12		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_BMS_ERROR)					// BMS error message ID
#define CAN1ID13		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_BMS_INFO)					// BMS info message ID
#define CAN1ID14		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_BMS_MISC)					// BMS misc info message ID
#define CAN1ID15		MAKE_CAN_ID(CHARGER_NID,RECEIVER,CHARGER_CTL)				// Charger command message ID

// Receive mailbox handle
#define RBOX_OTHER				0		// Mailbox that cathes all accepted messages with ID that is not assigned to another receiver mailbox (all slave's messages)
#define RBOX_CURRENT			1		// Message containing current measurement
#define RBOX_DRIVE_VOLT			2		// Message containing drive's voltage measurement
#define RBOX_CHARGER_STATUS		3		// Message containing drive's voltage measurement
#define RBOX_CHARGER_ERROR		4		// Message containing drive's voltage measurement
#define RBOX_CHARGER_ACTUAL_1	5		// Message containing drive's voltage measurement

// Transmit mailbox handle
#define TBOX_BMS_STATE			6		// BMS status message
#define	TBOX_VOLT_MODULE_1TO4	7		// Individual module voltage
#define TBOX_VOLT_MODULE_5TO6	8		// Individual module voltage
#define TBOX_PACK_INFO			9		// Battery pack info : Total voltage, SOC
#define TBOX_DTC				10		// Diagnostic Trouble Code
#define TBOX_DRIVE				11		// Drive commands
#define TBOX_BMS_ERROR			12		// BMS error message
#define TBOX_BMS_INFO			13		// BMS info message
#define TBOX_BMS_MISC			14		// BMS misc info message
#define TBOX_CHARGER			15		// Charger commands
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
/*****************************************************************************
 CAN2
******************************************************************************/
/* Define if this channel is used */
#undef USE_CAN2							

/* Hardware initialisation */
#define CANCTL0_CAN2  0x00		/* clocked in Wait mode, timer disabled, wake-up disabled */
//#define CANCTL1_CAN2  CANE|LOOPB		/* MSCAN enabled, clock = OSC_CLK, loop back enabled, listen only disabled, wake-up filter disabled */
#define CANCTL1_CAN2  CANE		/* MSCAN enabled, clock = OSC_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN2  0x4f		/* SJW = 1,  prescaler = 16 */
#define CANBTR1_CAN2  0x14		/* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN2  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN2  0			/* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN2  IDAM0		/* 4 x 16-bit filters */
#define CANIDAR0_CAN2 0
#define CANIDAR1_CAN2 0
#define CANIDMR0_CAN2 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN2 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN2  16			/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN2  4           /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN2ID1 		0x700
/* define TXBOXSIZE_CAN0 transmit identifiers */

/*****************************************************************************
 CAN13
******************************************************************************/
/* Define if this channel is used */
#undef USE_CAN3							

/* Hardware initialisation */
#define CANCTL0_CAN3  0x00		/* clocked in Wait mode, timer disabled, wake-up disabled */
//#define CANCTL1_CAN3  CANE|LOOPB		/* MSCAN enabled, clock = OSC_CLK, loop back enabled, listen only disabled, wake-up filter disabled */
#define CANCTL1_CAN3  CANE		/* MSCAN enabled, clock = OSC_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN3  0x43		/* SJW = 1,  prescaler = 3 */
#define CANBTR1_CAN3  0x14		/* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN3  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN3  0			/* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN3  IDAM0		/* 4 x 16-bit filters */
#define CANIDAR0_CAN3 0
#define CANIDAR1_CAN3 0
#define CANIDMR0_CAN3 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN3 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN3  16			/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN3  4           /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN3ID1 		0x700
  /* define TXBOXSIZE_CAN0 transmit identifiers */


/*****************************************************************************
 CAN4
******************************************************************************/
/* Define if this channel is used */
#undef USE_CAN4							

/* Hardware initialisation */
#define CANCTL0_CAN4  0x00		/* clocked in Wait mode, timer disabled, wake-up disabled */
//#define CANCTL1_CAN4  CANE|LOOPB		/* MSCAN enabled, clock = OSC_CLK, loop back enabled, listen only disabled, wake-up filter disabled */
#define CANCTL1_CAN4  CANE		/* MSCAN enabled, clock = OSC_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN4  0x4f		/* SJW = 1,  prescaler = 16 */
#define CANBTR1_CAN4  0x14		/* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN4  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN4  0			/* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN4  IDAM0		/* 4 x 16-bit filters */
#define CANIDAR0_CAN4 0
#define CANIDAR1_CAN4 0
#define CANIDMR0_CAN4 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN4 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN4  16			/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN4  4           /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN4ID1 		0x700
  /* define TXBOXSIZE_CAN0 transmit identifiers */



#endif  /* XGCAN_INI */

