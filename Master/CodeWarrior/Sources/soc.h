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
* Parameters: V_min         : le voltage de la cellul la moins chargée
*             T_moyenne     : Température moyenne des cellules
*             presSOC       : avant dernier SOC calculé
*Return       lastSOC       : dernier SOC calculé                  :      
*/

void calcul_avec_tension(void);
//*************
//  Functions
//*************  
      
/*
* calcul_avec_courant: calcul le soc en integrant le courant entrant. 
* 
*
* Parameters: Cellcurent  : vecteur conteant les 100 derniéres mesures de courant
*             lasrpres    : la derniere valeur de soc calculée 
*             presSoc     : l'avant deriner SOC calculé 
* Return : lastSOC        : dernier SOC calculé
*/
unsigned int calcul_avec_courant ( long int * gCurrentMsr,unsigned int presSOC, unsigned int lastSOC);
//*************
//  Functions
//*************      
/*
* calcul_voltage_min:  trouve la cellule la moins chargé et garde son voltage. 
* Parameters: CellVolt [] : tableaux contenant les voltages de toutes les cellules
* Return :    voltage_min
*/
//unsigned int calcul_V_min (unsigned int *CellVolt[], unsigned int V_min);
//*************
//  Functions
//*************     
/*
* calcul_temperature_voltage: Calcul la temperature moyenne. 
* Parameters: CellTemp [] : tableau  conteant les températures de toutes les cellules
* Return :    temperature_moyenne  : température moyenne des cellules 
*/

void  calcul_temperature_moyenne (void);

//*************
//  Functions
//*************     
/*
* nb_cycle_cellules: Calcul la temperature moyenne. 
* Parameters: lastSOC : dernier SOC calculé
*             presSOC : avant dernier SOC calculé 
* Return :    NB_CYCLE  : 
*/


int nb_cycle_cellules (unsigned int lastSOC,unsigned int presSOC, int NB_CYCLE);
     
#endif 