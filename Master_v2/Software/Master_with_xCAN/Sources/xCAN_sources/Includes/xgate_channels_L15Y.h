/* XGate interrupt flag defines */

/******************************************************************************
													Copyright (c) Freescale 2005
File Name	 :	$RCSfile: xgate_channels_L15Y.h,v $

Engineer		 :	$Author: ttz778 $

Location		 :	EKB

Date Created	  :	04/04/2005

Current Revision :	$Revision: 1.0 $

Notes            :   #defines for easy management of the banked XGate 
                     interrupt flags when using the EKB header files -
                     
                        to clear the xgate interrupt flag in mcu interrupt xyz:
                           XGATE.XGIF_WORD_XYZ = XGIF_MASK_XYZ;
                     
                        to check if the source of an mcu interrupt was 
                        the equivalent XGATE channel xyz:
                           if( XGATE.XGIF_WORD_XYZ & XGIF_MASK_XYZ )
                     
                     #defines of XGate channel IDs for use with the sif()
                     instruction.

                        to generate a specific mcu interrupt xyz:
                           sif( XGCHID_XYZ );

*******************************************************************************
Freescale reserves the right to make changes without further notice to any
product herein to improve reliability, function or design. Freescale does
not assume any liability arising out ot the application or use of any
product, circuit, or software described herein; neither does it convey any
license under its patent rights nor the rights of others. Freescale products
are not designed, intended, or authorized for use as components in systems
intended for surgical implant into the body, or other applications intended
to support life, or for any other application in which the failure of the
Freescale product could create a situation where personal injury or death may
occur. Should Buyer purchase or use Freescale products for any such unintended
or unauthorized application, Buyer shall idemnify and hold Freescale and its
officers, employees, subsidiaries, affiliates, and distributors harmless
against all claims costs, damages, and expenses, and reasonable attorney fees
arising out of, directly or indirectly, any claim of personal injury or death
associated with such unintended or unathorized use, even if such claim alleges
that Freescale was negligent regarding the design or manufacture of the part.
Freescale and the Freescale logo* are registered trademarks of Freescale Ltd.
******************************************************************************/

#ifndef XGIF_H        /*prevent duplicated includes*/
#define XGIF_H

/************************* Macros ******************************************/

#define XGIF_CLEAR_CHANNEL_BIT(b)  { XGATE.xgif.byte[(XGATE_MAX_NUM_VECTOR - (b) - 1) >> 3]= (1U << ((b) & 0x7)); }
#define XGIF_READ_CHANNEL_BIT(b)   ((XGATE.xgif.byte[(XGATE_MAX_NUM_VECTOR - (b) - 1) >> 3] & (1U << ((b) & 0x7))) != 0)

/************************* #defines ******************************************/

#define XGATE_MAX_NUM_VECTOR 128

