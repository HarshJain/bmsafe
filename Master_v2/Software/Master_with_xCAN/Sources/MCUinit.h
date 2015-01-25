/*
** ###################################################################
**     This code is generated by the Device Initialization Tool.
**     It is overwritten during code generation.
**     USER MODIFICATION ARE PRESERVED ONLY INSIDE EXPLICITLY MARKED SECTIONS.
**
**     Project   : Master_with_xCAN
**     Processor : MC9S12XDP512BMPV
**     Version   : Component 02.003, Driver 01.05, CPU db: 2.87.229
**     Datasheet : MC9S12XDP512RMV2 Rev. 2.18 May 2008
**     Date/Time : 16/10/2014, 8:06 PM
**     Abstract  :
**         This module contains device initialization code 
**         for selected on-chip peripherals.
**     Contents  :
**         Function "MCU_init" initializes selected peripherals
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################
*/

#ifndef __Master_with_xCAN_H
#define __Master_with_xCAN_H 1

/* Include shared modules, which are used for whole project */


/* User declarations and definitions */
/*   Code, declarations and definitions here will be preserved during code generation */
/* End of user declarations and definitions */
#pragma CODE_SEG DEFAULT

extern void MCU_init(void);
/*
** ===================================================================
**     Method      :  MCU_init (component MC9S12XDP512_112)
**
**     Description :
**         Device initialization code for selected peripherals.
** ===================================================================
*/



/*lint -save  -e765 Disable MISRA rule (8.10) checking. */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isr_default(void);
/*
** ===================================================================
**     Interrupt handler : isr_default
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iPIT3_sci_continuous(void);
/*
** ===================================================================
**     Interrupt handler : iPIT3_sci_continuous
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iPIT2_100Hz(void);
/*
** ===================================================================
**     Interrupt handler : iPIT2_100Hz
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iPIT1_safety_check(void);
/*
** ===================================================================
**     Interrupt handler : iPIT1_safety_check
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iPIT0_1kHz(void);
/*
** ===================================================================
**     Interrupt handler : iPIT0_1kHz
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCIIsr(void);
/*
** ===================================================================
**     Interrupt handler : SCIIsr
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVpwmesdn(void);
/*
** ===================================================================
**     Interrupt handler : isrVpwmesdn
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVcan1tx(void);
/*
** ===================================================================
**     Interrupt handler : isrVcan1tx
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iCAN1RX_reception(void);
/*
** ===================================================================
**     Interrupt handler : iCAN1RX_reception
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVcan0err(void);
/*
** ===================================================================
**     Interrupt handler : isrVcan0err
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVcan0wkup(void);
/*
** ===================================================================
**     Interrupt handler : isrVcan0wkup
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVcan0tx(void);
/*
** ===================================================================
**     Interrupt handler : isrVcan0tx
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void iCANRX_reception(void);
/*
** ===================================================================
**     Interrupt handler : iCANRX_reception
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void isrVportj(void);
/*
** ===================================================================
**     Interrupt handler : isrVportj
**
**     Description :
**         User interrupt service routine. 
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#pragma CODE_SEG DEFAULT



/*lint -restore Enable MISRA rule (8.10) checking. */



/* END Master_with_xCAN */

#endif
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.02 [04.44]
**     for the Freescale HCS12X series of microcontrollers.
**
** ###################################################################
*/