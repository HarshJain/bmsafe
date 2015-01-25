/******************************************************************************
		Copyright (c) Freescale Semiconductor 2005
File Name		 :	$RCSfile: xgcan_drv.h,v $

Engineer		 :	$Author: ttz778 $

Location		 :	EKB

Date Created	 :	29/03/2004

Current Revision :	$Revision: 1.2 $

Notes            :  

******************************************************************************/
#ifndef XGCAN_DRV_H
#define XGCAN_DRV_H

#ifdef __MWERKS__
#include "hidef.h"
#endif
#include "per_XDx512_L15Y.h"
#include "xgCAN_ini.h"

#define CAN_ERR_OK      0     /* no error return value */
#define CAN_ERR         1     /* error return value */
#define MSGLENGTH       8
#define NONEWCANMSG		255			/* no new message return value */
#define RXSTATUSMASK    1			/* append 'L' if *pRxStatus is changed to tU32 */
#define TXSTATUSMASK    1			/* append 'L' if *pTxStatus is changed to tU32 */

									/* semaphores */
#define CANSEMXG	0
#define CANSEM		XGSEM0
#define SETCANSEM	XGSEMM0|XGSEM0
#define CLRCANSEM	XGSEMM0	

#define CAN_SID(id)	(id << 5)	/* macro to shift std ID into register */
#define CAN_SID_MSK 	0xFFF8	/* includes RTR and IDE, should both be 0 for data frame with std id */

									/* XGATE channels */
#define CAN4TX_CHANNEL	0x48
#define CAN4RX_CHANNEL	0x49
#define CAN4ERR_CHANNEL	0x4A
#define CAN4WUP_CHANNEL	0x4B
#define CAN3TX_CHANNEL	0x4C
#define CAN3RX_CHANNEL	0x4D
#define CAN3ERR_CHANNEL	0x4E
#define CAN3WUP_CHANNEL	0x4F
#define CAN2TX_CHANNEL	0x50
#define CAN2RX_CHANNEL	0x51
#define CAN2ERR_CHANNEL	0x52
#define CAN2WUP_CHANNEL	0x53
#define CAN1TX_CHANNEL	0x54
#define CAN1RX_CHANNEL	0x55
#define CAN1ERR_CHANNEL	0x56
#define CAN1WUP_CHANNEL	0x57
#define CAN0TX_CHANNEL	0x58
#define CAN0RX_CHANNEL	0x59
#define CAN0ERR_CHANNEL	0x5A
#define CAN0WUP_CHANNEL	0x5B

typedef const struct {
  tCANCTL0	canctl0;		/* control register 0 */
  tCANCTL1	canctl1;		/* control register 1 */
  tCANBTR0	canbtr0;		/* bus timing register 0	*/
  tCANBTR1	canbtr1;		/* bus timing register 1	*/
  tCANRIER	canrier;		/* receiver interrupt enable register */
  tCANTIER	cantier;		/* transmitter interrupt enable register */
  tCANIDAC	canidac;		/* identifier acceptance control register */
  tCID		canid[2];		/* ID UNION - identifier acceptance/mask registers */
} CAN_Init_Struct;

typedef struct 
{
	tMSCAN   *pCAN;			/* pointer to MSCAN registers */
	tU16     *pID;			/* pointer to mailbox Identifiers */
	tU08     (*pBuffer)[MSGLENGTH];		/* pointer to mailbox data buffers */
	tU08     *pLength;		/* pointer to mailbox data lengths */
	tU16     *pRxStatus;	/* pointer to RxStatus */
	tU16     *pTxStatus;	/* pointer to TxStatus */
	tU08     RxBoxSize;		/* number  of receive mailboxes */
}XGCANstruct;


#ifdef USE_CAN0
#define BOXSIZE_CAN0    (RXBOXSIZE_CAN0 + TXBOXSIZE_CAN0)
extern CAN_Init_Struct CAN_Init_CAN0;
extern tU16 ID_Table_CAN0[BOXSIZE_CAN0];
extern const XGCANstruct ChannelCAN0;
extern tU16 RxStatus_CAN0;
extern tU16 TxStatus_CAN0;
extern tU08 MsgData_CAN0[BOXSIZE_CAN0][MSGLENGTH];
extern tU08 MsgLen_CAN0[BOXSIZE_CAN0];
#ifdef __MWERKS__
#pragma DATA_SEG XGATE_DATA
#endif
extern XGCANstruct XGCAN0;
#ifdef __MWERKS__
#pragma DATA_SEG DEFAULT
#endif
#endif /* USE_CAN0 */

