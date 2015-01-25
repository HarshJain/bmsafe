/*! \file defines.h
 * 	This file contains all general definitions and structures.
 */


#ifndef DEFINES_H
#define DEFINES_H

// ---------------- Battery Cell Specifications --------------------
// These values are in between the limits. For more safety, we use conservative values below the maximum limits and over the minimum
// limits specified in the datasheet and also depending on our needs.
#define	MIN_DISCH_CELL_TEMP		-270	// Minimum discharge cell temperature (0.1 °C)
#define MAX_DISCH_CELL_TEMP		470		// Maximum discharge cell temperature (0.1 °C)
#define LOW_DISCH_CELL_TEMP		-200	// Low discharge cell temperature (0.1 °C)
#define HIGH_DISCH_CELL_TEMP	400		// High discharge cell temperature (0.1 °C)
#define MIN_CHARGE_CELL_TEMP	0		// Minimum charging cell temperature (0.1 °C)
#define MAX_CHARGE_CELL_TEMP	400		// Maximum charging cell temperature (0.1 °C)
#define LOW_CHARGE_CELL_TEMP	50		// Low charging cell temperature (0.1 °C)
#define HIGH_CHARGE_CELL_TEMP	350		// High charging cell temperature (0.1 °C)

#define MIN_CELL_VOLTAGE		2550	// Minimum discharge voltage (mV)
#define MAX_CELL_VOLTAGE		4100	// Maximum cell voltage (mV)
#define LOW_CELL_VOLTAGE		3000	// Low cell voltage (mV)
#define HIGH_CELL_VOLTAGE		4000	// High cell voltage (mV)

#define MAX_MEAN_CHARGE_CURRENT	2000	// Maximum mean charge current (mA)
#define MAX_MEAN_DISCH_CURRENT	5000	// Maximum mean discharge current (mA)
#define HIGH_PEAK_DISCH_CURRENT	8500	// High peak discharge current (mA)
#define MAX_PEAK_DISCH_CURRENT	10000	// Maximum peak discharge current(mA) (less than 10 secondes)


// ----------------- Basic configurations -----------------
// Minimum configuration required by the BMS (to be change depending on usage needs)

#define DTC_ENABLE				// Enable Data Trouble Codes (useful if there is a data acquisition system)

#define N_MOD           1		// Number of BMS slave on the network
#define N_CELL          12		// Number of cells in a battery pack

#define N_MAX_IGNORE_TEMP 	15 		// Nombre de sonde de température maximum que le master peut ignorer
#define ECART_MIN_TEMP		15		// Écart absolu minimal entre les températures pour pouvoir ignorer celles-ci


// ------------------ Advance configurations --------------------
// Refer to documentation for modifying these configuration. The default value should work OK.

#define PACK_VOLT_CALC_PERIOD_MS 500    		//!< Nombre de milisecondes entre chaque calcul de la tension totale du pack
#define FLASH_LED_PERIOD         25     		//!< 4 Hz (0.01 sec)
#define IDLE_TIMER_COUNT_PERIOD  100    		//!< 100 = 1 HZ (0.01 sec)
#define DELAI_CELLMINVOLT		 300 			//!< (1 sec.) Délai avant le déclenchement d'une erreur cellMinVolt : ECART_MIN_TEMP = Temps en sec./1000
#define BMS_STATUS_MSG_DELAY_MS			500		//!< [ms] Delay between each BMS status message is send
#define BMS_PING_MSG_DELAY_MS			700		//!< [ms] Delay between two BMS master PING messages are sent
#define BMS_BALANCING_MSG_DELAY_MS		100		//!< [ms] Delay between two balancing commands are sent (when balancing)
#define BALANCING_FUZZ_VALUE_mV			5		//!< [mV] Fuzz  used in cell balancing

#define PRECHARGE_TIMEOUT_MS	 		7000	//!< [ms]	Delay before the timeout for the pre-charge process (if pre-charge is too long)
#define PRECHARGE_CHARGE_MODE_DELAY_MS	100		//!< [ms] Delay before pre-charge is done when the BMS is in CHARGE_MODE
#define PRECHARGE_DELAY_MS				5000	//!< [ms] Delay before pre-charge is done when USE_DRIVE_VOLTAGE = 0 and BMS in normal mode

#define USE_DRIVE_VOLTAGE		 		0		//!< 1 = use drive voltage for pre-charge, (other value) = use time delay for pre-charge (when not in charge mode)
#define DRIVE_REQUEST_DELAY_MS			1000	//!< [ms] Delay between two drive's info request
#define DRIVE_VOLTAGE_RCV_TIMEOUT_MS	500		//!< [ms] Delay before we consider the drive's voltage (gDriveVoltage) value as outated

