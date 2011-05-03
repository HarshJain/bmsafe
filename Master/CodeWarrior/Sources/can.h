#ifndef CAN_H
#define CAN_H

#include <hidef.h>
#include "defines.h"

//**************************
//  Variables declaration
//************************** 
               
extern unsigned int RXID;
extern uchar idleCount[N_MOD_MAX];
extern uchar addrCANLSB;		                  //adresse du module CAN récepteur
extern uchar addrCANMSB;



//************************
//  Functions prototypes
//************************ 

//void init_portM(void);
//void init_CAN0(uchar addrLSB, uchar addrMSB);

unsigned char CAN0SendInt(unsigned long id, unsigned char priority, unsigned char start,
                           unsigned char length, unsigned int *txdata );
unsigned char CAN0SendCommand(unsigned char command, unsigned char slaveID,
                              unsigned int param1,   unsigned int  param2);
unsigned char CAN1SendChar(unsigned long id, unsigned char priority,
                           unsigned char length, unsigned char *txdata );


#endif