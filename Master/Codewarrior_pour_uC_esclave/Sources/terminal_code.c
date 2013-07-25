
#include <hidef.h>              /* common defines and macros */
#include "derivative.h"         /* derivative-specific definitions */

#include <stdio.h>               // pour utiliser printf et sprintf
#include "terminal_code.h"
#include "relays.h"
#include "defines.h"
#include "can.h"


//**************************
//  Variables definitions
//************************** 

// définitions générales
const char* formatTemp = "%2.1f oC";       //Temperature en dixième de oC
const char* formatVolt = "%1.3f V";        //1 mV precision
const char* formatCurr = "%d A";           //Les courants en ampères (paramètres)
const char* formatCurrMa = "%1.3f A";      //Les courants en ampères (précision 10 mA)
const char* formatTime = "%d ms";          //Temps en ms
const char* formatCellId = " (pack #%u cell #%u)";          //Temps en ms

// système d'index pour les menus
unsigned int niveau_1 = 0;                  // 1 = configuration, 2 = status, 3 = commandes
unsigned int niveau_2 = 0;

// strings du menu d'affichage des paramètres                                            
unsigned int gSciDisplayNumModule;                       // ID du slave à afficher
unsigned int gSciInterruptMode = 0;                      //0 = mode menu, 1 = mode assignation

unsigned char gGuiBuffer[GUI_RX_BUFFER_SIZE];            //Un tampon contenant les bytes reçus du lien avec l'usager à traiter.
unsigned char gGuiReadIndex = 0;
unsigned char gGuiWriteIndex = 0;
unsigned char gGuiBufferFull = 0;


unsigned int printStatTime = 3600;	// Nombre de temps (en secondes) pendant lequel on envoi les données statistiques
unsigned int timeFollow = 0; 		// Simple compteur pour pouvoir savoir combien de temps on a fait (ou cycle)
int sendFreq = 25;					// Fréquence d'envoie des données (100 = 1 envoie par seconde, 25 = 4 par sec, ...)

int can_debug = 0;					// Si = 1, des informations sur le CAN sont affichées dans la console jusqu'à l'appui de Backspace


//***************************************************************
// Menu principal (niveau 0)
//***************************************************************
const char *mainMenu =
"1- Paramètres\n\r\
2- Status\n\r\
3- Commandes\n\n\r\
Attente d'une instruction\n\n\r";

                        
//**************************************************************  
// sous-menu (niveau 1)  
//***************************************************************      

//menu des paramètres (niveau 1)						 
const char *parametersMenu =
"1- minDischargeCellTemp\n\r\
2- maxDischargeCellTemp\n\r\
3- lowDischargeCellTemp\n\r\
4- highDischargeCellTemp\n\r\
5- minChargeCellTemp\n\r\
6- maxChargeCellTemp\n\r\
7- lowChargeCellTemp\n\r\
8- highChargeCellTemp\n\r\
9- minCellVoltage\n\r\
a- maxCellVoltage\n\r\
b- lowCellVoltage\n\r\
c- highCellVoltage\n\r\
d- maxMeanChargeCurrent\n\r\
e- maxMeanDischargeCurrent\n\r\
f- maxDischargePeakCurrent\n\r\
g- manualMode\n\r\
h- ignoreErrors\n\r\
i- ignoreIntState\n\n\r\
Attente d'une instruction\n\n\n\r";
                       