/* XGate channel identifier */
#define XGCHID_SWI7    0x32  // Channel 32 - XGATE Software Trigger 7           
#define XGCHID_SWI6    0x33  // Channel 33 - XGATE Software Trigger 6           
#define XGCHID_SWI5    0x34  // Channel 34 - XGATE Software Trigger 5           
#define XGCHID_SWI4    0x35  // Channel 35 - XGATE Software Trigger 4           
#define XGCHID_SWI3    0x36  // Channel 36 - XGATE Software Trigger 3           
#define XGCHID_SWI2    0x37  // Channel 37 - XGATE Software Trigger 2           
#define XGCHID_SWI1    0x38  // Channel 38 - XGATE Software Trigger 1           
#define XGCHID_SWI0    0x39  // Channel 39 - XGATE Software Trigger 0           
#define XGCHID_PITCH3  0x3A  // Channel 3A - Periodic Interrupt Timer 3          
#define XGCHID_PITCH2  0x3B  // Channel 3B - Periodic Interrupt Timer 2          
#define XGCHID_PITCH1  0x3C  // Channel 3C - Periodic Interrupt Timer 1          
#define XGCHID_PITCH0  0x3D  // Channel 3D - Periodic Interrupt Timer 0          
//#define XGCHID_      0x3E  // Channel 3E - Reserved                         
#define XGCHID_API     0x3F  // Channel 3F - Autonomous Periodical interrupt API
#define XGCHID_LVI     0x40  // Channel 40 - Low Voltage interrupt LVI
#define XGCHID_IIC1    0x41  // Channel 41 - IIC1 Bus                 
#define XGCHID_SCI5    0x42  // Channel 42 - SCI5                     
#define XGCHID_SCI4    0x43  // Channel 43 - SCI4                     
#define XGCHID_SCI3    0x44  // Channel 44 - SCI3                     
#define XGCHID_SCI2    0x45  // Channel 45 - SCI2                     
#define XGCHID_PWMES   0x46  // Channel 46 - PWM Emergency Shutdown   
#define XGCHID_PTP     0x47  // Channel 47 - Port P Interrupt         
#define XGCHID_CAN4TX  0x48  // Channel 48 - CAN4 transmit            
#define XGCHID_CAN4RX  0x49  // Channel 49 - CAN4 receive             
#define XGCHID_CAN4ERR 0x4A  // Channel 4A - CAN4 errors              
#define XGCHID_CAN4WUP 0x4B  // Channel 4B - CAN4 wake-up             
#define XGCHID_CAN3TX  0x4C  // Channel 4C - CAN3 transmit            
#define XGCHID_CAN3RX  0x4D  // Channel 4D - CAN3 receive             
#define XGCHID_CAN3ERR 0x4E  // Channel 4E - CAN3 errors              
#define XGCHID_CAN3WUP 0x4F  // Channel 4F - CAN3 wake-up             
#define XGCHID_CAN2TX  0x50  // Channel 50 - CAN2 transmit
#define XGCHID_CAN2RX  0x51  // Channel 51 - CAN2 receive 
#define XGCHID_CAN2ERR 0x52  // Channel 52 - CAN2 errors  
#define XGCHID_CAN2WUP 0x53  // Channel 53 - CAN2 wake-up 
#define XGCHID_CAN1TX  0x54  // Channel 54 - CAN1 transmit
#define XGCHID_CAN1RX  0x55  // Channel 55 - CAN1 receive 
#define XGCHID_CAN1ERR 0x56  // Channel 56 - CAN1 errors  
#define XGCHID_CAN1WUP 0x57  // Channel 57 - CAN1 wake-up 
#define XGCHID_CAN0TX  0x58  // Channel 58 - CAN0 transmit
#define XGCHID_CAN0RX  0x59  // Channel 59 - CAN0 receive 
#define XGCHID_CAN0ERR 0x5A  // Channel 5A - CAN0 errors  
#define XGCHID_CAN0WUP 0x5B  // Channel 5B - CAN0 wake-up 
#define XGCHID_FLASH   0x5C  // Channel 5C - FLASH 
#define XGCHID_EEPROM  0x5D  // Channel 5D - EEPROM
#define XGCHID_SPI2    0x5E  // Channel 5E - SPI2  
#define XGCHID_SPI1    0x5F  // Channel 5F - SPI1
#define XGCHID_IIC0    0x60  // Channel 60 - IIC0 Bus                         
//#define XGCHID_      0x61  // Channel 61 - Reserved                         
#define XGCHID_SCM     0x62  // Channel 62 - CRG Self Clock Mode              
#define XGCHID_PLLLOCK 0x63  // Channel 63 - CRG PLL lock                     
#define XGCHID_PACBOF  0x64  // Channel 64 - Pulse Accumulator B Overflow     
#define XGCHID_MODUF   0x65  // Channel 65 - Modulus Down Counter underflow   
#define XGCHID_PTH     0x66  // Channel 66 - Port H                           
#define XGCHID_PTJ     0x67  // Channel 67 - Port J                           
#define XGCHID_ATD1    0x68  // Channel 68 - ATD1                             
#define XGCHID_ADT0    0x69  // Channel 69 - ATD0                             
#define XGCHID_SCI1    0x6A  // Channel 6A - SCI1                             
#define XGCHID_SCI0    0x6B  // Channel 6B - SCI0                             
#define XGCHID_SPI0    0x6C  // Channel 6C - SPI0                             
#define XGCHID_PACIPE  0x6D  // Channel 6D - Pulse accumulator input edge     
#define XGCHID_PACAOF  0x6E  // Channel 6E - Pulse accumulator A overflow     
#define XGCHID_ECTOF   0x6F  // Channel 6F - Enhanced Capture Timer overflow  
#define XGCHID_ECTCH7  0x70  // Channel 70 - Enhanced Capture Timer channel 7                                 
#define XGCHID_ECTCH6  0x71  // Channel 71 - Enhanced Capture Timer channel 6 
#define XGCHID_ECTCH5  0x72  // Channel 72 - Enhanced Capture Timer channel 5 
#define XGCHID_ECTCH4  0x73  // Channel 73 - Enhanced Capture Timer channel 4 
#define XGCHID_ECTCH3  0x74  // Channel 74 - Enhanced Capture Timer channel 3 
#define XGCHID_ECTCH2  0x75  // Channel 75 - Enhanced Capture Timer channel 2 
#define XGCHID_ECTCH1  0x76  // Channel 76 - Enhanced Capture Timer channel 1 
#define XGCHID_ECTCH0  0x77  // Channel 77 - Enhanced Capture Timer channel 0 
#define XGCHID_RTI     0x78  // Channel 78 - Real Time Interrupt 

