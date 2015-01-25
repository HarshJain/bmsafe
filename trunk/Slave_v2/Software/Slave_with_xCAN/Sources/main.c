// Freescale Headers
#include <hidef.h>      		/* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// C STD Headers
#include <string.h>
#include <stdio.h>

// BMS specific Headers
#include "defines.h"
#include "ltc6803.h"
#include "CANSlave.h"
#include "temp.h"
#include "taskManager.h"

// xCAN library specific headers
#include "per_XDx512_L15Y.h"
#include "xgCAN_drv.h"
#include "xgate_vectors.h"
#include "xgate_init.h"


//-------------------------------------------------
// Global variables decleration and initialisation
//-------------------------------------------------

uint32  gElapsedTime = 0;	               	// le temps en dixièmes de secondes ecoule depuis l'activation du timer PIT0
uint32 	gElapsedTime_ms = 0;				// Elapsed time in ms				
uint8   gSlaveID = 0;                	    // le numéro d'identification du module, lu sur le port E.
int16  	gVoltages[NB_CELL];	         	    // table containing the Cell Voltages in mV
int16   gTemp[NB_CELL];         	  	    // table contenant les temperatures des cellules (300 = 30.0 oC)
uint16  gTempRaw[NB_CELL];            	   	// table contenant les lectures de l'ADC
uint16  gBalThres = 0;          	       	// Balancing target voltage
uint16  gBalanceVector = 0;             	// The n-th bit indicates the balancing status of the n-th cell. 1 = discharge enable.
uint16 	g_canMsgCount = 0;					// Number of pending messages we need to process
flags_t gFlags = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };  	// Les drapeaux globaux utilisés
TemperatureState	gTempState = T_STANDBY;	// Début du cycle de mesure (étape 0 : attente du temps pour la mesure)
VoltageState 		gVoltState = V_STANDBY;	// (enum) Stores the voltage measure state


// LTC6803 configuration register initialisation
	// ---------------------------------------------
	// CFGR0 : 0b11
	// 	bit 7 = X : Read only bit for watchdog timer state 
	// 	bit 6 = 1 : GPIO2 pins pull down off
	// 	bit 5 = 1 : GPIO2 pins pull down off
	// 	bit 4 = 0 : Level polling mode (after polling command, SDO is low when LTC6802 is busy)
	//	bit 3 = 0 : Do not ignore cell 11 and 12
	//	bit 2-0   : Comparator duty cycle = 001 (comparator off, value used to verify config integrity, do not change)
	//
	//	Discharge cells off (CFGR1 and CFGR2[3:0])
	// 	No cell interrupts  (not used) (CFGR2[7:4] and CRGR3)
	// 	CFGR4 : Undervoltage comparaison Voltage = 2.688V  (not really used)
	// 	CFGR5 : Overvoltage comparaison Voltage = 4.2V     (not really used)
Config_Reg_t 		g_ltcConfig = { 0b11110001, 0, 0, 0, 112, 175 }; // CFG0 default read = 0b01000000


#ifdef DEBUG_CODE
uint32 debug_main = 0;
uint32 debug_spiSend = 0;
uint32 debug_1 = 0;
uint32 debug_2 = 0;
uint32 debug_3 = 0;
uint32 debug_4 = 0;
uint32 debug_5 = 0;
uint32 debug_6 = 0;
#endif


//--------------------------
// Functions prototypes
//--------------------------

void MCU_init(void);		// MCU initialisation (auto-generated function)
void Parameters_init(void);	// Variable initialisation
void Timers_init(void);		// Timers initialisation

//---------------------------
// Functions
//---------------------------

void main(void) {

	// Initialization of the uC peripherals
	// --------------------------------------------------------
	// Timers initiazed for
	//    cell temperature measurements every 1 seconds (with uC ADC)
	//    cell voltage measurements every 0.1 second (with external battery monitor)
	//    we have a precision of 10 bits on the temperature measurements
	//    we have a precision of 12 bits on the voltage measurements

	
	MCU_init(); 	// MCU initialisation (auto-generated function in MCUinit.c)
	Int_Init();		// XGATE Interrupts configuration
	XGATE_Init();	// XGATE (co-proessor) initialisation
	
	Parameters_init();	// Variables initialisation (temperature table, voltage table, slave ID, LED outputs, ...)
	Timers_init();		// Timer initalisation (start timers)

	
	#ifdef CAN_ENABLE
	gFlags.canTxError = CAN_SendInitReport(gSlaveID); 							// Send initalisation report to BMS master
	gFlags.canTxError = CAN_SendFirmwareRevision(gSlaveID, FIRMWARE_REVISION);	// Send firmware revision to the master
	#endif


	#ifdef SPI_ENABLE
	// Write LTC6803 configuration registers
	ltcWriteConfig(&g_ltcConfig);
	#endif // SPI_ENABLE


	// Main Loop
	while (1) {

	#ifdef DEBUG_CODE
	debug_main++;
	#endif

	updateInputs();
	UpdateTimeDependentFlags();

	
	manage_CAN();
	manage_temperatures();
	manage_voltages();
	manage_equilibration();
	manage_LED();
	
    // On entre en mode veille, en attendant le prochain interrupt.
    // Ce sera un timer qui indique le moment de prendre des mesures ou la
    // réception d'un message sur le port CAN.

    // Le module SPI entrera lui aussi en mode veille puisque SPI1CR2_SPISWAI = 1
    // On envoie l'ADC en stop mode quand il a terminé de prendre ses mesures.
    // Le périphérique CAN n'entre pas en mode wait car dans ce cas on perd le msg
    // qui a réveillé l'interface CAN.

    // TODO: mettre le module CAN en wait mode et tjrs envoyer un msg bidon à partir du maître
    // quand il envoie une commande. Cependant, les mesg des autres esclaves vont constamment
    // réveiller l'interface, sauf si on utilise le Twup...

	//TODO : décommanter
//    asm {
//      WAI
//    };    

	} //end while(1)

} //end main



void Parameters_init(void)	// Variable initialisation
{
	uint8 i;

	// Initialisation des tableaux de mesures
	for (i = 0; i < NB_CELL; i++) {
	  gTemp[i] = 210;
	  gVoltages[i] = 3000;
	}

	// Reading slave ID on port S
	gSlaveID = PTS & 0x0F;		// Only the 4 lower bits define the ID
	gSlaveID = gSlaveID ^ 0x0F;	// Invert the bits to get the good ID
	if (gSlaveID == 0 || (gSlaveID > 15)) {
	  gFlags.badSlaveId = 1;  // TODO: on ne fait rien avec ca...
	}
 
	// Turning off LEDs after initialisation (turned ON by default)
	turnOffLEDs();


}


void Timers_init(void)
{
	PITCE_PCE0 = 1; // Turn on PIT0 (Periodic Interrupt Timer 0) which has a 10 Hz frequency
	PITCE_PCE1 = 1;	// TUrn on PIT1 (Periodic Interrupt Timer 1)
	PITCE_PCE1 = 1; // Turn ON PIT2
	PITCFLMT_PITE = 1; //Turn on PIT module

}