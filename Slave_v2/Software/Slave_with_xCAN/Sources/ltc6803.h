#ifndef LTC6803_H
#define LTC6803_H

#include "defines.h"


#define CSBI				PTH_PTH3	// PTH3 is the Chip Select bit of SPI bus
#define SPI_SEND_TIMEOUT	5			// Le timeout du préiphérique SPI en dixièmes de secondes
#define SPI_CONN_TIMEOUT	10			// Temps de non-réponse avant de lancer une erreur de connexion MCU-LTC
#define OPENWIRE_THRESHOLD	200			// 200 mV
#define NB_CHAR_VOLTAGE		18			//Number of voltages 8-bit registers needed to transfer all 10 measurements
#define SELF_CHECK_PERIOD	10			// Number of tenth of seconds (seconds/10) between each self check
#define LTC_DIAGNOSTIC_PERIOD 30		// Number of tenth of seconds between each LTC diagnostic test


/*****************
* Command codes
*****************/
#define WRCFG	0x01	//Write Config
#define RDCFG   0x02  	//Read config
#define RDCV	0x04	//Read Cell Voltage
#define STCVAD	0x10	//Cell Volatge A/D Conversion (on all cells) (STCVAD-> ALL)
#define STOWAD	0x20	//Open_Wire_AD_Conversion (on all cells)
#define PLINT	0x50	//Poll Interupt Status
#define STCVDC	0x60 	//Cell Volatge A/D Conversion With Discharge
#define STOWDC	0x70	//Open Wire A/D Conversion With Discharge
#define CLEAR	0x1D	// STCVAD->CLEAR command used for testing
#define STEST2	0x1F	// Self test 2 (STCVAD->Self Test2)
#define PLADC	0x40	// POLL ADC : SDO is hold low if device is not done converting
#define DIAGN	0x52	// Start Diganose
#define RDDGNR	0x54	// Read diagnostic register

#define JUNK_BYTE 0x00

/*********************************
* BMS Config Register structures 
*********************************/

struct Configuration_Register_t
{
	uint8 CFGR0;	// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
	uint8 CFGR1;	// CFGR1 => DCC8 ... DCC1; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
	uint8 CFGR2;	// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
	uint8 CFGR3;	// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
	uint8 CFGR4;	// CFGR4 => VUV[7] ... VUV[0] ; VUV = Undervoltage comparaison Voltage (VUV * 16 * 1.5mV)
	uint8 CFGR5;	// CFGR5 => VOV[7] ... VOV[0] ; VOV = Overvoltage comparaison Voltage (VOV * 16 * 1.5mV)
};
typedef struct Configuration_Register_t Config_Reg_t;

/*************************
* Functions prototypes
*************************/

// Low level functions (called by medium level function, see below)
uint8 SPI_send(uint8 sendByte);
void convertVoltages(uint8* voltagesByte, int16 *voltagesInt);
uint8 ltcComputeCRC(uint8 *array, uint8 len); // Compute CRC for an byte array
uint8 ltcComputePEC(uint8 byte); // Compute CRC for one byte

// Medium level functions (called by other functions of th program)
void ltcMakeConfigRegister(Config_Reg_t *ltcConfig, uint16 balanceVector);
void ltcWriteConfig(Config_Reg_t *ltcConfig);
uint8 ltcReadConfig(uint8* rcvConfig);
void ltcStartVoltageMeasure(void);
void ltcStartOpenWireADC(void);
uint8 ltcReadCellVoltage(int16 *voltagesInt);
uint8 ltcReadCellVoltageRaw(uint8 *voltagesByte); // Does not convert the data
uint8 ltcVerifyOpenWire(void);	// Detect open connections
uint8 ltcConversionFinished(); // Poll LTC6803 to verify if the conversion are done
uint8 ltcClearADC(void); // Check if LTC6803 is correctly responding by using CLEAR command
uint8 ltcSelfCheck(void); // Do a LTC6803 self check (see documentation)
uint8 ltcDiagnostic(void); // Do a LTC6803 diagnostic


// High level functions (call medium level function, called by other function of the program)
uint8 ltcConfigurationOK(Config_Reg_t *ltcConfig);	// Check if the LTC configuration registers are OK, returns 1 if OK



extern Config_Reg_t g_ltcConfig;


#endif   //LTC6803_H