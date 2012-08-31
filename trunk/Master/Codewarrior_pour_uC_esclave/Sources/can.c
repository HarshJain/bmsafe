#include <hidef.h>      /* common defines and macros */
#include "can.h"
#include "derivative.h"
#include "defines.h"

//*************
//  Functions
//************* 

unsigned char CAN0SendChar(unsigned int id, unsigned char priority, unsigned char length, unsigned char *txdata)
{
   unsigned char index = 0;
   unsigned char txbuffer = 0;

   if(length > 8)  return CAN_ERR_MAX_LEN;
   if (!CAN0TFLG)  return CAN_ERR_BUFFER_FULL;    /* Are all the tansmit buffers full? */

   CAN0TBSEL = CAN0TFLG;         /* Select lowest empty buffer */
   txbuffer = CAN0TBSEL;         /* Backup selected buffer */
   CAN0TXIDR0 = (unsigned char) (id>>3);  // Load Id to IDR Registers. Id is on 11 bits,
   CAN0TXIDR1 = (unsigned char) (id<<5);  // and must be left justified.
                                                
   /* Load data to Tx buffer Data Segment Registers */
   for (index=0; index<length; index++)
      *(&CAN0TXDSR0 + index) = txdata[index];   

   CAN0TXDLR = length;           /* Set data length code */
   CAN0TXTBPR = priority;        /* Set priority. As compared to other buffered TX messages */
   CAN0TFLG = txbuffer;          /* Start transmission. Write of 1 clears the flag. */

   while ( (CAN0TFLG & txbuffer) != txbuffer);  /* Wait for transmission completion.
                                                * When the buffer is empty, the flag goes to 1.*/
   return CAN_NO_ERR;
}
 
 
unsigned char CAN0SendEquiCommand(unsigned int balVector, unsigned int balThres, unsigned char slaveId)
{
    unsigned int txdata[2];  
    
    txdata[0] = balVector;
    txdata[1] = balThres;

    return CAN0SendChar(   MAKE_CAN_ID(slaveId, CAN_EQUI_COMMAND_ID),
                           CAN_EQUI_COMMAND_PRIORITY,
                           4,
                           (unsigned char*) txdata);
}


unsigned char CAN0RequestSlaveFirmware(unsigned char slaveId)
{
    return CAN0SendChar(   MAKE_CAN_ID(slaveId, CAN_REQUEST_FIRMWARE_REV_ID),
                           CAN_REQUEST_FIRMWARE_REV_PRIORITY,
                           0,
                           NULL);
}


 unsigned char CAN1SendChar(unsigned int id, unsigned char priority, unsigned char length, unsigned char *txdata)
{
   unsigned char index = 0;
   unsigned char txbuffer = 0;

   if(length > 8)  return CAN_ERR_MAX_LEN;
   if (!CAN1TFLG)  return CAN_ERR_BUFFER_FULL;    /* Are all the tansmit buffers full? */

   CAN1TBSEL = CAN1TFLG;         /* Select lowest empty buffer */
   txbuffer = CAN1TBSEL;         /* Backup selected buffer */
   CAN1TXIDR0 = (id<<5);         /* Load Id to IDR Registers. Id is on 11 bits,
                                                                              * and must be left justified. */
                                                
   /* Load data to Tx buffer Data Segment Registers */
   for (index=0; index<length; index++)
      *(&CAN1TXDSR0 + index) = txdata[index];   

   CAN1TXDLR = length;           /* Set data length code */
   CAN1TXTBPR = priority;        /* Set priority. As compared to other buffered TX messages */
   CAN1TFLG = txbuffer;          /* Start transmission. Write of 1 clears the flag. */

   while ( (CAN0TFLG & txbuffer) != txbuffer);  /* Wait for transmission completion.
                                                 * When the buffer is empty, the flag goes to 1.*/
   return CAN_NO_ERR;
}


