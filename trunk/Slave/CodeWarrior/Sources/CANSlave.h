#ifndef CANSLAVE_H
#define CANSLAVE_H

#include "Defines.h"


void CANInit(void);
unsigned char CAN0SendChar(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata );
unsigned char CAN0SendInt(unsigned long id, unsigned char priority, unsigned char start, unsigned char length, unsigned int *txdata );
unsigned char CAN0SendInt2(unsigned long id, unsigned char priority, unsigned char length, unsigned int *txdata );
unsigned char CAN0SendCommand(unsigned char command, unsigned char id, unsigned int param1, unsigned int param2);
void CAN0SendVoltages(void);
void CAN0SendTemp(void);

extern unsigned char idOffset;
extern unsigned char slaveID ;

#endif