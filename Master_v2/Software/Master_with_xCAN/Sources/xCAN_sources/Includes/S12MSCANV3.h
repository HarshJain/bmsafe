/*****************************************************************************/
/*                                             COPYRIGHT (c) FREESCALE 2005  */
/*                                                                           */
/* File Name          :    $RCSfile: S12MSCANV3.h,v $                        */
/*                                                                           */
/* Current Revision :      $Revision: 1.0 $                                  */
/*                                                                           */
/* PURPOSE: header file for S12X msCAN register block                        */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE S12X EVB AND HAS ONLY BEEN GIVEN A     *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/* DESCRIPTION:  Defines the msCAN register block as a datastructure         */
/*               of type tMSCAN.                                             */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR     DATE       DESCRIPTION OF CHANGE                          */
/* ---  ------    --------    ---------------------                          */
/* 1.0  r27624    12/11/03    initial coding based on S12MSCANV2.            */  
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

#ifndef S12MSCANV3_H    /*prevent duplicated includes */
#define S12MSCANV3_H

#ifndef S12_COMMON_H    /*prevent duplicated includes */
#include "S12_COMMON.h"
#endif

typedef union uCANCTL0
  {
  tU08   byte;
  struct
    {
    tU08 initrq :1;     /*initialization mode request */
    tU08 slprq  :1;     /*sleep mode request */
    tU08 wupe    :1;    /*wake-up enable */
    tU08 time    :1;    /*time stamp enable  */
    tU08 synch  :1;     /*synchronized status */
    tU08 cswai  :1;     /*msCAN stops in wait mode */
    tU08 rxact  :1;     /*receiver active status */
    tU08 rxfrm  :1;     /*frame recieved flag */
    }bit;
  }tCANCTL0;

#define INITRQ 0x01  /*bit masks */
#define SLPRQ   0x02
#define WUPE   0x04
#define TIME   0x08
#define SYNCH  0x10
#define CSWAI  0x20
#define RXACT  0x40
#define RXFRM  0x80

typedef union uCANCTL1
  {
  tU08   byte;
  struct
    {
    tU08 initak :1;     /*initialization mode acknowledge */
    tU08 slpak  :1;     /*sleep mode acknowledge*/
    tU08 wupm   :1;     /*wake-up mode low pass filter */
    tU08 borm   :1;     /*bus-off recovery mode */
    tU08 listen   :1;      /*listen only mode */
    tU08 loopb :1;      /*loop back self test mode */
    tU08 clksrc   :1;      /*msCAN clock source */
    tU08 cane    :1;      /*msCAN enabled */
    }bit;
  }tCANCTL1;

#define INITAK 0x01  /*bit masks */
#define SLPAK  0x02
#define WUPM   0x04
#define BORM   0x08
#define LISTEN 0x10
#define LOOPB  0x20
#define CLKSRC 0x40
#define CANE   0x80

typedef union uCANBTR0
  {
  tU08   byte;
  struct
    {
    tU08 brp   :6;      /*baud rate prescaler */
    tU08 sjw   :2;      /*synchronization jump width */
    }bit;
  }tCANBTR0;

#define BRP0   0x01  /*bit masks */
#define BRP1   0x02
#define BRP2   0x04
#define BRP3   0x08
#define BRP4   0x10
#define BRP5   0x20
#define SJW0   0x40
#define SJW1   0x80

typedef union uCANBTR1
  {
  tU08   byte;
  struct
    {
    tU08 tseg1 :4;      /*time segment bits  */
    tU08 tseg2 :3;
    tU08 samp    :1;    /*sample rate (0:1 per bit;1:3 per bit)   */
    }bit;
  }tCANBTR1;

#define TSEG10 0x01  /*bit masks */
#define TSEG11 0x02
#define TSEG12 0x04
#define TSEG13 0x08
#define TSEG20 0x10
#define TSEG21 0x20
#define TSEG22 0x40
#define SAMP     0x80

typedef union uCANRFLG
  {
  tU08   byte;
  struct
    {
    tU08 rxf     :1;    /*receive buffer full flag */
    tU08 ovrif :1;      /*overrun flag */
    tU08 tstat :2;      /*transmitter status bits */
    tU08 rstat :2;      /*receiver status bits */
    tU08 cscif :1;      /*status change interrupt flag   */
    tU08 wupif :1;      /*wake-up flag */
    }bit;
  }tCANRFLG;

#define RXF    0x01  /*bit masks */
#define OVRIF  0x02
#define TSTAT0 0x04
#define TSTAT1 0x08
#define RSTAT0 0x10
#define RSTAT1 0x20
#define CSCIF  0x40
#define WUPIF  0x80

typedef union uCANRIER
  {
  tU08   byte;
  struct
    {
    tU08 rxfie  :1;     /*receiver full interrupt enable */
    tU08 ovrie  :1;     /*overrun interrupt enable */
    tU08 tstate :2;     /*transmitter status change enable */
    tU08 rstate :2;     /*receiver status change enable */
    tU08 cscie  :1;     /*status change interrupt enable */
    tU08 wupie  :1;     /*wake-up interrupt enable */
    }bit;
  }tCANRIER;

