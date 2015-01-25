/*****************************************************************************/
/*                                             COPYRIGHT (c) FREESCALE 2005  */
/*                                                                           */
/* File Name          :    $RCSfile: S12XGATEV2.h,v $                        */
/*                                                                           */
/* Current Revision :      $Revision: 1.0 $                                  */
/*                                                                           */
/* PURPOSE: header file for S12X XGATE register block                        */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE S12X EVB AND HAS ONLY BEEN GIVEN A     *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/*                                                                           */
/* DESCRIPTION:  Defines the Interrupt register block as a datastructure     */
/*               of type tXGATE.                                             */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR       DATE       DESCRIPTION OF CHANGE                        */
/* ---  ------      --------    ---------------------                        */
/* 1.0  r32151      24/02/05    Initial coding based on S12XGATEV1.h         */
/* 2.0  r32151      13/04/05    Added word/byte structure access for the 128 */
/*                              interrupt flag bits to support macro         */
/*                              testing and clearing of any bit.             */
/*                              Added alternative (more intuitive) bit masks */
/*                              for the control register mask bits.          */
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

#ifndef S12XGATEV2_H      /*prevent duplicated includes */
#define S12XGATEV2_H

#ifndef S12_COMMON_H      /*prevent duplicated includes */
#include "S12_COMMON.h"
#endif


#define XGIE      0x01  /*bit masks for XGMCTL  */
#define XGSWEIF   0x02
#define XGFACT    0x08
#define XGSS      0x10
#define XGDBG     0x20
#define XGFRZ     0x40
#define XGE       0x80
#define XGIEM     0x0100   
#define XGSWEIFM  0x0200
#define XGFACTM   0x0800
#define XGSSM     0x1000
#define XGDBGM    0x2000
#define XGFRZM    0x4000
#define XGEM      0x8000