#ifdef USE_CAN1
#define BOXSIZE_CAN1    (RXBOXSIZE_CAN1 + TXBOXSIZE_CAN1)
extern CAN_Init_Struct CAN_Init_CAN1;
extern tU16 ID_Table_CAN1[BOXSIZE_CAN1];
extern const XGCANstruct ChannelCAN1;
extern tU16 RxStatus_CAN1;
extern tU16 TxStatus_CAN1;
extern tU08 MsgData_CAN1[BOXSIZE_CAN1][MSGLENGTH];
extern tU08 MsgLen_CAN1[BOXSIZE_CAN1];
#ifdef __MWERKS__
#pragma DATA_SEG XGATE_DATA
#endif
extern XGCANstruct XGCAN1;
#ifdef __MWERKS__
#pragma DATA_SEG DEFAULT
#endif
#endif /* USE_CAN1 */

#ifdef USE_CAN2
#define BOXSIZE_CAN2    (RXBOXSIZE_CAN2 + TXBOXSIZE_CAN2)
extern CAN_Init_Struct CAN_Init_CAN2;
extern tU16 ID_Table_CAN2[BOXSIZE_CAN2];
extern const XGCANstruct ChannelCAN2;
extern tU16 RxStatus_CAN2;
extern tU16 TxStatus_CAN2;
extern tU08 MsgData_CAN2[BOXSIZE_CAN2][MSGLENGTH];
extern tU08 MsgLen_CAN2[BOXSIZE_CAN2];
#ifdef __MWERKS__
#pragma DATA_SEG XGATE_DATA
#endif
extern XGCANstruct XGCAN2;
#ifdef __MWERKS__
#pragma DATA_SEG DEFAULT
#endif
#endif /* USE_CAN2 */

#ifdef USE_CAN3
#define BOXSIZE_CAN3    (RXBOXSIZE_CAN3 + TXBOXSIZE_CAN3)
extern CAN_Init_Struct CAN_Init_CAN3;
extern tU16 ID_Table_CAN3[BOXSIZE_CAN3];
extern const XGCANstruct ChannelCAN3;
extern tU16 RxStatus_CAN3;
extern tU16 TxStatus_CAN3;
extern tU08 MsgData_CAN3[BOXSIZE_CAN3][MSGLENGTH];
extern tU08 MsgLen_CAN3[BOXSIZE_CAN3];
#ifdef __MWERKS__
#pragma DATA_SEG XGATE_DATA
#endif
extern XGCANstruct XGCAN3;
#ifdef __MWERKS__
#pragma DATA_SEG DEFAULT
#endif
#endif /* USE_CAN3 */

#ifdef USE_CAN4
#define BOXSIZE_CAN4    (RXBOXSIZE_CAN4 + TXBOXSIZE_CAN4)
extern CAN_Init_Struct CAN_Init_CAN4;
extern tU16 ID_Table_CAN4[BOXSIZE_CAN4];
extern const XGCANstruct ChannelCAN4;
extern tU16 RxStatus_CAN4;
extern tU16 TxStatus_CAN4;
extern tU08 MsgData_CAN4[BOXSIZE_CAN4][MSGLENGTH];
extern tU08 MsgLen_CAN4[BOXSIZE_CAN4];
#ifdef __MWERKS__
#pragma DATA_SEG XGATE_DATA
#endif
extern XGCANstruct XGCAN4;
#ifdef __MWERKS__
#pragma DATA_SEG DEFAULT
#endif
#endif /* USE_CAN4 */


void InitCAN(const XGCANstruct *channel, CAN_Init_Struct *CAN_Init);
tU08 SendCANMsg(const XGCANstruct *channel, tU08 box);
tU08 WriteCANMsg(const XGCANstruct *channel, tU08 box, tU08 *len, tU08 *data);
tU08 WriteCANMsgId(const XGCANstruct *channel, tU08 box, tU16 *id);
void ReadCANMsg(const XGCANstruct *channel, tU08 box, tU08 *len, tU08 *data);
void ReadCANMsgId(const XGCANstruct *channel, tU08 box, tU16 *id);
tU08 FindNewCANMsg(const XGCANstruct *channel);
tU08 CheckCANTxStatus(const XGCANstruct *channel, tU08 box);
tU08 CheckCANRxStatus(const XGCANstruct *channel, tU08 box);
#endif
