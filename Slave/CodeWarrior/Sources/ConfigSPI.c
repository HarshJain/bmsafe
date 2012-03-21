#include "ConfigSPI.h"
#include "derivative.h"
#include "Defines.h"

//----------------------------------
// BMS Config Register definitions
//----------------------------------


Config_Reg_t gConfig_Reg = 	{
								0b11101001,		// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
								0b00000000,		// CFGR1 => DCC7 ... DCC0; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
								0b00000000,		// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
								0b00000000,		// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
								0b01110000,		// CFGR4 => VUV7 ... VUV0 ; VUV = Undervoltage comparaison Voltage (70  = 2,688V)
								0b10101111,		// CFGR5 => VOV7 ... VOV0 ; VOV = Overvoltage comparaison Voltage  (175 = 4,200V)                          
								};


/*
Config_Reg_t gConfig_Reg = 	{
								0b11100001,		// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
								0b00000000,		// CFGR1 => DCC7 ... DCC1; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
								0b00000000,		// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
								0b11111100,		// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
								0b01110000,		// CFGR4 => VUV7 ... VUV0 ; VUV = Undervoltage comparaison Voltage (70  = 2,688V)
								0b10101111,		// CFGR5 => VOV7 ... VOV0 ; VOV = Overvoltage comparaison Voltage  (175 = 4,200V)                          
								};
*/

extern unsigned int 	   gInt_Voltage_Table[NB_CELL];			      // Int table containing the Cell Voltages in mV
extern unsigned int     gBalanceVector;

/**************
   Functions
**************/


void SPI_send(unsigned char sendByte)
{
  SPI1DR = sendByte;			// Put sendByte on Serial Data Register
  while ((SPI1SR & 0xA0) !=0xA0);
  sendByte = SPI1DR; // Return received byte and clear SPIF flag
}


///////////////////////////////////////////////////////////////////////////////
unsigned char SPI_receive(unsigned char sendByte)
{
  SPI1DR = sendByte;    		   	// Put sendByte on Serial Data Register
  while ((SPI1SR & 0xA0) != 0xA0);
  return SPI1DR;
}


////////////////////////////////////////////////////////////////////////////////
void Define_Config(void)
{
   gConfig_Reg.CFGR1 = (unsigned char) (gBalanceVector & 0x00FF);
   gConfig_Reg.CFGR2 = (gConfig_Reg.CFGR2 & 0xF0) | (unsigned char)((gBalanceVector & 0x0F00) >> 8);
}

/////////////////////////////////////////////////////////////////////////////
void Write_Config(void)
{
	CSBI = 0; 					   	// Set CSBI to 0
	SPI_send(WRCFG); 	            // Send the Write Config command (0x01) via SPI
	SPI_send(gConfig_Reg.CFGR0); 	// CFGR0 => WDT | GPIO2 | GPIO1 | LVLPL | Cell10 | CDC[2] | CDC[1] | CDC[0] 
	SPI_send(gConfig_Reg.CFGR1);	// CFGR1 => DCC7 ... DCC0; DCCn = Discharge_Cell_n enable (1 = on, 0 = off)
	SPI_send(gConfig_Reg.CFGR2);	// CFGR2 => MC4I | MC3I | MC2I | MC1I | DCC12 | DCC11 | DCC10 | DCC9 ; MCnI = Mask Cell n
	SPI_send(gConfig_Reg.CFGR3);	// CFGR3 => MC12I | MC11I | MC10I | MC9I | MC8I | MC7I | MC6I | MC5I 
	SPI_send(gConfig_Reg.CFGR4);	// CFGR4 => VUV7 ... VUV0 ; VUV = Undervoltage comparaison Voltage
	SPI_send(gConfig_Reg.CFGR5);	// CFGR5 => VOV7 ... VOV0 ; VOV = Overvoltage comparaison Voltage
	CSBI = 1;					      // Set CSBI to 1 to complete command sequence
}

/////////////////////////////////////////////////////////////////////////////
void Read_Config(unsigned char* rcvConfig)
{

   unsigned int i;
   
	CSBI = 0;
	SPI_send(RDCFG);
	
   for(i=0; i<6; i++) {
      rcvConfig[i] = SPI_receive(0x00);
   }
     
	CSBI = 1;                    
					          	
}


///////////////////////////////////////////////////////////////////////////////
void Start_Voltage_AD_Conversion()
{
   unsigned char rcvByte = 0;
  
	CSBI = 0; 		
	SPI_send(STCVAD); 	// Send the Cell Volatge A/D Conversion command (0x10) via SPI	
	while(!rcvByte) {rcvByte = SPI_receive(0x00);}
   CSBI = 1;
}