typedef union uXGIF
{
volatile tU16 word[7];
volatile tU08 byte[15];
volatile struct {
            struct {
               tU08 XGIF_78     :1;		/* Channel 120 (Hex78) corresponds to HCS12X Vector   7 at 0xFFF0 */
               tU08 XGIF_79     :1;																			 /* Channel 121 (Hex79) corresponds to HCS12X Vector   6 at 0xFFF2 */
               tU08 XGIF_7A     :1;																			 /* Channel 122 (Hex7A) corresponds to HCS12X Vector   5 at 0xFFF4 */
               tU08 XGIF_7B     :1;																			 /* Channel 123 (Hex7B) corresponds to HCS12X Vector   4 at 0xFFF6 */
               tU08 XGIF_7C     :1;																			 /* Channel 124 (Hex7C) corresponds to HCS12X Vector   3 at 0xFFF8 */
               tU08 XGIF_7D     :1;																			 /* Channel 125 (Hex7D) corresponds to HCS12X Vector   2 at 0xFFFA */
               tU08 XGIF_7E     :1;																			 /* Channel 126 (Hex7E) corresponds to HCS12X Vector   1 at 0xFFFC */
               tU08 XGIF_7F     :1;																			 /* Channel 127 (Hex7F) corresponds to HCS12X Vector   0 at 0xFFFE */
            } bits_78_7F;
            struct {
               tU08 XGIF_70     :1;																			 /* Channel 112 (Hex70) corresponds to HCS12X Vector  15 at 0xFFE0 */
               tU08 XGIF_71     :1;																			 /* Channel 113 (Hex71) corresponds to HCS12X Vector  14 at 0xFFE2 */
               tU08 XGIF_72     :1;																			 /* Channel 114 (Hex72) corresponds to HCS12X Vector  13 at 0xFFE4 */
               tU08 XGIF_73     :1;																			 /* Channel 115 (Hex73) corresponds to HCS12X Vector  12 at 0xFFE6 */
               tU08 XGIF_74     :1;																			 /* Channel 116 (Hex74) corresponds to HCS12X Vector  11 at 0xFFE8 */
               tU08 XGIF_75     :1;																			 /* Channel 117 (Hex75) corresponds to HCS12X Vector  10 at 0xFFEA */
               tU08 XGIF_76     :1;																			 /* Channel 118 (Hex76) corresponds to HCS12X Vector   9 at 0xFFEC */
               tU08 XGIF_77     :1;																			 /* Channel 119 (Hex77) corresponds to HCS12X Vector   8 at 0xFFEE */
            } bits_70_77;
            struct {
               tU08 XGIF_68     :1;																			 /* Channel 104 (Hex68) corresponds to HCS12X Vector  23 at 0xFFD0 */
               tU08 XGIF_69     :1;																			 /* Channel 105 (Hex69) corresponds to HCS12X Vector  22 at 0xFFD2 */
               tU08 XGIF_6A     :1;																			 /* Channel 106 (Hex6A) corresponds to HCS12X Vector  21 at 0xFFD4 */
               tU08 XGIF_6B     :1;																			 /* Channel 107 (Hex6B) corresponds to HCS12X Vector  20 at 0xFFD6 */
               tU08 XGIF_6C     :1;																			 /* Channel 108 (Hex6C) corresponds to HCS12X Vector  19 at 0xFFD8 */
               tU08 XGIF_6D     :1;																			 /* Channel 109 (Hex6D) corresponds to HCS12X Vector  18 at 0xFFDA */
               tU08 XGIF_6E     :1;																			 /* Channel 110 (Hex6E) corresponds to HCS12X Vector  17 at 0xFFDC */
               tU08 XGIF_6F     :1;																			 /* Channel 111 (Hex6F) corresponds to HCS12X Vector  16 at 0xFFDE */
            } bits_68_6F;
            struct {
               tU08 XGIF_60     :1;																			 /* Channel  96 (Hex60) corresponds to HCS12X Vector  31 at 0xFFC0 */
               tU08 XGIF_61     :1;																			 /* Channel  97 (Hex61) corresponds to HCS12X Vector  30 at 0xFFC2 */
               tU08 XGIF_62     :1;																			 /* Channel  98 (Hex62) corresponds to HCS12X Vector  29 at 0xFFC4 */
               tU08 XGIF_63     :1;																			 /* Channel  99 (Hex63) corresponds to HCS12X Vector  28 at 0xFFC6 */
               tU08 XGIF_64     :1;																			 /* Channel 100 (Hex64) corresponds to HCS12X Vector  27 at 0xFFC8 */
               tU08 XGIF_65     :1;																			 /* Channel 101 (Hex65) corresponds to HCS12X Vector  26 at 0xFFCA */
               tU08 XGIF_66     :1;																			 /* Channel 102 (Hex66) corresponds to HCS12X Vector  25 at 0xFFCC */
               tU08 XGIF_67     :1;																			 /* Channel 103 (Hex67) corresponds to HCS12X Vector  24 at 0xFFCE */
            } bits_60_67;
            struct {
               tU08 XGIF_58     :1;																			 /* Channel  88 (Hex58) corresponds to HCS12X Vector  39 at 0xFFB0 */
               tU08 XGIF_59     :1;																			 /* Channel  89 (Hex59) corresponds to HCS12X Vector  38 at 0xFFB2 */
               tU08 XGIF_5A     :1;																			 /* Channel  90 (Hex5A) corresponds to HCS12X Vector  37 at 0xFFB4 */
               tU08 XGIF_5B     :1;																			 /* Channel  91 (Hex5B) corresponds to HCS12X Vector  36 at 0xFFB6 */
               tU08 XGIF_5C     :1;																			 /* Channel  92 (Hex5C) corresponds to HCS12X Vector  35 at 0xFFB8 */
               tU08 XGIF_5D     :1;																			 /* Channel  93 (Hex5D) corresponds to HCS12X Vector  34 at 0xFFBA */
               tU08 XGIF_5E     :1;																			 /* Channel  94 (Hex5E) corresponds to HCS12X Vector  33 at 0xFFBC */
               tU08 XGIF_5F     :1;																			 /* Channel  95 (Hex5F) corresponds to HCS12X Vector  32 at 0xFFBE */
            } bits_58_5F;
            struct {
               tU08 XGIF_50     :1;																			 /* Channel  80 (Hex50) corresponds to HCS12X Vector  47 at 0xFFA0 */
               tU08 XGIF_51     :1;																			 /* Channel  81 (Hex51) corresponds to HCS12X Vector  46 at 0xFFA2 */
               tU08 XGIF_52     :1;																			 /* Channel  82 (Hex52) corresponds to HCS12X Vector  45 at 0xFFA4 */
               tU08 XGIF_53     :1;																			 /* Channel  83 (Hex53) corresponds to HCS12X Vector  44 at 0xFFA6 */
               tU08 XGIF_54     :1;																			 /* Channel  84 (Hex54) corresponds to HCS12X Vector  43 at 0xFFA8 */
               tU08 XGIF_55     :1;																			 /* Channel  85 (Hex55) corresponds to HCS12X Vector  42 at 0xFFAA */
               tU08 XGIF_56     :1;																			 /* Channel  86 (Hex56) corresponds to HCS12X Vector  41 at 0xFFAC */
               tU08 XGIF_57     :1;																			 /* Channel  87 (Hex57) corresponds to HCS12X Vector  40 at 0xFFAE */
            } bits_50_57;
            struct {
               tU08 XGIF_48     :1;																			 /* Channel  72 (Hex48) corresponds to HCS12X Vector  55 at 0xFF90 */
               tU08 XGIF_49     :1;																			 /* Channel  73 (Hex49) corresponds to HCS12X Vector  54 at 0xFF92 */
               tU08 XGIF_4A     :1;																			 /* Channel  74 (Hex4A) corresponds to HCS12X Vector  53 at 0xFF94 */
               tU08 XGIF_4B     :1;																			 /* Channel  75 (Hex4B) corresponds to HCS12X Vector  52 at 0xFF96 */
               tU08 XGIF_4C     :1;																			 /* Channel  76 (Hex4C) corresponds to HCS12X Vector  51 at 0xFF98 */
               tU08 XGIF_4D     :1;																			 /* Channel  77 (Hex4D) corresponds to HCS12X Vector  50 at 0xFF9A */
               tU08 XGIF_4E     :1;																			 /* Channel  78 (Hex4E) corresponds to HCS12X Vector  49 at 0xFF9C */
               tU08 XGIF_4F     :1;																			 /* Channel  79 (Hex4F) corresponds to HCS12X Vector  48 at 0xFF9E */
            } bits_48_4F;
            struct {
               tU08 XGIF_40     :1;																			 /* Channel  64 (Hex40) corresponds to HCS12X Vector  63 at 0xFF80 */
               tU08 XGIF_41     :1;																			 /* Channel  65 (Hex41) corresponds to HCS12X Vector  62 at 0xFF82 */
               tU08 XGIF_42     :1;																			 /* Channel  66 (Hex42) corresponds to HCS12X Vector  61 at 0xFF84 */
               tU08 XGIF_43     :1;																			 /* Channel  67 (Hex43) corresponds to HCS12X Vector  60 at 0xFF86 */
               tU08 XGIF_44     :1;																			 /* Channel  68 (Hex44) corresponds to HCS12X Vector  59 at 0xFF88 */
               tU08 XGIF_45     :1;																			 /* Channel  69 (Hex45) corresponds to HCS12X Vector  58 at 0xFF8A */
               tU08 XGIF_46     :1;																			 /* Channel  70 (Hex46) corresponds to HCS12X Vector  57 at 0xFF8C */
               tU08 XGIF_47     :1;																			 /* Channel  71 (Hex47) corresponds to HCS12X Vector  56 at 0xFF8E */
            } bits_40_47;
            struct {
               tU08 XGIF_38     :1;																			 /* Channel  56 (Hex38) corresponds to HCS12X Vector  71 at 0xFF70 */
               tU08 XGIF_39     :1;																			 /* Channel  57 (Hex39) corresponds to HCS12X Vector  70 at 0xFF72 */
               tU08 XGIF_3A     :1;																			 /* Channel  58 (Hex3A) corresponds to HCS12X Vector  69 at 0xFF74 */
               tU08 XGIF_3B     :1;																			 /* Channel  59 (Hex3B) corresponds to HCS12X Vector  68 at 0xFF76 */
               tU08 XGIF_3C     :1;																			 /* Channel  60 (Hex3C) corresponds to HCS12X Vector  67 at 0xFF78 */
               tU08 XGIF_3D     :1;																			 /* Channel  61 (Hex3D) corresponds to HCS12X Vector  66 at 0xFF7A */
               tU08 XGIF_3E     :1;																			 /* Channel  62 (Hex3E) corresponds to HCS12X Vector  65 at 0xFF7C */
               tU08 XGIF_3F     :1;																			 /* Channel  63 (Hex3F) corresponds to HCS12X Vector  64 at 0xFF7E */
            } bits_38_3F;
            struct {
               tU08 XGIF_30     :1;																			 /* Channel  48 (Hex30) corresponds to HCS12X Vector  79 at 0xFF60 */
               tU08 XGIF_31     :1;																			 /* Channel  49 (Hex31) corresponds to HCS12X Vector  78 at 0xFF62 */
               tU08 XGIF_32     :1;																			 /* Channel  50 (Hex32) corresponds to HCS12X Vector  77 at 0xFF64 */
               tU08 XGIF_33     :1;																			 /* Channel  51 (Hex33) corresponds to HCS12X Vector  76 at 0xFF66 */
               tU08 XGIF_34     :1;																			 /* Channel  52 (Hex34) corresponds to HCS12X Vector  75 at 0xFF68 */
               tU08 XGIF_35     :1;																			 /* Channel  53 (Hex35) corresponds to HCS12X Vector  74 at 0xFF6A */
               tU08 XGIF_36     :1;																			 /* Channel  54 (Hex36) corresponds to HCS12X Vector  73 at 0xFF6C */
               tU08 XGIF_37     :1;																			 /* Channel  55 (Hex37) corresponds to HCS12X Vector  72 at 0xFF6E */
            } bits_30_37;
            struct {
               tU08 XGIF_28     :1;																			 /* Channel  40 (Hex28) corresponds to HCS12X Vector  87 at 0xFF50 */
               tU08 XGIF_29     :1;																			 /* Channel  41 (Hex29) corresponds to HCS12X Vector  86 at 0xFF52 */
               tU08 XGIF_2A     :1;																			 /* Channel  42 (Hex2A) corresponds to HCS12X Vector  85 at 0xFF54 */
               tU08 XGIF_2B     :1;																			 /* Channel  43 (Hex2B) corresponds to HCS12X Vector  84 at 0xFF56 */
               tU08 XGIF_2C     :1;																			 /* Channel  44 (Hex2C) corresponds to HCS12X Vector  83 at 0xFF58 */
               tU08 XGIF_2D     :1;																			 /* Channel  45 (Hex2D) corresponds to HCS12X Vector  82 at 0xFF5A */
               tU08 XGIF_2E     :1;																			 /* Channel  46 (Hex2E) corresponds to HCS12X Vector  81 at 0xFF5C */
               tU08 XGIF_2F     :1;																			 /* Channel  47 (Hex2F) corresponds to HCS12X Vector  80 at 0xFF5E */
            } bits_28_2F;
            struct {
               tU08 XGIF_20     :1;																			 /* Channel  32 (Hex20) corresponds to HCS12X Vector  95 at 0xFF40 */
               tU08 XGIF_21     :1;																			 /* Channel  33 (Hex21) corresponds to HCS12X Vector  94 at 0xFF42 */
               tU08 XGIF_22     :1;																			 /* Channel  34 (Hex22) corresponds to HCS12X Vector  93 at 0xFF44 */
               tU08 XGIF_23     :1;																			 /* Channel  35 (Hex23) corresponds to HCS12X Vector  92 at 0xFF46 */
               tU08 XGIF_24     :1;																			 /* Channel  36 (Hex24) corresponds to HCS12X Vector  91 at 0xFF48 */
               tU08 XGIF_25     :1;																			 /* Channel  37 (Hex25) corresponds to HCS12X Vector  90 at 0xFF4A */
               tU08 XGIF_26     :1;																			 /* Channel  38 (Hex26) corresponds to HCS12X Vector  89 at 0xFF4C */
               tU08 XGIF_27     :1;																			 /* Channel  39 (Hex27) corresponds to HCS12X Vector  88 at 0xFF4E */
            } bits_20_27;
            struct {
               tU08 XGIF_18     :1;																			 /* Channel  24 (Hex18) corresponds to HCS12X Vector 103 at 0xFF30 */
               tU08 XGIF_19     :1;																			 /* Channel  25 (Hex19) corresponds to HCS12X Vector 102 at 0xFF32 */
               tU08 XGIF_1A     :1;																			 /* Channel  26 (Hex1A) corresponds to HCS12X Vector 101 at 0xFF34 */
               tU08 XGIF_1B     :1;																			 /* Channel  27 (Hex1B) corresponds to HCS12X Vector 100 at 0xFF36 */
               tU08 XGIF_1C     :1;																			 /* Channel  28 (Hex1C) corresponds to HCS12X Vector  99 at 0xFF38 */
               tU08 XGIF_1D     :1;																			 /* Channel  29 (Hex1D) corresponds to HCS12X Vector  98 at 0xFF3A */
               tU08 XGIF_1E     :1;																			 /* Channel  30 (Hex1E) corresponds to HCS12X Vector  97 at 0xFF3C */
               tU08 XGIF_1F     :1;																			 /* Channel  31 (Hex1F) corresponds to HCS12X Vector  96 at 0xFF3E */
            } bbits_18_1F;
            struct {
               tU08 XGIF_10     :1;																			 /* Channel  16 (Hex10) corresponds to HCS12X Vector 111 at 0xFF20 */
               tU08 XGIF_11     :1;																			 /* Channel  17 (Hex11) corresponds to HCS12X Vector 110 at 0xFF22 */
               tU08 XGIF_12     :1;																			 /* Channel  18 (Hex12) corresponds to HCS12X Vector 109 at 0xFF24 */
               tU08 XGIF_13     :1;																			 /* Channel  19 (Hex13) corresponds to HCS12X Vector 108 at 0xFF26 */
               tU08 XGIF_14     :1;																			 /* Channel  20 (Hex14) corresponds to HCS12X Vector 107 at 0xFF28 */
               tU08 XGIF_15     :1;																			 /* Channel  21 (Hex15) corresponds to HCS12X Vector 106 at 0xFF2A */
               tU08 XGIF_16     :1;																			 /* Channel  22 (Hex16) corresponds to HCS12X Vector 105 at 0xFF2C */
               tU08 XGIF_17     :1;																			 /* Channel  23 (Hex17) corresponds to HCS12X Vector 104 at 0xFF2E */
            } bits_10_17;
            struct {
               tU08 XGIF_08     :1;																			 /* Channel   8 (Hex08) corresponds to HCS12X Vector 119 at 0xFF10 */
               tU08 XGIF_09     :1;																			 /* Channel   9 (Hex09) corresponds to HCS12X Vector 118 at 0xFF12 */
               tU08 XGIF_0A     :1;																			 /* Channel  10 (Hex0A) corresponds to HCS12X Vector 117 at 0xFF14 */
               tU08 XGIF_0B     :1;																			 /* Channel  11 (Hex0B) corresponds to HCS12X Vector 116 at 0xFF16 */
               tU08 XGIF_0C     :1;																			 /* Channel  12 (Hex0C) corresponds to HCS12X Vector 115 at 0xFF18 */
               tU08 XGIF_0D     :1;																			 /* Channel  13 (Hex0D) corresponds to HCS12X Vector 114 at 0xFF1A */
               tU08 XGIF_0E     :1;																			 /* Channel  14 (Hex0E) corresponds to HCS12X Vector 113 at 0xFF1C */
               tU08 XGIF_0F     :1;																			 /* Channel  15 (Hex0F) corresponds to HCS12X Vector 112 at 0xFF1E */
            } bits_08_0F;
            struct {
               tU08 XGIF_00     :1;																			 /* Channel   0 (Hex00) corresponds to HCS12X Vector 127 at 0xFF00 */
               tU08 XGIF_01     :1;																			 /* Channel   1 (Hex01) corresponds to HCS12X Vector 126 at 0xFF02 */
               tU08 XGIF_02     :1;																			 /* Channel   2 (Hex02) corresponds to HCS12X Vector 125 at 0xFF04 */
               tU08 XGIF_03     :1;																			 /* Channel   3 (Hex03) corresponds to HCS12X Vector 124 at 0xFF06 */
               tU08 XGIF_04     :1;																			 /* Channel   4 (Hex04) corresponds to HCS12X Vector 123 at 0xFF08 */
               tU08 XGIF_05     :1;																			 /* Channel   5 (Hex05) corresponds to HCS12X Vector 122 at 0xFF0A */
               tU08 XGIF_06     :1;																			 /* Channel   6 (Hex06) corresponds to HCS12X Vector 121 at 0xFF0C */
               tU08 XGIF_07     :1;																			 /* Channel   7 (Hex07) corresponds to HCS12X Vector 120 at 0xFF0E */
            } bits_00_07;
         } bit;
} tXGIF;

