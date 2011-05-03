#ifndef SOC_H
#define SOC_H

#include <hidef.h>
#include "defines.h"

//*************
//  Functions
//*************    

void initTables(void);
  
/*
* calcul_avec_tension       : Calcul le SOC. 
* Parameters: V_min         : le voltage de la cellul la moins charg�e
*             T_moyenne     : Temp�rature moyenne des cellules
*             presSOC       : avant dernier SOC calcul�
*Return       lastSOC       : dernier SOC calcul�                  :      
*/

void calcul_avec_tension(void);
//*************
//  Functions
//*************  
      
/*
* calcul_avec_courant: calcul le soc en integrant le courant entrant. 
* 
*
* Parameters: Cellcurent  : vecteur conteant les 100 derni�res mesures de courant
*             lasrpres    : la derniere valeur de soc calcul�e 
*             presSoc     : l'avant deriner SOC calcul� 
* Return : lastSOC        : dernier SOC calcul�
*/
unsigned int calcul_avec_courant ( long int * gCurrentMsr,unsigned int presSOC, unsigned int lastSOC);
//*************
//  Functions
//*************      
/*
* calcul_voltage_min:  trouve la cellule la moins charg� et garde son voltage. 
* Parameters: CellVolt [] : tableaux contenant les voltages de toutes les cellules
* Return :    voltage_min
*/
//unsigned int calcul_V_min (unsigned int *CellVolt[], unsigned int V_min);
//*************
//  Functions
//*************     
/*
* calcul_temperature_voltage: Calcul la temperature moyenne. 
* Parameters: CellTemp [] : tableau  conteant les temp�ratures de toutes les cellules
* Return :    temperature_moyenne  : temp�rature moyenne des cellules 
*/

void  calcul_temperature_moyenne (void);

//*************
//  Functions
//*************     
/*
* nb_cycle_cellules: Calcul la temperature moyenne. 
* Parameters: lastSOC : dernier SOC calcul�
*             presSOC : avant dernier SOC calcul� 
* Return :    NB_CYCLE  : 
*/


int nb_cycle_cellules (unsigned int lastSOC,unsigned int presSOC, int NB_CYCLE);
     
#endif 