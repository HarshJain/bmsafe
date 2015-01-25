#ifndef DEFINES_H
#define DEFINES_H


#define FIRMWARE_REVISION       1	// Increment when modifying code

//------------------
// BMS Parameters
//------------------

#define NB_CELL					12	// Number of cells for this slave 
#define NB_TERMISTORS			12	// Nombre de termistors utilisés
#define VOLT_MEASURE_PERIOD     1	// Le nombre de dixièmes de secondes entre chaque mesure de tensions
#define NB_VOLT_SAMPLES       	10	// Le nombre de mesures de tension entre chaque envoie sur le bus CAN
#define TEMP_MEASURE_PERIOD    	10	// Le nombre de dixièmes secondes entre chaque mesure de températures
#define NB_TEMP_SAMPLES        	5	// Le nombre de mesures de températures entre chaque envoie sur le bus CAN
#define BAL_DELTA_VOLT          0	// Balancing margin. Reaching voltage_goal minus margin stops balancing
#define MASTER_COM_TIMEOUT_MS	1000 // [ms] Time before considering that communication with the BMS master is lost

#define CAN_ENABLE		// Enable CAN communication
#define SPI_ENABLE		// Enable SPI communication with LTC chip
#define DTC_ENABLE		// Enable Data Trouble Codes

//-----------------------------
//	General port definition
//-----------------------------

#define LED2	PORTK_PK2
#define LED3	PORTK_PK1
#define LED4	PORTK_PK0
#define LED_ON	0
#define LED_OFF 1
#define DISCH_MONITOR 	PORTD_PD1			// Input : 0 = the discharge circuit is plugged, 1 = no discharge circuit plugged

//----------------------------------
// Types, structures and enums
//----------------------------------

#define int8 char
#define int16 int
#define int32 long

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

typedef struct {
  uint16 balancingActive  	: 1;	// 0 = Normal mode ; 1 = Discharge Cell mode
  uint16 equiStatusChange 	: 1;   	// 1 = les registres de config. du mon. de batteries ont changes et doivent etre réenvoyés
  uint16 voltTimeout  		: 1;	// 1 = les mesures de voltages doivent etre prises par le moniteur de batterie
  uint16 ADC0done  			: 1;    // 1 = les mesures de l'ADC0 sont pretes a etre recuperees
  uint16 ADC1done  			: 1;    // 1 = les mesures de l'ADC1 sont pretes a etre recuperees
  uint16 firmwareRequest	: 1;    // 1 = le maître demande l'envoie de la révision du firmware
  uint16 spiTimeout  		: 1;    // 1 = au moins un timeout s'est produit sur la communication SPI
  uint16 badSlaveId  		: 1;    // 1 = l'id du slave n'a pas été déterminé correctement
  uint16 ltcConversionFail	: 1;	// 1 = la LTC n'a pas réussi à compléter sa conversion
  uint16 canTxError  		: 1;    // La variable contient le numéro de l'erreur de transmission. TODO : cette variable ne devrait pas tenir sur un seul bit
  uint16 canRxError			: 1;	// 1 = CAN reception error, 0 = no CAN Reception error
  uint16 slaveDoubleID		: 1;	// 1 = There is another slave with the same ID !
  uint16 openWire			: 1;	// 1 = There is an open wire error
  uint16 master_online		: 1;	// 0 = The slave does not see the BMS master on the CAN network
  uint16 disch_circuit_plugged : 1;	// 1 = The discharge circuit is connected to the slave
  uint16 					: 1;
} flags_t;


typedef enum {
	V_STANDBY,			// LTC is not busy, no measure is going on
	// Voltag measurement steps
	MEASURE_START,		// Ready to start voltage measurement (voltage measure period has timed out)
	MEASURE_WAIT,		// Waiting the LTC to finish its conversions
	// Self test steps
	SELF_TEST_START,	// Ready to start a self check on the LTC (self check period has timed out)
	SELF_TEST_WAIT,		// Waiting the LTC to finish the self test
	// LTC diagnostic steps
	DIAGNOSTIC_START,
	DIAGNOSTIC_WAIT
} VoltageState;

typedef enum {
	T_STANDBY,		// No conversion are going on
	START,			// Ready to start temperature ADC conversion (temperature measure period has timed out)
	WAIT			// Waiging the MCU to finish conversions
} TemperatureState;

// -----------------------------------
// Global variables declaration
// -----------------------------------
extern uint32 gElapsedTime;			// Elapsed time in 100 ms, Will progressively be replace by gElapsedTime_ms
extern uint32 gElapsedTime_ms;		// Elapsed time in ms
extern uint8 gSlaveID;
extern int16 gTemp[NB_CELL];
extern uint16 gTempRaw[NB_CELL];
extern uint16 gBalanceVector;
extern uint16 gBalThres;
extern flags_t gFlags;
extern TemperatureState gTempState;	// Temperature mesurement state
extern VoltageState gVoltState;		// Voltage measurement state
extern uint16 g_canMsgCount;			// Number of pending messages we need to process
extern int16 gVoltages[NB_CELL];


#define DEBUG_CODE
#ifdef DEBUG_CODE
extern uint32 debug_main;
extern uint32 debug_spiSend;
extern uint32 debug_1;
extern uint32 debug_2;
extern uint32 debug_3;
extern uint32 debug_4;
extern uint32 debug_5;
extern uint32 debug_6;
#endif


#endif //DEFINES_H