#define _7F_70    word[0]
#define _6F_60    word[1]
#define _5F_50    word[2]
#define _4F_40    word[3]
#define _3F_30    word[4]
#define _2F_20    word[5]
#define _1F_10    word[6]
#define _0F_00    word[7]

#define _7F_78    byte[ 0]
#define _77_70    byte[ 1]
#define _6F_68    byte[ 2]
#define _67_60    byte[ 3]
#define _5F_58    byte[ 4]
#define _57_50    byte[ 5]
#define _4F_48    byte[ 6]
#define _47_40    byte[ 7]
#define _3F_38    byte[ 8]
#define _37_30    byte[ 9]
#define _2F_28    byte[10]
#define _27_20    byte[11]
#define _1F_18    byte[12]
#define _17_10    byte[13]
#define _0F_08    byte[14]
#define _07_00    byte[15]

/* definitions to support previous header file versions */
#define xgif_70 xgif.word[0]       /* XGATE Channel Interrupt Flag 70 - 78 */
#define xgif_60 xgif.word[1]       /* XGATE Channel Interrupt Flag 60 - 6F */
#define xgif_50 xgif.word[2]       /* XGATE Channel Interrupt Flag 50 - 5F */				 
#define xgif_40 xgif.word[3]       /* XGATE Channel Interrupt Flag 40 - 4F */
#define xgif_30 xgif.word[4]       /* XGATE Channel Interrupt Flag 30 - 3F */
#define xgif_20 xgif.word[5]       /* XGATE Channel Interrupt Flag 20 - 2F */
#define xgif_10 xgif.word[6]       /* XGATE Channel Interrupt Flag 10 - 1F */
#define xgif_00 xgif.word[7]       /* XGATE Channel Interrupt Flag 09 - 0F */


