#ifndef CONFIGSPI_H
#define CONFIGSPI_H

#include "defines.h"


//*********************************
//  BMS Config Register structures 
//*********************************

struct Configuration_Register_t
{
	unsigned char CFGR0;	// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
	unsigned char CFGR1;	// CFGR1 => DCC7 ... DCC0; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
	unsigned char CFGR2;	// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
	unsigned char CFGR3;	// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
	unsigned char CFGR4;	// CFGR4 => VUV7 ... VUV0 ; VUV = Undervoltage comparaison Voltage
	unsigned char CFGR5;	// CFGR5 => VOV7 ... VOV0 ; VOV = Overvoltage comparaison Voltage
};
typedef struct Configuration_Register_t Config_Reg_t;


//*************************
//  Variables declaration
//*************************

extern Config_Reg_t gConfig_Reg;


//*************************
//  Functions prototypes
//*************************


void SPI_send(unsigned char sendByte);
unsigned char SPI_receive(unsigned char sendByte);
void Define_Config(void);
void Write_Config(void);
void Read_Config(unsigned char* rcvConfig);
void Start_Voltage_AD_Conversion(void);
void Start_Open_Wire_AD_Conversion(void);
void Start_Voltage_AD_Conversion_with_Discharge(void);
void Start_Open_Wire_AD_Conversion_with_Discharge(void);
void char_Table_Voltage_2_int_table(unsigned char* char_Voltage_Table);
void Read_Cell_Voltage(void);
char Verify_OpenWire(void);


#endif   //CONFIGSPI