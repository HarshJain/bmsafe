#include <termio.h>
#include <hidef.h>          /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define TERMIO_SPEED  1


char TERMIO_GetChar(void) {

   // NOTE: when using simulator you may want to open the 'Terminal' component
   while (!(SCI0SR1 & 0x20)){};     /* wait for input */
   return SCI0DRL;
}


void TERMIO_PutChar(char ch) {

   while (!(SCI0SR1 & 0x80)) {};    /* wait for output buffer empty */
   SCI0DRL = ch;
}


void TERMIO_Init(void) {
 
   SCI0BDL = TERMIO_SPEED;    // for the simulator we use 1 to speed up the output. On hardware please use 52 (9600 bps). */
   SCI0CR2 = 0x0C;            // 8 bit, TE and RE set */   
}


void TERMIO_PutString(const char *buffer){
   
  int i=0;
  
  while (buffer[i] != '\0') {
  
      TERMIO_PutChar(buffer[i]);
      i++;
  }
}