//menu d'affichage de données (niveau 1)						 
const char *statusMenu =
"1- Affichage du status\n\r\
2- Affichage des tensions et températures\n\r\
3- Mode continu: affichage des tensions et températures\n\r\
4- Affichage des erreurs\n\r\
5- Affichage des flags\n\r\
6- Afficher les sondes de températures ignorées\n\r\
7- Afficher la table de correspondance ID / index \n\r\
8- Debuggage CAN \n\r\
Attente d'une instruction\n\n\n\r";

 
//menu de commande (niveau 1)						 
const char *commandsMenu =
"1- Activer équilibration\n\r\
2- Désactiver équilibration\n\r\
3- Fermer relais\n\r\
4- Ouvrir relais\n\r\
5- Activer charge\n\r\
6- Désactiver charge\n\n\r\
Attente d'une instruction\n\n\n\r";	   				

                          
//Indications à afficher pour guider l'usager de modification des paramètres
const char *help =
"Commandes supplémentaires utiles\n\r\
Appuyer sur la touche 'm' pour afficher le menu principal\n\r\
Appuyer sur la touche 'Enter' pour saisir une donnée\n\r\
Appuyer sur la touche 'BS' pour retourner au menu précédent ou interrompre l'action en cours\n\n\n\n\r";


			
//*****************************************************************************
// sciByteReception
//
// Description:   Dispose of a received byte. 
//
//*****************************************************************************
void sciByteReception(unsigned char rcvByte)
{
   unsigned char i;

   static char inputBuf[20] = "";
   static unsigned char bufPos = 0;
   float inputParam = 0;
   static unsigned char point = 0;

   //*
   //**************************************************
   // "m" KEY	  
   //**************************************************
   if (rcvByte == 'm') {   
      niveau_1 = 0;                 // 1 = configuration, 2 = status, 3 = commandes
      niveau_2 = 0;
      SCIshowMenu(0);               // input = 0 -> show the menu for the actual levels
      gSciInterruptMode = 0;
      for(i=0; i<20; i++)
         inputBuf[i] = 0;
   }

   //**************************************************
   // BS KEY
   //
   // Lorsque l'usager appuie sur BS, la saisie continue de données est interrompue.
   // Le menu de niveau supérieur est réaffiché
   //**************************************************	
   else if(rcvByte == 0x7F || rcvByte == 0x08) {			   
		if((niveau_1 == 2) && (niveau_2 == 3))        //Désactivation du mode continu
		{ 
			PITCE_PCE3 = 0;
			printStatTime = 0; 
			timeFollow = 0;

			gSciInterruptMode = 0;
			SCIupAlevel();
			SCIshowMenu(0);
			
		}

		if((niveau_1 == 2) && (niveau_2 == 8))  // Désactivation du debuggage CAN continu
		{
			can_debug = 0;
		}
		
		for(i=0; i<20; i++)
			inputBuf[i] = 0;
		
   }	

   //**************************************************
   // MODE MENU
   //
   // Affiche le menu approprié selon les niveaux 1, 2 et 3 et la commande recu	  
   //**************************************************
   else if  (gSciInterruptMode == 0) {
      SCIshowMenu(rcvByte);    
   }

   //**************************************************
   // MODE ASSIGNATION
   //	
   // Assignation de la valeur lorsque l'usager appuie sur "enter"
   //**************************************************
   else if (rcvByte == 0xD && gSciInterruptMode == 1) {    // CR == enter key	

      if(sscanf(inputBuf, "%f", &inputParam) > 0) {
         SCIassignation(inputParam);
      } else {
         SCIprintString("Error reading the entered value.\n");
      }

      if(gSciInterruptMode != 2) {
         gSciInterruptMode = 0;
         SCIupAlevel();
         SCIshowMenu(0);
      }

      inputParam = 0;
      bufPos = 0;
      point = 0;

      for(i=0; i<20; i++)
         inputBuf[i] = 0;
   }

   //**************************************************
   // MODE USER_INPUT	
   //**************************************************
   else if (gSciInterruptMode == 1) { 

      if(rcvByte == '-'){
         if(bufPos == 0){
            inputBuf[bufPos++] = rcvByte;
            SCIPutChar(rcvByte);
         }
      } else if(rcvByte == '.'){
         if((bufPos != 0) && (bufPos < 19) && !point){
            inputBuf[bufPos++] = rcvByte;
            point = 1;
            SCIPutChar(rcvByte);
         }
      } else if((rcvByte >= '0') && (rcvByte <= '9')){
         if(bufPos < 19){
            inputBuf[bufPos++] = rcvByte; 
            SCIPutChar(rcvByte);
         }
      } 
   }
	//*/   
}
         
         
//*****************************************************************************
// SCIPutChar
//
// Description:   Print a single character. 
//
//*****************************************************************************
void SCIPutChar(char ch) {
   while (!(SCI5SR1 & 0x80)) {};
   SCI5DRL = ch;
}

//*****************************************************************************
// SCIprintString
//
// Description:   Print a string of characters. The NULL ('\0') char must be
//                present at the end. 
//
//*****************************************************************************
void SCIprintString(char* charBuf)
{
   int i=0;

   while (charBuf[i] != '\0') {
      SCIPutChar(charBuf[i]);
      i++;
   }
   SCIPutChar(charBuf[i]);
}


