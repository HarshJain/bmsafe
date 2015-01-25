
#include "xgate_init.h"

/******************************************************************************
Function Name	:	XGATE_Init
Engineer		:	r27624	
Date			:	24/02/2004
Parameters		:	None
Returns			:	None
Notes			:	
******************************************************************************/
void XGATE_Init(void) 
{
#ifdef __MWERKS__
                                               /* initialise XGATE vector base */
	XGATE->xgvbr = (int)((void*__far)XGATEVectorTable)-0x00c0;	
#else	 /* Cosmic */
                                               /* initialise XGATE vector base */
	XGATE->xgvbr = (int)XGATEVectorTable-0x00c0;	
#endif
                  /* interrupt enable, enable XGATE, stop XGATE in freeze mode */
	XGATE->xgmctl = 0xFBC1; /* XGE | XGFRZ | XGIE;	 */
}

/******************************************************************************
Function Name	:	SetIntPrio
Engineer		:	r27624	
Date			:	24/02/2004
Parameters		:	None
Returns			:	None
Notes			:	channel is equal to lower byte of CPU vector address / 2
******************************************************************************/
void SetIntPrio(char channel, char prio)	
{
	Interrupt->int_cfaddr = (channel << 1) & 0xf0;
	Interrupt->int_cfdata[channel & 0x07].byte = prio;
}

/******************************************************************************
Function Name	:	GetIntPrio
Engineer		:	r54430	
Date			:	24/03/2004
Parameters		:	None
Returns			:	None
Notes			:	channel is equal to lower byte of vector address 
                    i.e. Reset is channel 0xFE, SCI0 is channel 0xD6, etc. 
******************************************************************************/
char GetIntPrio(char channel)	
{
	Interrupt->int_cfaddr = (channel << 1) & 0xf0;
	return(Interrupt->int_cfdata[channel & 0x07].byte);
}

/******************************************************************************
Function Name	:	Int_Init
Engineer		:	r27624	
Date			:	24/02/2004
Parameters		:	None
Returns			:	None
Notes			:	 
******************************************************************************/
void Int_Init(void) 
{
#ifdef USE_CAN0
  SetIntPrio(CAN0RX_CHANNEL, RQST|2);	/* assign CAN0 Receive to XGATE, priority 2 */
	SetIntPrio(CAN0TX_CHANNEL, RQST|2);	/* assign CAN0 Transmit to XGATE, priority 2 */
#endif
#ifdef USE_CAN1
	SetIntPrio(CAN1RX_CHANNEL, RQST|2);	/* assign CAN1 Receive to XGATE, priority 2 */
	SetIntPrio(CAN1TX_CHANNEL, RQST|2);	/* assign CAN1 Transmit to XGATE, priority 2 */
#endif
#ifdef USE_CAN2
	SetIntPrio(CAN2RX_CHANNEL, RQST|2);	/* assign CAN2 Receive to XGATE, priority 2 */
	SetIntPrio(CAN2TX_CHANNEL, RQST|2);	/* assign CAN2 Transmit to XGATE, priority 2 */
#endif
#ifdef USE_CAN3
	SetIntPrio(CAN3RX_CHANNEL, RQST|2);	/* assign CAN3 Receive to XGATE, priority 2 */
	SetIntPrio(CAN3TX_CHANNEL, RQST|2);	/* assign CAN3 Transmit to XGATE, priority 2 */
#endif
#ifdef USE_CAN4
	SetIntPrio(CAN4RX_CHANNEL, RQST|2);	/* assign CAN4 Receive to XGATE, priority 2 */
	SetIntPrio(CAN4TX_CHANNEL, RQST|2);	/* assign CAN4 Transmit to XGATE, priority 2 */
#endif

	_asm("CLI");												/* clear interrupt mask bit */
}
