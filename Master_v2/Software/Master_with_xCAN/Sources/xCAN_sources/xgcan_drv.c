/******************************************************************************
							Copyright (c) Freescale Semiconductor 2005
File Name		 :	$RCSfile: xgcan_drv.c,v $

Engineer		 :	$Author: ttz778 $

Location		 :	EKB

Date Created	 :	29/04/2004

Current Revision :	$Revision: 1.1 $

Notes            :  

******************************************************************************/

#include "xgCAN_drv.h"

#ifdef USE_CAN0

tU16 ID_Table_CAN0[] = {0 				
#if(BOXSIZE_CAN0 > 1)                     
                       ,CAN_SID(CAN0ID1)			 
#if(BOXSIZE_CAN0 > 2)                     
                       ,CAN_SID(CAN0ID2)
#if(BOXSIZE_CAN0 > 3)                     
                       ,CAN_SID(CAN0ID3)
#if(BOXSIZE_CAN0 > 4)                     
                       ,CAN_SID(CAN0ID4)
#if(BOXSIZE_CAN0 > 5)                     
                       ,CAN_SID(CAN0ID5)
#if(BOXSIZE_CAN0 > 6)                     
                       ,CAN_SID(CAN0ID6)
#if(BOXSIZE_CAN0 > 7)                     
                       ,CAN_SID(CAN0ID7)
#if(BOXSIZE_CAN0 > 8)                     
                       ,CAN_SID(CAN0ID8)
#if(BOXSIZE_CAN0 > 9)                     
                       ,CAN_SID(CAN0ID9)
#if(BOXSIZE_CAN0 > 10)                     
                       ,CAN_SID(CAN0ID10)
#if(BOXSIZE_CAN0 > 11)                     
                       ,CAN_SID(CAN0ID11)
#if(BOXSIZE_CAN0 > 12)                     
                       ,CAN_SID(CAN0ID12)
#if(BOXSIZE_CAN0 > 13)                     
                       ,CAN_SID(CAN0ID13)
#if(BOXSIZE_CAN0 > 14)                     
                       ,CAN_SID(CAN0ID14)
#if(BOXSIZE_CAN0 > 15)                     
                       ,CAN_SID(CAN0ID15)
#if(BOXSIZE_CAN0 > 16)                     
                       ,CAN_SID(CAN0ID16)
#if(BOXSIZE_CAN0 > 17)                     
                       ,CAN_SID(CAN0ID17)
#if(BOXSIZE_CAN0 > 18)                     
                       ,CAN_SID(CAN0ID18)
#if(BOXSIZE_CAN0 > 19)                    
                       ,CAN_SID(CAN0ID19)
#if(BOXSIZE_CAN0 > 20)                     
                       ,CAN_SID(CAN0ID20)
#if(BOXSIZE_CAN0 > 21)                     
                       ,CAN_SID(CAN0ID21)
#if(BOXSIZE_CAN0 > 22)                     
                       ,CAN_SID(CAN0ID22)
#if(BOXSIZE_CAN0 > 23)                     
                       ,CAN_SID(CAN0ID23)
#if(BOXSIZE_CAN0 > 24)                     
                       ,CAN_SID(CAN0ID24)
#if(BOXSIZE_CAN0 > 25)                     
                       ,CAN_SID(CAN0ID25)
#if(BOXSIZE_CAN0 > 26)                     
                       ,CAN_SID(CAN0ID26)
#if(BOXSIZE_CAN0 > 27)                     
                       ,CAN_SID(CAN0ID27)
#if(BOXSIZE_CAN0 > 28)                     
                       ,CAN_SID(CAN0ID28)
#if(BOXSIZE_CAN0 > 29)                     
                       ,CAN_SID(CAN0ID29)
#if(BOXSIZE_CAN0 > 30)                     
                       ,CAN_SID(CAN0ID30)
#if(BOXSIZE_CAN0 > 31)                     
                       ,CAN_SID(CAN0ID31)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
                       };

tU08 MsgData_CAN0[BOXSIZE_CAN0][MSGLENGTH];
tU08 MsgLen_CAN0[BOXSIZE_CAN0];

tU16 RxStatus_CAN0 = 0;
tU16 TxStatus_CAN0 = 0;