typedef union uXGSWT
{
  tU16  word;
  struct
  {
    tU08 xgswtm;        /* XGATE Software Trigger Mask Register */
    tU08 xgswtr;        /* XGATE Software Trigger Register */
  }byte;
}tXGSWT;

#define XGSWTM0 0x0100
#define XGSWTM1 0x0200
#define XGSWTM2 0x0400
#define XGSWTM3 0x0800
#define XGSWTM4 0x1000
#define XGSWTM5 0x2000
#define XGSWTM6 0x4000
#define XGSWTM7 0x8000

/* alternative mask bit defines */

#define XGSWT0M 0x0100
#define XGSWT1M 0x0200
#define XGSWT2M 0x0400
#define XGSWT3M 0x0800
#define XGSWT4M 0x1000
#define XGSWT5M 0x2000
#define XGSWT6M 0x4000
#define XGSWT7M 0x8000

#define XGSWT0 0x01
#define XGSWT1 0x02
#define XGSWT2 0x04
#define XGSWT3 0x08
#define XGSWT4 0x10
#define XGSWT5 0x20
#define XGSWT6 0x40
#define XGSWT7 0x80

typedef union uXGSEM
{
  tU16  word;
  struct
  {
    tU08 xgsemm;        /* XGATE Semaphore Mask Register */
    tU08 xgsemr;        /* XGATE Semaphore Register */
  }byte;
}tXGSEM;

