#ifndef DEFINES_H
#define DEFINES_H


#define N_MOD           10
#define N_CELL          10

#define N_CURR_MSR      32                         //Nombre de measures de courants utilisées pour calculer la moyenne mobile
#define K_HALL1         2342                        //Low range hall effet sensor gain. Égal à 18.30 * 128 
#define K_HALL2         27412                       //High range hall effet sensor gain. Égal à 214.16 * 128

#define COM_IDLE_COUNT_MAX       10       //Number of com timeout timer cycles without
                                          //communication from a slave before the error flag is set                                       

#define USER_INT_SPEED  26        //1: pour simulation,  52: 9600 bps,  26: 19200 bps,  13: 38400 bps,  9: 56000 bps

#define NORMAL_MODE     0
#define CHARGE_MODE     1
#define STAND_BY_MODE   2
#define ERROR_MODE      3

#define WARNING_LED         PTS_PTS7
#define INTERLOCK_STATE     PTS_PTS6
#define IGNITION            PTS_PTS5

#define INTERLOCK_CLOSED    1
#define INTERLOCK_OPEN      0



//----------------------
// Types
//----------------------

typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;



typedef struct
{    
    uint16 slaveTimeout : 1;                // kk Slave communication timeout 
    uint16 cellOpenConnection : 1;          // Cell open connection detected
    uint16 cellMaxVolt : 1;                 // A cell reached maximal voltage
    uint16 cellMinVolt : 1;                 // A cell reached minimal voltage
    uint16 cellMaxTemp : 1;                 // A cell reached maximal temperature
    uint16 cellMinTemp : 1;                 // A cell reached minimal temperature
    uint16 maxMeanDischargeCurrent : 1;     // Maximal mean current reached
    uint16 maxPeakCurrent : 1;              // Maximal peak current reached
    uint16 : 8;
} errors_t;

typedef struct
{
    uint16 equilibrating : 1;               // 1: Indique la batterie est en équilibration
    uint16 charging : 1;					// 1: on est en mode charge, utilise les paramètres appropriés dans la détection d'erreurs.
    uint16 ignition : 1;                    // 1: L'ignition est à ON
    uint16 interlockClosed : 1;              // 1: le circuit d'interlock est fermé
	uint16 allCellsKnown : 1;               // 1: On a reçu une valeur de tension et de température pour toutes les cellules
    uint16 currentKnown : 1;                // 1: on a mesuré au moins une fois le courant.
    uint16 interlockStateKnown : 1;         // 1: on a lu au moins une fois le interlock state.
    uint16 relaysClosed : 1;                // 1: Les relais sont fermés.
    uint16 cellLowVolt : 1;                 // Une cellule a une tension sous le seuil dangereux
    uint16 cellHighVolt : 1;                // Une cellule a une tension au-dessus du seuil dangereux
    uint16 cellLowTemp : 1;                 // Une cellule a une température sous le seuil dangereux
    uint16 cellHighTemp : 1;                // Une cellule a une température au-dessus du seuil dangereux
    uint16 highPeakCurrent : 1;                 // Le courant moyen sur 10 secondes est au-dessus d'un seuil dangereux
    uint16 : 3;
} flags_t;


typedef struct
{
   int minDischargeCellTemp;                    // [dixieme oC] Minimal operating temperature in normal mode
   int maxDischargeCellTemp;                    // [dixieme oC] Maximal operating temperature in normal mode
   int lowDischargeCellTemp;                    // [dixieme oC] Low threshold operating temperature in normal mode
   int highDischargeCellTemp;                   // [dixieme oC] High threshold operating temperature in normal mode
   int minChargeCellTemp;                       // [dixieme oC] Minimal operating temperature in charge mode
   int maxChargeCellTemp;                       // [dixieme oC] Maximal operating temperature in charge mode
   int lowChargeCellTemp;                       // [dixieme oC] Low threshold operating temperature in charge mode
   int highChargeCellTemp;                      // [dixieme oC] High threshold operating temperature in charge mode
   unsigned int minCellVoltage;                 // [mV] Minimal cell voltage
   unsigned int maxCellVoltage;                 // [mV] Maximal cell voltage
   unsigned int lowCellVoltage;                 // [mV] Low threshold cell voltage
   unsigned int highCellVoltage;                // [mV] High threshold cell voltage
   unsigned int maxMeanChargeCurrent;           // [A] Maximal charge current
   unsigned int maxMeanDischargeCurrent;        // [A] Maximal mean discharge current
   unsigned int highPeakDischargeCurrent;       // [A] Maximal mean discharge current
   unsigned int maxPeakDischargeCurrent;        // [A] Maximal peak discharge current
   unsigned char manualMode;                    // 1: aucune gestion des relais automatique.
} params_t;


//Global variables
extern uint16 gCellVolt[N_MOD][N_CELL];
extern uint16 *gHighestCellVoltage;
extern uint16 *gLowestCellVoltage;

extern int gCellTemp[N_MOD][N_CELL];
extern int *gHighestCellTemp;
extern int *gLowestCellTemp;

extern long int gMeanCurrent;  

extern errors_t gError;
extern flags_t gFlags;

extern uint8 gMode;
extern uint16 gSlaveEquiStatus; 
extern uint16 gSlaveComState;
extern uint8 idleCount[N_MOD];
extern uint8 gSlaveReset[N_MOD];
extern uint8 gSlaveRev[N_MOD];

extern params_t gParams;


#endif //DEFINES_H