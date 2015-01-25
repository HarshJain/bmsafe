/*****************************************************************************/
/*                                             COPYRIGHT (c) Freescale 2005  */
/*                                                                           */
/* File Name          :    $RCSfile: s12_common.h,v $                          */
/*                                                                           */
/* Current Revision :      $Revision: 1.1 $                                  */
/*                                                                           */
/* PURPOSE: header file for HCS12 common definitions                         */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE S12 EVB AND HAS ONLY BEEN GIVEN A      *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/*                                                                           */
/* DESCRIPTION:  common definitions for HCS12 core registers block.          */
/* The standard definitions used to describe register datastructures.        */
/* The following conventions are used -                                      */           
/*                                                                           */
/* names starting with 's' followed by a capital letter denote structs       */
/* names starting with 'u' followed by a capital letter denote unions        */
/* names starting with 't' followed by a capital letter denote typedefs      */
/*                                                                           */
/* MCU register definition is done in separate files, describing each        */
/* peripheral register block as a datastructure. Register naming follows as  */ 
/* close as possible the names used in the device specification.             */
/* Registers containing individual flags are defined as a union of the byte  */ 
/* and as a bitfield structure.                                              */
/*                                                                           */
/* Compiler issues (portability) -                                           */
/* ANSI C defines bitfields as 'int', obviously a problem when defining 8 bit*/
/* registers, most compilers have options to modify this.                    */
/* Cosmic compiler doesn't have a switch, but allows bitfields to be defined */ 
/* as chars.                                                                 */
/* ANSI C does not define bitfield ordering (LSB first in used), other       */
/* compilers may require modification to these files or use of a compiler    */
/* switch.                                                                   */
/*                                                                           */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR       DATE       DESCRIPTION OF CHANGE                        */
/* ---  ------      --------    ---------------------                        */
/* 1.00  r32151      28/04/00   -                                            */
/* 1.01  r32151      10/05/00   - fixed embedded comment on line 39          */
/*                              - added closing comment                      */
/* 1.00  r32151      26/07/01   - changed CPU id MCSDP256 to S12DP256        */
/*                              - Added disclaimer and reformated file info. */
/* 1.1   r32151      11/03/02   - Added definitions for TRUE & FALSE         */
/*                              - cleaned up file info                       */
/*                              - Modified revision numbering to match RCS   */
/* 1.1   r32151      11/03/02   - Added definitions for TRUE & FALSE         */
/* 1.2   r32151      21/01/05   - Updated to Freescale, included bit         */
/*                                definitions and macros from S12_COMMON.h   */                   
/* 1.3   r32151      28/02/05   - Removed definitions of TRUE and FALSE      */
/*                                to avoid conflict with STDTYPES.H          */ 
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

#ifndef S12_COMMON_H        /*prevent duplicated includes*/
#define S12_COMMON_H

/*Warning Disable*/
#pragma MESSAGE DISABLE C1106		//Non-Standard Bitfields

/******************************************************************************
Standard Type Definitions

These macros allow for easier porting to other compilers. If porting change
these defines to the required values for the chosen compiler.
******************************************************************************/
typedef unsigned char   tU08;   /*unsigned 8 bit definition */
typedef unsigned int    tU16;   /*unsigned 16 bit definition*/
typedef unsigned long   tU32;   /*unsigned 32 bit definition*/

typedef signed char     tS08;   /*signed 8 bit definition */
typedef signed int      tS16;   /*signed 16 bit definition*/
typedef signed long     tS32;   /*signed 32 bit definition*/

/******************************************************************************
Macros FOR legacy compatibility.
******************************************************************************/
typedef tU08   UINT8;      /* unsigned 8-bit  */
typedef tU16   UINT16;     /* unsigned 16-bit */
typedef tU32   UINT32;     /* unsigned 32-bit */
typedef tS08   INT8;       /* signed 8-bit    */
typedef tS16   INT16;      /* signed 16-bit   */
typedef tS32   INT32;      /* signed 32-bit   */

/******************************************************************************
Macros to allow generic register / bit access.
******************************************************************************/
typedef union uREG08    /*8 bit register with byte and bit access*/
  {
  tU08  byte;           /*access whole register e.g. var.byte   = 0xFF;*/
  struct                /*access bit at a time  e.g. var.bit._7 = 1;   */
    {
    tU08 _0 :1;
    tU08 _1 :1;
    tU08 _2 :1;
    tU08 _3 :1;
    tU08 _4 :1;
    tU08 _5 :1;
    tU08 _6 :1;
    tU08 _7 :1;  
    }bit;
  }tREG08;

typedef union uREG16    /*16 bit register with word and byte access*/
  {
  tU16  word;           /*access whole word    */
  struct                /*access byte at a time*/
    {
    tREG08  msb;
    tREG08  lsb;
    }byte;
  }tREG16;
  



/******************************************************************************
Common Bit masks
******************************************************************************/

#define BIT0    0x01    
#define BIT1    0x01 << 1
#define BIT2    0x01 << 2
#define BIT3    0x01 << 3
#define BIT4    0x01 << 4
#define BIT5    0x01 << 5
#define BIT6    0x01 << 6
#define BIT7    0x01 << 7
#define BIT8    0x01 << 8
#define BIT9    0x01 << 9
#define BITA    0x01 << 10
#define BITB    0x01 << 11
#define BITC    0x01 << 12
#define BITD    0x01 << 13
#define BITE    0x01 << 14
#define BITF    0x01 << 15

/******************************************************************************
State Macros 
******************************************************************************/

/* NOTE: definitions for TRUE and FALSE can be found in Metrowerks STDTYPES.H*/

#ifndef CLEAR
#define CLEAR 0u
#endif

#ifndef SET
#define SET 1u
#endif

#ifndef FAIL
#define FAIL 0u
#endif
                        
#ifndef PASS
#define PASS 1u
#endif

/******************************************************************************
Macros for setting / clearing bits
******************************************************************************/
#define BITSETM(port,bit)  (port |= bit)
                        /* bit is value */
#define BITSET(port,bit)   (port |= (UINT8)1 << bit)
                        /* bit is mask */
#define BITCLRM(port,bit)  (port &= (UINT8)~bit)
                        /* bit is value */
#define BITCLR(port,bit)   (port &= (UINT8)~(1 << bit))

#endif /*S12_COMMON_H */