const XGCANstruct ChannelCAN0 = {CAN0
                                ,ID_Table_CAN0		
                                ,MsgData_CAN0
                                ,MsgLen_CAN0
                                ,&RxStatus_CAN0	
                                ,&TxStatus_CAN0
                                ,RXBOXSIZE_CAN0};

CAN_Init_Struct CAN_Init_CAN0 = {CANCTL0_CAN0 
                                ,CANCTL1_CAN0	
                                ,CANBTR0_CAN0 
                                ,CANBTR1_CAN0
                                ,CANRIER_CAN0 
                                ,CANTIER_CAN0 
                                ,CANIDAC_CAN0
                                ,CANIDAR0_CAN0
                                ,CANIDMR0_CAN0
                                ,CANIDAR1_CAN0
                                ,CANIDMR1_CAN0};

#endif /* USE_CAN0 */

#ifdef USE_CAN1
CAN_Init_Struct CAN_Init_CAN1 = {CANCTL0_CAN1 
                                ,CANCTL1_CAN1	
                                ,CANBTR0_CAN1 
                                ,CANBTR1_CAN1
                                ,CANRIER_CAN1 
                                ,CANTIER_CAN1 
                                ,CANIDAC_CAN1
                                ,CANIDAR0_CAN1
                                ,CANIDMR0_CAN1
                                ,CANIDAR1_CAN1
                                ,CANIDMR1_CAN1};

tU16 ID_Table_CAN1[] = {0 				
#if(BOXSIZE_CAN1 > 1)                     
                       ,CAN_SID(CAN1ID1)			 
#if(BOXSIZE_CAN1 > 2)                     
                       ,CAN_SID(CAN1ID2)
#if(BOXSIZE_CAN1 > 3)                     
                       ,CAN_SID(CAN1ID3)
#if(BOXSIZE_CAN1 > 4)                     
                       ,CAN_SID(CAN1ID4)
#if(BOXSIZE_CAN1 > 5)                     
                       ,CAN_SID(CAN1ID5)
#if(BOXSIZE_CAN1 > 6)                     
                       ,CAN_SID(CAN1ID6)
#if(BOXSIZE_CAN1 > 7)                     
                       ,CAN_SID(CAN1ID7)
#if(BOXSIZE_CAN1 > 8)                     
                       ,CAN_SID(CAN1ID8)
#if(BOXSIZE_CAN1 > 9)                     
                       ,CAN_SID(CAN1ID9)
#if(BOXSIZE_CAN1 > 10)                     
                       ,CAN_SID(CAN1ID10)
#if(BOXSIZE_CAN1 > 11)                     
                       ,CAN_SID(CAN1ID11)
#if(BOXSIZE_CAN1 > 12)                     
                       ,CAN_SID(CAN1ID12)
#if(BOXSIZE_CAN1 > 13)                     
                       ,CAN_SID(CAN1ID13)
#if(BOXSIZE_CAN1 > 14)                     
                       ,CAN_SID(CAN1ID14)
#if(BOXSIZE_CAN1 > 15)                     
                       ,CAN_SID(CAN1ID15)
#if(BOXSIZE_CAN1 > 16)                     
                       ,CAN_SID(CAN1ID16)
#if(BOXSIZE_CAN1 > 17)                     
                       ,CAN_SID(CAN1ID17)
#if(BOXSIZE_CAN1 > 18)                     
                       ,CAN_SID(CAN1ID18)
#if(BOXSIZE_CAN1 > 19)                    
                       ,CAN_SID(CAN1ID19)
#if(BOXSIZE_CAN1 > 20)                     
                       ,CAN_SID(CAN1ID20)
#if(BOXSIZE_CAN1 > 21)                     
                       ,CAN_SID(CAN1ID21)
#if(BOXSIZE_CAN1 > 22)                     
                       ,CAN_SID(CAN1ID22)
#if(BOXSIZE_CAN1 > 23)                     
                       ,CAN_SID(CAN1ID23)
#if(BOXSIZE_CAN1 > 24)                     
                       ,CAN_SID(CAN1ID24)
#if(BOXSIZE_CAN1 > 25)                     
                       ,CAN_SID(CAN1ID25)
#if(BOXSIZE_CAN1 > 26)                     
                       ,CAN_SID(CAN1ID26)
#if(BOXSIZE_CAN1 > 27)                     
                       ,CAN_SID(CAN1ID27)
#if(BOXSIZE_CAN1 > 28)                     
                       ,CAN_SID(CAN1ID28)
#if(BOXSIZE_CAN1 > 29)                     
                       ,CAN_SID(CAN1ID29)
#if(BOXSIZE_CAN1 > 30)                     
                       ,CAN_SID(CAN1ID30)
#if(BOXSIZE_CAN1 > 31)                     
                       ,CAN_SID(CAN1ID31)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
                       };

