#include <hidef.h>      /* common defines and macros */
#include "derivative.h"
#include "CANSlave.h"
#include "defines.h"
#include "type.h"

/******************************************************************************
* CAN0SendChar: Transmit an array of char by CAN0
*
* Parameters:  id: the usual CAN ID on 11 bits right justified
*              priority: the transmit buffer priority
*              length: the number of bytes to send
*              *txdata: a pointer to the first byte
*
* Return :
*****************************************************************************/
uint8 CAN0SendChar(uint16 id, uint8 priority, uint8 length, uint8* txdata) {
  uint8 index = 0;
  uint8 txbuffer = 0;

  if (length > 8)  return CAN_ERR_MAX_LEN;
  if (!CAN0TFLG)  return CAN_ERR_BUFFER_FULL;  // Are all the tansmit buffers full? 

  CAN0TBSEL = CAN0TFLG;                       // Select lowest empty buffer
  txbuffer = CAN0TBSEL;                       // Backup selected buffer
  CAN0TXIDR0 = (uint8)(id >> 3);             // Load Id to IDR Registers. Id is on 11 bits,
  CAN0TXIDR1 = (uint8)(id << 5);             // and must be left justified.

  // Load data to Tx buffer Data Segment Registers
  for (index=0; index<length; index++)
    *(&CAN0TXDSR0 + index) = txdata[index];   

  CAN0TXDLR = length;           // Set data length code
  CAN0TXTBPR = priority;        // Set priority. As compared to other buffered TX messages
  CAN0TFLG = txbuffer;          // Start transmission. Write of 1 clears the flag.

  // Wait for transmission completion.
  // When the buffer is empty, the flag goes to 1.
  while ((CAN0TFLG & txbuffer) != txbuffer) {};
  return CAN_NO_ERR;
}


uint8 CAN0SendEquiStatus(uint16 balVector, uint16 balThres, uint8 slaveId) {
  uint16 txdata[3] = {
    balVector,
    balThres,
    // For transmission error detection purpose
    0xFFFF
  }; 

  return CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_EQUI_REPORT_ID),
    CAN_EQUI_STATUS_PRIORITY,
    6,
    (uint8*)txdata);
}


uint8 CAN0SendVoltages(uint16 *voltages, uint8 slaveId) {
  uint8 errorFlag = CAN_NO_ERR;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_VOLTAGES_1TO4_ID),
    CAN_VOLTAGES_PRIORITY,
    8,
    (uint8*)&voltages[0]);

  if (errorFlag != CAN_NO_ERR)  return errorFlag;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_VOLTAGES_5TO8_ID),
    CAN_VOLTAGES_PRIORITY,
    8,
    (uint8*)&voltages[4]);

  if (errorFlag != CAN_NO_ERR)  return errorFlag;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_VOLTAGES_9TO10_ID),
    CAN_VOLTAGES_PRIORITY,
    4,
    (uint8*)&voltages[8]);                                

  return errorFlag;   
}	


uint8 CAN0SendTemp(int16* temperatures, uint8 slaveId) {
  uint8 errorFlag = CAN_NO_ERR;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_TEMP_1TO4_ID),
    CAN_TEMP_PRIORITY,
    8,
    (uint8*)&temperatures[0]);

  if (errorFlag != CAN_NO_ERR)  return errorFlag;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_TEMP_5TO8_ID),
    CAN_TEMP_PRIORITY,
    8,
    (uint8*)&temperatures[4]);

  if (errorFlag != CAN_NO_ERR)  return errorFlag;

  errorFlag = CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_TEMP_9TO10_ID),
    CAN_TEMP_PRIORITY,
    4,
    (uint8*)&temperatures[8]);

  return errorFlag;   
}	


uint8 CAN0SendInitStatus(uint8 slaveId) {
  return CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_INIT_REPORT_ID),
    CAN_INIT_STATUS_PRIORITY,
    0,
    NULL);
}