#define TCS_CONNECTION_TIMEOUT_MS		400		//!< [ms] Delay before considering the TCS offline (if no TCS message was received) 
#define CURRENT_RCV_TIMEOUT_MS			300		//!< [ms] Delay before we consider the current value (gCurrentFlow_mA) as outated


#define COM_IDLE_COUNT_MAX     10       //Number of com timeout timer cycles (1 second) without
                                        //communication from a slave before the error flag is set
                                        //Must be < 255


// ---------------------- Port definitions -----------------------
// Change only if using a different microcontroller or different pin configuration

// - Segments de l'afficheur 7 segments : 1 = éteint, 0 = allumé
#define SEGMENT_A           PTJ_PTJ0
#define SEGMENT_B           PORTK_PK4
#define SEGMENT_C           PTT_PTT7                                
#define SEGMENT_D           PTT_PTT5
#define SEGMENT_E           PTT_PTT6
#define SEGMENT_F           PTJ_PTJ1
#define SEGMENT_G           PORTK_PK5                                                                                                     
#define DOT         		PTT_PTT4

#define WARNING_DASH_LED    PORTA_PA4	// 1 : allumée, 0 : éteinte (fonctionne en logique non-inversée), MCR4
#define INTERLOCK_STATE     PTM_PTM6
#define ERROR_RESET         PTS_PTS4    //TODO: à tester
#define IMD_STATE           PTS_PTS1	// 1: No fault, 0: Fault
#define IMD_MHS             PTS_PTS2	// To calculate impedance between HV and GND
#define ERROR_RST_DASH		PTM_PTM7	//

// ------------------ Error Code ----------------------
// Will be use for 7-segment display
#define CellMaxTemp_ErrorCode 5
#define CellMaxVolt_ErrorCode 7
#define CellMinVolt_ErrorCode 8
#define CellMinTemp_ErrorCode 6
#define SlaveTimeout_ErrorCode 9
#define GenericErrorState_ErrorCode 'E'
#define ImdError_ErrorCode 'F'

#define CellHighTemp_WarningCode 1
#define CellLowVolt_WarningCode 4
#define CellHighVolt_WarningCode 3
#define CellLowTemp_WarningCode 2

#define RelaysCloses_MessageCode 'C'
#define InterlockClosed_MessageCode ',' // Le point est éteint
#define InterlockOpen_MessageCode '.'   // Le point est allumé

#define NoError_MessageCode 'A'

// ------------------ Other definitions ----------------------
// General defines (do not change)

#define NORMAL_MODE     0
#define CHARGE_MODE     1
#define STAND_BY_MODE   2
#define ERROR_MODE      3

//----------------------
// Types definition
//----------------------

typedef signed char		int8;
typedef signed int		int16;
typedef signed long		int32;
typedef unsigned char   uint8;
typedef unsigned int    uint16;
typedef unsigned long   uint32;

//---------------------------------
// Structure definition
//----------------------------------

typedef struct
{    
    uint16 slaveTimeout : 1;                //!< Slave communication timeout 
    uint16 cellMaxVolt : 1;                 //!< A cell reached maximal voltage
    uint16 cellMinVolt : 1;                 //!< A cell reached minimal voltage
    uint16 cellMaxTemp : 1;                 //!< A cell reached maximal temperature
    uint16 cellMinTemp : 1;                 //!< A cell reached minimal temperature
    uint16 maxMeanCurrent : 1;              //!< Maximal mean current reached
    uint16 maxPeakCurrent : 1;              //!< Maximal peak current reached
    uint16 : 1;
} errors_t;