tU16 RxStatus_CAN1 = 0;
tU16 TxStatus_CAN1 = 0;

tU08 MsgData_CAN1[BOXSIZE_CAN1][MSGLENGTH];
tU08 MsgLen_CAN1[BOXSIZE_CAN1];

const XGCANstruct ChannelCAN1 = {CAN1
					            ,ID_Table_CAN1		
					            ,MsgData_CAN1
					            ,MsgLen_CAN1
					            ,&RxStatus_CAN1	
					            ,&TxStatus_CAN1
					            ,RXBOXSIZE_CAN1};

#endif /* USE_CAN1 */

#ifdef USE_CAN2
CAN_Init_Struct CAN_Init_CAN2 = {CANCTL0_CAN2 
                                ,CANCTL1_CAN2	
                                ,CANBTR0_CAN2 
                                ,CANBTR1_CAN2
                                ,CANRIER_CAN2 
                                ,CANTIER_CAN2 
                                ,CANIDAC_CAN2
                                ,CANIDAR0_CAN2
                                ,CANIDMR0_CAN2
                                ,CANIDAR1_CAN2
                                ,CANIDMR1_CAN2};

tU16 ID_Table_CAN2[] = {0 				
#if(BOXSIZE_CAN2 > 1)                     
                       ,CAN_SID(CAN2ID1)			 
#if(BOXSIZE_CAN2 > 2)                     
                       ,CAN_SID(CAN2ID2)
#if(BOXSIZE_CAN2 > 3)                     
                       ,CAN_SID(CAN2ID3)
#if(BOXSIZE_CAN2 > 4)                     
                       ,CAN_SID(CAN2ID4)
#if(BOXSIZE_CAN2 > 5)                     
                       ,CAN_SID(CAN2ID5)
#if(BOXSIZE_CAN2 > 6)                     
                       ,CAN_SID(CAN2ID6)
#if(BOXSIZE_CAN2 > 7)                     
                       ,CAN_SID(CAN2ID7)
#if(BOXSIZE_CAN2 > 8)                     
                       ,CAN_SID(CAN2ID8)
#if(BOXSIZE_CAN2 > 9)                     
                       ,CAN_SID(CAN2ID9)
#if(BOXSIZE_CAN2 > 10)                     
                       ,CAN_SID(CAN2ID10)
#if(BOXSIZE_CAN2 > 11)                     
                       ,CAN_SID(CAN2ID11)
#if(BOXSIZE_CAN2 > 12)                     
                       ,CAN_SID(CAN2ID12)
#if(BOXSIZE_CAN2 > 13)                     
                       ,CAN_SID(CAN2ID13)
#if(BOXSIZE_CAN2 > 14)                     
                       ,CAN_SID(CAN2ID14)
#if(BOXSIZE_CAN2 > 15)                     
                       ,CAN_SID(CAN2ID15)
#if(BOXSIZE_CAN2 > 16)                     
                       ,CAN_SID(CAN2ID16)
#if(BOXSIZE_CAN2 > 17)                     
                       ,CAN_SID(CAN2ID17)
#if(BOXSIZE_CAN2 > 18)                     
                       ,CAN_SID(CAN2ID18)
#if(BOXSIZE_CAN2 > 19)                    
                       ,CAN_SID(CAN2ID19)
#if(BOXSIZE_CAN2 > 20)                     
                       ,CAN_SID(CAN2ID20)
#if(BOXSIZE_CAN2 > 21)                     
                       ,CAN_SID(CAN2ID21)
#if(BOXSIZE_CAN2 > 22)                     
                       ,CAN_SID(CAN2ID22)
#if(BOXSIZE_CAN2 > 23)                     
                       ,CAN_SID(CAN2ID23)
#if(BOXSIZE_CAN2 > 24)                     
                       ,CAN_SID(CAN2ID24)
#if(BOXSIZE_CAN2 > 25)                     
                       ,CAN_SID(CAN2ID25)
#if(BOXSIZE_CAN2 > 26)                     
                       ,CAN_SID(CAN2ID26)
#if(BOXSIZE_CAN2 > 27)                     
                       ,CAN_SID(CAN2ID27)
#if(BOXSIZE_CAN2 > 28)                     
                       ,CAN_SID(CAN2ID28)
#if(BOXSIZE_CAN2 > 29)                     
                       ,CAN_SID(CAN2ID29)
#if(BOXSIZE_CAN2 > 30)                     
                       ,CAN_SID(CAN2ID30)
#if(BOXSIZE_CAN2 > 31)                     
                       ,CAN_SID(CAN2ID31)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
                       };

