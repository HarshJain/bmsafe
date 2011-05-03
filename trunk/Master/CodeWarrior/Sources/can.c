#include "can.h"
#include "derivative.h"
#include "defines.h"


//**************************
//  Variables definitions
//**************************

uint RXID = 0;
uchar idleCount[N_MOD_MAX];
uchar addrCANLSB = 0xB0;		         //adresse du module CAN récepteur
uchar addrCANMSB = 0x00;
uchar errorflag = NO_ERR;



//*************
//  Functions
//************* 



/*
 * CAN0SendInt: Transmit an array of int by CAN0
 *
 * Parameters:  id: the usual CAN ID times 0x200000
 *              priority: the transmit buffer priority
 *              start: the first item of the array you want to transmit
 *              length: the length of the table of int
 *              *txdata: the name of the array
 *
 * Return :
 */
unsigned char CAN0SendInt(unsigned long id, unsigned char priority, unsigned char start,
                           unsigned char length, unsigned int txdata[] )
 {
    
    unsigned char index;
    unsigned char MSB;
    unsigned char txbuffer = {0};

    if (!CAN0TFLG)              /* Is Transmit Buffer full?? */
        return ERR_BUFFER_FULL;

    CAN0TBSEL = CAN0TFLG;       /* Select lowest empty buffer */
    txbuffer = CAN0TBSEL;		/* Backup selected buffer */
    
    /* Load Id to IDR Registers */
    *((unsigned long *) ((unsigned long)(&CAN0TXIDR0)))= id;
        
    for (index=0;index<length;index++) {           //H: La modification que j'essaie pour pouvoir utiliser les int
        MSB = txdata[index+start] >> 8;
        *(&CAN0TXDSR0 + 2*index) = MSB; 
        *(&CAN0TXDSR0 + 2*index + 1) = (unsigned char) txdata[index+start];  /* Load data to Tx buffer
                                                  * Data Segment Registers*/                                          
    }

    CAN0TXDLR = length*2;                        /* Set Data Length Code */
    CAN0TXTBPR = priority;                       /* Set Priority */

    CAN0TFLG = txbuffer;	                     /* Start transmission */
                
    while ( (CAN0TFLG & txbuffer) != txbuffer);  /* Wait for Transmission 
                                                  * completion 
                                                  */
    return NO_ERR;
 }
 
 
unsigned char CAN0SendCommand(unsigned char command, unsigned char id, //if slaveID = 3F: Broadcast. Not implemented.
                              unsigned int param1,   unsigned int  param2)
{
    unsigned int txdata[4] = {0,0,0,0};
    errorflag = NO_ERR;
    
    txdata[0] = command;
    txdata[1] = param1;
    txdata[2] = param2;
    txdata[3] = 0xFFFF;
    
    errorflag = CAN0SendInt(ID_500, 0x03, 0, 4, txdata);//ID,priorité,start,length

    return errorflag;
}