uint8 CAN0SendFirmwareRevision(uint8 slaveId) {
  uint8 firmRev = FIRMWARE_REVISION;

  return CAN0SendChar(
    MAKE_CAN_ID(slaveId, CAN_FIRMWARE_REVISION_ID),
    CAN_FIRMWARE_REVISION_PRIORITY,
    1,
    &firmRev);
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
* CANInit: a description of the function functionName.
*
* Bit Timing Definitions 
* ----------------------
*
* CAN Clock Source (External oscillator) = 16Mhz 
* BitRate = 125Khz 
* Total Time Quanta = 16
* Sincronization Jump Width = 4 Time Quanta
* 1 sample  
* Sample point at 75% of Bit Timing 
*
* CAN_BRP = ((CAN Clock Source)/fTq) - 1
* fTq = (Bit Rate) * (Total Time Quanta)
* Total Time Quanta = (SYNCH_SEG+(TSEG1+1)+(TSEG2+1))          
* Total Time Quanta =      1    +    11   +    4     = 16 Time Quanta
* fTq = 125Khz * 16 Time Quanta = 2Mhz
* CAN_BRP = (16Mhz/2Mhz) - 1 = 8 - 1 = 7 
* TSEG1 = 10
* TSEG2 = 3
* SJW = (Synchronization Jump Width-1) = 3        
*
* Another line of the description.
*
* Parameters: None
*
* Return : None
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*void CANInit(void) {

CAN0CTL0 = 0x01;          /// Enter Initialization Mode 
//  
//  0b00000001
//    ||||||||__ Enter Initialization Mode
//    |||||||___ Sleep Mode Request bit
//    ||||||____ Wake-Up disabled
//    |||||_____ Time stamping disabled
//    ||||______ Synchronized Status
//    |||_______ CAN not affected by Wait
//    ||________ Receiver Active Status bit
//    |_________ Received Frame Flag bit
///

while (!(CAN0CTL1&0x01)){};	/// Wait for Initialization Mode acknowledge
// INITRQ bit = 1
///

CAN0CTL1 = 0b11000000;
//CAN0CTL1 = 0b10000000;
/// Enable MSCAN module and LoopBack Mode          //Modif
//  
//  0b10100000
//    ||||||||__ Initialization Mode Acknowledge
//    |||||||___ Sleep Mode Acknowledge
//    ||||||____ Wake-up low-pass filter disabled
//    |||||_____ Unimplemented
//    ||||______ Listen Only Mode disabled
//    |||_______ Loop Back Mode enabled
//    ||________ Ext Osc/Xtal as Clock Source
//    |_________ MSCAN Module enabled
///

CAN0BTR0 = 0x40;
//CAN0BTR0 = 0xC7;
/// Synch Jump = 3 Tq clock Cycles 
//
//  0b11000111
//    ||||||||__
//    |||||||___\
//    ||||||____ |
//    |||||_____ |_ CAN Clock Prescaler = 7
//    ||||______ |
//    |||_______ |
//    ||________/
//    |_________>- SJW = 3
///

CAN0BTR1 = 0x3A;	      /// Set Number of samples per bit, TSEG1 and TSEG2
//
//  0b00111010
//    ||||||||__
//    |||||||___|
//    ||||||____|- TSEG1 = 10
//    |||||_____|
//    ||||______
//    |||_______\_ TSEG2 = 3
//    ||________/
//    |_________ One sample per bit
///

CAN0IDAC = 0x10;          /// Set four 16-bit Filters 
//  
//  0b00010000
//    ||||||||__ 
//    |||||||___\_ Filter Hit Indicator 
//    ||||||____/
//    |||||_____ Unimplemented 
//    ||||______ 
//    |||_______>- Four 16-bit Acceptance Filters   
//    ||________ 
//    |_________>- Unimplemented 
///

// Acceptance Filters /
CAN0IDAR0 = ACC_CODE_ID100_HIGH;    //|\    16 bit Filter 0
CAN0IDMR0 = MASK_CODE_ST_ID_HIGH;   //| \__ Accepts Standard Data Frame Msg 
CAN0IDAR1 = ACC_CODE_ID100_LOW;     //| /   with ID 0x100
CAN0IDMR1 = MASK_CODE_ST_ID_LOW;    //|/

CAN0IDAC  = 0x10;                   // Set four 16-bit Filters /

CAN0IDAR2 = 0x00;                   //|\    16 bit Filter 1
CAN0IDMR2 = MASK_CODE_ST_ID_HIGH;   //| \__ Accepts Standard Data Frame Msg
CAN0IDAR3 = 0x00;                   //| /   with ID 0x000
CAN0IDMR3 = MASK_CODE_ST_ID_LOW;    //|/

CAN0IDAR4 = 0x00;                   //|\    16 bit Filter 2
CAN0IDMR4 = MASK_CODE_ST_ID_HIGH;   //| \__ Accepts Standard Data Frame Msg
CAN0IDAR5 = 0x00;                   //| /   with ID 0x000
CAN0IDMR5 = MASK_CODE_ST_ID_LOW;    //|/

CAN0IDAR6 = 0x00;                   //|\    16 bit Filter 3
CAN0IDMR6 = MASK_CODE_ST_ID_HIGH;   //| \__ Accepts Standard Data Frame Msg
CAN0IDAR7 = 0x00;                   //| /   with ID 0x000
CAN0IDMR7 = MASK_CODE_ST_ID_LOW;    //|/

CAN0CTL0 = 0x00;            /// Exit Initialization Mode Request /
while ((CAN0CTL1&0x01) != 0){};	/// Wait for Normal Mode /

}*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
void main () {    

unsigned char txbuff[] = "ABCDEFGH";
uchar i = 0;       
// 2 next comments only for chip simulation
CANInit();                // Initialize MSCAN12 Module 

while(!(CAN0CTL0&0x10));  // Wait for Synchronization 

CAN0RFLG = 0xC3;           //Reset Receiver Flags
*  
*  0b11000011
*    ||||||||__ Receive Buffer Full Flag
*    |||||||___ Overrun Interrupt Flag
*    ||||||____ 
*    |||||_____>- Transmitter Status Bits
*    ||||______ 
*    |||_______>- Receiver Status Bits
*    ||________ CAN Status Change Interrupt Flag
*    |_________ Wake-Up Interrupt Flag
*

CAN0RIER = 0x00;          // Enable Receive Buffer Full Interrupt       //MODIF1      (était 1)
*  
*  0b00000001
*    ||||||||__ Receive Buffer Full Int enabled
*    |||||||___ Overrun Int disabled
*    ||||||____ 
*    |||||_____>- Tx Status Change disabled
*    ||||______ 
*    |||_______>- Rx Status Change disabled
*    ||________ Status Change Int disabled
*    |_________ Wake-Up Int disabled
*


EnableInterrupts;
PITCE = 0x01;

for (;;)
{
}

}
*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
* ModuleISR: a description of the function functionName.
* Another line of the description.
*
* Parameters: None
* MSCAN0 RECEIVE ISR 
* DESCRIPTION:
* Interrupt asserted when a message has been received and shifted into 
* the foreground buffer of the receiver FIFO.
*
* Return : None
*/
/*#pragma CODE_SEG NON_BANKED

void interrupt CAN0RxISR(void) {
unsigned char length, index;
unsigned char rxdata[8];

length = (CAN0RXDLR & 0x0F);
for (index=0; index<length; index++)
rxdata[index] = *(&CAN0RXDSR0 + index); // Get received data

CAN0RFLG = 0x01;   //Clear RXF
}*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
//procédure appelée lorsque le buffer de transmission est vide et est prêt à transmettre
void interrupt CANTX_trans(void) {
//lancement Timer 0  
PITCE = 0x01; //channel 0 enable.
CAN0TIER_TXEIE0 = 0;
}
*/
//#pragma CODE_SEG DEFAULT
