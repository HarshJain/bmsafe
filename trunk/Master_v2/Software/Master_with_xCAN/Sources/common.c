#include "common.h"
#include "derivative.h"
#include "defines.h"
#include "TimedFunction.h"


/***********************
      Common functions
************************/
int16  gBalanceThreshold_mV = 0; // Actual balance threshold in mV

//******************************************************************************
// Find the address of the highest and lowest cell voltage or temperature (same algorithm)
// Parameters :
//	(input)		cellTable : voltage table / temperature table
//	(output)	lowestAddr / highestAddr : table address of the lowest / highest
//	(output)	lowestSlaveID / highestSlaveID : ID of the slave which has the corresponding cell
//	(output)	lowestCellNumber / highestCellNumber : cell number of the lowest / highest
//******************************************************************************
void findMaxMin(uint16 cellTable[][N_CELL], int16 **lowestAddr, int16 **highestAddr, uint8 *lowestSlaveID, uint8 *highestSlaveID,
				uint8 *lowestCellNumber, uint8 *highestCellNumber)
{
	uint8 i,j;
	int16 max, min; // signed type for supporting negative temperature measurement
	
	// Initialisation with first table element
	max = cellTable[0][0];
	min = cellTable[0][0];
	*lowestAddr = &cellTable[0][0];
	*highestAddr = &cellTable[0][0];
	*lowestSlaveID = idOf(0);
	*highestSlaveID = idOf(0);
	
	// Search the min and max
	for(i = 0; i < N_MOD; i++)	// First dimension (array)
	{
		for(j = 0; j < N_CELL; j++)	// Second dimension (array)
		{
			if(cellTable[i][j] > max)	// If it is a new maximum
			{
				max = cellTable[i][j];
				*highestAddr = &cellTable[i][j];
				*highestSlaveID = idOf(i);
				*highestCellNumber = j;
			}
			else if(cellTable[i][j] < min) // If it is a new minimum
			{
				min = cellTable[i][j];
				*lowestAddr = &cellTable[i][j];
				*lowestSlaveID = idOf(i);
				*lowestCellNumber = j;
			}
		}
	}
}


//******************************************************
//	Retourne l'index de l'esclave ayant l'ID id
//**********************************************************

int16 indexOf(uint8 id)
{
	int16 i;
	for(i = 0; i < N_MOD; i++)
	{
		if(idTable[i] == id)
			return i;
	}
	
	// Retourne -1 si l'ID n'est pas dans le tableau
	return -1; 
}

//*********************************************************
//	Retourne l'ID d'un eslave correspondant à l'index donné ou -1 si l'index est invalide
//**********************************************************

uint8 idOf(int16 index)
{
	if(index < N_MOD && index >= 0)
		return idTable[index];
	else
		return -1; // Ne devrait jamais arriver ici pour un index valide
}

//***************************************************************************
//	Initialise une nouvelle correspondance dans la table de correspondance ID / Index
//		Retour : l'index de l'ID ajouté ou -1 s'il n'y a plus de place dans le tableau
//		
//		Si -1 est retourné, cela signifie probablement qu'il y a plus d'esclaves de branchés que N_MOD
//****************************************************************************

int16 addId(uint8 id)
{
	int16 i;
	for(i = 0; i < N_MOD; i++)
	{
		if(idTable[i] == 0)
		{
			idTable[i] = id;
			return i;
		}
		else if(idTable[i] == id) // L'index est déjà dans le tableau
		{
			return i;
		}
	}
	
	return -1; // Si le tableau est plein
}

void resetIDtable()
{
	int16 i;
	for(i = 0; i < N_MOD; i++)
	{
		idTable[i] = 0;
	}
}

void resetIgnoreTempTable()
{
	int16 i;
	for(i = 0; i < N_MAX_IGNORE_TEMP; i++)
	{
		gCellIgnoreTemp[i][0] = -1;	// slave ID
		gCellIgnoreTemp[i][1] = 0;	// Cell #
	}
	
	gNumberOfIgnoredTemp = 0;
}