/*

void init_portM() {

//TXCAN0 est sur la broche 44 = PTM1
//RXCAN0 est sur la broche 42 = PTM0
  DDRM_DDRM0 = 0; //config en entrée
  DDRM_DDRM1 = 1; //config en entrée
  MODRR = 0x00; 
}


//Initialisation du bus CAN 0
//
//Baud rate = 125 KHz
//Bit time = Time Quanta(Tq) * (Total Time Quanta)
//Bit Time = Prescaler value/Fclk*(1+Time Segment 1+Time Segment2)
//Fclk = 2 MHz
//Fq = 1 MHz --> Prescaler = 2
//Time seg 1 = 4
//Time seg 2 = 3

//Baud Rate = 1 Mbps
//Fclk = 8 MHz
//Bit Time = Prescaler value/Fclk*(1+Time Segment 1+Time Segment2)
//Prescaler = 1
//Time seg 1 = 4
//Time seg 2 = 3
//
//CAN en mode 2.0A : identificateur sur 11 bits
//filtres de l'identificateur : 4 registres de 16 bits
//adresse CAN : 0x00 | addr_CAN 

void init_CAN0(uchar addrLSB, uchar addrMSB) {

  CAN0CTL0_INITRQ = 1; //mode d'initialisation, nécessaire pour initialiser le module CAN
  
  //while (!(CAN0CTL1&0x01)){};	// Wait for Initialization Mode acknowledge INITAK bit = 1
  while (CAN0CTL1_INITAK == 0){};	// Wait for Initialization Mode acknowledge INITAK bit = 1
  
  //initialisation des modes de fonctionnement du CAN
  //on allume le mscan
  CAN0CTL1_CANE =1;
  // on utilise comme horloge le bus clock
  CAN0CTL1_CLKSRC =1;
  // on désactive le mode loop back self test
  CAN0CTL1_LOOPB = 0;
  // on ne se met pas en mode ecoute continu
  CAN0CTL1_LISTEN =0;
   // Bus off recovery automatic
  CAN0CTL1_BORM =0;
  // config du wake up dès qu'on a un état dominant (0) sur le bus
  CAN0CTL0_WUPE = 1;
  CAN0CTL1_WUPM =0;
  
  // on regle le timming
  //Tolerance pour la resynchronisation (maximum = 4 Tq)
  //Time quanta a 2 clock cycle
  //CAN0BTR0 = 0xC7;
  CAN0BTR0 = 0x40;
  //un seul echantillon par symbole
  //3 Tq sur SEG2
  //4 Tq sur SEG1
  CAN0BTR1 = 0x3A;
 
  // Acceptance Filters
  CAN0IDAC = 0x10;		// 4 16-bit identifier filters
  //CAN0IDAC = 0x30;      // Filters closed. No messages comes in.
  
  // Identifier Mask register
  //Chaque bit mis à 0 indique que la comparaison avec le bits correspondant
  //dans CAN0IDARx va se faire 
  //Chaque bits mis a 1 indique que la comparaison ne vas pas se faie
  //avec le bit corrrespondant dans CAN0IDARx
  //on utilise un identificateur de 11 bits (spéc CAN 2.0A), on ignore le IDE 
  //--> on n'a besoin que de 11 bits pour le masque de l'identificateur
  
  
  //CAN0IDMR0 = 0x00;     //8 bits utilisés pour la comparaison
  //CAN0IDMR1 = 0x07;		  //Ignore le IDE  |  3 bits de LSB(ID CAN) utilisés
  //définition des 11 bits de l'identificateur du module CAN
  //
  //CAN0IDAR0 = addrMSB;                 // Identifiant module CAN
  //CAN0IDAR1 = addrLSB;              // Identifiant module CAN 
  
  //CAN0IDMR2 = 0x00;     //Pas utilisé
  //CAN0IDMR3 = 0x07;     //Pas utilisé
  //CAN0IDAR2 = 0x00;                 
  //CAN0IDAR3 = 0x00;
  
  //CAN0IDMR4 = 0x00;     //Pas utilisé
  //CAN0IDMR5 = 0x07;     //Pas utilisé
  //CAN0IDAR4 = 0x00;                 
  //CAN0IDAR5 = 0x00; 
  
  //CAN0IDMR6 = 0x00;     //Pas utilisé
  //CAN0IDMR7 = 0x07;     //Pas utilisé 
  //CAN0IDAR6 = 0x00;                 
  //CAN0IDAR7 = 0x00;
  
  //CAN0IDMR0 = 0x00;     //8 bits utilisés pour la comparaison
  //CAN0IDMR1 = 0x07;		  //Ignore le IDE  |  3 bits de LSB(ID CAN) utilisés
  //définition des 11 bits de l'identificateur du module CAN
  //
  CAN0IDMR0 = 0xFF;      // Passe-tout
  CAN0IDMR1 = 0xFF;      // Passe-tout
  CAN0IDAR0 = addrMSB;   // Identifiant module CAN
  CAN0IDAR1 = addrLSB;   // Identifiant module CAN 
  
  CAN0IDMR2 = 0xFF;     //Pas utilisé
  CAN0IDMR3 = 0xFF;     //Pas utilisé

  
  CAN0IDMR4 = 0xFF;     //Pas utilisé
  CAN0IDMR5 = 0xFF;     //Pas utilisé

  
  CAN0IDMR6 = 0xFF;     //Pas utilisé
  CAN0IDMR7 = 0xFF;     //Pas utilisé  
  
  
  
  //ATTENTION : on ne peut pas écrire dans les registres d'autorisation d'interruption
  //en mode initialisation
  
  // on sort du mode d'initialisation pour revenir dans le mode normal 
  CAN0CTL0_INITRQ = 0;            
  //while ((CAN0CTL1&0x01) != 0){};	//on attend le retour du mode normal
  while (CAN0CTL1_INITAK == 1){};	//on attend le retour du mode normal
     
  //activation de l'interruption lors de la réception, pas lors de la transmisison
  CAN0RIER_RXFIE = 1;		// enable receive interrupts 
  CAN0TIER = 0x00;		// no Emit buffer empty interrupts  
}
*/

/*
 * CAN1SendChar: Transmit an array of char by CAN0
 *
 * Parameters:  id: the usual CAN ID times 0x200000
 *              priority: the transmit buffer priority
 *              length: the length of the table of char
 *              *txdata: the name of the array
 *
 * Return :
 */
unsigned char CAN1SendChar(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata ){
    
    unsigned char index;
    unsigned char txbuffer = {0};

    if (!CAN1TFLG)              /* Is Transmit Buffer full?? */
        return ERR_BUFFER_FULL;

    CAN1TBSEL = CAN1TFLG;       /* Select lowest empty buffer */
    txbuffer = CAN1TBSEL;		/* Backup selected buffer */
    
    /* Load Id to IDR Registers */
    *((unsigned long *) ((unsigned long)(&CAN1TXIDR0)))= id;
    //CAN0RXIDR0 <<3 | CAN0RXIDR1 >> 5
    //CAN1TXIDR0 = 0b11000011;
    //CAN1TXIDR1 = 0b000;
        
    for (index=0;index<length;index++) {
        *(&CAN1TXDSR0 + index) = txdata[index];  /* Load data to Tx buffer 
                                                  * Data Segment Registers
                                                  */
    }

    CAN1TXDLR = length;                          /* Set Data Length Code */
    CAN1TXTBPR = priority;                       /* Set Priority */

    CAN1TFLG = txbuffer;	                     /* Start transmission */
                
    while ( (CAN1TFLG & txbuffer) != txbuffer);  /* Wait for Transmission 
                                                  * completion 
                                                  */
    return NO_ERR;
 }