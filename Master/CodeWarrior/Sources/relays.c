#include "relays.h"
#include "derivative.h"
#include "common.h"


 //*****************************************************************************
// OpenRelays
//
// Description:    
//
//*****************************************************************************

  void PeriphInit(void){
    
  // PWME_PWME0 = 1;        // PWM0 ENABLED
  // PWMPOL_PPOL0 = 0;      // LE PWM COMMENCE SUR UN ZERO
  // PWMCLK_PCLK0 = 0;      // CLOCK A FOR PWN CLOCK SOURCE
  // PWMPRCLK_PCKA2 = 1;    // PRESCALER : BUS CLOCK (16/2 MHZ) DIVIDED BY 128 (62,5 KHZ)
  // PWMPRCLK_PCKA1 = 1; 
  // PWMPRCLK_PCKA0 = 1; 
  // PWMCAE_CAE0 = 0;       // LEFT ALLIGNEMENT
  // PWMPER0 = 0xFF;        // PERIODE DE 256 PERIODES DE CLOCK A (245 Hz) 
  // PWMDTY0 = 0x7F;        // DUTY CYCLE 50 %
  
  
  // DDRA = 0xFF;              // LES PINS DU PORT A EN SORTIE
  
  
  }


//*****************************************************************************
// OpenRelays
//
// Description:    
//
//*****************************************************************************
void CloseRelays(unsigned int num) {

   PORTA_PA0 = 1;
   delayMs(num);
   PORTA_PA1 = 1;
   delayMs(num);
   PORTA_PA2 = 1;
   delayMs(num);
   PORTA_PA3 = 1;
   delayMs(num);

}


void OpenRelays(unsigned int num) {

   PORTA_PA0 = 0;
   PORTA_PA1 = 0;
   PORTA_PA2 = 0;
   PORTA_PA3 = 0;
   delayMs(num);
   
}

void Precharge(unsigned int time) {

   PORTA_PA4 = 1;     
   delayMs(time);
   PORTA_PA4 = 0;
   
}
   