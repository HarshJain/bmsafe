#ifndef TERMINAL_CODE_H
#define TERMINAL_CODE_H

#include "defines.h"

//**************************
//  Variables declaration
//************************** 

extern unsigned int niveau_1;       // 1 = paramètres, 2 = status, 3 = commandes
extern unsigned int niveau_2;
extern unsigned int num_module;     // numéro du slave à afficher
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
void get_cells_data(unsigned int volt[][N_CELL], int temp[][N_CELL], int numer_mod);


#endif //TERMINAL_CODE_H
