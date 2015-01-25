/******************************************************************************
*                                                  COPYRIGHT (c) FREESCALE 2005   
* FILE NAME: per_XDx512_L15Y.c     REVISION 1.0                                        
*                                                                           
* PURPOSE: Declaration of peripheral variables for an MC9S12XDx512
*          Mask Set: L15Y                    
*                                                                           
*******************************************************************************
*******************************************************************************  
** THIS CODE IS ONLY INTENDED AS AN EXAMPLE FOR THE CODEWARRIOR COMPILER AND **  
**      THE HCS12X EVB AND HAS ONLY BEEN GIVEN A MIMIMUM LEVEL OF TEST.      **  
**  IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES AND NO PROMISE OF SUPPORT.   **  
*******************************************************************************  
*******************************************************************************
*                                                                           
* DESCRIPTION:  declarations of control register blocks for on-chip peripherals.        
*                                                                           
* AUTHOR: r32151         LOCATION: EKB                 LAST EDIT DATE: 25/02/05  
*                                                                           
******************************************************************************/
/******************************************************************************
* Peripheral   Peripheral UG    Version   Doc. Order #      Header File
* ----------   ---------------  -------   ---------------   ---------------
* ATD0         ATD_10B8C        V03       S12ATD10B8CV3/D   S12ATD10B8CV3.H
* ATD1         ATD_10B16C       V04       S12ATD10B16CV4/D  S12ATD10B16CV4.H
* CRG          CRG              V06       S12CRGV6/D        S12CRGV6.H
* DEBUG        S12X_DBG         V02       S12XDBGV2/D       S12XDBGV2.H
* EBI          S12X_EBI         V02       S12XEBIV2/D       S12XEBIV2.H
* EEPROM       EETSX4K          V02       S12XEETX4KV2/D    S12XEETX4KV2.H
* FLASH        FTSX512K4        V02       S12XFTX512K4V2/D  S12XFTX512K4V2.H
* IIC          IIC              V02       S12IICV2/D        S12IICV2.H
* INTERRUPT    S12X_INTERRUPT   V01       S12XINTV1/D       S12XINTV1.H
* MMC          S12X_MMC         V02       S12XMMCV2/D       S12MMCIV2.H
* MSCAN        MSCAN            V03       S12MSCANV3/D      S12MSCANV3.H
* PIM          PIM_9XDP512      V02       S12XDP512PIMV2/D  S12XDP512PIMV2.H
* PIT          PIT_24B4C        V01       S12PIT24B4CV1/D   S12PIT24B4CV1.H
* PWM          PWM_8B8C         V01       S12PWM8B8CV1/D    S12PWM8B8CV1.H
* SCI          SCI              V05       S12SCIV5/D        S12SCIV5.H
* SPI          SPI              V04       S12SPIV4/D        S12SPIV4.H
* TIMER        ECT_16B8C        V02       S12ECT16B8CV2/D   S12ECT16B8CV2.H
* VREG         VREG             V05       S12VREG_3V3V5/D   S12VREG_3V3V5.H
* XGATE        XGATE            V02       S12XGATEV2/D      S12XGATEV2.H	  
******************************************************************************/
/*===========================================================================*/
/* Freescale reserves the right to make changes without further notice to any*/
/* product herein to improve reliability, function, or design. Freescale does*/
/* not assume any  liability arising  out  of the  application or use of any */
/* product,  circuit, or software described herein;  neither  does it convey */
/* any license under its patent rights  nor the  rights of others.  Freescale*/
/* products are not designed, intended,  or authorized for use as components */
/* in  systems  intended  for  surgical  implant  into  the  body, or  other */
/* applications intended to support life, or  for any  other application  in */
/* which the failure of the Freescale product  could create a situation where*/
/* personal injury or death may occur. Should Buyer purchase or use Freescale*/
/* products for any such intended  or unauthorized  application, Buyer shall */
/* indemnify and  hold  Freescale  and its officers, employees, subsidiaries,*/
/* affiliates,  and distributors harmless against all claims costs, damages, */
/* and expenses, and reasonable  attorney  fees arising  out of, directly or */
/* indirectly,  any claim of personal injury  or death  associated with such */
/* unintended or unauthorized use, even if such claim alleges that  Freescale*/
/* was negligent regarding the  design  or manufacture of the part. Freescale*/
/* and the Freescale logo* are registered trademarks of Freescale Ltd.       */
/*****************************************************************************/

#include "per_XDx512_L15Y.h"


volatile tINT*     Interrupt  = (tINT*)0x0120;  /* Interrupt Control Registers */
//volatile tMSCAN   CAN0     @0x0140;    /* MSCAN Module 0 */
//volatile tMSCAN   CAN1     @0x0180;    /* MSCAN Module 1 */
//volatile tMSCAN   CAN2     @0x01C0;    /* MSCAN Module 2 */
//volatile tMSCAN   CAN3     @0x0200;    /* MSCAN Module 3 */
//volatile tMSCAN   CAN4     @0x0280;    /* MSCAN Module 4 */
volatile tXGATE*   XGATE    = (tXGATE*)0x0380;    /* XGATE Module */


  
  

