#include "common.h"
#include "derivative.h"
#include "defines.h"

/***********************
      Common functions
************************/


// Find biggest unsigned integer
unsigned int findMaxUint(unsigned int *table, unsigned char numElements, unsigned char *index)
{
	unsigned int max = table[0];
	unsigned char i = 0;
	
	for(i=1; i<numElements; i++) {
		if (table[i] > max) {
			max = table[i];
			*index = i;
		}
	}
	
	return max;
}

                
// Find smallest unsigned integer
unsigned int findMinUint(unsigned int *table, unsigned char numElements, unsigned char *index)
{
	unsigned int min = table[0];
	unsigned char i;
	
	for(i=0; i<numElements; ++i) {
		if (table[i] < min) {
			min = table[i];
			*index = i;
		}
	}
	
	return min;
}


// Find biggest integer
int findMaxInt(int *table, unsigned char numElements, unsigned char *index)
{
	int max = table[0];
	unsigned char i = 0;
	
	for(i=1; i<numElements; i++) {
		if (table[i] > max) {
			max = table[i];
			*index = i;
		}
	}
	
	return max;
}

                
// Find smallest integer
int findMinInt(int *table, unsigned char numElements, unsigned char *index)
{
	int min = table[0];
	unsigned char i;
	
	for(i=0; i<numElements; ++i) {
		if (table[i] < min) {
			min = table[i];
			*index = i;
		}
	}
	
	return min;
}

//******************************************************************************
// Finds min & max temperature
//******************************************************************************

void temperatureCompare(int cellTemp[][N_CELL], int *lowestT, int *maxT)
{
  unsigned char indexMax = 0;
  unsigned char indexMin = 0;
  unsigned char i = 0;
  int lowTable[N_MOD];
  int maxTable[N_MOD];
  
  for(i=0; i<N_MOD; i++){
    lowTable[i] = findMinInt(cellTemp[i], N_CELL, &indexMin);
    maxTable[i] = findMaxInt(cellTemp[i], N_CELL, &indexMax);                
  }

  *lowestT = findMinInt(lowTable, N_MOD, &indexMin);
  *maxT =    findMaxInt(maxTable, N_MOD, &indexMax);
}


//******************************************************************************
// Finds min & max voltages
//******************************************************************************

void voltageCompare(unsigned int cellVolt[][N_CELL], unsigned int *lowestV, unsigned int *maxV)
{
  unsigned char indexMax = 0;
  unsigned char indexMin = 0;
  unsigned char i = 0;
  unsigned int lowTable[N_MOD];
  unsigned int maxTable[N_MOD];
  
  for(i=0; i<N_MOD; i++){
    lowTable[i] = findMinUint(cellVolt[i], N_CELL, &indexMin);
    maxTable[i] = findMaxUint(cellVolt[i], N_CELL, &indexMax);                
  }

  *lowestV = findMinUint(lowTable, N_MOD, &indexMin);
  *maxV =    findMaxUint(maxTable, N_MOD, &indexMax);
}