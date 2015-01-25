#include "ltc6803.h"
#include "derivative.h"
#include "defines.h"



/******************************************************************************
* Sends a single byte on the SPI1 peripheral. The function blocks until the
* byte is sent. Returns the received byte.
******************************************************************************/
uint8 SPI_send(uint8 sendByte)
{
   uint32 startTime;
  
   while(!SPI1SR_SPTEF){} // True means the data register is not empty and some data needs to be send, we wait
   
   startTime = gElapsedTime;
   SPI1DR = sendByte;     // Put sendByte on Serial Data Register
   
   // SPIF : 0 = transfer not yet completed, 1 = New data copied to SPIDR
   while (!SPI1SR_SPIF) {
      if((gElapsedTime - startTime) > SPI_SEND_TIMEOUT) {
         gFlags.spiTimeout = 1;
         break;
      }
   }
   
   return SPI1DR;         // Return received byte and clear SPIF flag
}


/******************************************************************************
* Use the equilibration vector to make the new ltc6803 config register .
******************************************************************************/
void ltcMakeConfigRegister(Config_Reg_t *ltcConfig, uint16 balanceVector)
{
   ltcConfig->CFGR1 = (uint8) (balanceVector & 0x00FF);
   ltcConfig->CFGR2 = (ltcConfig->CFGR2 & 0xF0) | (uint8)((balanceVector & 0x0F00) >> 8);
}


/******************************************************************************
* Sends the configuration register to the battery monitor chip (LTC6803).
******************************************************************************/
void ltcWriteConfig(Config_Reg_t *ltcConfig)
{
	CSBI = 0; 					   	// Set CSBI to 0
	SPI_send(WRCFG); 	   			// Send the Write Config command (0x01) by SPI
	SPI_send(ltcComputePEC(WRCFG));	// Send the PEC (packet error code) of WRCFG command
	
	// Send the config registers 0 to 5
	SPI_send(ltcConfig->CFGR0);  	
	SPI_send(ltcConfig->CFGR1);
	SPI_send(ltcConfig->CFGR2);
	SPI_send(ltcConfig->CFGR3);
	SPI_send(ltcConfig->CFGR4);
	SPI_send(ltcConfig->CFGR5);
	
	// Calculate and send the PEC for the 6 configure registers sent
	SPI_send(ltcComputeCRC((uint8*)ltcConfig,6));
	
	CSBI = 1;					      // Set CSBI to 1 to complete command sequence
}


/******************************************************************************
* Reads the configuration of the LTC chip
******************************************************************************/
uint8 ltcReadConfig(uint8* rcvConfig)
{
	uint8 i, pec, crc;
   
	CSBI = 0; 						// Select the SPI device (LTC chip)
	SPI_send(RDCFG); 				// Send Read config command
	SPI_send(ltcComputePEC(RDCFG)); // Sent the PEC (packet error code) byte
	
	for(i=0; i<6; i++)      	//6 config registers
		rcvConfig[i] = SPI_send(JUNK_BYTE); // Receive and store the config registers

	pec = SPI_send(JUNK_BYTE); 	// Receive de PEC byte
	CSBI = 1;      				// Release the Slave Select signal

	crc = ltcComputeCRC(rcvConfig, 6);   // Compute the CRC (PEC)
   
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
	uint8 rcvByte = 0;
  
	CSBI = 0; 		
	SPI_send(STCVAD); 					// Send the Cell Volatge A/D Conversion command (0x10) via SPI
	SPI_send(ltcComputePEC(STCVAD));	// Send the PEC (packet error code) byte
 
	CSBI = 1;  
}

/******************************************************************************
* Starts the Oen Wire ADC sequence by sendint the corresponding command
******************************************************************************/
void ltcStartOpenWireADC()
{
	uint8 rcvByte = 0;
	uint32 startTime = 0;
   
	CSBI = 0;
	SPI_send(STOWAD); 	// Send the Cell Volatge A/D Conversion command (0x20) via SPI
	SPI_send(ltcComputePEC(STOWAD)); // Send the PEC byte
	
	//Active wait of the LTC6803 response (takes about 1 ms) with a high level signal (level polling)
	startTime = gElapsedTime;
	while(!rcvByte)
	{
		rcvByte = SPI_send(JUNK_BYTE);
	
		if((gElapsedTime - startTime) > SPI_CONN_TIMEOUT)
		{
			// After SPI_CONN_TIMEOUT, we consider the SPI connection is lost
			gFlags.spiTimeout = 1;		// SPI timeout indicator
			break;
		}
	}
	
	CSBI = 1;		   	// Set CSBI to 1
}


