/*****************************************************************************/
/*                                             COPYRIGHT (c) FREESCALE 2005  */
/*                                                                           */
/* File Name          :    $RCSfile: S12XINTV1.h,v $                         */
/*                                                                           */
/* Current Revision :      $Revision: 1.0 $                                  */
/*                                                                           */
/* PURPOSE: header file for S12X Interrupt register block                    */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE S12X EVB AND HAS ONLY BEEN GIVEN A     *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/* DESCRIPTION:  Defines the s12x Interrupt register block as a              */
/*               datastructure of type tINTERRUPT.                           */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR    DATE        DESCRIPTION OF CHANGE                          */
/* ---  ------    --------    ---------------------                          */
/* 1.0  r32151    24/02/05    Initial coding based on s12x_interrupt.h       */
/*                                                                           */
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

#ifndef S12XINTV1_H        /*prevent duplicated includes */
#define S12XINTV1_H

#ifndef S12_COMMON_H       /*prevent duplicated includes */
#include "S12_COMMON.h"
#endif


typedef union uINT_CFDATA
  {
  tU08   byte;
  struct
    {
    tU08 priolvl :3;     /* priority level */
    tU08 rsv     :4;
    tU08 rqst    :1;     /* XGATE request */
    }bit;
  }tINT_CFDATA;

#define PRIOLVL0  0  
#define PRIOLVL1  1
#define PRIOLVL2  2
#define PRIOLVL3  3
#define PRIOLVL4  4
#define PRIOLVL5  5
#define PRIOLVL6  6
#define PRIOLVL7  7
#define RQST      0x80  /*bit mask  */


typedef struct             /* Interrupt module used on the S12XDP256 */
  {
           tU08        rsv0;          /* reserved (maintaining memory map)      */
  volatile tU08        ivbr;          /* Interrupt Vector Base Register         */
           tU08        rsv2;          /* reserved (maintaining memory map)      */
           tU08        rsv3;          /* reserved (maintaining memory map)      */
           tU08        rsv4;          /* reserved (maintaining memory map)      */
           tU08        rsv5;          /* reserved (maintaining memory map)      */
  volatile tU08        int_xgprio;    /* XGATE Interrupt Priority Config Reg     */
  volatile tU08        int_cfaddr;    /* Interrupt Request Config Address Reg    */
  volatile tINT_CFDATA int_cfdata[8]; /* Interrupt Request Config Data Registers */
  }tINT;

#endif /* S12XINTV1_H   */