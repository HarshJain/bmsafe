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


// autres strings utilisés (pour afficher le nom 
// des paramètres modifiés)
extern char *list_param_securite[15];
extern char *list_param_equilibrage[5];
extern char *list_param_mesure[11];
extern char *list_param_comm[2];


// système d'index pour les menus

extern unsigned int niveau_1;  // 0 = configuration, 2 = status, 3 = commandes
extern unsigned int niveau_2;
extern unsigned int niveau_3;
extern unsigned int num_module; // numéro du slave à afficher
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
* Parameters: array[] : vecteur conteant les string à printer
*             index   : identification de l'array à printer
*             options : 0 saut de ligne (CR + LF)
*             options : 1 tab entre 2 données
*
* Return : None
*/
void SCITx(char *array[], unsigned int index, unsigned int options);




/*
* show_menu: Affiche un menu 
*            L'usager peut interagir avec le MCU.
*                     
* Parameters:  Tableau de char* contenant les strings à afficher
*                              
*
* Return : 
*/ 
void show_menu(char *menu[], unsigned int nb_elements);




/*
* print_string: Convertie un int en un array de char et print dans le terminal.
* 
* Parameters: int arg: valeur à imprimer stocké dans un entier
*                      type : -0 aucune dimension
*                             -1 voltage
*                             -2 temperature                           
*
* Return : Aucun
*
* NOTE: Cette fonction fait juster shooter le data au terminal
*        (i.e ne gère aucun saut de ligne)
*/ 
void print_string(int arg, int type);   
   



/*
* show_parameters: Affiche les paramètres actuel du BMS
*
* Parameters:  char* menu[] array contenant des strings à afficher.
*			         int parameter[] array contenant les valeurs des paramètres
* Return : aucun.
*/
void get_parameters(char *menu[], int param[]);




/*
* get_cells_data: Affiche les voltage et la température de chaque cellule
*
* Parameters:  int *volt[]: array 2 dimensions contenant les voltages
*			         int *temp[]: array 2 dimensions contenant les températures
*              int numer_mod : numéro du module pour lequel on veut les informations
*              int nb_cell : nombre de cellule par module
* Return : aucun.
*/
void get_cells_data(unsigned int volt[][N_CELL_SLV_MAX], int temp[][N_CELL_SLV_MAX], int numer_mod, int nb_cell);


/*
* skip_lines: exécute 1 ou 2 sauts de ligne
*
* Parameters:  int *number: nombre de ligne à sauter.
*			        
*
* Return : aucun.
*/
void skip_lines (unsigned int number);


#endif //TERMINAL_CODE_H
