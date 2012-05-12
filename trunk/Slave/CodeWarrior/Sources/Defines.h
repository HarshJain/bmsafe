#ifndef DEFINES_H
#define DEFINES_H


typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;


//Global variables declaration
extern unsigned long gElapsedTime;
extern unsigned char gSPItimeout;
extern unsigned char gSlaveID;
extern int gTemp[10];
extern uint8 gADC0done;
extern uint8 gADC1done;
extern uint8 gVoltTimeout;
extern uint8 gBalanceFlag;
extern uint8 gEquiStatusChange;
extern uint16 gBalanceVector;
extern uint16 gBalThres;

//------------------
// BMS Parameters
//------------------

#define NB_CELL    	   10		/* Number of cells for this slave */
#define BAL_DELTA_VOLT  0     /* Balancing margin. Reaching voltage_goal-margin stops balancing*/


typedef struct errors_t
{
   uint8 spiTimeout : 1;         // 1 = au moins un timeout s'est produit sur la communication SPI
   uint8 badSlaveId : 1;         // 1 = l'id du slave n'a pas été déterminé correctement
   uint8 canTxError : 1;           // La variable contient le numéro de l'erreur de transmission.
} errors_t;


#endif //DEFINES_H