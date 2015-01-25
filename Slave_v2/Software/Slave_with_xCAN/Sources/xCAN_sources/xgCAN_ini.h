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
#include "CANSlave.h"

#define CAN_RECEIVE_NOTIFICATION  /* to enable CAN RX interrupt for notification */

/* define CANBENCH to measure XGATE execution time */

/*****************************************************************************
 CAN0
******************************************************************************/
/* Define if this channel is used */
#define USE_CAN0
//#define USE_CAN1
//#define USE_CAN2
//#define USE_CAN3
//#define USE_CAN4						

/* Hardware initialisation */
#define CANCTL0_CAN0  0x00		    /* clocked in Wait mode, timer disabled, wake-up disabled */
#define CANCTL1_CAN0  CANE|CLKSRC	/* MSCAN enabled, clock = BUS_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN0  0x43		    /* SJW = 1,  prescaler = 4 */
#define CANBTR1_CAN0  0x3A		    /* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN0  RXFIE 	    /* receiver buffer full interrupt enable */
#define CANTIER_CAN0  0			    /* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN0  IDAM0		    /* 4 x 16-bit filters */
#define CANIDAR0_CAN0 0
#define CANIDAR1_CAN0 0
#define CANIDMR0_CAN0 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN0 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN0  3		/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN0  9       /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
// Has to match the message ID of the mailbox declared below
// (ex : CAN0ID1 is the mailbox 1 ID, mailbox 1 is the RBOX_FIRMWARE_REVISION (see lower), so the ID has to correspond to the message ID of the Firmware revision message)
#define CAN0ID1 		MAKE_CAN_ID(MASTER_NID,EMITTER,CAN_REQUEST_FIRMWARE_REV_ID)	// Firmware revision request ID (when broadcast)
#define CAN0ID2 		0x555		// Reserved
/* define TXBOXSIZE_CAN0 transmit identifiers */
// Random : transmission message IDs will always change depending on the slave ID so the following declerations are useless for the BMS slave
#define CAN0ID3			0x001
#define CAN0ID4 		0x012
#define CAN0ID5 		0x10E
#define CAN0ID6 		0x10D
#define CAN0ID7 		0x10C
#define CAN0ID8 		0x10B
#define CAN0ID9 		0x109
#define CAN0ID10		0x100
#define CAN0ID11		MAKE_CAN_ID(DAQ_NID,RECEIVER,CAN_DTC)	// Data Trouble Code (this one does not change)

/* optionally, define a handle for each mailbox as well */
// #define MAILBOX_NAME		mailbox_number
// Receive mailbox (3)
#define RBOX_OTHER  			0		// Mailbox that cathes all accepted messages with ID that is not assigned to another receiver mailbox
#define RBOX_FIRMWARE_REVISION	1		// Firmware Revision request
#define RBOX_EQUILIBRATION_CMD	2		// Equilibration command (when broadcasted)
// Transmission mailbox (8)
#define TBOX_FIRMWARE_REVISION 	4		// Firmware revision response
#define TBOX_INIT_REPORT		3 		// Initialisation report
#define TBOX_TEMP_1TO4 			5		// Temperature cell 1 to 4
#define TBOX_TEMP_5TO8 			6		// Temperature cell 5 to 8
#define TBOX_TEMP_9TO12			7		// Temperature cell 9 to 12
#define TBOX_VOLT_1TO4			8		// Voltage cell 1 to 4
#define TBOX_VOLT_5TO8 			9		// Voltage cell 5 to 8
#define TBOX_VOLT_9TO12			10		// Voltage cell 9 to 12
#define TBOX_DTC				11		// Data Trouble Code













/*****************************************************************************
 CAN1 - unused
******************************************************************************/
/* Define if this channel is used */
#undef USE_CAN1

/* Hardware initialisation */
#define CANCTL0_CAN1  0x00		    /* clocked in Wait mode, timer disabled, wake-up disabled */
//#define CANCTL1_CAN1  CANE|LOOPB	/* MSCAN enabled, clock = OSC_CLK, loop back enabled, listen only disabled, wake-up filter disabled */
#define CANCTL1_CAN1  CANE	      	/* MSCAN enabled, clock = OSC_CLK, loop back disabled, listen only disabled, wake-up filter disabled */
#define CANBTR0_CAN1  0x43		    /* SJW = 1,  prescaler = 4 */
#define CANBTR1_CAN1  0x14		    /* SAMP = 0, TSEG2 = 2, TSEG1 = 5, 8 Tq per bit */
#define CANRIER_CAN1  RXFIE 	    /* receiver buffer full interrupt disable */
#define CANTIER_CAN1  0			      /* transmit buffer empty interrupts disabled */

#define CANIDAC_CAN1  IDAM0		    /* 4 x 16-bit filters */
#define CANIDAR0_CAN1 0
#define CANIDAR1_CAN1 0
#define CANIDMR0_CAN1 0xFFFFFFFF	/* mask all bits */
#define CANIDMR1_CAN1 0xFFFFFFFF	/* mask all bits */

/* Mailbox initialisation */
#define RXBOXSIZE_CAN1  16			/* number of receive mailboxes, limited to max 16 by RxStatus size */
#define TXBOXSIZE_CAN1  4           /* number of transmit mailboxes, limited to max 16 by TxStatus size */

/* define (RXBOXSIZE_CAN0 - 1) receive identifiers, least frequent first */
#define CAN1ID1 		0x700
/* define TXBOXSIZE_CAN0 transmit identifiers */

  
/*****************************************************************************
 CAN2 - unused
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
 CAN13 - unused
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
 CAN4 - unused
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

