#include "ltc6802.h"
#include "derivative.h"
#include "defines.h"



/******************************************************************************
* Sends a single byte on the SPI1 peripheral. The function blocks until the
* byte is sent. Returns the received byte.
******************************************************************************/
unsigned char SPI_send(unsigned char sendByte)
{
   unsigned long startTime;
  
   SPI1DR = sendByte;     // Put sendByte on Serial Data Register

   startTime = gElapsedTime;
   while (!SPI1SR_SPIF) {
      if((gElapsedTime-startTime) > SPI_SEND_TIMEOUT) {
         gSPItimeout = 1;
         break;
      }
   }

   return SPI1DR;         // Return received byte and clear SPIF flag
}

/******************************************************************************
* Use the equilibration vector to make the new ltc6802 config register .
******************************************************************************/
void ltcMakeConfigRegister(Config_Reg_t *ltcConfig, unsigned int config)
{
   ltcConfig->CFGR1 = (unsigned char) (config & 0x00FF);
   ltcConfig->CFGR2 = (ltcConfig->CFGR2 & 0xF0) | (unsigned char)((config & 0x0F00) >> 8);
}


/******************************************************************************
* Sends the configuration register to the battery monitor (LTC6802).
******************************************************************************/
void ltcWriteConfig(Config_Reg_t *ltcConfig)
{
	CSBI = 0; 					   	// Set CSBI to 0
	SPI_send(WRCFG); 	            // Send the Write Config command (0x01) via SPI
	SPI_send(ltcConfig->CFGR0);   // Send the config register
	SPI_send(ltcConfig->CFGR1);
	SPI_send(ltcConfig->CFGR2);
	SPI_send(ltcConfig->CFGR3);
	SPI_send(ltcConfig->CFGR4);
	SPI_send(ltcConfig->CFGR5);
	CSBI = 1;					      // Set CSBI to 1 to complete command sequence
}

/////////////////////////////////////////////////////////////////////////////
unsigned char ltcReadConfig(unsigned char* rcvConfig)
{
   unsigned char i, pec, crc;
   
	CSBI = 0;
	SPI_send(RDCFG);
	
   for(i=0; i<6; i++)      //6 config registers
      rcvConfig[i] = SPI_send(JUNK_BYTE);

   pec = SPI_send(JUNK_BYTE);
	CSBI = 1;      

   crc = ltcComputeCRC(rcvConfig, 6);   
   
   if(crc == pec)
      return 0;
   else
      return 1;
}


/******************************************************************************
* Sends the command to start the voltage measurements.
******************************************************************************/
void ltcStartVoltageMeasure()
{
   unsigned char rcvByte = 0;
  
	CSBI = 0; 		
	SPI_send(STCVAD); 	// Send the Cell Volatge A/D Conversion command (0x10) via SPI	
	while(!rcvByte) {rcvByte = SPI_send(JUNK_BYTE);}  //Active wait of the LTC6802 response
   CSBI = 1;                                         //with a high level signal (level polling)
}


///////////////////////////////////////////////////////////////////////////////
void ltcStartOpenWireADC()
{
   unsigned char rcvByte = 0;
   
	CSBI = 0; 		   	// Set CSBI to 0
	SPI_send(STOWAD); 	// Send the Cell Volatge A/D Conversion command (0x20) via SPI
   while(!rcvByte) {rcvByte = SPI_send(JUNK_BYTE);}
	CSBI = 1;		   	// Set CSBI to 1
}


/////////////////////////////////////////////////////////////////////////////// 
void convertVoltages(unsigned char* voltagesByte, unsigned int *voltagesInt)
{
	int n = 0;
	int Cell = 0;
	
	for(; n < NB_CHAR_VOLTAGE; n = n + 3)            
	{
		voltagesInt[Cell] = ((unsigned int)(voltagesByte[n+1] & 0xF) << 8) + voltagesByte[n];
		voltagesInt[Cell] = (voltagesInt[Cell]*3) >> 1;         //From binary ADC value to voltage : V = ADC x 1.5mV
		voltagesInt[Cell+1] = ((unsigned int)voltagesByte[n+2]<<4) | ((voltagesByte[n+1])>>4);
		voltagesInt[Cell+1] = (voltagesInt[Cell+1]*3) >> 1;     //From binary ADC value to voltage : V = ADC x 1.5mV
		if(Cell == 8)                       // hack to compensate the fact that the LTC6802-2 is underevaluating the top cell voltage
	   	voltagesInt[Cell+1] += 92; // when uncommented: balancing doesn't work anymore                                      
		Cell = Cell + 2; 
	}
}


/******************************************************************************
* Send the command to read the voltage measurements.
******************************************************************************/
unsigned char ltcReadCellVoltage(unsigned int *voltagesInt)
{	
	unsigned char voltagesByte[NB_CHAR_VOLTAGE];
   unsigned char pec, crc;
	unsigned int i;
	
	CSBI = 0;
	SPI_send(RDCV);
   
	// Loop to put received measurements (15 8-bits voltages registers)
	for (i = 0; i < NB_CHAR_VOLTAGE; i++)
	  voltagesByte[i] = SPI_send(JUNK_BYTE);	    

   pec = SPI_send(JUNK_BYTE);
	CSBI = 1;
	
   crc = ltcComputeCRC(voltagesByte, 15);   
 
   if(!gSPItimeout)     //We prevent changes to the voltages table while the LTC6802 chip is in standby mode.
      convertVoltages(voltagesByte, voltagesInt);  //to avoid erroneous data sent to the master.
      
   if(crc == pec)
      return 0;
   else
      return 1;
}


/******************************************************************************
* Verify if a cell wire is disconnected and returns 1 in that case. Else it
* returns 0.
******************************************************************************/
unsigned char ltcVerifyOpenWire(void)
{
	unsigned char i, txError=0;
	unsigned int Voltage_Table_A[NB_CELL];
	unsigned int Voltage_Table_B[NB_CELL];
	
   do {
     	ltcStartVoltageMeasure();
      txError = ltcReadCellVoltage(Voltage_Table_A); 
   } while(txError);
   do {
     	ltcStartVoltageMeasure();
      txError = ltcReadCellVoltage(Voltage_Table_B); 
   } while(txError);
   
	// Compare values read on the different mode 
   // If V_cell_b(n+1) - V_cell_a(n+1) >= 200 mV
   // then cell n is disconnected. So we cannot detect a
   // disconnection on cell 10, and the voltage of cell 1 is not used.
   // This method doesnt work when the voltage on the disconnected cell
   // is low. So we add 2 more conditions to indicate an open-wire.
	for (i=1; i<NB_CELL; i++) {
		if(   ((int)Voltage_Table_B[i] - (int)Voltage_Table_A[i]) >= OPENWIRE_THRESHOLD ||
            Voltage_Table_B[i] > 4500) {
			return i;
      } else if(Voltage_Table_B[i] < 500) {
         return i+1;
      }
	}
	
	return 0;
}

 
// Reference: Computation of CRC (wikipedia)
// Most significant bit first (big-endian)
// x^8+x^2+x^1+1 = (1) 0000 0111 = 0x07
unsigned char ltcComputeCRC(unsigned char *array, unsigned char len)
{
   unsigned char rem = 0, i, j;

   for(i=0; i<len; i++) {
      rem = rem ^ array[i];
      for(j=0; j<8; j++) {
         if(rem & 0x80) {   // if leftmost (most significant) bit is set
            rem = (rem << 1) ^ 0x07;
         } else {
            rem = rem << 1;
         }
      }
   }
 
   return rem;
 }