/*
* sciGetCellsData: Affiche les voltage et la température de chaque cellule
*
* Parameters:  int *volt[]: array 2 dimensions contenant les voltages
*			   int *temp[]: array 2 dimensions contenant les températures
*              int numer_mod : numéro (ID) du module pour lequel on veut les informations
*              int nb_cell : nombre de cellule par module
* Return : aucun.
*/
void sciGetCellsData(unsigned int volt[][N_CELL], int temp[][N_CELL], int numer_mod)
{
   char cbuffer[8]; //buffer pour la conversion en char
   int nb_char;     // nombre de char dans le buffer (<8)
   int char_pos;  // position du char dans la loop d'écriture du *char
   int junk;
   char buf[40];

   unsigned int cell = 0; // cellule pour laquelle on affiche des donnés   
  
   int index = indexOf(numer_mod); //On récupère l'index du module esclave à afficher
   
   if(index == -1)
   {
		SCIprintString("Numéro de module invalide\n");
		return;
   }
   
   for(cell=0; cell < N_CELL; cell++)
   {
      nb_char = sprintf(cbuffer,"cell %d:", cell);

      for(char_pos=0; char_pos <= nb_char; char_pos++)    // envoie dans le SCI un string du type "cell i:"
         SCIPutChar(cbuffer[char_pos]);

      SCIPutChar(0x20);                       //space
      junk = sprintf(buf, formatVolt, (float)volt[index][cell]/1000.0);
      SCIprintString(buf);
      SCIPutChar(0x20);                       //space   

      junk = sprintf(buf, formatTemp, (float)temp[index][cell]/10.0);
      SCIprintString(buf);
      SCIprintString("\n\r");
   }
     
}

//*****************************************************************************
// SCIprintStatus
//
// Description:   Print the status of the BMS. Including:
//                - Last current measurement
//                - Minimal cell voltage, maximal cell voltage
//                - Minimal cell temperature, maximal cell temperature
//                - Error flags
//
//*****************************************************************************
void SCIprintStatus(void)
{
    int junk;
    char buf[70];

    SCIprintString("-------------\n\rStatus du BMS\n\r-------------\n\r");
    
    SCIprintString("Tension totale du pack: ");
    junk = sprintf(buf, formatVolt, (float)gTotalPackVoltage/1000.0);
    SCIprintString(buf);
    
    SCIprintString("\n\rCourant moyen: ");
    junk = sprintf(buf, formatCurrMa, (float)gMeanCurrent/1000.0);
    SCIprintString(buf);

    SCIprintString("\n\rTension de cellule minimale: ");
    junk = sprintf(buf, formatVolt, (float)*gLowestCellVoltage/1000.0);
    SCIprintString(buf);
    junk = sprintf(buf, formatCellId, gLowestVoltageCellSlaveId, gLowestVoltageCellNum);
    SCIprintString(buf);
    
    SCIprintString("\n\rTension de cellule maximale: ");
    junk = sprintf(buf, formatVolt, (float)*gHighestCellVoltage/1000.0);
    SCIprintString(buf);
    junk = sprintf(buf, formatCellId, gHighestVoltageCellSlaveId, gHighestVoltageCellNum);
    SCIprintString(buf);
    
    SCIprintString("\n\rTempérature de cellule minimale: ");
    junk = sprintf(buf, formatTemp, (float)*gLowestCellTemp/10.0);
    SCIprintString(buf);
    junk = sprintf(buf, formatCellId, gLowestTempCellSlaveId, gLowestTempCellNum);
    SCIprintString(buf);

    SCIprintString("\n\rTempérature de cellule maximale: ");
    junk = sprintf(buf, formatTemp, (float)*gHighestCellTemp/10.0);
    SCIprintString(buf);
    junk = sprintf(buf, formatCellId, gHighestTempCellSlaveId, gHighestTempCellNum);
    SCIprintString(buf);

    
    SCIprintString("\n\n\n\r");
}


//*****************************************************************************
// SCIprintErrors
//
// Description:   Print the BMS error flags.
//
//*****************************************************************************
void SCIprintErrors(void)
{
   unsigned int i=0;
   char buf[20];
   int junk;
   unsigned int tmp=0;

   SCIprintString("Erreurs:\n\n\r");

   if(gError.slaveTimeout){
      SCIprintString("Expiration de la communication avec les modules: ");
      for(i=0; i<N_MOD; i++) {
         tmp = (1<<i);
         if((gSlaveComState & tmp) == tmp){
			
			if(idOf(i) == 0) // On n'a jamais reçu de paquet de cet esclave
				SCIprintString("? ");
			else
				junk = sprintf(buf,"%d ", idOf(i));

            SCIprintString(buf);
         }
      }
      SCIprintString("\n\r");
   } 

   if(gError.cellMaxVolt)
      SCIprintString("Tension de cellules maximale atteinte\n\r");

   if(gError.cellMinVolt)
      SCIprintString("Tension de cellules minimale atteinte\n\r");

   if(gError.cellMaxTemp)
      SCIprintString("Température de cellule maximale atteinte\n\r");

   if(gError.cellMinTemp)
      SCIprintString("Température de cellule minimale atteinte\n\r");

   if(gError.maxPeakCurrent)
      SCIprintString("Courant moyen sur 10 s maximal atteint\n\r");

    SCIprintString("\n\r");
}