uint8 addIgnoreTemp(uint8 slave_ID, uint8 cell_index)
{
	//On ajoute la sonde à ignorer à la liste
	if(gNumberOfIgnoredTemp < N_MAX_IGNORE_TEMP)
	{
		gCellIgnoreTemp[gNumberOfIgnoredTemp][0] = slave_ID;
		gCellIgnoreTemp[gNumberOfIgnoredTemp][1] = cell_index;
		
		gNumberOfIgnoredTemp++;
		return 1;
	}
	
	return 0;
}


// Returns 1 if the temperature sensor of this cell is ignored, returns 0 otherwise
uint8 isIgnoredTemp(uint8 slave_ID, uint8 cell_index)
{
	int i = 0;
	
	for(i = 0; i < gNumberOfIgnoredTemp; i++)
	{
		if(slave_ID == gCellIgnoreTemp[i][0] && cell_index == gCellIgnoreTemp[i][1])
			return 1;
	}
	
	return 0;
}


// Decide wheater or not to ignore the temperature of a particular sensor
//
//	We ignore the temperature sensor if the temperature of the adjacent temperature sensor have a big difference with the one
//	we are looking at. This means there is a problem with that sensor since there can not be a big difference between two
//	temperature values of adjacent cells (cell electrodes are connected together and close one to the other, there is a good
//	temperature transfer between the cells. We do not ignore if one of the adjacent temperature sensor is already ignored.
//
// Returns 1 if we decide to ignore this temperature sensor
// Returns 0 if we do not want to ignore this temperature sensor
uint8 ignoreThisTemperatureSensor(uint8 slave_ID, uint8 cell_index)
{
	uint8 index = indexOf(slave_ID);
	
	if(isIgnoredTemp(slave_ID, cell_index))
		return 1;	// The temperature sensor is already ignored	
	
	//
	// If the thermistance is disconnected (open circuit), the temperature will be at its minimum (-40°C)
	// In this case we ignore
	//
	if(gCellTemp[slave_ID][cell_index] == -400) // TODO 181 : Magic number (change -400 by a define or variable)
		return addIgnoreTemp(slave_ID,cell_index); // In this case, we ignore
	
	
	//
	// Check if an adjacent sensor is already ignored
	//
	if(cell_index != 0)
		if(isIgnoredTemp(slave_ID, cell_index - 1))
			return 0;		// Do not ignore if the adjacent sensor is already ignored

	if(cell_index != N_CELL - 1)
		if(isIgnoredTemp(slave_ID, cell_index + 1))
			return 0;		// Do not ignore if the adjacent sensor is already ignored
	
	//
	// If the temperature is an extreme, we test if we want to ignore or not
	//
	
	if(gCellTemp[slave_ID][cell_index] < gParams.minDischargeCellTemp)
	{
		if(cell_index == 0) // It is the first sensor
			return ((gCellTemp[index][cell_index + 1] - gCellTemp[slave_ID][cell_index]) > ECART_MIN_TEMP);
		else if(cell_index == N_CELL - 1)	// It is the last sensor
			return ((gCellTemp[index][cell_index - 1] - gCellTemp[slave_ID][cell_index]) > ECART_MIN_TEMP);
		else
		{
			if(	(gCellTemp[index][cell_index + 1] - gCellTemp[slave_ID][cell_index]) > ECART_MIN_TEMP && 	// Difference between the previous temperature is big
				(gCellTemp[index][cell_index - 1] - gCellTemp[slave_ID][cell_index]) > ECART_MIN_TEMP)		// Difference between the next temperature is big
				return addIgnoreTemp(slave_ID,cell_index);	// Ignore temperature sensor
			else
				return 0;	// Don't ignore temperature sensor
		}
	}
	else if (gCellTemp[slave_ID][cell_index] > gParams.maxDischargeCellTemp)
	{
		if(cell_index == 0) // It is the first sensor
			return (gCellTemp[slave_ID][cell_index] - (gCellTemp[index][cell_index + 1]) > ECART_MIN_TEMP);
		else if(cell_index == N_CELL - 1)	// It is the last sensor
			return (gCellTemp[slave_ID][cell_index] - (gCellTemp[index][cell_index - 1]) > ECART_MIN_TEMP);
		else
		{
			if(	gCellTemp[slave_ID][cell_index] - (gCellTemp[index][cell_index + 1]) > ECART_MIN_TEMP && 	// Difference between the previous temperature is big
				gCellTemp[slave_ID][cell_index] - (gCellTemp[index][cell_index - 1]) > ECART_MIN_TEMP)		// Difference between the next temperature is big
				return addIgnoreTemp(slave_ID,cell_index);	// Ignore temperature sensor
			else
				return 0;	// Don't ignore temperature sensor
		}
	}
	else
		return 0;	// Do not ignore
}

