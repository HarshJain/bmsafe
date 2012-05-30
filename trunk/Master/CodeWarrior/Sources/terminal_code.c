
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

// d�finitions g�n�rales
const char* formatTemp = "%2.1f oC";       //Temperature en dixi�me de oC
const char* formatVolt = "%1.3f V";        //1 mV precision
const char* formatCurr = "%d A";           //Les courants en amp�res (param�tres)
const char* formatCurrMa = "%1.3f A";      //Les courants en amp�res (pr�cision 10 mA)
const char* formatTime = "%d ms";          //Temps en ms

// syst�me d'index pour les menus
unsigned int niveau_1 = 0;                  // 1 = configuration, 2 = status, 3 = commandes
unsigned int niveau_2 = 0;

// strings du menu d'affichage des param�tres                                            
unsigned int num_module;                  // num�ro du slave � afficher
unsigned int sci_interrupt_mode = 0;      //0 = mode menu, 1 = mode assignation


//***************************************************************
// Menu principal (niveau 0)
//***************************************************************
char *mainMenu =
"1- Param�tres\n\r\
2- Status\n\r\
3- Commandes\n\n\r\
Attente d'une instruction\n\n\r";

                        
//**************************************************************  
// sous-menu (niveau 1)  
//***************************************************************      

//menu des param�tres (niveau 1)						 
char *parametersMenu =
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
g- manualMode\n\n\r\
Attente d'une instruction\n\n\n\r";
                       
//menu d'affichage de donn�es (niveau 1)						 
char *statusMenu =
"1- Affichage du STATUS\n\r\
2- Affichage des tensions et temp�ratures\n\r\
3- Mode continu: affichage des tensions et temp�ratures\n\n\r\
Attente d'une instruction\n\n\n\r";

 
//menu de commande (niveau 1)						 
 char *commandsMenu =