tU16 RxStatus_CAN2 = 0;
tU16 TxStatus_CAN2 = 0;

tU08 MsgData_CAN2[BOXSIZE_CAN2][MSGLENGTH];
tU08 MsgLen_CAN2[BOXSIZE_CAN2];

const XGCANstruct ChannelCAN2 = {CAN2
					            ,ID_Table_CAN2		
					            ,MsgData_CAN2
					            ,MsgLen_CAN2
					            ,&RxStatus_CAN2	
					            ,&TxStatus_CAN2
					            ,RXBOXSIZE_CAN2};

#endif /* USE_CAN2 */

#ifdef USE_CAN3
CAN_Init_Struct CAN_Init_CAN3 = {CANCTL0_CAN3 
                                ,CANCTL1_CAN3	
                                ,CANBTR0_CAN3 
                                ,CANBTR1_CAN3
                                ,CANRIER_CAN3 
                                ,CANTIER_CAN3 
                                ,CANIDAC_CAN3
                                ,CANIDAR0_CAN3
                                ,CANIDMR0_CAN3
                                ,CANIDAR1_CAN3
                                ,CANIDMR1_CAN3};

tU16 ID_Table_CAN3[] = {0 				
#if(BOXSIZE_CAN3 > 1)                     
                       ,CAN_SID(CAN3ID1)			 
#if(BOXSIZE_CAN3 > 2)                     
                       ,CAN_SID(CAN3ID2)
#if(BOXSIZE_CAN3 > 3)                     
                       ,CAN_SID(CAN3ID3)
#if(BOXSIZE_CAN3 > 4)                     
                       ,CAN_SID(CAN3ID4)
#if(BOXSIZE_CAN3 > 5)                     
                       ,CAN_SID(CAN3ID5)
#if(BOXSIZE_CAN3 > 6)                     
                       ,CAN_SID(CAN3ID6)
#if(BOXSIZE_CAN3 > 7)                     
                       ,CAN_SID(CAN3ID7)
#if(BOXSIZE_CAN3 > 8)                     
                       ,CAN_SID(CAN3ID8)
#if(BOXSIZE_CAN3 > 9)                     
                       ,CAN_SID(CAN3ID9)
#if(BOXSIZE_CAN3 > 10)                     
                       ,CAN_SID(CAN3ID10)
#if(BOXSIZE_CAN3 > 11)                     
                       ,CAN_SID(CAN3ID11)
#if(BOXSIZE_CAN3 > 12)                     
                       ,CAN_SID(CAN3ID12)
#if(BOXSIZE_CAN3 > 13)                     
                       ,CAN_SID(CAN3ID13)
#if(BOXSIZE_CAN3 > 14)                     
                       ,CAN_SID(CAN3ID14)
#if(BOXSIZE_CAN3 > 15)                     
                       ,CAN_SID(CAN3ID15)
#if(BOXSIZE_CAN3 > 16)                     
                       ,CAN_SID(CAN3ID16)
#if(BOXSIZE_CAN3 > 17)                     
                       ,CAN_SID(CAN3ID17)
#if(BOXSIZE_CAN3 > 18)                     
                       ,CAN_SID(CAN3ID18)
#if(BOXSIZE_CAN3 > 19)                    
                       ,CAN_SID(CAN3ID19)
#if(BOXSIZE_CAN3 > 20)                     
                       ,CAN_SID(CAN3ID20)
#if(BOXSIZE_CAN3 > 21)                     
                       ,CAN_SID(CAN3ID21)
#if(BOXSIZE_CAN3 > 22)                     
                       ,CAN_SID(CAN3ID22)
#if(BOXSIZE_CAN3 > 23)                     
                       ,CAN_SID(CAN3ID23)
#if(BOXSIZE_CAN3 > 24)                     
                       ,CAN_SID(CAN3ID24)
#if(BOXSIZE_CAN3 > 25)                     
                       ,CAN_SID(CAN3ID25)
#if(BOXSIZE_CAN3 > 26)                     
                       ,CAN_SID(CAN3ID26)
#if(BOXSIZE_CAN3 > 27)                     
                       ,CAN_SID(CAN3ID27)
#if(BOXSIZE_CAN3 > 28)                     
                       ,CAN_SID(CAN3ID28)
#if(BOXSIZE_CAN3 > 29)                     
                       ,CAN_SID(CAN3ID29)
#if(BOXSIZE_CAN3 > 30)                     
                       ,CAN_SID(CAN3ID30)
#if(BOXSIZE_CAN3 > 31)                     
                       ,CAN_SID(CAN3ID31)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
                       };