///////////////////////////////////////////////////////////////////////////////
void Start_Open_Wire_AD_Conversion()
{
   unsigned char rcvByte = 0;
   
	CSBI = 0; 		   	// Set CSBI to 0
	SPI_send(STOWAD); 	// Send the Cell Volatge A/D Conversion command (0x20) via SPI
   while(!rcvByte) {rcvByte = SPI_receive(0x00);}
	CSBI = 1;		   	// Set CSBI to 1
}


///////////////////////////////////////////////////////////////////////////////
void Start_Voltage_AD_Conversion_with_Discharge()
{
   unsigned char rcvByte = 0;

	CSBI = 0; 		
	SPI_send(STCVDC); 	// Send the Cell Volatge A/D Conversion command (0x10) via SPI
	while(!rcvByte) {rcvByte = SPI_receive(0x00);}
	CSBI = 1;	
}


///////////////////////////////////////////////////////////////////////////////
void Start_Open_Wire_AD_Conversion_with_Discharge()
{
   unsigned char rcvByte = 0;
   
	CSBI = 0; 		// Set CSBI to 0
	SPI_send(STOWDC); 	// Send the Cell Volatge A/D Conversion command (0x20) via SPI
	while(!rcvByte) {rcvByte = SPI_receive(0x00);}
	CSBI = 1;		// Set CSBI to 1
}


/////////////////////////////////////////////////////////////////////////////// 
void char_Table_Voltage_2_int_table(unsigned char* char_Voltage_Table)
{
	int n = 0; // Modif 29 Janvier 2012
	int Cell = 0;
	
	for(; n < NB_CHAR_VOLTAGE; n = n + 3)     // Modif 29 Janvier              
	{
		gInt_Voltage_Table[Cell] = ((unsigned int)(char_Voltage_Table[n+1] & 0xF) << 8) + char_Voltage_Table[n];
		gInt_Voltage_Table[Cell] = (gInt_Voltage_Table[Cell]*3) >> 1;         //From binary ADC value to voltage : V = ADC x 1.5mV
		gInt_Voltage_Table[Cell+1] = ((unsigned int)char_Voltage_Table[n+2]<<4) | ((char_Voltage_Table[n+1])>>4);
		gInt_Voltage_Table[Cell+1] = (gInt_Voltage_Table[Cell+1]*3) >> 1;     //From binary ADC value to voltage : V = ADC x 1.5mV
		if(Cell == 8)                       // hack to compensate the fact that the LTC6802-2 is underevaluating the top cell voltage
	   	gInt_Voltage_Table[Cell+1] += 92; // when uncommented: balancing doesn't work anymore                                      
		Cell = Cell + 2; 
	}
}


///////////////////////////////////////////////////////////////////////////////
void Read_Cell_Voltage()
{	
	unsigned char 	char_Voltage_Table[NB_CHAR_VOLTAGE];
	unsigned int i;
	
	CSBI = 0;
	SPI_send(RDCV);
	
	// Loop to put reveiced values in table memory (gNb_char_Voltage because of the PEC byte)
	for (i = 0; i < NB_CHAR_VOLTAGE; i++)	{
	  char_Voltage_Table[i] = SPI_receive(0x00);	    
	}

	CSBI = 1;
	
	char_Table_Voltage_2_int_table(char_Voltage_Table);
}


/////////////////////////////////////////////////////////////////////////////// 
char Verify_OpenWire()
{
	int i;
	unsigned int Voltage_Table_A[10] = {0,0,0,0,0,0,0,0,0,0};
	unsigned int Voltage_Table_B[10] = {0,0,0,0,0,0,0,0,0,0};	
	
	Start_Voltage_AD_Conversion();
	Read_Cell_Voltage();
	for(i = 0; i < NB_CELL; i++)
	{
		Voltage_Table_A[i] = gInt_Voltage_Table[i];
	}
		
	Start_Open_Wire_AD_Conversion();
	Read_Cell_Voltage();
	for(i = 0; i < NB_CELL; i++)
	{
		Voltage_Table_B[i] = gInt_Voltage_Table[i];
	}
	
	// Compare values read on the different mode  
	for (i = 0; i < NB_CELL; i++)
	{
		if( Voltage_Table_B[i] - Voltage_Table_A[i] >= Voltage_th )
		{
			return OPENWIRE;
		}
	}
	
	return notOPENWIRE;
}