void updateMinMaxVoltage()
{
	// Find the minimum and maximum and update pointers and related variables
	findMaxMin(gCellVolt, &gLowestCellVoltage,	&gHighestCellVoltage, &gLowestVoltageCellSlaveId, &gHighestVoltageCellSlaveId,
						&gLowestVoltageCellNum, &gHighestVoltageCellNum);
}

void updateMinMaxTemperature()
{
	int16 *temp_high; 
	int16 *temp_low;
	uint8 id_low;
	uint8 id_high;
	uint8 num_low;
	uint8 num_high;
	
	
	// Find the minimum and maximum and update pointers and related variables
	findMaxMin(gCellTemp, &temp_low, &temp_high, &id_low, &id_high,
				&num_low, &num_high);
	

	// We update the values if we don't ignore these temperatures
	
	if(!ignoreThisTemperatureSensor(id_low,num_low)) // Lowest temperature
	{		
		gLowestCellTemp = temp_low;
		gLowestTempCellSlaveId = id_low;
		gLowestTempCellNum = num_low;
	}
	
	if(!ignoreThisTemperatureSensor(id_high,num_high)) // Highest temperature
	{
		gHighestCellTemp = temp_high;
		gHighestTempCellSlaveId = id_high;
		gHighestTempCellNum = num_high;
	}
		

}

void initialiseMainVariables()
{
	//These initialization values assure us that
    //the system stays in the error state until all the
    //real values are received. gSlaveReset and gSlaveRev
    //are not critical.
	uint i,j;  
	for(i=0; i < N_MOD; i++)	// First dimension
	{
        for(j=0; j < N_CELL; j++)	// Second dimension
		{
            gCellVolt[i][j] = 0;		// 0V
            gCellTemp[i][j] = 0x8000;	// 800°C
        }
		
        idleCount[i] = COM_IDLE_COUNT_MAX;
        gSlaveReset[i] = 0;
        gSlaveRev[i] = 0;
		idTable[i] = 0;
    }
	
	
	// Flags initialisation
	gFlags.equilibrating = 0;               // 1= Indique la batterie est en équilibration
	gFlags.interlockClosed = 0;             // 1= le circuit d'interlock est fermé
	gFlags.errorState = 1;                  // 1= une erreur a été détectée par le passée et n'a pas été correctement réinitialisée
	gFlags.errorReset = 0;                  // 1= On a appuyé sur le bouton pour le reset des erreurs du BMS et du IMD.
	gFlags.ImdError = 0;                    // 1= indique qu'une faute de masse a été détectée par le IMD.
	gFlags.cellLowVolt = 1;                 // Une cellule a une tension sous le seuil dangereux
	gFlags.cellHighVolt = 1;                // Une cellule a une tension au-dessus du seuil dangereux
	gFlags.cellLowTemp = 1;                 // Une cellule a une température sous le seuil dangereux
	gFlags.cellHighTemp = 1;                // Une cellule a une température au-dessus du seuil dangereux
	gFlags.highPeakCurrent = 0;             // Le courant moyen sur 10 secondes est au-dessus d'un seuil dangereux
	gFlags.driveVoltageOutdated = 1;		// 1 = The drive voltage value is out of date
	gFlags.driveSendRequestVoltage = 0;		// 1 = timeout for sending a voltage request to the drive
	gFlags.currentMeasureOutdated = 1;		// 1 = the current measure is outdated
	gFlags.charger_online = 0;				// 1 = the charger is connected to the CAN network
	gFlags.TCS_online = 0;					// 0 = the BMS does not see the TCS on the CAN network
	gFlags.drive_online = 0;				// 0 = the drive does not seem to be connected on the network
	
}



