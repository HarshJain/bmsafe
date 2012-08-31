#ifndef TERMINAL_CODE_H
#define TERMINAL_CODE_H

#include "defines.h"


#define USER_INT_SPEED        26        //1: pour simulation,  52: 9600 bps,  26: 19200 bps,  13: 38400 bps,  9: 56000 bps
#define GUI_RX_BUFFER_SIZE    20


//**************************
//  Variables declaration
//************************** 

extern unsigned int gSciDisplayNumModule;                // numéro du slave à afficher
extern unsigned char gGuiBuffer[GUI_RX_BUFFER_SIZE];     //Un tampon contenant les bytes reçus du lien avec l'usager à traiter.
extern unsigned char gGuiReadIndex;
extern unsigned char gGuiWriteIndex;
extern unsigned char gGuiBufferFull;

//************************
//  Functions prototypes
//************************ 

void SCIPutChar(char ch);
void SCIprintString(char* charBuf);
void SCIprintStatus(void);
void SCIprintErrors(void);
void SCIprintFlags(void);
void SCIshowMenu(unsigned char input);
void SCIupAlevel(void);
void SCIassignation(float user_input);
void sciGetCellsData(unsigned int volt[][N_CELL], int temp[][N_CELL], int numer_mod);
void sciByteReception(unsigned char rcvByte);


#endif //TERMINAL_CODE_H