/* XGate Interrupt Flag word identifier */
#define XGIF_WORD_SWI7    xgif_30  // Channel 32 - XGATE Software Trigger 7           
#define XGIF_WORD_SWI6    xgif_30  // Channel 33 - XGATE Software Trigger 6           
#define XGIF_WORD_SWI5    xgif_30  // Channel 34 - XGATE Software Trigger 5           
#define XGIF_WORD_SWI4    xgif_30  // Channel 35 - XGATE Software Trigger 4           
#define XGIF_WORD_SWI3    xgif_30  // Channel 36 - XGATE Software Trigger 3           
#define XGIF_WORD_SWI2    xgif_30  // Channel 37 - XGATE Software Trigger 2           
#define XGIF_WORD_SWI1    xgif_30  // Channel 38 - XGATE Software Trigger 1           
#define XGIF_WORD_SWI0    xgif_30  // Channel 39 - XGATE Software Trigger 0           
#define XGIF_WORD_PITCH3  xgif_30  // Channel 3A - Periodic Interrupt Timer 3          
#define XGIF_WORD_PITCH2  xgif_30  // Channel 3B - Periodic Interrupt Timer 2          
#define XGIF_WORD_PITCH1  xgif_30  // Channel 3C - Periodic Interrupt Timer 1          
#define XGIF_WORD_PITCH0  xgif_30  // Channel 3D - Periodic Interrupt Timer 0          
//#define XGIF_WORD_      xgif_30  // Channel 3E - Reserved                         
#define XGIF_WORD_API     xgif_30  // Channel 3F - Autonomous Periodical interrupt API
#define XGIF_WORD_LVI     xgif_40  // Channel 40 - Low Voltage interrupt LVI
#define XGIF_WORD_IIC1    xgif_40  // Channel 41 - IIC1 Bus                 
#define XGIF_WORD_SCI5    xgif_40  // Channel 42 - SCI5                     
#define XGIF_WORD_SCI4    xgif_40  // Channel 43 - SCI4                     
#define XGIF_WORD_SCI3    xgif_40  // Channel 44 - SCI3                     
#define XGIF_WORD_SCI2    xgif_40  // Channel 45 - SCI2                     
#define XGIF_WORD_PWMES   xgif_40  // Channel 46 - PWM Emergency Shutdown   
#define XGIF_WORD_PTP     xgif_40  // Channel 47 - Port P Interrupt         
#define XGIF_WORD_CAN4TX  xgif_40  // Channel 48 - CAN4 transmit            
#define XGIF_WORD_CAN4RX  xgif_40  // Channel 49 - CAN4 receive             
#define XGIF_WORD_CAN4ERR xgif_40  // Channel 4A - CAN4 errors              
#define XGIF_WORD_CAN4WUP xgif_40  // Channel 4B - CAN4 wake-up             
#define XGIF_WORD_CAN3TX  xgif_40  // Channel 4C - CAN3 transmit            
#define XGIF_WORD_CAN3RX  xgif_40  // Channel 4D - CAN3 receive             
#define XGIF_WORD_CAN3ERR xgif_40  // Channel 4E - CAN3 errors              
#define XGIF_WORD_CAN3WUP xgif_40  // Channel 4F - CAN3 wake-up             
#define XGIF_WORD_CAN2TX  xgif_50  // Channel 50 - CAN2 transmit
#define XGIF_WORD_CAN2RX  xgif_50  // Channel 51 - CAN2 receive 
#define XGIF_WORD_CAN2ERR xgif_50  // Channel 52 - CAN2 errors  
#define XGIF_WORD_CAN2WUP xgif_50  // Channel 53 - CAN2 wake-up 
#define XGIF_WORD_CAN1TX  xgif_50  // Channel 54 - CAN1 transmit
#define XGIF_WORD_CAN1RX  xgif_50  // Channel 55 - CAN1 receive 
#define XGIF_WORD_CAN1ERR xgif_50  // Channel 56 - CAN1 errors  
#define XGIF_WORD_CAN1WUP xgif_50  // Channel 57 - CAN1 wake-up 
#define XGIF_WORD_CAN0TX  xgif_50  // Channel 58 - CAN0 transmit
#define XGIF_WORD_CAN0RX  xgif_50  // Channel 59 - CAN0 receive 
#define XGIF_WORD_CAN0ERR xgif_50  // Channel 5A - CAN0 errors  
#define XGIF_WORD_CAN0WUP xgif_50  // Channel 5B - CAN0 wake-up 
#define XGIF_WORD_FLASH   xgif_50  // Channel 5C - FLASH 
#define XGIF_WORD_EEPROM  xgif_50  // Channel 5D - EEPROM
#define XGIF_WORD_SPI2    xgif_50  // Channel 5E - SPI2  
#define XGIF_WORD_SPI1    xgif_50  // Channel 5F - SPI1
#define XGIF_WORD_IIC0    xgif_60  // Channel 60 - IIC0 Bus                         
//#define XGIF_WORD_      xgif_60  // Channel 61 - Reserved                         
#define XGIF_WORD_SCM     xgif_60  // Channel 62 - CRG Self Clock Mode              
#define XGIF_WORD_PLLLOCK xgif_60  // Channel 63 - CRG PLL lock                     
#define XGIF_WORD_PACBOF  xgif_60  // Channel 64 - Pulse Accumulator B Overflow     
#define XGIF_WORD_MODUF   xgif_60  // Channel 65 - Modulus Down Counter underflow   
#define XGIF_WORD_PTH     xgif_60  // Channel 66 - Port H                           
#define XGIF_WORD_PTJ     xgif_60  // Channel 67 - Port J                           
#define XGIF_WORD_ATD1    xgif_60  // Channel 68 - ATD1                             
#define XGIF_WORD_ADT0    xgif_60   // Channel 69 - ATD0                             
#define XGIF_WORD_SCI1    xgif_60  // Channel 6A - SCI1                             
#define XGIF_WORD_SCI0    xgif_60  // Channel 6B - SCI0                             
#define XGIF_WORD_SPI0    xgif_60  // Channel 6C - SPI0                             
#define XGIF_WORD_PACIPE  xgif_60  // Channel 6D - Pulse accumulator input edge     
#define XGIF_WORD_PACAOF  xgif_60  // Channel 6E - Pulse accumulator A overflow     
#define XGIF_WORD_ECTOF   xgif_60  // Channel 6F - Enhanced Capture Timer overflow  
#define XGIF_WORD_ECTCH7  xgif_70  // Channel 70 - Enhanced Capture Timer channel 7                                 
#define XGIF_WORD_ECTCH6  xgif_70  // Channel 71 - Enhanced Capture Timer channel 6 
#define XGIF_WORD_ECTCH5  xgif_70  // Channel 72 - Enhanced Capture Timer channel 5 
#define XGIF_WORD_ECTCH4  xgif_70  // Channel 73 - Enhanced Capture Timer channel 4 
#define XGIF_WORD_ECTCH3  xgif_70  // Channel 74 - Enhanced Capture Timer channel 3 
#define XGIF_WORD_ECTCH2  xgif_70  // Channel 75 - Enhanced Capture Timer channel 2 
#define XGIF_WORD_ECTCH1  xgif_70  // Channel 76 - Enhanced Capture Timer channel 1 
#define XGIF_WORD_ECTCH0  xgif_70  // Channel 77 - Enhanced Capture Timer channel 0 
#define XGIF_WORD_RTI     xgif_70   // Channel 78 - Real Time Interrupt 

