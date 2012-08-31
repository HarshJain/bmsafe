#include "relays.h"
#include "derivative.h"
#include "common.h"
#include "defines.h"
#include "terminal_code.h"



unsigned int gClosingRelays = 0;
unsigned char gCloseRelayState = 0;
unsigned int gTimerLimit = 0;


//*****************************************************************************
// CloseRelays
//
// Description:    
//
//*****************************************************************************
void CloseRelays(unsigned char option) {

   if((option == START_SEQUENCE) && gClosingRelays) {    //On essaie de repartir une s�quence
      SCIprintString("Une s�quence de fermeture des relais est d�j� en cours.\n\n\n\r"); 
      return;                                            //et une autre est d�j� en cours
   }
   
   if((option == CONTINUE_SEQUENCE) && (gCloseRelayState == 0)) {  //Une s�quence a �t� interrompue
      gClosingRelays = 0;                                //on r�initialise la s�quence.
      SCIprintString("La s�quence de fermeture des relais est interrompue.\n\n\n\r");
      return;                                            //Si OpenRelays a interrompu la s�quence, les relais seront ouverts.
   }                                                     //Si CloseRelays a �t� appel� avec CONTINUE_SEQUENCE au lieu de
                                                         //START_SEQUENCE, des relais pourraient demeur�s ferm�. 
                                                         //Par contre la situation peut �tre r�tablie en appelant OpenRelays.

   gClosingRelays = 1;

   switch(gCloseRelayState)
   {
      case 0:
         DISCH_RLY = DISCH_RLY_OPEN;      //Ouverture du circuit de d�charge
         MCR1_HVN = RLY_CLOSED;           //Fermeture d'un relais circuit principal
         delayRelayMs(RELAY_DELAY);
         SCIprintString("La s�quence de fermeture des relais est activ�e.\n\n\n\r");
         break;
      
      case 1:
         MCR2_P2P = RLY_CLOSED;        //Fermeture d'un relais circuit principal
         delayRelayMs(RELAY_DELAY);
         break;
      
      case 2:
         MCR4_HVP = RLY_CLOSED;        //Fermeture d'un relais circuit principal
         delayRelayMs(RELAY_DELAY);
         break;
      
      case 3:
         MCR5_PRE = RLY_CLOSED;        //Fermeture relais du circuit de pr�charge
         delayRelayMs(PRECHARGE_DELAY);
         break;
      
      case 4:
         MCR3_P1N = RLY_CLOSED;        //Fermeture dernier relais circuit principal
         delayRelayMs(RELAY_DELAY);
         break;
         
      case 5:
         MCR5_PRE = RLY_OPEN;       //Ouverture de relais de pr�charge
         break;
      
      default:
         gCloseRelayState = 5;
   }

   gCloseRelayState++;
   if(gCloseRelayState > 5) {    //La s�quence de fermeture des relais est termin�e
      gCloseRelayState = 0;
      gClosingRelays = 0;
      gFlags.relaysClosed = 1;
      
      //TODO: enlever cette commande de cette fonction, ajouter une commande pour fermer mosfet manuellement.
      SUPPLY_24V_RLY = 1;        //On peut maintenant alimenter la barre 24V � partir du DC-DC.
      
	   SCIprintString("Les relais sont maintenant ferm�s.\n\n\n\r");
   }
}


void OpenRelays(void) {

   if(gClosingRelays)  {    //Les relais sont en train de se fermer.
      gCloseRelayState = 0;
      SCIprintString("La s�quence de fermeture des relais est en cours d'interruption.\n\r");
   }
      
   MCR1_HVN = RLY_OPEN;
   MCR2_P2P = RLY_OPEN;
   MCR3_P1N = RLY_OPEN;
   MCR4_HVP = RLY_OPEN;
   MCR5_PRE = RLY_OPEN;
   DISCH_RLY = DISCH_RLY_CLOSED;
    
   gFlags.relaysClosed = 0;
   
   SUPPLY_24V_RLY = 0;        //Le DC-DC n'alimente plus la barre 24V
   
   SCIprintString("Relais ouverts.\n\n\n\r");
}


//*****************************************************************************
// delayMs
//
// Description: Function that waits a number of milliseconds.
// Entr�e: si numMs = 0 alors on continue avec l'ancien d�lai demand�.
//
//*****************************************************************************
void delayRelayMs(unsigned int numMs)
{
   gTimerLimit = numMs;
   PITCE_PCE0 = 1;           //PIT0 (1 ms timeout) activated
}


