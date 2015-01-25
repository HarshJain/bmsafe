/*****************************************************************************/
/*                                             COPYRIGHT (c) Freescale 2005  */
/*                                                                           */
/* File Name          :    $RCSfile: S12XDBGV2.h,v $                         */
/*                                                                           */
/* Current Revision :      $Revision: 1.0 $                                  */
/*                                                                           */
/* PURPOSE: header file for S12X Debug register block                        */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE S12X EVB AND HAS ONLY BEEN GIVEN A     *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/* DESCRIPTION: Defines the Debug block as a datastructure of type tDBG.     */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR    DATE        DESCRIPTION OF CHANGE                          */
/* ---  ------    --------    ---------------------                          */
/* 1.0  r32151    26/04/04    Initial Coding.                                */
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

#ifndef S12XDBGV2_H        /*prevent duplicated includes */
#define S12XDBGV2_H

#ifndef S12_COMMON_H       /*prevent duplicated includes */
#include "S12_COMMON.h"
#endif

typedef union uDBGC1
  {
  tU08   byte;
  struct
    {
    tU08 comrv  :2;     /*comparator register visibility bits */
    tU08 dbgbrk :2;     /*breakpoint enable bits */
    tU08 bdm    :1;     /*bdm enable */
    tU08 xgsbpe :1;     /*xgate software breakpoint enable */
    tU08 trig   :1;     /*trigger request */
    tU08 arm    :1;     /*debugger armed */
    }bit;
  }tDBGC1;

#define ARM     0x10
#define TRIG    0x20
#define XGSBPE  0x40
#define BDM     0x80

typedef union uDBGSR
  {
  tU08   byte;
  struct
    {
    tU08 ssf    :3;     /*state sequence flags */
    tU08        :3;     /*not used */
    tU08 extf   :1;     /*external tag hit flag */
    tU08 tbf    :1;     /*trace buffer full flag */
    }bit;
  }tDBGSR;

#define EXTF    0x40
#define TBF     0x80

typedef union uDBTGCR
  {
  tU08   byte;
  struct
    {
    tU08 talign   :2;      /*trigger align bits */
    tU08 trcmod   :2;      /*trace mode bits */
    tU08 trange   :2;      /*trace range bits */
    tU08 tsource  :2;      /*trace source bits */
    }bit;
  }tDBGTCR;

typedef union uDBGC2
  {
  tU08   byte;
  struct
    {
    tU08 abcm   :2;     /*A and B Comparator Match Control */
    tU08 cdcm   :2;     /*C and D Comparator Match Control */
    tU08        :4;     /*not used */
    }bit;
  }tDBGC2;
  
typedef union uDBGXCTL
  {
  tU08   byte;
  struct
    {
    tU08 compe  :1;     /*Determines if comparator is enabled */
    tU08 src    :1;     /*Determines mapping of comparator to S12X_CPU or XGATE */
    tU08 rwe    :1;     /*Read/Write enable Bit */
    tU08 rw     :1;     /*Read/Write Comparator Value Bit */
    tU08 brk    :1;     /*break */
    tU08 tag    :1;     /*Tag select */
    tU08 sz     :1;     /*Size Comparator Value Bit / Not Data Bus Compare */
    tU08 sze    :1;     /*Size Comparator Enable Bit */
    }bit;
  }tDBGXCTL;

#define COMPE   0x01
#define SRC     0x02
#define RWE     0x04
#define RW      0x08
#define BRK     0x10
#define TAG     0x20
#define SZ      0x40
#define NDB     0x40
#define SZE     0x80

typedef struct {    
  volatile tDBGC1   dbgc1;   /* Debug Control Register 1 */
  volatile tDBGSR   dbgsr;   /* Debug Status Register */
  volatile tDBGTCR  dbgtcr;  /* Debug Trace Control Register */
  volatile tDBGC2   dbgc2;   /* Debug Control Register 1 */
  volatile tREG16   dbgtb;   /* Debug Trace Buffer Register */
  volatile tREG08   dbgcnt;  /* Debug Count Register */
  volatile tREG08   dbgscrx; /* Debug Control Registers 1-3 */
  volatile tDBGXCTL dbgxctl; /* Debug Comparator Control Registers AC/BD */
  volatile tREG08   dbgxah;  /* Debug Comparator Address High */
  volatile tREG08   dbgxam;  /* Debug Comparator Address Mid */
  volatile tREG08   dbgxal;  /* Debug Comparator Address Low */
  volatile tREG08   dbgxdh;  /* Debug Comparator Data High */
  volatile tREG08   dbgxdl;  /* Debug Comparator Data Mask High */
  volatile tREG08   dbgxdhm; /* Debug Comparator Data Mask Low */
  volatile tREG08   dbgxdlm;
} tDBG;

#endif /*S12XDBGV2_H */