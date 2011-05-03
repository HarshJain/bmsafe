#ifndef COMMON_H
#define COMMON_H

#include <hidef.h>
#include "defines.h"

//**************************
//  Variables declaration
//************************** 
               



//************************
//  Functions prototypes
//************************ 

unsigned int find_MaxVolt(unsigned int *tableau, unsigned char numElements, unsigned char* index);
unsigned int find_MinVolt(unsigned int *tableau, unsigned char numElements, unsigned char* index);
int find_MaxTemp(int *table, unsigned char numElements, uchar *index);
int find_MinTemp(int *table, unsigned char numElements, uchar *index);
void temperatureCompare(void);
void voltageCompare(void);
void delayMs(unsigned int numMs);


#endif