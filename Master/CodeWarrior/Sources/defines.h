#ifndef DEFINES_H
#define DEFINES_H

//changements depuis dernier commit
// - Correction erreur temp max/min avec patch
// - ajout interrupt 'safety'
// - GUI traité dans main
// - params ignoreErrors et ignoreIntState
// - signaux errorReset et imdError
// - flags errorState
// - affichage des erreurs par menu
// - affichage des flags dans gui
// - toutes erreurs à l'initialisation
// - pression errorRst nécessaire pour reset erreurs bms et imd
// - ignition n'a plus aucun rôle dans le reset des erreurs
// - plus d'attente active lors de la fermeture des relais
// - limites courant stockées en mA au lieu de A pour éviter *1000
// - Calcul de la tension totale jsute une fois par seconde


#define N_MOD           10
#define N_CELL          10

#define N_CURR_MSR              32                   /Nombre de measures de courants utilisées pour calculer la moyenne mobile
#define K_HALL1                 2342                //Low range hall effet sensor gain. Égal à 18.30 * 128 
#define K_HALL2                 27412               //High range hall effet sensor gain. Égal à 214.16 * 128
#define CURRENT_MEASURE_PERIOD  10                  //Nombre de période de 10 ms entre chaque mesure de courant

#define PACK_VOLT_CALC_PERIOD   1         //Nombre de secondes entre chaque calcul de la tension totale du pack

#define COM_IDLE_COUNT_MAX     10       //Number of com timeout timer cycles (1 second) without
                                        //communication from a slave before the error flag is set
                                        //Must be < 255

#define NORMAL_MODE     0
#define CHARGE_MODE     1
#define STAND_BY_MODE   2
#define ERROR_MODE      3

#define WARNING_LED         PTT_PTT4
#define WARNING_DASH_LED    PTS_PTS7
#define INTERLOCK_STATE     PTS_PTS6
#define ERROR_RESET         PTS_PTS3         //TODO: à tester
#define IMD_STATE           PTS_PTS2  //PT1AD0_PT1AD02 (correspond à  AD0L pin 2)  //TODO: à tester
#define IGNITION            PTS_PTS5
#define SUPPLY_24V_RLY      PTS_PTS0      //Mosfet à la sortie du DC-DC

//----------------------
// Types
//----------------------

typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;



typedef struct
{    
    uint16 slaveTimeout : 1;                // Slave communication timeout 
    uint16 cellMaxVolt : 1;                 // A cell reached maximal voltage
    uint16 cellMinVolt : 1;                 // A cell reached minimal voltage
    uint16 cellMaxTemp : 1;                 // A cell reached maximal temperature
    uint16 cellMinTemp : 1;                 // A cell reached minimal temperature
    uint16 maxMeanCurrent : 1;              // Maximal mean current reached
    uint16 maxPeakCurrent : 1;              // Maximal peak current reached
    uint16 : 9;
} errors_t;

typedef struct
{
    uint16 equilibrating : 1;               // 1: Indique la batterie est en équilibration
    uint16 charging : 1;					     // 1: on est en mode charge, utilise les paramètres appropriés dans la détection d'erreurs.
    uint16 ignition : 1;                    // 1: L'ignition est à ON
    uint16 interlockClosed : 1;             // 1: le circuit d'interlock est fermé
    uint16 errorReset : 1;                  // 1: On a appuyé sur le bouton pour le reset des erreurs du BMS et du IMD.
    uint16 ImdError : 1;                    // 1: indique qu'une faute de masse a été détectée par le IMD.
    uint16 errorState : 1;                  // 1: une erreur a été détectée par le passée et n'a pas été correctement réinitialisée
    uint16 relaysClosed : 1;                // 1: les relais sont fermés.
    uint16 totalPackTime : 1;               // 1: c'est le moment de calculer la tension totale du pack
    uint16 cellLowVolt : 1;                 // Une cellule a une tension sous le seuil dangereux
    uint16 cellHighVolt : 1;                // Une cellule a une tension au-dessus du seuil dangereux
    uint16 cellLowTemp : 1;                 // Une cellule a une température sous le seuil dangereux
    uint16 cellHighTemp : 1;                // Une cellule a une température au-dessus du seuil dangereux
    uint16 highPeakCurrent : 1;             // Le courant moyen sur 10 secondes est au-dessus d'un seuil dangereux
    uint16 : 2;
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
   unsigned long int maxMeanChargeCurrent;      // [mA] Maximal charge current
   unsigned long int maxMeanDischargeCurrent;   // [mA] Maximal mean discharge current
   unsigned long int highPeakDischargeCurrent;  // [mA] Maximal mean discharge current
   unsigned long int maxPeakDischargeCurrent;   // [mA] Maximal peak discharge current
   unsigned char manualMode;                    // 1: aucune gestion des relais automatique.
   unsigned char ignoreErrors;                  // 1: ignorer les erreurs qui proviennent du BMS en mode manuel, en ce qui concerne l'ouverture des relais
   unsigned char ignoreIntState;                // 1: ignorer l'état de l'interlock lors de l'ouverture et de la fermeture des relais en mode manuel
} params_t;


//Global variables
extern uint16 gCellVolt[N_MOD][N_CELL];
extern uint16 *gHighestCellVoltage;
extern uint16 *gLowestCellVoltage;
extern uint32 gTotalPackVoltage;

extern uint8 gLowestVoltageCellNum;
extern uint8 gLowestVoltageCellSlaveId;
extern uint8 gHighestVoltageCellNum;
extern uint8 gHighestVoltageCellSlaveId;

extern int gCellTemp[N_MOD][N_CELL];
extern int *gHighestCellTemp;
extern int *gLowestCellTemp;

extern uint8 gLowestTempCellNum;
extern uint8 gLowestTempCellSlaveId;
extern uint8 gHighestTempCellNum;
extern uint8 gHighestTempCellSlaveId;

extern long int gMeanCurrent;  

extern errors_t gError;
extern flags_t gFlags;
extern params_t gParams;

extern uint16 gSlaveEquiStatus; 
extern uint16 gSlaveComState;
extern uint8 idleCount[N_MOD];
extern uint8 gSlaveReset[N_MOD];
extern uint8 gSlaveRev[N_MOD];



#endif //DEFINES_H