/////////////////////////////////////////////////////////////////////////////// 
void convertVoltages(uint8* voltagesByte, int16 *voltagesInt)
{
	uint8 n = 0;
	uint8 Cell = 0;
    
	for(n=0; n < NB_CHAR_VOLTAGE; n+=3)            
	{
		voltagesInt[Cell] = ((int16)(voltagesByte[n+1] & 0xF) << 8) + voltagesByte[n];
		voltagesInt[Cell] = ( (voltagesInt[Cell] - 512) * 3) >> 1;         //From binary ADC value to voltage : V = (ADC - 512) x 1.5mV
		voltagesInt[Cell+1] = ((int16)voltagesByte[n+2]<<4) | ((voltagesByte[n+1])>>4);
		voltagesInt[Cell+1] = ((voltagesInt[Cell+1] - 512) * 3) >> 1;     //From binary ADC value to voltage : V = (ADC - 512) x 1.5mV                              
		Cell = Cell + 2;
	}
}


/******************************************************************************
* Send the command to read the voltage measurements, convert and store the data.
******************************************************************************/
uint8 ltcReadCellVoltage(int16 *voltagesInt)
{
	uint8 voltagesByte[NB_CHAR_VOLTAGE];
	uint8 pec, crc;
	uint16 i;
	
	CSBI = 0;
	SPI_send(RDCV);					// Send the command
	SPI_send(ltcComputePEC(RDCV));	// Send PEC byte for the command
   
	// Loop to store received voltage measurements (18 8-bits voltages registers)
	for (i = 0; i < NB_CHAR_VOLTAGE; i++)
	  voltagesByte[i] = SPI_send(JUNK_BYTE);

	pec = SPI_send(JUNK_BYTE);
	CSBI = 1;
	
	crc = ltcComputeCRC(voltagesByte, NB_CHAR_VOLTAGE);
 
	if(crc != pec) // No point of making the conversion if the error code is not valid
		return 1;
 
	if(!gFlags.spiTimeout)     //We prevent changes to the voltages table while the LTC6803 chip is in standby mode.
		convertVoltages(voltagesByte, voltagesInt);  //to avoid erroneous data sent to the master

	return 0; // Normal return (crc == pec)
}

/******************************************************************************
* Send the command to read the voltage measurements and store de data (without converting it)
******************************************************************************/
uint8 ltcReadCellVoltageRaw(uint8 *voltagesByte)
{
	uint8 pec, crc;
	uint16 i;
	
	CSBI = 0;
	SPI_send(RDCV);					// Send the Read Voltage command
	SPI_send(ltcComputePEC(RDCV));	// Send PEC byte for the command
   
	// Loop to put received measurements (18 8-bits voltages registers)
	for (i = 0; i < NB_CHAR_VOLTAGE; i++)
	  voltagesByte[i] = SPI_send(JUNK_BYTE);

	pec = SPI_send(JUNK_BYTE);
	CSBI = 1;
  
	crc = ltcComputeCRC(voltagesByte, NB_CHAR_VOLTAGE);
 
	if(crc != pec)
		return 1;
	else
		return 0; // Normal return : crc == pec
}
/******************************************************************************
* Verify if a cell wire is disconnected.
* Returns the number of the cell with an open connection if there is, ortherwise return 0
*
* For more details : see LTC6803 datasheet, section Open-Connector Detection
******************************************************************************/
uint8 ltcVerifyOpenWire(void)
{
	uint8 i, txError=0;
	int16 Voltage_Table_A[NB_CELL];
	int16 Voltage_Table_B[NB_CELL];
	
	// First reading, store in Table A
	do {
     	ltcStartOpenWireADC();
		txError = ltcReadCellVoltage(Voltage_Table_A);
	} while(txError);
   
	// Second reading, store in Table B
	do {
     	ltcStartOpenWireADC();
		txError = ltcReadCellVoltage(Voltage_Table_B);
	} while(txError);
   
	// Compare values read
	// If V_cell_b(n+1) - V_cell_a(n+1) >= 200 mV
	// then cell n is disconnected. So we cannot detect a
	// disconnection on cell 12, and the voltage of cell 1 is not used.
	// This method doesnt work when the voltage on the disconnected cell
	// is low. So we add 2 more conditions to indicate an open-wire.
	
	
	// Check open connection for the bottom cell
	if(Voltage_Table_A[0] < 0 || Voltage_Table_B[0] < 0) // true means V-/C0 must be open
		return 1; // Return the cell number
	
	// Check open connection for the top cell
	if(Voltage_Table_A[NB_CELL - 1] < 0 || Voltage_Table_B[NB_CELL - 1] < 0) // true means C12 must be open
		return NB_CELL; // return the cell number
	
	// Check for the middle cells TODO : on garde toute ces étapes ?
	for (i=1; i < NB_CELL; i++) { // from 1 to NB_Cell - 1
	
		if(((int)Voltage_Table_B[i] - (int)Voltage_Table_A[i]) >= OPENWIRE_THRESHOLD ||
            Voltage_Table_B[i] > 5000) // true means there must be an open connection on cell i
		{
			return i; // Return the cell number
		}
	}
	
	return 0;
}