typedef struct
{
    uint16 equilibrating : 1;               //!< 1: Indique la batterie est en équilibration
    uint16 interlockClosed : 1;             //!< 1: le circuit d'interlock est fermé
    uint16 errorState : 1;                  //!< 1: une erreur a été détectée par le passée et n'a pas été correctement réinitialisée
	uint16 errorReset : 1;                  //!< 1: On a appuyé sur le bouton pour le reset des erreurs du BMS et du IMD.
    uint16 ImdError : 1;                    //!< 1: indique qu'une faute de masse a été détectée par le IMD.
    uint16 cellLowVolt : 1;                 //!< Une cellule a une tension sous le seuil dangereux
    uint16 cellHighVolt : 1;                //!< Une cellule a une tension au-dessus du seuil dangereux
    uint16 cellLowTemp : 1;                 //!< Une cellule a une température sous le seuil dangereux
    uint16 cellHighTemp : 1;                //!< Une cellule a une température au-dessus du seuil dangereux
    uint16 highPeakCurrent : 1;             //!< Le courant moyen sur 10 secondes est au-dessus d'un seuil dangereux
	uint16 driveVoltageOutdated : 1;		//!< 1 = The drive voltage value is out of date
	uint16 driveSendRequestVoltage : 1;		//!< 1 = timeout for sending a voltage request to the drive
	uint16 currentMeasureOutdated : 1;		//!< 1 = the current measure is outdated
	uint16 charger_online : 1;				//!< 1 = the charger is connected to the CAN network
	uint16 TCS_online : 1;					//!< 0 = the BMS does not see the TCS on the CAN network
	uint16 drive_online : 1;				//!< 0 = the drive does not seem to be connected on the network
} flags_t;

typedef struct
{
   int16 minDischargeCellTemp;                    // [dixieme oC] Minimal operating temperature in normal mode
   int16 maxDischargeCellTemp;                    // [dixieme oC] Maximal operating temperature in normal mode
   int16 lowDischargeCellTemp;                    // [dixieme oC] Low threshold operating temperature in normal mode
   int16 highDischargeCellTemp;                   // [dixieme oC] High threshold operating temperature in normal mode
   int16 minChargeCellTemp;                       // [dixieme oC] Minimal operating temperature in charge mode
   int16 maxChargeCellTemp;                       // [dixieme oC] Maximal operating temperature in charge mode
   int16 lowChargeCellTemp;                       // [dixieme oC] Low threshold operating temperature in charge mode
   int16 highChargeCellTemp;                      // [dixieme oC] High threshold operating temperature in charge mode
   uint16 minCellVoltage;                 // [mV] Minimal cell voltage
   uint16 maxCellVoltage;                 // [mV] Maximal cell voltage
   uint16 lowCellVoltage;                 // [mV] Low threshold cell voltage
   uint16 highCellVoltage;                // [mV] High threshold cell voltage
   int32 maxMeanChargeCurrent;      // [mA] Maximal charge current
   int32 maxMeanDischargeCurrent;   // [mA] Maximal mean discharge current
   int32 highPeakDischargeCurrent;  // [mA] Maximal mean discharge current
   int32 maxPeakDischargeCurrent;   // [mA] Maximal peak discharge current
   uint8 manualMode;                    // 1: aucune gestion des relais automatique.
   uint8 ignoreErrors;                  // 1: ignorer les erreurs qui proviennent du BMS en mode manuel, en ce qui concerne l'ouverture des relais
   uint8 ignoreInterlockState;                // 1: ignorer l'état de l'interlock lors de l'ouverture et de la fermeture des relais en mode manuel
} params_t;

//----------------------------------------------
// Global variables extern definition
//----------------------------------------------
extern int16 gCellVolt[N_MOD][N_CELL];
extern int16 *gHighestCellVoltage;
extern int16 *gLowestCellVoltage;
extern uint32 gTotalPackVoltage;

extern uint8 gLowestVoltageCellNum;
extern uint8 gLowestVoltageCellSlaveId;
extern uint8 gHighestVoltageCellNum;
extern uint8 gHighestVoltageCellSlaveId;

extern int16 gCellTemp[N_MOD][N_CELL];
extern int16 *gHighestCellTemp;
extern int16 *gLowestCellTemp;
extern int16 gCellIgnoreTemp[N_MAX_IGNORE_TEMP][2];
extern uint8 gNumberOfIgnoredTemp;					// Number of ignored temperature sensors

extern uint8 gLowestTempCellNum;
extern uint8 gLowestTempCellSlaveId;
extern uint8 gHighestTempCellNum;
extern uint8 gHighestTempCellSlaveId;

extern int32 gCurrentFlow_mA;

extern errors_t gError;
extern flags_t gFlags;
extern params_t gParams;

extern uint16 gSlaveComState;
extern uint8 idleCount[N_MOD];
extern uint8 gSlaveReset[N_MOD];
extern uint8 gSlaveRev[N_MOD];
extern uint8 gSlaveLastRevision;
extern uint8 idTable[N_MOD];

extern uint16 gImpedance;   // in kilo ohms
extern uint32 gDriveVoltage;


extern uint8 gMode;

extern uint16 gCloseRelayRequest;
extern uint16 gOpenRelayRequest;

#endif //DEFINES_H