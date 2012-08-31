#ifndef TERMIO_H
#define TERMIO_H

 
void TERMIO_PutChar(char ch);  
char TERMIO_GetChar(void);  
void TERMIO_Init(void); 
void TERMIO_PutString(const char *buffer);

#endif

