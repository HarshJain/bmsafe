#ifndef LTC6802_H
#define LTC6802_H

#include "defines.h"


#define CSBI	PTH_PTH3	            // PTH3 is the Chip Select bit of SPI bus
#define SPI_SEND_TIMEOUT   3        // SPI timeout in seconds.
#define OPENWIRE_THRESHOLD	200      // 200 mV
#define NB_CHAR_VOLTAGE    15       //Number of voltages 8-bit registers needed to transfer all 10 measurements

/*****************
* Command codes
*****************/
#define WRCFG	0x01	//Write Config
#define RDCFG  0x02  //Read config
#define RDCV	0x04	//Read Cell Voltage
#define STCVAD	0x10	//Cell Volatge A/D Conversion (on all cells)
#define STOWAD	0x20	//Open_Wire_AD_Conversion (on all cells)
#define PLINT	0x50	//Poll Interupt Status
#define STCVDC	0x60 	//Cell Volatge A/D Conversion With Discharge
#define STOWDC	0x70	//Open Wire A/D Conversion With Discharge

#define JUNK_BYTE 0x00

/*********************************
* BMS Config Register structures 
*********************************/

struct Configuration_Register_t
{
	unsigned char CFGR0;	// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
	unsigned char CFGR1;	// CFGR1 => DCC7 ... DCC0; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
	unsigned char CFGR2;	// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
	unsigned char CFGR3;	// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
	unsigned char CFGR4;	// CFGR4 => VUV7 ... VUV0 ; VUV = Undervoltage comparaison Voltage (VUV * 16 * 1.5mV)
	unsigned char CFGR5;	// CFGR5 => VOV7 ... VOV0 ; VOV = Overvoltage comparaison Voltage (VOV * 16 * 1.5mV)
};
typedef struct Configuration_Register_t Config_Reg_t;


/*************************
* Functions prototypes
*************************/

unsigned char SPI_send(unsigned char sendByte);
void ltcMakeConfigRegister(Config_Reg_t *ltcConfig, unsigned int config);
void ltcWriteConfig(Config_Reg_t *ltcConfig);
unsigned char ltcReadConfig(unsigned char* rcvConfig);
void ltcStartVoltageMeasure(void);
void ltcStartOpenWireADC(void);
void convertVoltages(unsigned char* voltagesByte, unsigned int *voltagesInt);
unsigned char ltcReadCellVoltage(unsigned int *voltagesInt);
unsigned char ltcVerifyOpenWire(void);
unsigned char ltcComputeCRC(unsigned char *array, unsigned char len);


#endif   //LTC6802_H