//*****************************************************************************
// SCIprintFlags
//
// Description:   Print the BMS flags.
//
//*****************************************************************************
void SCIprintFlags(void)
{
   char buf[30];
   int junk;

   SCIprintString("Flags:\n\n\r");

   junk = sprintf(buf,"equilibrating = %d\n\r", gFlags.equilibrating);
   SCIprintString(buf);
   
   junk = sprintf(buf,"charging = %d\n\r", gFlags.charging);
   SCIprintString(buf);
   
   junk = sprintf(buf,"ignition = %d\n\r", gFlags.ignition);
   SCIprintString(buf);
   
   junk = sprintf(buf,"interlockClosed = %d\n\r", gFlags.interlockClosed);
   SCIprintString(buf);
   
   junk = sprintf(buf,"errorReset = %d\n\r", gFlags.errorReset);
   SCIprintString(buf);
   
   junk = sprintf(buf,"imdError = %d\n\r", gFlags.ImdError);
   SCIprintString(buf);
   
   junk = sprintf(buf,"errorState = %d\n\r", gFlags.errorState);
   SCIprintString(buf);
   
   junk = sprintf(buf,"relaysClosed = %d\n\r", gFlags.relaysClosed);
   SCIprintString(buf);
   
   junk = sprintf(buf,"totalPackTime = %d\n\r", gFlags.totalPackTime);
   SCIprintString(buf);
   
   junk = sprintf(buf,"cellLowVolt = %d\n\r", gFlags.cellLowVolt);
   SCIprintString(buf);
   
   junk = sprintf(buf,"cellHighVolt = %d\n\r", gFlags.cellHighVolt);
   SCIprintString(buf);
   
   junk = sprintf(buf,"cellLowTemp = %d\n\r", gFlags.cellLowTemp);
   SCIprintString(buf);
   
   junk = sprintf(buf,"cellHighTemp = %d\n\r", gFlags.cellHighTemp);
   SCIprintString(buf);
   
   junk = sprintf(buf,"highPeakCurrent = %d\n\n\n\r", gFlags.highPeakCurrent);
   SCIprintString(buf);

}

//*****************************************************************************
// SCIupAlevel
//
// Description:   Brings the user up a level in the menu hierarchy. Also
//                stops the current value input action and the continuous
//                display mode.
//
//*****************************************************************************
void SCIupAlevel(void)
{
   if(niveau_2 != 0)
      niveau_2 = 0;  	      
    else  
      niveau_1 = 0; 
}