#define RXFIE    0x01   /*bit masks */
#define OVRIE    0x02
#define TSTATE0  0x04
#define TSTATE1  0x08
#define RSTATE0  0x10
#define RSTATE1  0x20
#define CSCIE    0x40
#define WUPIE    0x80

typedef union uCANTFLG
  {
  tU08   byte;
  struct
    {
    tU08 txe   :3;      /*transmitter buffer 0 empty flags  */
    tU08       :5;      /*not used */
    }bit;
  }tCANTFLG;

#define TXE0   0x01  /*bit masks */
#define TXE1   0x02
#define TXE2   0x04

typedef union uCANTIER
  {
  tU08   byte;
  struct
    {
    tU08 txeie  :3;     /*transmitter empty interrupt enable */
    tU08        :5;     /*not used */
    }bit;
  }tCANTIER;

#define TXEIE0 0x01  /*bit masks */
#define TXEIE1 0x02
#define TXEIE2 0x04


typedef union uCANTARQ
  {
  tU08   byte;
  struct
    {
    tU08 abtrq  :3;     /*transmitter abort request */
    tU08        :5;     /*not used */
    }bit;
  }tCANTARQ;

#define ABTRQ0 0x01
#define ABTRQ1 0x02
#define ABTRQ2 0x04

typedef union uCANTAAK
  {
  tU08   byte;
  struct
    {
    tU08 abtak  :3;     /*transmitter abort acknowledge  */
    tU08        :5;     /*not used */
    }bit;
  }tCANTAAK;

#define ABTAK0 0x01
#define ABTAK1 0x02
#define ABTAK2 0x04

typedef union uCANTBSEL
  {
  tU08   byte;
  struct
    {
    tU08 tx    :3;      /*transmitter buffer select   */
    tU08       :5;      /*not used */
    }bit;
  }tCANTBSEL;

#define TX0    0x01
#define TX1    0x02
#define TX2    0x04

typedef union uCANIDAC
  {
  tU08   byte;
  struct
    {
    tU08 idhit  :3;     /*identifier acceptance hit indicator */
    tU08        :1;     /*not used */
    tU08 idam0  :1;     /*identifier acceptance mode */
    tU08 idam1  :1;     /*identifier acceptance mode */
    tU08        :2;     /*not used */
    }bit;
  }tCANIDAC;

#define IDHIT0 0x01  /*bit masks */
#define IDHIT1 0x02
#define IDHIT2 0x04
#define IDAM0  0x10  
#define IDAM1  0x20

typedef union uCANMISC
  {
  tU08   byte;
  struct
    {
    tU08 bohold   :1;      /*bus-off hold bit */
    tU08          :7;      /*not used */
    }bit;
  }tCANMISC;

#define BOHOLD 0x01  /*bit masks */

typedef union uCANDLR
  {
  tU08   byte;
  struct
    {
    tU08 dlc   :4;      /*data length code */
    tU08 rsv   :4;      /* not used */
    }bit;
  }tCANDLR;

typedef union uCANID
  {
  tU32   l;
  tU16   w[2];
  tU08   b[4];
  }tCANID;

typedef struct
  {
  tCANID canidar;
  tCANID canidmr;
  }tCID;

typedef struct
  {
  tCANID id;                  /*message id */
  tU08      dsr[8];           /*8 x data bytes */
  tCANDLR   dlr;              /*data length value */
  tU08      rsv;              /*not used */
  tU16      tsr;              /*time stamp register */
  }tRXBUF;

typedef struct
  {
  tCANID id;                  /*message id */
  tU08      dsr[8];           /*8 x data bytes */
  tCANDLR   dlr;              /*data length value */
  tU08      tbpr;             /*transmit buffer priority */
  tU16      tsr;              /*time stamp register */
  }tTXBUF;

typedef struct                
  {
  volatile tCANCTL0   canctl0;      /*control register 0 */
  volatile tCANCTL1   canctl1;      /*control register 1 */
  volatile tCANBTR0   canbtr0;      /*bus timing register 0 */
  volatile tCANBTR1   canbtr1;      /*bus timing register 1 */
  volatile tCANRFLG   canrflg;      /*receiver flag register */
  volatile tCANRIER   canrier;      /*receiver interrupt enable register */
  volatile tCANTFLG   cantflg;      /*transmitter flag register   */
  volatile tCANTIER   cantier;      /*transmitter interrupt enable register */
  volatile tCANTARQ   cantarq;      /*transmitter message abort control register */
  volatile tCANTAAK   cantaak;      /*transmitter message abort flag register */
  volatile tCANTBSEL  cantbsel;     /*transmitter buffer select */
  volatile tCANIDAC   canidac;      /*identifier acceptance control register */
           tU08       rsv0c;        /*reserved */
  volatile tCANMISC   canmisc;      /*miscellaneous bits register */      
  volatile tREG08     canrxerr;     /*receive error counter */
  volatile tREG08     cantxerr;     /*transmit error counter */
  volatile tCID         canid[2];   /*ID UNION - identifier acceptance/mask registers */
  volatile tRXBUF     rxbuf;        /*BUFFER STRUCTURE - foreground receive buffer */
  volatile tTXBUF     txbuf;        /*BUFFER STRUCTURE - foreground transmit buffer */
  }tMSCAN;

#endif /*S12MSCANV3_H */