#define XGSEMM0 0x0100
#define XGSEMM1 0x0200
#define XGSEMM2 0x0400
#define XGSEMM3 0x0800
#define XGSEMM4 0x1000
#define XGSEMM5 0x2000
#define XGSEMM6 0x4000
#define XGSEMM7 0x8000

/* alternative mask bit defines */
#define XGSEM0M 0x0100
#define XGSEM1M 0x0200
#define XGSEM2M 0x0400
#define XGSEM3M 0x0800
#define XGSEM4M 0x1000
#define XGSEM5M 0x2000
#define XGSEM6M 0x4000
#define XGSEM7M 0x8000

#define XGSEM0 0x01
#define XGSEM1 0x02
#define XGSEM2 0x04
#define XGSEM3 0x08
#define XGSEM4 0x10
#define XGSEM5 0x20
#define XGSEM6 0x40
#define XGSEM7 0x80

typedef union uXGCCR
{
  tU08   byte;
  struct
  {
    tU08 xgc   :1;      /* XGATE Carry Flag */
    tU08 xgv   :1;      /* XGATE Overflow Flag */
    tU08 xgz   :1;      /* XGATE Zero Flag */
    tU08 xgn   :1;      /* XGATE Sign Flag */
    tU08       :4;      /* not used */
  }bit;
}tXGCCR;