tU16 RxStatus_CAN3 = 0;
tU16 TxStatus_CAN3 = 0;

tU08 MsgData_CAN3[BOXSIZE_CAN3][MSGLENGTH];
tU08 MsgLen_CAN3[BOXSIZE_CAN3];

const XGCANstruct ChannelCAN3 = {CAN3
					            ,ID_Table_CAN3		
					            ,MsgData_CAN3
					            ,MsgLen_CAN3
					            ,&RxStatus_CAN3	
					            ,&TxStatus_CAN3
					            ,RXBOXSIZE_CAN3};

#endif  /* USE_CAN3 */

#ifdef USE_CAN4
CAN_Init_Struct CAN_Init_CAN4 = {CANCTL0_CAN4 
                                ,CANCTL1_CAN4	
                                ,CANBTR0_CAN4 
                                ,CANBTR1_CAN4
                                ,CANRIER_CAN4 
                                ,CANTIER_CAN4 
                                ,CANIDAC_CAN4
                                ,CANIDAR0_CAN4
                                ,CANIDMR0_CAN4
                                ,CANIDAR1_CAN4
                                ,CANIDMR1_CAN4};

tU16 ID_Table_CAN4[] = {0 			
#if(BOXSIZE_CAN4 > 1)                     
                       ,CAN_SID(CAN4ID1)			 
#if(BOXSIZE_CAN4 > 2)                     
                       ,CAN_SID(CAN4ID2)
#if(BOXSIZE_CAN4 > 3)                     
                       ,CAN_SID(CAN4ID3)
#if(BOXSIZE_CAN4 > 4)                     
                       ,CAN_SID(CAN4ID4)
#if(BOXSIZE_CAN4 > 5)                     
                       ,CAN_SID(CAN4ID5)
#if(BOXSIZE_CAN4 > 6)                     
                       ,CAN_SID(CAN4ID6)
#if(BOXSIZE_CAN4 > 7)                     
                       ,CAN_SID(CAN4ID7)
#if(BOXSIZE_CAN4 > 8)                     
                       ,CAN_SID(CAN4ID8)
#if(BOXSIZE_CAN4 > 9)                     
                       ,CAN_SID(CAN4ID9)
#if(BOXSIZE_CAN4 > 10)                     
                       ,CAN_SID(CAN4ID10)
#if(BOXSIZE_CAN4 > 11)                     
                       ,CAN_SID(CAN4ID11)
#if(BOXSIZE_CAN4 > 12)                     
                       ,CAN_SID(CAN4ID12)
#if(BOXSIZE_CAN4 > 13)                     
                       ,CAN_SID(CAN4ID13)
#if(BOXSIZE_CAN4 > 14)                     
                       ,CAN_SID(CAN4ID14)
#if(BOXSIZE_CAN4 > 15)                     
                       ,CAN_SID(CAN4ID15)
#if(BOXSIZE_CAN4 > 16)                     
                       ,CAN_SID(CAN4ID16)
#if(BOXSIZE_CAN4 > 17)                     
                       ,CAN_SID(CAN4ID17)
#if(BOXSIZE_CAN4 > 18)                     
                       ,CAN_SID(CAN4ID18)
#if(BOXSIZE_CAN4 > 19)                    
                       ,CAN_SID(CAN4ID19)
#if(BOXSIZE_CAN4 > 20)                     
                       ,CAN_SID(CAN4ID20)
#if(BOXSIZE_CAN4 > 21)                     
                       ,CAN_SID(CAN4ID21)
#if(BOXSIZE_CAN4 > 22)                     
                       ,CAN_SID(CAN4ID22)
#if(BOXSIZE_CAN4 > 23)                     
                       ,CAN_SID(CAN4ID23)
#if(BOXSIZE_CAN4 > 24)                     
                       ,CAN_SID(CAN4ID24)
#if(BOXSIZE_CAN4 > 25)                     
                       ,CAN_SID(CAN4ID25)
#if(BOXSIZE_CAN4 > 26)                     
                       ,CAN_SID(CAN4ID26)
#if(BOXSIZE_CAN4 > 27)                     
                       ,CAN_SID(CAN4ID27)
#if(BOXSIZE_CAN4 > 28)                     
                       ,CAN_SID(CAN4ID28)
#if(BOXSIZE_CAN4 > 29)                     
                       ,CAN_SID(CAN4ID29)
#if(BOXSIZE_CAN4 > 30)                     
                       ,CAN_SID(CAN4ID30)
#if(BOXSIZE_CAN4 > 31)                     
                       ,CAN_SID(CAN4ID31)
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
                       };

