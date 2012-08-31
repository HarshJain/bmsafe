#ifndef COMMON_H
#define COMMON_H

#include <hidef.h>
#include "defines.h"

///************************
//  Functions prototypes
//************************ 

unsigned int findMaxUint(unsigned int *tableau, unsigned char numElements, unsigned char* index);
unsigned int findMinUint(unsigned int *tableau, unsigned char numElements, unsigned char* index);
int findMaxInt(int *table, unsigned char numElements, uchar *index);
int findMinInt(int *table, unsigned char numElements, uchar *index);
void temperatureCompare(int cellTemp[][N_CELL], int *lowestT, int *maxT);
void voltageCompare(unsigned int cellVolt[][N_CELL], unsigned int *lowestV, unsigned int *maxV);



#endif