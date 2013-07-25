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


//******************************************************
//	Retourne l'index de l'esclave ayant l'ID id
//**********************************************************

int indexOf(uint8 id)
{
	int i;
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

uint8 idOf(int index)
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

int addId(uint8 id)
{
	int i;
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
	int i;
	for(i = 0; i < N_MOD; i++)
	{
		idTable[i] = 0;
	}
}

void resetIgnoreTempTable()
{
	int i;
	for(i = 0; i < N_MAX_IGNORE_TEMP; i++)
	{
		gCellIgnoreTemp[i][0] = -1;
		gCellIgnoreTemp[i][1] = 0;
	}
}


// ***********************************************************************
// Modifie l'affichage de l'afficheur 7 segments selon le paramètre donné
//	Paramètre :
//	0 à 9 affiche les chiffres de 0 à 9
//  A,E,C : affiche la lettre correspondante
//  '.' : allume le point
//  ',' : éteint le point
// ***********************************************************************

void display(int no)
{
	switch(no)
	{
		case 0:
		// Affichage de 0
			SEGMENT_A = 0; // Éteint = 1
			SEGMENT_B = 0; // Allumé = 0
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 1;
			break;
	
		case 1:
		// Affichage de 1
			SEGMENT_A = 1;
			SEGMENT_B = 0;
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 1;
			break;
			
		case 2:
		// Affichage de 2
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
			break;
			
		case 3:
		// Affichage de 3
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
			break;
			
		case 4:
		//Affichage de 4
           SEGMENT_A = 1;
           SEGMENT_B = 0; 
           SEGMENT_C = 0;
           SEGMENT_D = 1;
           SEGMENT_E = 1;
           SEGMENT_F = 0;
           SEGMENT_G = 0;
			break;
			
		case 5:
		//Affichage de 5
           SEGMENT_A = 0;
           SEGMENT_B = 1; 
           SEGMENT_C = 0;
           SEGMENT_D = 0;
           SEGMENT_E = 1;
           SEGMENT_F = 0;
           SEGMENT_G = 0;
			break;
			
		case 6:
		//Affichage de 6
			SEGMENT_A = 0;
			SEGMENT_B = 1; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 7:
		//Affichage de 7
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 1;
			break;
			
		case 8:
		//Affichage de 8
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 9:
		//Affichage de 9
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
		
		case 'A': // no = 65
		//Affichage de la lettre A
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
		
		case 'C': //no = 67
		//Affichage de la lettre C
           SEGMENT_A = 0;
           SEGMENT_B = 1; 
           SEGMENT_C = 1;
           SEGMENT_D = 0;
           SEGMENT_E = 0;
           SEGMENT_F = 0;
           SEGMENT_G = 1;
			break;
			
		case 'E': //no = 69
		//Affichage de E
			SEGMENT_A = 0;
			SEGMENT_B = 1;
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 'F': // no = 70
			SEGMENT_A = 0;
			SEGMENT_B = 1;
			SEGMENT_C = 1;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 'P':
		// Affichage de P
			SEGMENT_A = 0;
			SEGMENT_B = 0;
			SEGMENT_C = 1;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case '.': // no = 46
			DOT = 0;
			break;
			
		case ',': // no = 44
			DOT = 1;
			break;		
			
		default:
		//Affichage de trois bandes horizontales
			SEGMENT_A = 0;
			SEGMENT_B = 1; 
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
	}
	
}