tU16 RxStatus_CAN4 = 0;
tU16 TxStatus_CAN4 = 0;

tU08 MsgData_CAN4[BOXSIZE_CAN4][MSGLENGTH];
tU08 MsgLen_CAN4[BOXSIZE_CAN4];

const XGCANstruct ChannelCAN4 = {CAN4
					            ,ID_Table_CAN4		
					            ,MsgData_CAN4
					            ,MsgLen_CAN4
					            ,&RxStatus_CAN4	
					            ,&TxStatus_CAN4
					            ,RXBOXSIZE_CAN4};

#endif  /* USE_CAN4 */

/******************************************************************************
Function Name	:	InitCAN
Engineer		:	r27624	
Date			 :	11/03/2004
Parameters :  channel - pointer to structure containing MSCAN address
              CAN_Init - pointer to structure containing initialisation data
Returns		 :	None
Notes			 :  Initialises the MSCAN module with values contained in the structure 
              pointed to by CAN_Init. The initialisation values are statically 
              defined in xgCAN_ini.h. This routine must be called after reset 
              and before any CAN messages are transmitted or received.
******************************************************************************/
void 
InitCAN(const XGCANstruct *channel, CAN_Init_Struct *CAN_Init) 
{
										/* set INITRQ */
	channel->pCAN->canctl0.byte = INITRQ;
										/* wait for INITAK to set */
	while(channel->pCAN->canctl1.bit.initak != 1)
	{
	}
										/* control reg 1 */
	channel->pCAN->canctl1.byte = CAN_Init->canctl1.byte;
										/* bit timing */
	channel->pCAN->canbtr0.byte = CAN_Init->canbtr0.byte;
	channel->pCAN->canbtr1.byte = CAN_Init->canbtr1.byte;
										/* filter config */
	channel->pCAN->canidac.byte = CAN_Init->canidac.byte;
										/* filter acceptance/mask */
	channel->pCAN->canid[0] = CAN_Init->canid[0];
	channel->pCAN->canid[1] = CAN_Init->canid[1];
										/* clear INITRQ */
	channel->pCAN->canctl0.byte = 0;
										/* wait for INITAK to clear */
	while(channel->pCAN->canctl1.bit.initak != 0)
	{
	}
										/* control reg 0 */
	channel->pCAN->canctl0.byte = CAN_Init->canctl0.byte;
										/* receive interrupts */
	channel->pCAN->canrier.byte = CAN_Init->canrier.byte;
										/* transmit interrupts */
	channel->pCAN->cantier.byte = CAN_Init->cantier.byte;
										/* wait for MSCAN to synchronize */
	while(channel->pCAN->canctl0.bit.synch != 1)
	{
	}
	return;
}