//*****************************************************************************
// SCIshowMenu
//
// Description:   Print the appropriate menu depending on the actual levels
//                (niveau_1, niveau_2 and niveau_3). For the input character
//                0, print the actual menu.
//
//*****************************************************************************
void SCIshowMenu(unsigned char input)
{
   char buf[40];
   int junk;
   
   //***************************************
   //menu principal (3 choix)		
   //***************************************
   if (niveau_1 == 0 && niveau_2 == 0)  															 
   {
      //chaque case permet d'afficher un sous-menu différent à l'écran
      switch (input) {
         case 0:    //Root menu
            SCIprintString(mainMenu);
            SCIprintString(help);
            break;
         case '1':  //affichage menu paramètres
            SCIprintString(parametersMenu);
            niveau_1 = 1; 
            break;
         case '2':  //Affichage menu status
            SCIprintString(statusMenu);
            niveau_1 = 2;
            break;
         case '3': //Affichage menu commande
            SCIprintString(commandsMenu);
            niveau_1 = 3;
            break;
      } 
   }

   //menu de paramètres
   else if (niveau_1 == 1 && niveau_2 == 0)  
   {	
      if(input == 0) {    //Root menu
         SCIprintString(parametersMenu);

      } else if((input >= '1' && input <= '9') || (input >= 'a' && input <= 'i')) {   //Bon choix

         switch (input) {

            case '1':  //minDischargeCellTemp
            junk = sprintf(buf, formatTemp, (float)gParams.minDischargeCellTemp/10.0);
            niveau_2 = 1;
            break;

            case '2':  //maxDischargeCellTemp
            junk = sprintf(buf,formatTemp, (float)gParams.maxDischargeCellTemp/10.0);
            niveau_2 = 2;                
            break;

            case '3': //lowDischargeCellTemp
            junk = sprintf(buf,formatTemp, (float)gParams.lowDischargeCellTemp/10.0);
            niveau_2 = 3;
            break;

            case '4': //highDischargeCellTemp
            junk = sprintf(buf,formatTemp, (float)gParams.highDischargeCellTemp/10.0);
            niveau_2 = 4;
            break;

            case '5': //minChargeCellTemp 
            junk = sprintf(buf,formatTemp, (float)gParams.minChargeCellTemp/10.0);
            niveau_2 = 5;                
            break;

            case '6': //maxChargeCellTemp
            junk = sprintf(buf,formatTemp, (float)gParams.maxChargeCellTemp/10.0);
            niveau_2 = 6;				
            break;

            case '7': //lowChargeCellTemp 
            junk = sprintf(buf,formatTemp, (float)gParams.lowChargeCellTemp/10.0);
            niveau_2 = 7;
            break;

            case '8': //highChargeCellTemp 
            niveau_2 = 8;
            junk = sprintf(buf,formatTemp, (float)gParams.highChargeCellTemp/10.0);			
            break;

            case '9': //minCellVoltage
            niveau_2 = 9;
            junk = sprintf(buf,formatVolt, (float)gParams.minCellVoltage/1000.0);			
            break;

            case 'a': //maxCellVoltage 
            niveau_2 = 10;
            junk = sprintf(buf,formatVolt, (float)gParams.maxCellVoltage/1000.0);			
            break;

            case 'b': //lowCellVoltage 
            niveau_2 = 11;
            junk = sprintf(buf,formatVolt, (float)gParams.lowCellVoltage/1000.0);			
            break;

            case 'c': //highCellVoltage                 
            niveau_2 = 12;
            junk = sprintf(buf,formatVolt, (float)gParams.highCellVoltage/1000.0);
            break;

            case 'd': //maxMeanChargeCurrent
            niveau_2 = 13;
            junk = sprintf(buf,formatCurr, gParams.maxMeanChargeCurrent);				
            break;

            case 'e': //maxMeanDischargeCurrent	
            niveau_2 = 14;
            junk = sprintf(buf,formatCurr, gParams.maxMeanDischargeCurrent);			
            break;

            case 'f': //maxPeakDischargeCurrent
            niveau_2 = 15;;
            junk = sprintf(buf,formatCurr, gParams.maxPeakDischargeCurrent);		
            break;

            case 'g': //manualMode
            niveau_2 = 16;
            junk = sprintf(buf,"%u", gParams.manualMode);		
            break;

            case 'h': //ignoreErrors
            niveau_2 = 17;
            junk = sprintf(buf,"%u", gParams.ignoreErrors);		
            break;

            case 'i': //ignoreIntState
            niveau_2 = 18;
            junk = sprintf(buf,"%u", gParams.ignoreIntState);		
            break;
         }

         SCIprintString("Valeur actuelle du paramètre: ");
         SCIprintString(buf);
         SCIprintString("\n\rEntrez une nouvelle valeur: ");
         gSciInterruptMode = 1;             //input mode (gSciInterruptMode = 1)	

      } else {
         SCIprintString("Choix erroné, essayez de nouveau.\n\n\n\r");
         SCIprintString(parametersMenu);

      }   //end else if root/bon/mauvais choix

   }  //end else if(menu de paramètres)	


   //***************************************
   //menu de status (3 choix)
   //***************************************
   else if (niveau_1 == 2 && niveau_2 == 0)  
   {													  
      //chaque case permet d'afficher un sous-menu du menu de configuration
      switch (input) {

         case 0:    //Root menu
            SCIprintString(statusMenu);
            break;

         case '1':  //affichage du STATUS du BMS 			
            SCIprintStatus(); 
            SCIprintString(statusMenu);
            break;

         case '2':  //Affichage des tensions et températures pour 1 module								
            niveau_2 = 2;
            SCIprintString("Entrer le # du module esclave suivi de la touche enter: ");
            gSciInterruptMode = 1;  //input mode (gSciInterruptMode =1)				
            break;

         case '3': //Affichage des données en mode continu pour tous les modules							
            niveau_2 = 3;				
            gSciInterruptMode = 2;
			printStatTime = 
			PITCE_PCE3 = 1;
            // NOTE: Dans le mode assignation, une interruption périodique est lancée
            // juste après la saise du module à afficher en loop.
            break;
         
         case '4':   //affichage des erreurs
            SCIprintErrors();
            SCIprintString(statusMenu);
			break;
         
         case '5':   //affichage des flags
            SCIprintFlags();
            SCIprintString(statusMenu);
			break;
			
		case '6':	//affichage des sondes de températures ignorées
			SCIprintIgnoreTemp(gCellIgnoreTemp);
			SCIprintString(statusMenu);
			break;
		
		case '7': 	//affichage de la table de correspondance ID / Index
			SCIprintIDtable(idTable);
			SCIprintString(statusMenu);
			break;
		
		case '8':   // affichage continu d'informations sur les paquets CAN reçus
			niveau_2 = 8;
			can_debug = 1;
			break;
			
		 default:
			SCIprintString("Choix erroné.\n\r");
			//SCIprintString(statusMenu);
			break;

      }
   }				
   //***************************************
   //menu de commandes (5 choix)
   //***************************************
   else if (niveau_1 == 3 && niveau_2 == 0)  
   {													  
      //chaque case permet d'afficher un sous-menu du menu de configuration
      switch (input)
      {
         case 0:    //Root menu
            SCIprintString(commandsMenu);
            break;

         case '1':  //Activer équilibration
            if(!gFlags.equilibrating){
               if(gParams.manualMode){
                  niveau_2 = 1;
                  SCIprintString("Entrez la tension à atteindre (en V): ");
                  gSciInterruptMode = 1;  //input mode (gSciInterruptMode =1)
               } else {
                  SCIprintString("Vous ne pouvez débuter l'équilibration (mode automatique).\n\n\n\r");
                  SCIprintString(commandsMenu);
               }	
            } else {
               SCIprintString("L'équilibration est déjà activée.\n\n\n\r");
               SCIprintString(commandsMenu);
            }
            break;

         case '2':  //Désactiver l'équilibration
            if(gFlags.equilibrating) {
               if(gParams.manualMode) {
                  CAN0SendEquiCommand(0, 4500, CAN_BROADCAST_ID); //4500: valeur supérieure à la tension max de cell, donc équilibration cesse
                  SCIprintString("\n\rL'équilibration est maintenant est désactivée\n\n\n\r");
                  gFlags.equilibrating = 0;
               } else {
                  SCIprintString("Vous ne pouvez terminer l'équilibration (mode automatique).\n\n\n\r");
               }
            } else {
               SCIprintString("\n\rL'équilibration est déjà désactivée\n\n\n\r");                  
            }
            SCIprintString(commandsMenu);
            break;

         case '3':  //Fermer Relais
            if(!gFlags.relaysClosed){
               if(gParams.manualMode) {
                  if(gFlags.interlockClosed || gParams.ignoreIntState) {
                     if(!gFlags.errorState || gParams.ignoreErrors) {
                        CloseRelays(START_SEQUENCE);
                     } else {
                        SCIprintString("Vous ne pouvez fermer les relais (erreurs).\n\n\n\r");
                     }
                  } else {
                     SCIprintString("Vous ne pouvez fermer les relais (interlock ouvert).\n\n\n\r");
                  }
               } else {
                  SCIprintString("Vous ne pouvez fermer les relais (mode automatique).\n\n\n\r");
               }
            } else {
               SCIprintString("Les relais sont déjà fermés.\n\n\n\r");
            }
            SCIprintString(commandsMenu);
            break;

         case '4':  //Ouvrir Relais				
            if(gParams.manualMode) {
               OpenRelays();
            } else {
               SCIprintString("Vous ne pouvez ouvrir les relais (mode automatique).\n\n\n\r");
            }
            SCIprintString(commandsMenu);
            break;

         case '5':  //Activer charge				
            if(!gFlags.charging){
               if(gParams.manualMode){
                  SCIprintString("Mode charge activé (pour changer les limites).\n\n\n\r");
                  gFlags.charging = 1;
               } else {
                  SCIprintString("Vous ne pouvez pas entrer en mode charge (mode automatique).\n\n\n\r");
               }
            } else {
               SCIprintString("Le mode charge est déjà activé.\n\n\n\r");
            }
            SCIprintString(commandsMenu);
            break;

         case '6':  //Désactiver charge
            if(gFlags.charging) {
               if(gParams.manualMode) {
                  gFlags.charging = 0;
                  SCIprintString("Mode charge désactivé.\n\n\n\r");
               } else {
                  SCIprintString("Vous ne pouvez pas sortir du mode charge (mode automatique).\n\n\n\r");
               }
            } else {
               SCIprintString("Le mode charge est déjà désactivé.\n\n\n\r");
            }
            SCIprintString(commandsMenu);
            break;  								 	
      }
   }		
}