"1- Activer �quilibration\n\r\
2- D�sactiver �quilibration\n\r\
3- Fermer relais\n\r\
4- Ouvrir relais\n\r\
5- Activer charge\n\r\
6- D�sactiver charge\n\n\r\
Attente d'une instruction\n\n\n\r";	   				

                          
//Indications � afficher pour guider l'usager de modification des param�tres
char *help =
"Commandes suppl�mentaires utiles\n\r\
Appuyer sur la touche 'm' pour afficher le menu principal\n\r\
Appuyer sur la touche 'Enter' pour saisir une donn�e\n\r\
Appuyer sur la touche 'BS' pour retourner au menu pr�c�dent ou interrompre l'action en cours\n\n\n\n\r";
			

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
* get_cells_data: Affiche les voltage et la temp�rature de chaque cellule
*
* Parameters:  int *volt[]: array 2 dimensions contenant les voltages
*			   int *temp[]: array 2 dimensions contenant les temp�ratures
*              int numer_mod : num�ro du module pour lequel on veut les informations
*              int nb_cell : nombre de cellule par module
* Return : aucun.
*/
void get_cells_data(unsigned int volt[][N_CELL], int temp[][N_CELL], int numer_mod)
{
   char cbuffer[8]; //buffer pour la conversion en char
   int nb_char;     // nombre de char dans le buffer (<8)
   int char_pos;  // position du char dans la loop d'�criture du *char
   int junk;
   char buf[40];

   unsigned int cell = 0; // cellule pour laquelle on affiche des donn�s
   
   for(cell=0; cell < N_CELL; cell++)
   {
      nb_char = sprintf(cbuffer,"cell %d:", cell);

      for(char_pos=0; char_pos <= nb_char; char_pos++)    // envoie dans le SCI un string du type "cell i:"
         SCIPutChar(cbuffer[char_pos]);

      SCIPutChar(0x20);                       //space
      junk = sprintf(buf, formatVolt, (float)volt[numer_mod][cell]/1000.0);
      SCIprintString(buf);
      SCIPutChar(0x20);                       //space   

      junk = sprintf(buf, formatTemp, (float)temp[numer_mod][cell]/10.0);
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
    
    SCIprintString("Courant moyen: ");
    junk = sprintf(buf, formatCurrMa, (float)gMeanCurrent/1000.0);
    SCIprintString(buf);

    SCIprintString("\n\rTension de cellule minimale: ");
    junk = sprintf(buf, formatVolt, (float)*gLowestCellVoltage/1000.0);
    SCIprintString(buf);

    SCIprintString("\n\rTension de cellule maximale: ");
    junk = sprintf(buf, formatVolt, (float)*gHighestCellVoltage/1000.0);
    SCIprintString(buf);

    SCIprintString("\n\rTemp�rature de cellule minimale: ");
    junk = sprintf(buf, formatTemp, (float)*gLowestCellTemp/10.0);
    SCIprintString(buf);

    SCIprintString("\n\rTemp�rature de cellule maximale: ");
    junk = sprintf(buf, formatTemp, (float)*gHighestCellTemp/10.0);
    SCIprintString(buf);
    SCIprintString("\n\n\r");
    
    SCIprintErrors();
    
    SCIprintString("\n\r");
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

   SCIprintString("Erreurs:\n\r");

   if(gError.slaveTimeout){
      SCIprintString("Expiration de la communication avec les modules: ");
      for(i=0; i<N_MOD; i++) {
         tmp = 1<<i;
         if((gSlaveComState & tmp) == tmp){
            junk = sprintf(buf,"%d ", i+1);
            SCIprintString(buf);
         }  
      }
      SCIprintString("\n\r");
   } 

   if(gError.cellOpenConnection)
      SCIprintString("Une cellule est d�connect�e\n\r");

   if(gError.cellMaxVolt)
      SCIprintString("Tension de cellules maximale atteinte\n\r");

   if(gError.cellMinVolt)
      SCIprintString("Tension de cellules minimale atteinte\n\r");

   if(gError.cellMaxTemp)
      SCIprintString("Temp�rature de cellule maximale atteinte\n\r");

   if(gError.cellMinTemp)
      SCIprintString("Temp�rature de cellule minimale atteinte\n\r");

   if(gError.maxPeakCurrent)
      SCIprintString("Courant moyen sur 10 s maximal atteint\n\r");


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
			//chaque case permet d'afficher un sous-menu diff�rent � l'�cran
			switch (input) {
                case 0:    //Root menu
                   SCIprintString(mainMenu);
                   SCIprintString(help);
                   break;
                case '1':  //affichage menu param�tres
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
        
//menu de param�tres
		else if (niveau_1 == 1 && niveau_2 == 0)  
		{	
            if(input == 0) {    //Root menu
                SCIprintString(parametersMenu);
                
            } else if((input >= '1' && input <= '9') || (input >= 'a' && input <= 'g')) {   //Bon choix
            
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
                }
                
                SCIprintString("Valeur actuelle du param�tre: ");
                SCIprintString(buf);
                SCIprintString("\n\rEntrez une nouvelle valeur: ");
                sci_interrupt_mode = 1;             //input mode (sci_interrupt_mode = 1)	
                
			} else {
                SCIprintString("Choix erron�, essayez de nouveau.\n\n\n\r");
                SCIprintString(parametersMenu);
                
            }   //end else if root/bon/mauvais choix
            
		}  //end else if(menu de param�tres)	

		
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

               case '2':  //Affichage des tensions et temp�ratures pour 1 module								
               niveau_2 = 2;
               SCIprintString("Entrer le # du module esclave suivi de la touche enter: ");
               sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
               break;

               case '3': //Affichage des donn�es en mode continu pour 1 module							
               niveau_2 = 3;				
               SCIprintString("Entrer le # du module esclave suivi de la touche enter: ");
               sci_interrupt_mode = 1;	//input mode (sci_interrupt_mode =1)
               // NOTE: Dans le mode assignation, une interruption p�riodique est lanc�e
               // juste apr�s la saise du module � afficher en loop.
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
               
			  case '1':  //Activer �quilibration
			   if(!gFlags.equilibrating){
                  if(gParams.manualMode){
                     niveau_2 = 1;
                     SCIprintString("Entrez la tension � atteindre (en V): ");
                     sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)
                  } else {
                     SCIprintString("Vous ne pouvez d�buter l'�quilibration (mode automatique).\n\n\n\r");
                     SCIprintString(commandsMenu);
                  }	
               } else {
                  SCIprintString("L'�quilibration est d�j� activ�e.\n\n\n\r");
                  SCIprintString(commandsMenu);
               }
               break;
				  
			  case '2':  //D�sactiver l'�quilibration
               if(gFlags.equilibrating) {
                  if(gParams.manualMode) {
                      CAN0SendEquiCommand(0, 4500, CAN_BROADCAST_ID);
                      SCIprintString("\n\rL'�quilibration est maintenant est d�sactiv�e\n\n\n\r");
                      gFlags.equilibrating = 0;
                  } else {
                      SCIprintString("Vous ne pouvez terminer l'�quilibration (mode automatique).\n\n\n\r");
                  }
               } else {
                   SCIprintString("\n\rL'�quilibration est d�j� d�sactiv�e\n\n\n\r");                  
               }
               SCIprintString(commandsMenu);
               break;
						
			  case '3':  //Fermer Relais
               if(!gFlags.relaysClosed){
                  if(gParams.manualMode) {
                      if(INTERLOCK_STATE == INTERLOCK_CLOSED) {
                          CloseRelays();
                          gFlags.relaysClosed = 1;
                          SCIprintString("Les relais sont maintenant ferm�s.\n\n\n\r");
                      } else {
                          SCIprintString("Vous ne pouvez fermer les relais (interlock ouvert).\n\n\n\r");
                      }
                  } else {
                      SCIprintString("Vous ne pouvez fermer les relais (mode automatique).\n\n\n\r");
                  }
               } else {
                 SCIprintString("Les relais sont d�j� ferm�s.\n\n\n\r");
               }
               SCIprintString(commandsMenu);
               break;
				
			  case '4':  //Ouvrir Relais				
               if(gFlags.relaysClosed) {
                  if(gParams.manualMode) {
                      OpenRelays();
                      gFlags.relaysClosed = 0;
                      SCIprintString("Relais ouverts.\n\n\n\r");
                  } else {
                    SCIprintString("Vous ne pouvez ouvrir les relais (mode automatique).\n\n\n\r");
                  }
               } else {
                  SCIprintString("Les relais sont d�j� ouverts.\n\n\n\r");
               }
               SCIprintString(commandsMenu);
               break;
           
              case '5':  //Activer charge				
               if(!gFlags.charging){
                  if(gParams.manualMode){
                     SCIprintString("Mode charge activ� (pour la d�tection d'erreurs).\n\n\n\r");
                     gFlags.charging = 1;
                  } else {
                     SCIprintString("Vous ne pouvez pas entrer en mode charge (mode automatique).\n\n\n\r");
                  }
               } else {
                    SCIprintString("Le mode charge est d�j� activ�.\n\n\n\r");
               }
               SCIprintString(commandsMenu);
               break;
           
              case '6':  //D�sactiver charge
               if(gFlags.charging) {
                   if(gParams.manualMode) {
                      gFlags.charging = 0;
                      SCIprintString("Mode charge d�sactiv�.\n\n\n\r");
                   } else {
                       SCIprintString("Vous ne pouvez pas sortir du mode charge (mode automatique).\n\n\n\r");
                   }
                } else {
                    SCIprintString("Le mode charge est d�j� d�sactiv�.\n\n\n\r");
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
   unsigned char ucNewParam = 0;
   char buf[70];
   int junk;
   unsigned int i=0;

	SCIprintString("\n\r");
    
	//Menu configuration -> Assignation des param�tres de s�curit�
	if(niveau_1 == 1)
	{
        SCIprintString("Nouvelle valeur du param�tre: ");
        
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
			      uiNewParam = (unsigned int)user_input;
			      gParams.maxMeanChargeCurrent = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam); 
					break;
			case 14:
			      uiNewParam = (unsigned int)user_input;
			      gParams.maxMeanDischargeCurrent = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam); 
                  break;
			case 15:
			      uiNewParam = (unsigned int)user_input;
			      gParams.maxPeakDischargeCurrent = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam);  
				  break;
            case 16:
			      ucNewParam = (unsigned char)user_input;
			      gParams.manualMode = ucNewParam;
			      junk = sprintf(buf, "%u", ucNewParam);  
				  break; 
		}
		
		SCIprintString(buf);
        SCIprintString("\n\n\r");
    } 
	
