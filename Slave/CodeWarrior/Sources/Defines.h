#ifndef DEFINES_H
#define DEFINES_H

#define FIRMWARE_REVISION       3

/*  Changements depuis rev 2 : plus de temp�ratures (jusqu'� 140 oC)
*/


//------------------
// BMS Parameters
//------------------

#define NB_CELL    	            10		/* Number of cells for this slave */
#define VOLT_MEASURE_PERIOD     1       //Le nombre de secondes entre chaque mesure de tensions
#define TEMP_MEASURE_PERIOD     5       //Le nombre de secondes entre chaque mesure de temp�ratures
#define BAL_DELTA_VOLT          0     /* Balancing margin. Reaching voltage_goal-margin stops balancing*/

//------------------
// Types
//------------------

typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;

typedef struct
{
    uint16 balancingActive : 1;	       // 0 = Normal mode ; 1 = Discharge Cell mode
    uint16 equiStatusChange : 1;   // 1 = les registres de config. du mon. de batteries ont changes et doivent etre r�envoy�s
    uint16 voltTimeout : 1;        // 1 = les mesures de voltages doivent etre prises par le moniteur de batterie
    uint16 ADC0done : 1;           // 1 = les mesures de l'ADC0 sont pretes a etre recuperees
    uint16 ADC1done : 1;           // 1 = les mesures de l'ADC1 sont pretes a etre recuperees
    uint16 firmwareRequest : 1;     // 1 = le ma�tre demande l'envoie de la r�vision du firmware
    uint16 spiTimeout : 1;         // 1 = au moins un timeout s'est produit sur la communication SPI
    uint16 badSlaveId : 1;         // 1 = l'id du slave n'a pas �t� d�termin� correctement
    uint16 canTxError : 1;           // La variable contient le num�ro de l'erreur de transmission.
    uint16  : 7;
} flags_t;


//Global variables declaration
extern unsigned long gElapsedTime;
extern unsigned char gSlaveID;
extern int gTemp[10];
extern uint16 gBalanceVector;
extern uint16 gBalThres;
extern flags_t gFlags;








#endif //DEFINES_H