//*****************************************************************************
// SCIassignation
//
// Description:   Assigns the value to the appropriate parameter, depending on
//                the actual levels (niveau_1, niveau_2 and niveau_3).
//
//*****************************************************************************
void SCIassignation(float user_input)
{
   int iNewParam = 0;
   unsigned int uiNewParam = 0;
   unsigned long int uliNewParam = 0;
   unsigned char ucNewParam = 0;
   char buf[70];
   int junk;
   unsigned int i=0;

	SCIprintString("\n\r");
    
	//Menu configuration -> Assignation des paramètres de sécurité
	if(niveau_1 == 1)
	{
        SCIprintString("Nouvelle valeur du paramètre: ");
        
		switch (niveau_2)
		{
			case 1: 
			      iNewParam = (int)(user_input*10);
			      gParams.minDischargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);						
					break;		
			case 2: 
			      iNewParam = (int)(user_input*10);
			      gParams.maxDischargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 3:
			      iNewParam = (int)(user_input*10);
			      gParams.lowDischargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);			
					break;
			case 4:
			      iNewParam = (int)(user_input*10);
			      gParams.highDischargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 5:
			      iNewParam = (int)(user_input*10);
			      gParams.minChargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 6:
			      iNewParam = (int)(user_input*10);
			      gParams.maxChargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 7:
			      iNewParam = (int)(user_input*10);
			      gParams.lowChargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 8:
			      iNewParam = (int)(user_input*10);
			      gParams.highChargeCellTemp = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);  
					break;
			case 9:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gParams.minCellVoltage = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 10:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gParams.maxCellVoltage = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 11:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gParams.lowCellVoltage = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 12:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gParams.highCellVoltage = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 13:
			      uliNewParam = (unsigned long int)(user_input*1000);
			      gParams.maxMeanChargeCurrent = uliNewParam;
			      junk = sprintf(buf,formatCurr, uliNewParam); 
					break;
			case 14:
			      uliNewParam = (unsigned long int)(user_input*1000);
			      gParams.maxMeanDischargeCurrent = uliNewParam;
			      junk = sprintf(buf,formatCurr, uliNewParam); 
                  break;
			case 15:
			      uliNewParam = (unsigned long int)(user_input*1000);
			      gParams.maxPeakDischargeCurrent = uliNewParam;
			      junk = sprintf(buf,formatCurr, uliNewParam);  
				  break;
         case 16:
			      ucNewParam = (unsigned char)user_input;
			      gParams.manualMode = ucNewParam;
			      junk = sprintf(buf, "%u", ucNewParam);  
				  break; 
         case 17:
			      ucNewParam = (unsigned char)user_input;
			      gParams.ignoreErrors = ucNewParam;
			      junk = sprintf(buf, "%u", ucNewParam);  
				   break;
         case 18:
			      ucNewParam = (unsigned char)user_input;
			      gParams.ignoreIntState = ucNewParam;
			      junk = sprintf(buf, "%u", ucNewParam);  
				   break;                
		}
		
		SCIprintString(buf);
        SCIprintString("\n\n\r");
    } 
	
