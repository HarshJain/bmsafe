#include "relays.h"
#include "derivative.h"
#include "common.h"
#include "defines.h"


//*****************************************************************************
// OpenRelays
//
// Description:    
//
//*****************************************************************************
void CloseRelays(void) {

    //Ouverture du circuit de décharge
    DISCH_RLY = DISCH_RLY_OPEN;

   //Fermeture des relais du circuit principal
   MCR1_HVN = RLY_CLOSED;
   delayMs(RELAY_DELAY);
   MCR2_P2P = RLY_CLOSED;
   delayMs(RELAY_DELAY);
   MCR4_HVP = RLY_CLOSED;
   delayMs(RELAY_DELAY);
   
   //Fermeture relais du circuit de précharge
   MCR5_PRE = RLY_CLOSED;  
   delayMs(PRECHARGE_DELAY);
   
   //Fermeture dernier relais circuit principal
   MCR3_P1N = RLY_CLOSED;
   delayMs(RELAY_DELAY);
   
   //Ouverture relais précharge
   MCR5_PRE = RLY_OPEN;
}


void OpenRelays(void) {
    MCR1_HVN = RLY_OPEN;
    MCR2_P2P = RLY_OPEN;
    MCR3_P1N = RLY_OPEN;
    MCR4_HVP = RLY_OPEN;  
    DISCH_RLY = DISCH_RLY_CLOSED;
}