typedef struct             /* XGATE module used on the S12XDP256  */
{
  volatile tU16         xgmctl;        /* XGATE Module Control Register */
  volatile tU08         xgchid;        /* XGATE Channel ID Register     */
           tU08         rsv0[3];       /* reserved */
  volatile tU16         xgvbr;         /* XGATE Vector Base Address Register */  
  volatile tXGIF        xgif;          /* XGATE Channel Interrupt Flag 70 - 78 */
  volatile tXGSWT       xgswt;         /* XGATE Software Trigger Register */
  volatile tXGSEM       xgsem;         /* XGATE Semaphore Register */
           tU08         rsv1c;         /* reserved byte */
  volatile tXGCCR       xgccr;         /* XGATE Condition Code Register */
  volatile tU16         xgpc;          /* XGATE Program Counter */
           tU16         rsv20;         /* reserved word */
  volatile tU16         xgr1;          /* XGATE Register 1 */
  volatile tU16         xgr2;          /* XGATE Register 2 */
  volatile tU16         xgr3;          /* XGATE Register 3 */
  volatile tU16         xgr4;          /* XGATE Register 4 */
  volatile tU16         xgr5;          /* XGATE Register 5 */
  volatile tU16         xgr6;          /* XGATE Register 6 */
  volatile tU16         xgr7;          /* XGATE Register 7 */
}tXGATE;

#endif /* S12XGATEV2_H  */