// Menu status -> sélection du module à afficher dans le terminal
	else if(niveau_1 == 2)
	{
	    gSciDisplayNumModule = (unsigned int) user_input; //assignation du nombre de module
	
		switch (niveau_2) {
        
			case 2: 
            junk = sprintf(buf,"\n\n\rAffichage des données du module %u\n\r", gSciDisplayNumModule);
            SCIprintString(buf);		
            sciGetCellsData(gCellVolt, gCellTemp, gSciDisplayNumModule);
            SCIprintString("\n\n\n\r");            
			break;
			/******************
			/ Modifié pour un affichage différent : pas besoin d'avoir un numéro de module
			/**************
			case 3:
            PITCE_PCE3 = 1; //activation de l'interrupt, le mode continu pourra
            gSciInterruptMode = 2;
            SCIprintString("\n\n\r");
            break;          //être interrompu en appuyant sur la touche backspace. 				
			*/
		}
	}
	//Menu commandes
	else if(niveau_1 == 3){
  		switch (niveau_2)
		{
			case 1:
			 
		        uiNewParam = (unsigned int)(user_input*1000);      //Balance threshold
                sprintf(buf, "Valeur entrée: %u mV\n\n\r", uiNewParam);
                SCIprintString(buf);
            
		      if((uiNewParam < gParams.maxCellVoltage) && (uiNewParam > gParams.minCellVoltage)) {
		      
		        //Envoi de la commande d'équilibration
                CAN0SendEquiCommand(0x03FF, uiNewParam, CAN_BROADCAST_ID);
		         
		        //On met les flags d'équilibration des modules à 1
		        for(i=0; i<N_MOD; i++)
		           gSlaveEquiStatus = gSlaveEquiStatus | (1<<i);
		         
		        //on active le flag interne d'équilibration     
		        gFlags.equilibrating = 1; 

                SCIprintString("L'équilibration est maintenant activée.\n\n\n\r");                
		         
		      } else {
		         SCIprintString("La valeur entrée n'est pas admissible.\n\n\n\r");   
		      }
			  break;
		} 
	   
	}
			
}

