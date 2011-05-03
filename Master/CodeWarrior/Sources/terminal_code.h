#ifndef TERMINAL_CODE_H
#define TERMINAL_CODE_H

#include "defines.h"

//**************************
//  Variables declaration
//************************** 

extern char *main_menu[4];

extern char *configuration_menu[5];
extern char *status_menu[4];
extern char *commande_menu[7];

extern char *configuration_menu_1[16];
extern char *configuration_menu_2[6];
extern char *configuration_menu_3[8];
extern char *configuration_menu_4[3];

extern char *help_string[4]; 
extern char *config_string[4];


extern char *status_menu_1[8];
extern char *status_menu_2[2];
extern char *status_menu_3[1];

extern char *commande_menu_1[2];
extern char *commande_menu_2[2];


// autres strings utilis�s (pour afficher le nom 
// des param�tres modifi�s)
extern char *list_param_securite[15];
extern char *list_param_equilibrage[5];
extern char *list_param_mesure[11];
extern char *list_param_comm[2];


// syst�me d'index pour les menus

extern unsigned int niveau_1;  // 0 = configuration, 2 = status, 3 = commandes
extern unsigned int niveau_2;
extern unsigned int niveau_3;
extern unsigned int num_module; // num�ro du slave � afficher
extern unsigned int sci_interrupt_mode;

//************************
//  Functions prototypes
//************************ 

void SCIPutChar(char ch);
void SCIprintString(char* charBuf);
void SCIprintStatus(void);
void SCIprintErrors(void);
void SCIshowMenu(unsigned char input);
void SCIupAlevel(void);
void SCIassignation(float user_input);
void SCIcontinuous(void);

/*
* SCITx: Write data byte to SCIDRL register to transmission. 
* 
*
* Parameters: array[] : vecteur conteant les string � printer
*             index   : identification de l'array � printer
*             options : 0 saut de ligne (CR + LF)
*             options : 1 tab entre 2 donn�es
*
* Return : None
*/
void SCITx(char *array[], unsigned int index, unsigned int options);




/*
* show_menu: Affiche un menu 
*            L'usager peut interagir avec le MCU.
*                     
* Parameters:  Tableau de char* contenant les strings � afficher
*                              
*
* Return : 
*/ 
void show_menu(char *menu[], unsigned int nb_elements);




/*
* print_string: Convertie un int en un array de char et print dans le terminal.
* 
* Parameters: int arg: valeur � imprimer stock� dans un entier
*                      type : -0 aucune dimension
*                             -1 voltage
*                             -2 temperature                           
*
* Return : Aucun
*
* NOTE: Cette fonction fait juster shooter le data au terminal
*        (i.e ne g�re aucun saut de ligne)
*/ 
void print_string(int arg, int type);   
   



/*
* show_parameters: Affiche les param�tres actuel du BMS
*
* Parameters:  char* menu[] array contenant des strings � afficher.
*			         int parameter[] array contenant les valeurs des param�tres
* Return : aucun.
*/
void get_parameters(char *menu[], int param[]);




/*
* get_cells_data: Affiche les voltage et la temp�rature de chaque cellule
*
* Parameters:  int *volt[]: array 2 dimensions contenant les voltages
*			         int *temp[]: array 2 dimensions contenant les temp�ratures
*              int numer_mod : num�ro du module pour lequel on veut les informations
*              int nb_cell : nombre de cellule par module
* Return : aucun.
*/
void get_cells_data(unsigned int volt[][N_CELL_SLV_MAX], int temp[][N_CELL_SLV_MAX], int numer_mod, int nb_cell);


/*
* skip_lines: ex�cute 1 ou 2 sauts de ligne
*
* Parameters:  int *number: nombre de ligne � sauter.
*			        
*
* Return : aucun.
*/
void skip_lines (unsigned int number);


#endif //TERMINAL_CODE_H
