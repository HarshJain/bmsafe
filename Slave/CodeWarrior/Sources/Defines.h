#ifndef DEFINES_H
#define DEFINES_H


//------------------
// BMS Parameters
//------------------

#define NB_CELL    	10		 //Number of cells for each slave
#define NB_CELL_MAX  10    //Maximum number of slaves possible

#if ((NB_CELL & 0x1) == 0)
   #define NB_CHAR_VOLTAGE  ((NB_CELL*3) >> 1)
#else
   #define NB_CHAR_VOLTAGE  (((NB_CELL*3) >> 1) + 1)
#endif



/*************************
       Define SPI
**************************/

//------------------
// SPI Bus pin
//------------------

#define MISO	PTH_PTH0	// PTH0 is the MISO of SPI bus
#define MOSI	PTH_PTH1	// PTH1 is the MOSI of SPI bus
#define SCK		PTH_PTH2	// PTH2 ist the clock of SPI bus
#define CSBI	PTH_PTH3	// PTH3 is the Chip Select bit of SPI bus

//------------------
// Command code 
//------------------

#define WRCFG	0x01	//Write Config
#define RDCFG  0x02  	//Read config
#define RDCV	0x04	//Read Cell Voltage
#define STCVAD	0x10	//Cell Volatge A/D Conversion
#define STOWAD	0x20	//Open_Wire_AD_Conversion
#define PLINT	0x50	//Poll Interupt Status
#define STCVDC	0x60 	//Cell Volatge A/D Conversion With Discharge
#define STOWDC	0x70	//Open Wire A/D Conversion With Discharge

 //----------------------
// Open Wire detection
//----------------------

#define OPENWIRE 	1	
#define notOPENWIRE	0
#define Voltage_th	200     // 200 mV



/*************************
      Define CAN
**************************/

/* ID Definition */
#define ID_580 0xB0000000    //Id 0x580 formatted to be loaded 
#define ID_680 0xD0000000    //in IDRx Registers in Tx Buffer (*0x200000)
#define ID_780 0xF0000000
#define ID_500 0xA0000000

//------------------
// CAN Commands
//------------------
#define COMMAND_BAL             0x03
#define COMMAND_BAL_STOP        0x04
#define COMMAND_BAL_DONE        0x05
#define BROADCAST_SLV           0x3F
#define MASTER                  0x4F                                     

/* Acceptance Code Definitions */ 
//#define ACC_CODE_ID100 0x2000
//#define ACC_CODE_ID100_HIGH ((ACC_CODE_ID100&0xFF00)>>8)
//#define ACC_CODE_ID100_LOW (ACC_CODE_ID100&0x00FF)


/* Mask Code Definitions */
//#define MASK_CODE_ST_ID 0x0007
//#define MASK_CODE_ST_ID_HIGH ((MASK_CODE_ST_ID&0xFF00)>>8)
//#define MASK_CODE_ST_ID_LOW (MASK_CODE_ST_ID&0xFF)

/* Error Flags Definition */
#define NO_ERR 0x00
#define ERR_BUFFER_FULL 0x80


/*************************
      Define Balancing
**************************/

#define BAL_DELTA_VOLT  0


#endif //DEFINES_H