// Reference: Computation of CRC (wikipedia)
// Most significant bit first (big-endian)
// x^8+x^2+x^1+1 = (1) 0000 0111 = 0x07
uint8 ltcComputeCRC(uint8 *array, uint8 len)
{
   uint8 i, j;
   uint8 rem = 0b01000001; // PEC initial

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
 
 
 // Compute CRC for one byte, usefule when the parameter is not a variable (can't send the adress)
 uint8 ltcComputePEC(uint8 byte)
 {
	return ltcComputeCRC(&byte, 1);
 }
 
 /******************************************************************************
  * Use to check if the LTC is accepting the commands
  * For more details : See "ADC Register Clear Command" section in LTC6803 datasheet
  ******************************************************************************/
 uint8 ltcClearADC()
 {
	uint8 voltageByte[NB_CHAR_VOLTAGE]; // Will contain the register data
	uint8 i, rcvByte = 0;
	uint16 startTime = 0;

	CSBI = 0; 		
	SPI_send(CLEAR); 					// Send the CLEAR command
	SPI_send(ltcComputePEC(CLEAR));		// Send the PEC (packet error code) byte for the command

	//Active wait of the LTC6803 response (takes about 1 ms) with a high level signal (level polling)
	startTime = gElapsedTime;
	while(!rcvByte)
	{
		rcvByte = SPI_send(JUNK_BYTE);

		if((gElapsedTime - startTime) > SPI_CONN_TIMEOUT)
		{
			// After SPI_CONN_TIMEOUT, we consider the SPI connection is lost
			gFlags.spiTimeout = 1;
			return 1;
		}
	}

	
	CSBI = 1;

	// Perform verification of the registers
	startTime = gElapsedTime;
	while(ltcReadCellVoltageRaw(voltageByte)) // Get the data, returns 0 when no errors
	{
		if(gElapsedTime - startTime < SPI_CONN_TIMEOUT)
			return 1;
	}

	for(i = 0; i < NB_CHAR_VOLTAGE; i++)
	{
		if(voltageByte[i] != 0xFF) // CLEAR should set all registers to 0xFF
			return 1; // CLEAR command did not work
	}

	return 0; // OK, the CLEAR command was accepted by the LTC
 }
 
 
// *******************************************************
// Poll LTC6803 to verify if the conversions are done
// Return : 1 = conversion finished
//			0 = conversion not finished
// *******************************************************
 uint8 ltcConversionFinished() 
 {
	uint8 rcvByte = 0;
	
	CSBI = 0;
	SPI_send(PLADC);					// Poll ADC command
	SPI_send(ltcComputePEC(PLADC)); 	// Send the PEC byte
		
	rcvByte = SPI_send(JUNK_BYTE);		// Check SDO state (rcvByte = SDO)

	CSBI = 1;							// Release the SPI device (LTC)
	
	if(rcvByte != 0) 	// If SDO is not low, conversions are finished
		return 1;
	
  	return 0;			// Return 0 (conversions are not done yet)
 }

// *******************************************************
// Read and verify the LTC configuration register
// Return : 1 = OK
//			0 = Registers have been reset
// *******************************************************
 uint8 ltcConfigurationOK(Config_Reg_t *ltcConfig)
 {
	static uint16 startTime = 0;
	uint8 rcvConfig[6];

	
	if(gFlags.spiTimeout)				// The spi communication has previously timed out
		ltcWriteConfig(ltcConfig);		// Write to the configuration register

	startTime = gElapsedTime;	// Time reference
	while(ltcReadConfig(rcvConfig) != 0)	// Read the configuration register values without error
	{
		if((gElapsedTime - startTime) > SPI_CONN_TIMEOUT)
		{
			// After SPI_CONN_TIMEOUT, we consider the SPI connection is lost
			gFlags.spiTimeout = 1;		// SPI timeout indicator
			return 0;					// Unable to read correctly the register values
		}
	}

	
	// Verify the register values
	
	// ------------- On vérifie que le WDT n'est pas dépassé ------------
		
	// Une condition vrai signifie que le registre de configuration du LTC6803
	// a ete reinitialise par le watchdog timer. On n'utilise pas le bit WTD pour
	// detecter une expiration du WTD (ne fonctionne pas). On utilise les bits CDC parce
	// que la valeur utilisee (1) est differente de la valeur par defaut (0).
	
	if((rcvConfig[0] & 0x01) == 0x00)		// True = config registers have been reset
	{
		ltcWriteConfig(ltcConfig);			// Write to the configuration register 
		return 0;
	}
	else
	{
		gFlags.spiTimeout = 0;				// Configuration registers are OK
		return 1;
	}

	// Quand le WTD expire, les registres de configuration sont reinitialises.
	// La puce est alors en mode standby, et aucune nouvelle mesure ne peut etre prise.
	// Une commande de lecture des tensions semble retourner la derniere mesure de tensions,
	// donc aucune valeur erronnee n'est transmise au maitre.
 }
 
// *******************************************************
// Perform a LTC chip self check
// Return : 0 = OK
//			1 = ERROR
// *******************************************************
 uint8 ltcSelfCheck()
 {
	uint8 voltageByte[NB_CHAR_VOLTAGE]; // Will contain the register data
	uint8 i, rcvByte = 0;
	uint16 startTime = 0;

	if(gVoltState == SELF_TEST_START)
	{
		CSBI = 0;
		SPI_send(STEST2); 					// Send the CLEAR command
		SPI_send(ltcComputePEC(STEST2));	// Send the PEC (packet error code) byte for the command
		CSBI = 1;
		
		gVoltState = SELF_TEST_WAIT;		// Performing a self test, wait for results
		
		return 0;
	}
	else if(gVoltState == SELF_TEST_WAIT)
	{
		gVoltState = V_STANDBY;
		
		// Perform verification of the registers
		startTime = gElapsedTime;
		while(ltcReadCellVoltageRaw(voltageByte)) // Get the data, returns 0 when no errors
		{
			if( (gElapsedTime - startTime) > SPI_CONN_TIMEOUT)
				return 1; // Unable to retreive data from the LTC
		}

		for(i = 0; i < NB_CHAR_VOLTAGE; i++)
		{
			if(voltageByte[i] != 0xAA) // CLEAR should set all registers to 0xFF
				return 1; // CLEAR command did not work
		}
		
		return 0;	// Successful self test
	}
	else // LTC is busy with other operations
		return 0;

 }
 
 // ----------------------------------------------------------------------------------
 // Does an LTC diagnostic (multiplexer and reference self test)
 //	This function will wait about 20 ms for the LTC chip to finish the diagnostic : do not call to often this function.
 //
 // Return : 0 = OK, 1 = ERROR
 // ----------------------------------------------------------------------------------
 uint8 ltcDiagnostic()
 {
	uint8 rcvByte = 0, pec = 0, crc = 1;
	uint16 startTime = 0;
	uint8 diagReg[2];				// Diagnostic register
	volatile uint16 refVoltage;		// Reference voltage (measure result)
	uint8 voltageBytes[NB_CHAR_VOLTAGE];

	if(gVoltState == DIAGNOSTIC_START) // Time to start the diagnostic test
	{
		CSBI = 0;

		SPI_send(DIAGN);					// Poll ADC command
		SPI_send(ltcComputePEC(DIAGN)); 	// Send the PEC byte

		rcvByte = SPI_send(JUNK_BYTE);		// Check SDO state (rcvByte = SDO)

		CSBI = 1;							// Release the SPI device (LTC)
		
		gVoltState = DIAGNOSTIC_WAIT;
	 }
	 else if(gVoltState == DIAGNOSTIC_WAIT)
	 {		
		CSBI = 0;
		// Read the test results (Diagnostic register)
		SPI_send(RDDGNR);
		SPI_send(ltcComputePEC(RDDGNR));
		
		diagReg[0] = SPI_send(JUNK_BYTE);
		diagReg[1] = SPI_send(JUNK_BYTE);
		
		pec = SPI_send(JUNK_BYTE);
	
		crc = ltcComputeCRC(diagReg, 2);
 
		if(crc != pec)
			return 1;		
		
		CSBI = 1;							// Release the SPI device (LTC)
				
		refVoltage = ((uint16)diagReg[0] | (((uint16)diagReg[1]) & 0b1111) << 8);
		refVoltage = ((refVoltage - 512) * 3) >> 1;		// Voltage = (ADC result - 512) * 1.5 mV
		
		gVoltState = V_STANDBY;
		
		if(refVoltage < 2900 && refVoltage > 2100) // Voltage between these values means the test is passed
			return 0;
		else
			return 1;	// Test fail
	}
 }