/* XGate Interrupt Flag word bit masks */
#define XGIF_MASK_SWI7    (0x0001<<0x02)  // Channel 32 - XGATE Software Trigger 7           
#define XGIF_MASK_SWI6    (0x0001<<0x03)  // Channel 33 - XGATE Software Trigger 6           
#define XGIF_MASK_SWI5    (0x0001<<0x04)  // Channel 34 - XGATE Software Trigger 5           
#define XGIF_MASK_SWI4    (0x0001<<0x05)  // Channel 35 - XGATE Software Trigger 4           
#define XGIF_MASK_SWI3    (0x0001<<0x06)  // Channel 36 - XGATE Software Trigger 3           
#define XGIF_MASK_SWI2    (0x0001<<0x07)  // Channel 37 - XGATE Software Trigger 2           
#define XGIF_MASK_SWI1    (0x0001<<0x08)  // Channel 38 - XGATE Software Trigger 1           
#define XGIF_MASK_SWI0    (0x0001<<0x09)  // Channel 39 - XGATE Software Trigger 0           
#define XGIF_MASK_PITCH3  (0x0001<<0x0A)  // Channel 3A - Periodic Interrupt Timer 3          
#define XGIF_MASK_PITCH2  (0x0001<<0x0B)  // Channel 3B - Periodic Interrupt Timer 2          
#define XGIF_MASK_PITCH1  (0x0001<<0x0C)  // Channel 3C - Periodic Interrupt Timer 1          
#define XGIF_MASK_PITCH0  (0x0001<<0x0D)  // Channel 3D - Periodic Interrupt Timer 0          
//#define XGIF_MASK_      (0x0001<<0x0E)  // Channel 3E - Reserved                         
#define XGIF_MASK_API     (0x0001<<0x0F)  // Channel 3F - Autonomous Periodical interrupt API
#define XGIF_MASK_LVI      0x0001         // Channel 40 - Low Voltage interrupt LVI
#define XGIF_MASK_IIC1    (0x0001<<0x01)  // Channel 41 - IIC1 Bus                 
#define XGIF_MASK_SCI5    (0x0001<<0x02)  // Channel 42 - SCI5                     
#define XGIF_MASK_SCI4    (0x0001<<0x03)  // Channel 43 - SCI4                     
#define XGIF_MASK_SCI3    (0x0001<<0x04)  // Channel 44 - SCI3                     
#define XGIF_MASK_SCI2    (0x0001<<0x05)  // Channel 45 - SCI2                     
#define XGIF_MASK_PWMES   (0x0001<<0x06)  // Channel 46 - PWM Emergency Shutdown   
#define XGIF_MASK_PTP     (0x0001<<0x07)  // Channel 47 - Port P Interrupt         
#define XGIF_MASK_CAN4TX  (0x0001<<0x08)  // Channel 48 - CAN4 transmit            
#define XGIF_MASK_CAN4RX  (0x0001<<0x09)  // Channel 49 - CAN4 receive             
#define XGIF_MASK_CAN4ERR (0x0001<<0x0A)  // Channel 4A - CAN4 errors              
#define XGIF_MASK_CAN4WUP (0x0001<<0x0B)  // Channel 4B - CAN4 wake-up             
#define XGIF_MASK_CAN3TX  (0x0001<<0x0C)  // Channel 4C - CAN3 transmit            
#define XGIF_MASK_CAN3RX  (0x0001<<0x0D)  // Channel 4D - CAN3 receive             
#define XGIF_MASK_CAN3ERR (0x0001<<0x0E)  // Channel 4E - CAN3 errors              
#define XGIF_MASK_CAN3WUP (0x0001<<0x0F)  // Channel 4F - CAN3 wake-up             
#define XGIF_MASK_CAN2TX   0x0001         // Channel 50 - CAN2 transmit
#define XGIF_MASK_CAN2RX  (0x0001<<0x01)  // Channel 51 - CAN2 receive 
#define XGIF_MASK_CAN2ERR (0x0001<<0x02)  // Channel 52 - CAN2 errors  
#define XGIF_MASK_CAN2WUP (0x0001<<0x03)  // Channel 53 - CAN2 wake-up 
#define XGIF_MASK_CAN1TX  (0x0001<<0x04)  // Channel 54 - CAN1 transmit
#define XGIF_MASK_CAN1RX  (0x0001<<0x05)  // Channel 55 - CAN1 receive 
#define XGIF_MASK_CAN1ERR (0x0001<<0x06)  // Channel 56 - CAN1 errors  
#define XGIF_MASK_CAN1WUP (0x0001<<0x07)  // Channel 57 - CAN1 wake-up 
#define XGIF_MASK_CAN0TX  (0x0001<<0x08)  // Channel 58 - CAN0 transmit
#define XGIF_MASK_CAN0RX  (0x0001<<0x09)  // Channel 59 - CAN0 receive 
#define XGIF_MASK_CAN0ERR (0x0001<<0x0A)  // Channel 5A - CAN0 errors  
#define XGIF_MASK_CAN0WUP (0x0001<<0x0B)  // Channel 5B - CAN0 wake-up 
#define XGIF_MASK_FLASH   (0x0001<<0x0C)  // Channel 5C - FLASH 
#define XGIF_MASK_EEPROM  (0x0001<<0x0D)  // Channel 5D - EEPROM
#define XGIF_MASK_SPI2    (0x0001<<0x0E)  // Channel 5E - SPI2  
#define XGIF_MASK_SPI1    (0x0001<<0x0F)  // Channel 5F - SPI1
#define XGIF_MASK_IIC0     0x0001         // Channel 60 - IIC0 Bus                         
//#define XGIF_MASK_      (0x0001<<0x01)  // Channel 61 - Reserved                         
#define XGIF_MASK_SCM     (0x0001<<0x02)  // Channel 62 - CRG Self Clock Mode              
#define XGIF_MASK_PLLLOCK (0x0001<<0x03)  // Channel 63 - CRG PLL lock                     
#define XGIF_MASK_PACBOF  (0x0001<<0x04)  // Channel 64 - Pulse Accumulator B Overflow     
#define XGIF_MASK_MODUF   (0x0001<<0x05)  // Channel 65 - Modulus Down Counter underflow   
#define XGIF_MASK_PTH     (0x0001<<0x06)  // Channel 66 - Port H                           
#define XGIF_MASK_PTJ     (0x0001<<0x07)  // Channel 67 - Port J                           
#define XGIF_MASK_ATD1    (0x0001<<0x08)  // Channel 68 - ATD1                             
#define XGIF_MASK_ADT0    (0x0001<<0x09)  // Channel 69 - ATD0                             
#define XGIF_MASK_SCI1    (0x0001<<0x0A)  // Channel 6A - SCI1                             
#define XGIF_MASK_SCI0    (0x0001<<0x0B)  // Channel 6B - SCI0                             
#define XGIF_MASK_SPI0    (0x0001<<0x0C)  // Channel 6C - SPI0                             
#define XGIF_MASK_PACIPE  (0x0001<<0x0D)  // Channel 6D - Pulse accumulator input edge     
#define XGIF_MASK_PACAOF  (0x0001<<0x0E)  // Channel 6E - Pulse accumulator A overflow     
#define XGIF_MASK_ECTOF   (0x0001<<0x0F)  // Channel 6F - Enhanced Capture Timer overflow  
#define XGIF_MASK_ECTCH7   0x0001         // Channel 70 - Enhanced Capture Timer channel 7                                 
#define XGIF_MASK_ECTCH6  (0x0001<<0x01)  // Channel 71 - Enhanced Capture Timer channel 6 
#define XGIF_MASK_ECTCH5  (0x0001<<0x02)  // Channel 72 - Enhanced Capture Timer channel 5 
#define XGIF_MASK_ECTCH4  (0x0001<<0x03)  // Channel 73 - Enhanced Capture Timer channel 4 
#define XGIF_MASK_ECTCH3  (0x0001<<0x04)  // Channel 74 - Enhanced Capture Timer channel 3 
#define XGIF_MASK_ECTCH2  (0x0001<<0x05)  // Channel 75 - Enhanced Capture Timer channel 2 
#define XGIF_MASK_ECTCH1  (0x0001<<0x06)  // Channel 76 - Enhanced Capture Timer channel 1 
#define XGIF_MASK_ECTCH0  (0x0001<<0x07)  // Channel 77 - Enhanced Capture Timer channel 0 
#define XGIF_MASK_RTI     (0x0001<<0x08)  // Channel 78 - Real Time Interrupt 