// Menu status -> s�lection du module � afficher dans le terminal
	else if(niveau_1 == 2)
	{
	    num_module = (unsigned int) user_input; //assignation du nombre de module
	
		switch (niveau_2) {
        
			case 2: 
            junk = sprintf(buf,"\n\n\rAffichage des donn�es du module %u\n\r", num_module);
            SCIprintString(buf);		
            get_cells_data(gCellVolt, gCellTemp, num_module-1);
            SCIprintString("\n\n\n\r");            
			break;
			
			case 3:
            PITCE_PCE3 = 1; //activation de l'interrupt, le mode continu pourra
            sci_interrupt_mode = 2;
            SCIprintString("\n\n\r");
            break;          //�tre interrompu en appuyant sur la touche backspace. 				
		}
	}
	//Menu commandes
	else if(niveau_1 == 3){
  		switch (niveau_2)
		{
			case 1:
			 
		        uiNewParam = (unsigned int)(user_input*1000);      //Balance threshold
                sprintf(buf, "Valeur entr�e: %u mV\n\n\r", uiNewParam);
                SCIprintString(buf);
            
		      if((uiNewParam < gParams.maxCellVoltage) && (uiNewParam > gParams.minCellVoltage)) {
		      
		        //Envoi de la commande d'�quilibration
                CAN0SendEquiCommand(0x03FF, uiNewParam, CAN_BROADCAST_ID);
		         
		        //On met les flags d'�quilibration des modules � 1
		        for(i=0; i<N_MOD; i++)
		           gSlaveEquiStatus = gSlaveEquiStatus | (1<<i);
		         
		        //on active le flag interne d'�quilibration     
		        gFlags.equilibrating = 1; 

                SCIprintString("L'�quilibration est maintenant activ�e.\n\n\n\r");                
		         
		      } else {
		         SCIprintString("La valeur entr�e n'est pas admissible.\n\n\n\r");   
		      }
			  break;
		} 
	   
	}
			
}