/******************************************************************************
Function Name	:	SendCANMsg
Engineer		:	r27624	
Date			:	18/03/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
Returns			:	error if box specifies a receive mailbox
Notes			:	Queues a transmit mailbox for transmission. The mailbox data is 
            subsequently loaded into the next available MSCAN buffer by 
            XGATE_CAN_Transmit
******************************************************************************/
tU08 
SendCANMsg(const XGCANstruct *channel, tU08 box) 
{
                    /* check for tx mailbox */
  if(box >= (channel->RxBoxSize))
  {
  	do								
  	{									/* lock semaphore */
  		XGATE->xgsem.word = SETCANSEM;
  	} 
  	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* sub Tx box offset */
  	box -= channel->RxBoxSize;
										/* set Tx status bit */
  	*(channel->pTxStatus) |= (TXSTATUSMASK<<box);
										/* enable Tx interrupts */
  	channel->pCAN->cantier.byte = TXE2|TXE1|TXE0;
										/* unlock semaphore */
  	XGATE->xgsem.word = CLRCANSEM;
  	
  	return(CAN_ERR_OK);
  }
  else
  {
    return(CAN_ERR);
  }
}

/******************************************************************************
Function Name	:	CheckCANTxStatus
Engineer		:	r54430	
Date			:	25/03/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
Returns		:	mailbox status
Notes			:	status = 1 if the transmit mailbox is queued for transmission and 
            awaiting transfer to MSCAN transmit buffer. Status = 0 otherwise 
******************************************************************************/
tU08 
CheckCANTxStatus(const XGCANstruct *channel, tU08 box) 
{
	tU08 status;

	do								
	{									/* lock semaphore */
		XGATE->xgsem.word = SETCANSEM;
	} 
	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
											/* sub Tx box offset */
	box -= channel->RxBoxSize;
											/* read Tx status bit */
	status = *(channel->pTxStatus) & (TXSTATUSMASK<<box);
										/* unlock semaphore */
	XGATE->xgsem.word = CLRCANSEM;

	return(status);
}

/******************************************************************************
Function Name	:	CheckCANRxStatus
Engineer		:	r27624	
Date			:	29/03/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
Returns		:	 mailbox status
Notes			:	 status = 1 if the receive mailbox contains a new message which has 
             not been read by ReadCANMsg. Status = 0 if the mailbox has not 
             received any new CAN message since the mailbox was last read.
******************************************************************************/
tU08 
CheckCANRxStatus(const XGCANstruct *channel, tU08 box) 
{
	tU08 status;

	do								
	{									/* lock semaphore */
		XGATE->xgsem.word = SETCANSEM;
	} 
	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
											/* read Rx status bit */
	status = *(channel->pRxStatus) & (RXSTATUSMASK<<box);
										/* unlock semaphore */
	XGATE->xgsem.word = CLRCANSEM;

	return(status);
}

/******************************************************************************
Function Name	:	WriteCANMsg
Engineer		:	r27624	
Date			:	18/03/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
					   len : length
					   data : pointer to source data
Returns		:	 error if box specifies a receive mailbox
Notes			:	 Writes the data and length to the transmit mailbox specified 
******************************************************************************/
tU08 
WriteCANMsg(const XGCANstruct *channel, tU08 box, tU08 *len, tU08 *data) 
{
	tU08 *pSrc;
	tU08 i;
                    /* check for tx mailbox */
  if(box >= (channel->RxBoxSize))
  {
	  do								
  	{									/* lock semaphore */
  		XGATE->xgsem.word = SETCANSEM;
  	} 
  	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* copy length */
  	channel->pLength[box] = *len;
										/* source data pointer */
  	pSrc = channel->pBuffer[box];
										/* copy data */
	  for(i = 0; i < *len; i++)
  	{
	  	*pSrc++ = *data++;
  	}
										/* unlock semaphore */
	  XGATE->xgsem.word = CLRCANSEM;
	  
	  return(CAN_ERR_OK);
  }
  else
  {
    return(CAN_ERR);
  }
}