// ****************************************
//	Imprime dans la console les températures et voltage de chaque cellules
// 	pendant un certain temps
//	
//	Cette fonction est appelée par une interruption de 100 Hz, ce qui permet de connaître la fréquence d'appel de la méthode
// ****************************************

void SCIprintStatData(unsigned int volt[][N_CELL], int temp[][N_CELL])
{
	char buf[70];
	int i,j;

	if(printStatTime == 0)
		return;
		
	if(timeFollow < (printStatTime*100))
	{
		if((timeFollow % sendFreq) == 0)
		{
			// On donne un titre au colonnes si c'est le premier appel
			if(timeFollow == 0)
			{
				sprintf(buf, "timestamp(ms),no_module,no_cellule,tension(mV),temperature(0.1degC)\n\r");
				SCIprintString(buf);
			}
			
			//On envoie les données sous le format : timestamp, no_module, no_cellule, tension, température
			for(i = 0; i < N_MOD; i++)
			{
				for(j = 0; j < N_CELL; j++)
				{
					sprintf(buf, "%d,%d,%d,%d,%d\n\r", timeFollow*10, idOf(i), j+1, volt[i][j], temp[i][j]);
					SCIprintString(buf);
				}
			}
		}
		
		timeFollow++;
	}
	else
	{
		printStatTime = 0; // Envoie des données terminés, on arrête d'envoyer
		timeFollow = 0;
		PITCE_PCE3 = 0; //On désactive l'interruption
	}

}

// Permet d'envoyer la valeur d'une variable sans avoir à créer de buffer dans d'autres fonctions
void SCIprintInt(char *nom, int donnee)
{
	char buf[100];

	sprintf(buf, "%s%d", nom, donnee);
	SCIprintString(buf);
}


void SCIprintIgnoreTemp(int ignoreTemp[][2])
{
	char buf[70];
	int i;
	
	SCIprintString("------- Sondes de températures ignorées --------\n\r");
	SCIprintString("Module esclave\tCellule\n\r");
	for(i = 0; i < N_MAX_IGNORE_TEMP; i++)
	{
		if(ignoreTemp[i][0] != -1)
		{
			sprintf(buf, "\t%d\t\t%d\n\r", ignoreTemp[i][0], ignoreTemp[i][1]);
			SCIprintString(buf);
		}
	}
	SCIprintString("\n\r");
	
}


void SCIprintIDtable(uint8 table[])
{
	char buf[70];
	int i;
	
	SCIprintString("------- Table de correspondance ID / Index --------\n\r");
	SCIprintString("\tIndex\tID (# du module esclave)\n\r");
	for(i = 0; i < N_MOD; i++)
	{
		if(table[i] != 0)
		{
			sprintf(buf, "\t  %d\t\t%d\n\r", i, table[i]);
			SCIprintString(buf);
		}
		else
		{
			sprintf(buf, "\t  %d\t\t%s\n\r", i, "-");
			SCIprintString(buf);
		}
	}
	SCIprintString("\n\r");
}