/* XGate channel bit name */
#define XGIF_SWI7    XGIF_32  // Channel 32 - XGATE Software Trigger 7           
#define XGIF_SWI6    XGIF_33  // Channel 33 - XGATE Software Trigger 6           
#define XGIF_SWI5    XGIF_34  // Channel 34 - XGATE Software Trigger 5           
#define XGIF_SWI4    XGIF_35  // Channel 35 - XGATE Software Trigger 4           
#define XGIF_SWI3    XGIF_36  // Channel 36 - XGATE Software Trigger 3           
#define XGIF_SWI2    XGIF_37  // Channel 37 - XGATE Software Trigger 2           
#define XGIF_SWI1    XGIF_38  // Channel 38 - XGATE Software Trigger 1           
#define XGIF_SWI0    XGIF_39  // Channel 39 - XGATE Software Trigger 0           
#define XGIF_PITCH3  XGIF_3A  // Channel 3A - Periodic Interrupt Timer 3          
#define XGIF_PITCH2  XGIF_3B  // Channel 3B - Periodic Interrupt Timer 2          
#define XGIF_PITCH1  XGIF_3C  // Channel 3C - Periodic Interrupt Timer 1          
#define XGIF_PITCH0  XGIF_3D  // Channel 3D - Periodic Interrupt Timer 0          
//#define XGIF_      XGIF_3E  // Channel 3E - Reserved                         
#define XGIF_API     XGIF_3F  // Channel 3F - Autonomous Periodical interrupt API
#define XGIF_LVI     XGIF_40  // Channel 40 - Low Voltage interrupt LVI
#define XGIF_IIC1    XGIF_41  // Channel 41 - IIC1 Bus                 
#define XGIF_SCI5    XGIF_42  // Channel 42 - SCI5                     
#define XGIF_SCI4    XGIF_43  // Channel 43 - SCI4                     
#define XGIF_SCI3    XGIF_44  // Channel 44 - SCI3                     
#define XGIF_SCI2    XGIF_45  // Channel 45 - SCI2                     
#define XGIF_PWMES   XGIF_46  // Channel 46 - PWM Emergency Shutdown   
#define XGIF_PTP     XGIF_47  // Channel 47 - Port P Interrupt         
#define XGIF_CAN4TX  XGIF_48  // Channel 48 - CAN4 transmit            
#define XGIF_CAN4RX  XGIF_49  // Channel 49 - CAN4 receive             
#define XGIF_CAN4ERR XGIF_4A  // Channel 4A - CAN4 errors              
#define XGIF_CAN4WUP XGIF_4B  // Channel 4B - CAN4 wake-up             
#define XGIF_CAN3TX  XGIF_4C  // Channel 4C - CAN3 transmit            
#define XGIF_CAN3RX  XGIF_4D  // Channel 4D - CAN3 receive             
#define XGIF_CAN3ERR XGIF_4E  // Channel 4E - CAN3 errors              
#define XGIF_CAN3WUP XGIF_4F  // Channel 4F - CAN3 wake-up             
#define XGIF_CAN2TX  XGIF_50  // Channel 50 - CAN2 transmit
#define XGIF_CAN2RX  XGIF_51  // Channel 51 - CAN2 receive 
#define XGIF_CAN2ERR XGIF_52  // Channel 52 - CAN2 errors  
#define XGIF_CAN2WUP XGIF_53  // Channel 53 - CAN2 wake-up 
#define XGIF_CAN1TX  XGIF_54  // Channel 54 - CAN1 transmit
#define XGIF_CAN1RX  XGIF_55  // Channel 55 - CAN1 receive 
#define XGIF_CAN1ERR XGIF_56  // Channel 56 - CAN1 errors  
#define XGIF_CAN1WUP XGIF_57  // Channel 57 - CAN1 wake-up 
#define XGIF_CAN0TX  XGIF_58  // Channel 58 - CAN0 transmit
#define XGIF_CAN0RX  XGIF_59  // Channel 59 - CAN0 receive 
#define XGIF_CAN0ERR XGIF_5A  // Channel 5A - CAN0 errors  
#define XGIF_CAN0WUP XGIF_5B  // Channel 5B - CAN0 wake-up 
#define XGIF_FLASH   XGIF_5C  // Channel 5C - FLASH 
#define XGIF_EEPROM  XGIF_5D  // Channel 5D - EEPROM
#define XGIF_SPI2    XGIF_5E  // Channel 5E - SPI2  
#define XGIF_SPI1    XGIF_5F  // Channel 5F - SPI1
#define XGIF_IIC0    XGIF_60  // Channel 60 - IIC0 Bus                         
//#define XGIF_      XGIF_61  // Channel 61 - Reserved                         
#define XGIF_SCM     XGIF_62  // Channel 62 - CRG Self Clock Mode              
#define XGIF_PLLLOCK XGIF_63  // Channel 63 - CRG PLL lock                     
#define XGIF_PACBOF  XGIF_64  // Channel 64 - Pulse Accumulator B Overflow     
#define XGIF_MODUF   XGIF_65  // Channel 65 - Modulus Down Counter underflow   
#define XGIF_PTH     XGIF_66  // Channel 66 - Port H                           
#define XGIF_PTJ     XGIF_67  // Channel 67 - Port J                           
#define XGIF_ATD1    XGIF_68  // Channel 68 - ATD1                             
#define XGIF_ADT0    XGIF_69  // Channel 69 - ATD0                             
#define XGIF_SCI1    XGIF_6A  // Channel 6A - SCI1                             
#define XGIF_SCI0    XGIF_6B  // Channel 6B - SCI0                             
#define XGIF_SPI0    XGIF_6C  // Channel 6C - SPI0                             
#define XGIF_PACIPE  XGIF_6D  // Channel 6D - Pulse accumulator input edge     
#define XGIF_PACAOF  XGIF_6E  // Channel 6E - Pulse accumulator A overflow     
#define XGIF_ECTOF   XGIF_6F  // Channel 6F - Enhanced Capture Timer overflow  
#define XGIF_ECTCH7  XGIF_70  // Channel 70 - Enhanced Capture Timer channel 7                                 
#define XGIF_ECTCH6  XGIF_71  // Channel 71 - Enhanced Capture Timer channel 6 
#define XGIF_ECTCH5  XGIF_72  // Channel 72 - Enhanced Capture Timer channel 5 
#define XGIF_ECTCH4  XGIF_73  // Channel 73 - Enhanced Capture Timer channel 4 
#define XGIF_ECTCH3  XGIF_74  // Channel 74 - Enhanced Capture Timer channel 3 
#define XGIF_ECTCH2  XGIF_75  // Channel 75 - Enhanced Capture Timer channel 2 
#define XGIF_ECTCH1  XGIF_76  // Channel 76 - Enhanced Capture Timer channel 1 
#define XGIF_ECTCH0  XGIF_77  // Channel 77 - Enhanced Capture Timer channel 0 
#define XGIF_RTI     XGIF_78  // Channel 78 - Real Time Interrupt 





#endif XGIF_H