/******************************************************************************
Function Name	:	ReadCANMsg
Engineer		:	r27624	
Date			:	18/03/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
					   len : pointer to length
					   data : pointer to destination data
Returns		:	 none
Notes			:	 reads the data and length from the specified receive or transmit 
             mailbox. Clears the corresponding RxStatus bit for receive mailboxes.
******************************************************************************/
void 
ReadCANMsg(const XGCANstruct *channel, tU08 box, tU08 *len, tU08 *data) 
{
	tU08 i;

	do								
	{									/* lock semaphore */
		XGATE->xgsem.word = SETCANSEM;
	} 
	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* copy length */
	*len = channel->pLength[box];
										/* copy data */
	for(i=0; i<*len; i++)
	{
		*data++ = channel->pBuffer[box][i];
	}
										/* receive mailbox? */
	if(box < (channel->RxBoxSize))
	{									/* clear RxStatus bit */
		*(channel->pRxStatus) &= ~(RXSTATUSMASK<<box);
	}
										/* unlock semaphore */
	XGATE->xgsem.word = CLRCANSEM;	
}

/******************************************************************************
Function Name	:	ReadCANMsgId
Engineer		:	r27624	
Date			:	6/04/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
					   id : pointer to identifier
Returns		:	 none
Notes			:	 reads the identifier from the specified receive or transmit 
             mailbox, shifts it to restore to 11-bit value. 
******************************************************************************/
void 
ReadCANMsgId(const XGCANstruct *channel, tU08 box, tU16 *id) 
{
	do								
	{									/* lock semaphore */
		XGATE->xgsem.word = SETCANSEM;
	} 
	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* shift identifier and write */
	*id = (channel->pID[box]) >> 5;
										/* unlock semaphore */
	XGATE->xgsem.word = CLRCANSEM;	
}

/******************************************************************************
Function Name	:	WriteCANMsgId
Engineer		:	r27624	
Date			:	6/04/2004
Parameters : channel : pointer to channel identifier structure
             box : mailbox number
					   id : pointer to identifier
Returns		:	 error if box specifies a receive mailbox
Notes			:	 writes the shifted identifier to the specified transmit mailbox 
******************************************************************************/
tU08 
WriteCANMsgId(const XGCANstruct *channel, tU08 box, tU16 *id) 
{										
                    /* check for tx mailbox */
  if(box >= (channel->RxBoxSize))
  {
    do								
	  {								/* lock semaphore */
		  XGATE->xgsem.word = SETCANSEM;
	  } 
	  while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* shift identifier and write */
	  channel->pID[box] = CAN_SID(*id);
										/* unlock semaphore */
	  XGATE->xgsem.word = CLRCANSEM;	
	  
	  return(CAN_ERR_OK);
  }
  else
  {
    return(CAN_ERR);
  }
}

/******************************************************************************
Function Name	:	FindNewCANMsg
Engineer		:	r27624	
Date			:	24/03/2004
Parameters : channel : pointer to channel identifier structure
Returns		 : mailbox number, or NONEWCANMSG if no new messages.
Notes			 : searches RxStatus_CANx starting at the highest order bit, 
             to find the first set bit indicating a new, unread, message
******************************************************************************/
tU08 
FindNewCANMsg(const XGCANstruct *channel) 
{
	tS08 box;
	tU16 rxStatus;

	do								
	{									/* lock semaphore */
		XGATE->xgsem.word = SETCANSEM;
	} 
	while(!(XGATE->xgsem.byte.xgsemr & CANSEM));	
										/* read RxStatus */
	rxStatus = *(channel->pRxStatus);
										/* unlock semaphore */
	XGATE->xgsem.word = CLRCANSEM;	
	
	for(box = (channel->RxBoxSize - 1); box >= 0; box--)
	{
										/* RxStatus bit set? */
		if((rxStatus & (RXSTATUSMASK<<box)) != 0)
		{								/* new message found */
			break;
		}
	}

	if(box >= 0)
	{									/* new message box # */
		return(box);
	}
	else
	{									/* no new messages */
		return(NONEWCANMSG);
	}
}

