#include "common.h"
#include "derivative.h"
#include "defines.h"
#include "parameters.h"

extern int glowestT;
extern int gmaxT;
extern unsigned int glowestV;
extern unsigned int gmaxV;
extern int gCellTemp[N_MOD_MAX][N_CELL_SLV_MAX];
extern unsigned int gCellVolt[N_MOD_MAX][N_CELL_SLV_MAX];

/***********************
      Common functions
************************/


// Find max voltage
unsigned int find_MaxVolt(unsigned int *table, unsigned char numElements, unsigned char *index)
{
	unsigned int Vmax = table[0];
	unsigned char i = 0;
	
	for(i=1; i<numElements; i++)
	{
		if (table[i] > Vmax)
		{
			Vmax = table[i];
			*index = i;
		}
	}
	return Vmax;
}

                
// Find min voltage
unsigned int find_MinVolt(unsigned int *table, unsigned char numElements, unsigned char *index)
{

	unsigned int Vmin = table[0];
	unsigned char i;
	
	for(i=0; i<numElements; ++i)
	{
		if (table[i] < Vmin)
		{
			Vmin = table[i];
			*index = i;
		}
	}
	return Vmin;
}

// Find max temperature
int find_MaxTemp(int *table, unsigned char numElements, unsigned char *index)
{
	int Tmax = table[0];
	unsigned char i = 0;
	
	for(i=1; i<numElements; i++)
	{
		if (table[i] > Tmax)
		{
			Tmax = table[i];
			*index = i;
		}
	}
	return Tmax;
}

                
// Find min temperature
int find_MinTemp(int *table, unsigned char numElements, unsigned char *index)
{

	int Tmin = table[0];
	unsigned char i;
	
	for(i=0; i<numElements; ++i)
	{
		if (table[i] < Tmin)
		{
			Tmin = table[i];
			*index = i;
		}
	}
	return Tmin;
}

//*****************************************************************************
// delayMs
//
// Description: Function that waits a number of milliseconds.
//
//*****************************************************************************
void delayMs(unsigned int numMs)
{
   unsigned int count = 0;
   
   PITCE = (PITCE | 0x1);           //PIT0 (1 ms timeout) activated
   
   while(count < numMs) {
      
      while(!(PITTF & 0x1)) {}      //Wait for the timer to reach 0
      
      PITTF = (PITTF | 0x1);        //Clear the flag
      count++;
   }
   
   PITCE = (PITCE & 0xFE);          //PIT0 disabled
}


//******************************************************************************
// Finds min & max temperature
//******************************************************************************

void temperatureCompare(void)
{
  unsigned char indexMax = 0;
  unsigned char indexMin = 0;
  unsigned char i = 0;
  int lowT[N_MOD_MAX];
  int maxT[N_MOD_MAX];
  
  for(i=0; i<gMesuresParams.N_MOD; i++)
  {
    lowT[i] = find_MinTemp(gCellTemp[i], gMesuresParams.N_CELL, &indexMin);
    maxT[i] = find_MaxTemp(gCellTemp[i], gMesuresParams.N_CELL, &indexMax);                
  }

  glowestT = find_MinTemp(lowT, gMesuresParams.N_MOD, &indexMin);
  gmaxT =    find_MaxTemp(maxT, gMesuresParams.N_MOD, &indexMax);
}


//******************************************************************************
// Finds min & max voltages
//******************************************************************************

void voltageCompare(void)
{
  unsigned char indexMax = 0;
  unsigned char indexMin = 0;
  unsigned char i = 0;
  unsigned int lowV[N_MOD_MAX];
  unsigned int maxV[N_MOD_MAX];
  
  for(i=0; i<gMesuresParams.N_MOD; i++)
  {
    lowV[i] = find_MinVolt(gCellVolt[i], gMesuresParams.N_CELL, &indexMin);
    maxV[i] = find_MaxVolt(gCellVolt[i], gMesuresParams.N_CELL, &indexMax);                
  }

  glowestV = find_MinVolt(lowV, gMesuresParams.N_MOD, &indexMin);
  gmaxV =    find_MaxVolt(maxV, gMesuresParams.N_MOD, &indexMax);
}