uint32 computePackVoltage(int16 cellVolt[][N_CELL])
{
    uint8 i, j;
    uint32 packVoltage = 0;
    
    for(i=0; i < N_MOD; i++){
        for(j=0; j < N_CELL; j++) {
            packVoltage = packVoltage + (uint32) cellVolt[i][j];
        }
    }
    
    return packVoltage;
}



void sendCellBalancingCommand(int16 voltageThreshold)
{
	uint8 i,j;
	uint8 balancingDone = 1;
	int16 balVector; // Balancing vector bit 0 = first cell, bit 11 = last cell (higher voltage)
	
	// Ensure correct voltageThreshold
	if(voltageThreshold < gParams.minCellVoltage ||	voltageThreshold > gParams.maxCellVoltage)
		return;
	
	// Build the balance vector for each battery pack (each slave circuit)
	for(i = 0; i < N_MOD; i++)
	{
		balVector = 0;
		for(j = 0; j < N_CELL; j++)
		{
			if(gCellVolt[i][j] > voltageThreshold)
			{
				// Ensure the temperature is ok before activating discharge for this cell
				if(gParams.highDischargeCellTemp > gCellTemp[i][j])
				{
					balVector |= (1 << j);	// Set the corresponding bit to 1 to activate discharge on this cell
				}
				balancingDone = 0; // As long as there is a cell with a voltage higher than the threshold
			}
		}
		
		// Send the balancing command to the slave
		CAN_SendBalancingCommand(idOf(i),voltageThreshold, balVector);
	}
	
	gFlags.equilibrating = 1;
	
	if(balancingDone == 1)
		stopCellBalancing();
}

void stopCellBalancing()
{
	uint8 i;
	
	gBalanceThreshold_mV = 0;
	
	
	// Build the balance vector for each battery pack (each slave circuit)
	for(i = 0; i < N_MOD; i++)
	{
		// Send the balancing command to the slave
		CAN_SendBalancingCommand(idOf(i),gParams.maxCellVoltage, 0); // Null balance vector
	}
	// Note : We could broadcast a Banlancing command instead to stop balancing, there would be less
	// messages sent for the same result
	
	gFlags.equilibrating = 0;
}

void manage_balancing()
{
	if(gFlags.equilibrating)
	{
		if(gMode == ERROR_MODE || gMode == NORMAL_MODE)
		{
			stopCellBalancing();
			return;
		}
		
		
		if(getTime() - timeRef_lastBalanceCommand > BMS_BALANCING_MSG_DELAY_MS)
		{
		
			// In charge mode, we need to ajust the threshold
			if(gMode == CHARGE_MODE || gMode == STAND_BY_MODE)
			{
				// The threshold is adjusted to the lowest voltage plus a fuzz factor
				gBalanceThreshold_mV = *gLowestCellVoltage + BALANCING_FUZZ_VALUE_mV;
			}		
		
			// Send the command
			sendCellBalancingCommand(gBalanceThreshold_mV);
			timeRef_lastBalanceCommand = getTime();
		}
		
	}
	else if(gMode == CHARGE_MODE)
	{
		// Start equilibrating automatically when in charge mode and
		// when the lowest cell voltage is over the mid value between the min and max voltage
		if(*gLowestCellVoltage > (gParams.maxCellVoltage + gParams.minCellVoltage)/2)
		{
			gFlags.equilibrating = 1;
		}
	}
}

