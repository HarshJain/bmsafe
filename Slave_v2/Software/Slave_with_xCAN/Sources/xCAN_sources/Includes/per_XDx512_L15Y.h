/******************************************************************************
*                                             COPYRIGHT (c) FREESCALE 2005   
* FILE NAME: per_XDx512_L15Y.h                                             
*                                                                           
* PURPOSE: external declaration of on-chip peripheral variables for HCS12XDP512.                    
*          Mask Set: L15Y                    
*                                                                           
*******************************************************************************

*******************************************************************************
*                                                                           
* DESCRIPTION:  external declarations for control register blocks of on-chip
*               peripherals.
*               Include this file in programs that interact with on-chip        
*               peripherals.                                                            
*                                                                           
* AUTHOR: r32151         LOCATION: EKB                 LAST EDIT DATE: 25/02/05  
*                                                                           
******************************************************************************/
/*******************************************************************
 *	This is a modified version of the original per_XDx512_L15Y.h file from Freescale.
 *	The modification intend to make the library compatible with the Device Initialisation tool
 *	of CodeWarrior. It makes the library compatible with the auto-generated files
 *	MCUinit.c and MCUinit.h
 *
 *	Library link : http://cache.freescale.com/files/microcontrollers/doc/app_note/AN2726SW.zip
 *	Documentation link : http://cache.freescale.com/files/microcontrollers/doc/app_note/AN2726.pdf
 *	General link : http://www.freescale.com/
 ********************************************************************/
                          /*prevent duplicated includes */
#ifndef per_XDx512_L15Y_H       
#define per_XDx512_L15Y_H


#if (__MWERKS__)
  #pragma MESSAGE DISABLE C1106
#endif
          
#include "S12_COMMON.h"

#include "S12MSCANV3.h"
#include "S12XGATEV2.h"
#include "S12XINTV1.h"
#include "S12XDBGV2.h"
#include "xgate_channels_L15Y.h"

#define CAN0 (tMSCAN*)0x140
#define CAN1 (tMSCAN*)0x180
#define CAN2 (tMSCAN*)0x1C0
#define CAN3 (tMSCAN*)0x200
#define CAN4 (tMSCAN*)0x280

extern volatile tINT*     Interrupt; /* Interrupt Control Registers */
//extern volatile tMSCAN   CAN0;      /* MSCAN Module 0 */
//extern volatile tMSCAN   CAN1;      /* MSCAN Module 1 */
//extern volatile tMSCAN   CAN2;      /* MSCAN Module 2 */
//extern volatile tMSCAN   CAN3;      /* MSCAN Module 3 */
//extern volatile tMSCAN   CAN4;      /* MSCAN Module 4 */
extern volatile tXGATE*   XGATE;     /* XGATE Module */

#endif /* per_XDx512_L15Y_H */
