#ifndef COMMON_H
#define COMMON_H

#include <hidef.h>
#include "defines.h"

extern int16  gBalanceThreshold_mV; // Current balance threshold

///************************
//  Functions prototypes
//************************ 

void findMaxMin(int16 cellTable[][N_CELL], int16 **lowestAddr, int16 **highestAddr, uint8 *lowestSlaveID, uint8 *highestSlaveID, uint8 *lowestCellNumber, uint8 *highestCellNumber);
void updateMinMax(void);

// Relié à la table de correspondance
uint8 idOf(int16 index);
int16 addId(uint8 id);
int16 indexOf(uint8 id);
void resetIDtable(void);

// Initialisation functions
void initialiseMainVariables(void);

//Other
uint32 computePackVoltage(int16 cellVolt[][N_CELL]);

// Températures ignorées
void resetIgnoreTempTable(void);
uint8 isIgnoredTemp(uint8 slave_ID, uint8 cell_index);
uint8 addIgnoreTemp(uint8 slave_ID, uint8 cell_index);
uint8 ignoreThisTemperatureSensor(uint8 slave_ID, uint8 cell_index);

// Equilibration of the cells
void sendCellBalancingCommand(int16 voltageThreshold); // Sends a equilibration command to each slave
void stopCellBalancing(void);


#endif