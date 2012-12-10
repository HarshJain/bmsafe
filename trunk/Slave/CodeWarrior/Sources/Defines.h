#ifndef DEFINES_H
#define DEFINES_H

#include "type.h"

#define FIRMWARE_REVISION       4

/**
 * Changements depuis rev 3 :  mesure de tension 10x par seconde et moyenne mobile expo sur 31 mesures
 * mesure de température une fois par seconde et moyenne mobile expo sur 7 mesures
 * envoie des valeurs sur le bus CAN à un rythme différent de la prise de mesure
 */

//------------------
// BMS Parameters
//------------------

#define NB_CELL    	           10  // Number of cells for this slave 
#define VOLT_MEASURE_PERIOD     1  // Le nombre de dixièmes de secondes entre chaque mesure de tensions
#define TEMP_MEASURE_PERIOD    10  // Le nombre de dixièmes secondes entre chaque mesure de températures
#define VOLT_SEND_PERIOD       10  // Le nombre de mesures de tension entre chaque envoie sur le bus CAN
#define TEMP_SEND_PERIOD        5  // Le nombre de mesures de températures entre chaque envoie sur le bus CAN
#define BAL_DELTA_VOLT          0  // Balancing margin. Reaching voltage_goal minus margin stops balancing

//------------------
// Types
//------------------
typedef struct {
  uint16 balancingActive  : 1;	   // 0 = Normal mode ; 1 = Discharge Cell mode
  uint16 equiStatusChange  : 1;   // 1 = les registres de config. du mon. de batteries ont changes et doivent etre réenvoyés
  uint16 voltTimeout  : 1;        // 1 = les mesures de voltages doivent etre prises par le moniteur de batterie
  uint16 ADC0done  : 1;           // 1 = les mesures de l'ADC0 sont pretes a etre recuperees
  uint16 ADC1done  : 1;           // 1 = les mesures de l'ADC1 sont pretes a etre recuperees
  uint16 firmwareRequest  : 1;    // 1 = le maître demande l'envoie de la révision du firmware
  uint16 spiTimeout  : 1;         // 1 = au moins un timeout s'est produit sur la communication SPI
  uint16 badSlaveId  : 1;         // 1 = l'id du slave n'a pas été déterminé correctement
  uint16 canTxError  : 1;         // La variable contient le numéro de l'erreur de transmission.
  uint16  : 7;
} flags_t;

// Global variables declaration
extern uint32 gElapsedTime;
extern uint8 gSlaveID;
extern int16 gTemp[10];
extern int16 gTempRaw[NB_CELL];
extern uint16 gBalanceVector;
extern uint16 gBalThres;
extern flags_t gFlags;

#endif //DEFINES_H