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

   if((option == START_SEQUENCE) && gClosingRelays) {    //On essaie de repartir une séquence
      SCIprintString("Une séquence de fermeture des relais est déjà en cours.\n\n\n\r"); 
      return;                                            //et une autre est déjà en cours
   }
   
   if((option == CONTINUE_SEQUENCE) && (gCloseRelayState == 0)) {  //Une séquence a été interrompue
      gClosingRelays = 0;                                //on réinitialise la séquence.
      SCIprintString("La séquence de fermeture des relais est interrompue.\n\n\n\r");
      return;                                            //Si OpenRelays a interrompu la séquence, les relais seront ouverts.
   }                                                     //Si CloseRelays a été appelé avec CONTINUE_SEQUENCE au lieu de
                                                         //START_SEQUENCE, des relais pourraient demeurés fermé. 
                                                         //Par contre la situation peut être rétablie en appelant OpenRelays.

   gClosingRelays = 1;

   switch(gCloseRelayState)
   {
      case 0:
         DISCH_RLY = DISCH_RLY_OPEN;      //Ouverture du circuit de décharge
         MCR1_HVN = RLY_CLOSED;           //Fermeture d'un relais circuit principal
         delayRelayMs(RELAY_DELAY);
         SCIprintString("La séquence de fermeture des relais est activée.\n\n\n\r");
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
         MCR5_PRE = RLY_CLOSED;        //Fermeture relais du circuit de précharge
         delayRelayMs(PRECHARGE_DELAY);
         break;
      
      case 4:
         MCR3_P1N = RLY_CLOSED;        //Fermeture dernier relais circuit principal
         delayRelayMs(RELAY_DELAY);
         break;
         
      case 5:
         MCR5_PRE = RLY_OPEN;       //Ouverture de relais de précharge
         break;
      
      default:
         gCloseRelayState = 5;
   }

   gCloseRelayState++;
   if(gCloseRelayState > 5) {    //La séquence de fermeture des relais est terminée
      gCloseRelayState = 0;
      gClosingRelays = 0;
      gFlags.relaysClosed = 1;
      
      //TODO: enlever cette commande de cette fonction, ajouter une commande pour fermer mosfet manuellement.
      SUPPLY_24V_RLY = 1;        //On peut maintenant alimenter la barre 24V à partir du DC-DC.
      
	   SCIprintString("Les relais sont maintenant fermés.\n\n\n\r");
   }
}


void OpenRelays(void) {

   if(gClosingRelays)  {    //Les relais sont en train de se fermer.
      gCloseRelayState = 0;
      SCIprintString("La séquence de fermeture des relais est en cours d'interruption.\n\r");
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
// Entrée: si numMs = 0 alors on continue avec l'ancien délai demandé.
//
//*****************************************************************************
void delayRelayMs(unsigned int numMs)
{
   gTimerLimit = numMs;
   PITCE_PCE0 = 1;           //PIT0 (1 ms timeout) activated
}


