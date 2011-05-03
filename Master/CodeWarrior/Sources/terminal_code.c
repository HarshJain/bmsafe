
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include <stdio.h> // pour utiliser printf et sprintf
#include "terminal_code.h"
#include "parameters.h"
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
const char* formatSOC = "%3.1f %%";        //Le SOC


//Declarations
extern unsigned int lastSOC;  
extern unsigned int glowestV;
extern unsigned int glowestT;
extern unsigned int gmaxV;
extern unsigned int gmaxT;
extern long int gLastCurrent;

extern unsigned int gCellVolt[N_MOD_MAX][N_CELL_SLV_MAX];
extern int gCellTemp[N_MOD_MAX][N_CELL_SLV_MAX];

extern int gErrorBits;
extern unsigned int gSlaveEquiStatus;
extern unsigned int gSlaveComState;
extern char gRelaysClosed;
extern char gActivEqui;
extern char gActivCharge;
                      
//**********************************************************                        

                       
// système d'index pour les menus

unsigned int niveau_1 = 0;  // 1 = configuration, 2 = status, 3 = commandes
unsigned int niveau_2 = 0;
unsigned int niveau_3 = 0;


// strings du menu d'affichage des paramètres                                            
unsigned int num_module;                  // numéro du slave à afficher
unsigned int sci_interrupt_mode = 0;      //0 = mode menu, 1 = mode assignation


//***************************************************************

// menu principal (niveau 0)
char *main_menu[4] = {
                        "1- Configuration",
                        "2- Status",
                        "3- Commandes",
                        "Wating for user input" 
                        };
//**************************************************************  
// sous-menu (niveau 1)  
                        

// menu de configuration (niveau 1)						 
char *configuration_menu[5] = {
                        "1- Paramètres de sécurité",
                        "2- Paramètres d'équilibration",
                        "3- Paramètres de mesure",
                        "4- Paramètres de communication",
                        "Waiting for user input"
                        };                     
// menu d'affichage de données (niveau 1)						 
char *status_menu[4] = {
                        "1- Affichage du STATUS",
                        "2- Affichage des tensions et températures",
                        "3- Mode continu: affichage des tensions et températures",
						"Waiting for user input"
                        }; 
                         
// menu de commande (niveau 1)						 
char *commande_menu[7] = {
                          "1- Activer équilibration",
                          "2- Désactiver équilibration",
                          "3- Fermer relais",
                          "4- Ouvrir relais",
                          "5- Activer charge",
                          "6- Désactiver charge", 
                          "Waiting for user input"						
                          }; 

//***************************************************************
// menu de configuration niveau 2 (choix des paramètres à modifier)	

// sécurité		
char *configuration_menu_1[16] = {
                                    "1- TD_MIN_LIM",
                                    "2- TD_MAX_LIM",
                                    "3- TD_MIN",
                                    "4- TD_MAX",
                                    "5- TC_MIN_LIM",
                                    "6- TC_MAX_LIM",
                                    "7- TC_MIN",
                                    "8- TC_MAX",
                                    "9- V_MIN_LIM",
                                    "q- V_MAX_LIM",
                                    "w- V_MIN",
								            "e- V_MAX",
                                    "r- IC_MAX",
                                    "t- ID_MAX",
                                    "y- IDP_MAX",
                                    "Waiting for user input"
									       };

// équilibrage								
char *configuration_menu_2[6] = {									
                                    "1- I_BAL",
                                    "2- V_BAL",
                                    "3- DV",
                                    "4- MAN_CHARGE",
                                    "5- DEMO_MODE",
									         "Waiting for user input"
									      };

// mesures									
char *configuration_menu_3[8] = {											
          									"1- DT_V",
          									"2- DT_VS",
          									"3- DT_T",
          									"4- DT_TS",
          									"5- DT_I",
          									"6- N_MOD",
          									"7- N_CELL",                  									                									                									                									
          									"Waiting for user input"
                						};

// communication									
char *configuration_menu_4[3] = {											
          									"1- DATA_RATE",
          									"2- BASE_ID",
          									"Waiting for user input"
									     };

//***********************************
// Indications à afficher pour guider l'usager
// de modification des paramètres

 char *help_string[4]= 
  {
     "Commandes supplémentaires utiles:",
     "Appuyer sur la touche 'm' pour afficher le menu principal",
	 "Appuyer sur la touche 'Enter' pour saisir une donnée",
     "Appuyer sur la touche 'BS' pour retourner au menu précédent ou interrompre l'action en cours"
  };
// Indications pour l'utilisateur lors du processus du processur 
// de modification des paramètres  
 char *config_string[4]= 
  {
     "Modification de ",
     "Valeurs actuelle du paramètre: ",
     "\nEntrez une nouvelle valeur",
     "Nouvelle valeur du paramètre: "    
  };  
//************************************


 //Liste des paramètres
                									
// liste de paramètres de sécurité                									
char *list_param_securite[15] = {
                                    "TD_MIN_LIM",
                                    "TD_MAX_LIM",
                                    "TD_MIN",
                                    "TD_MAX",
                                    "TC_MIN_LIM",
                                    "TC_MAX_LIM",
                                    "TC_MIN",
                                    "TC_MAX",
                                    "V_MIN_LIM",
                                    "V_MAX_LIM",
                                    "V_MIN",
								            "V_MAX",
                                    "IC_MAX",
                                    "ID_MAX",
                                    "IDP_MAX"
									                };
									                
// liste des paramètres d'équilibrage									                
char *list_param_equilibrage[5] = {									
                                    "I_BAL",
                                    "V_BAL",
                                    "DV",
                                    "MAN_CHARGE",
                                    "DEMO_MODE"  
									              };




// liste des paramètres de mesure							
char *list_param_mesure[11] = {											
                									"DT_V",
                									"DT_VS",
                									"DT_T",
                									"DT_TS",
                									"DT_I",
                									"N_MOD",
                									"N_CELL",  
                									"K_HALL1",
                									"O_HALL1",                									
                									"K_HALL2",                									
                									"O_HALL2"                									                									                									                								
                									};

									              
// liste des paramètres de communication									
char *list_param_comm[2] = {											
                									"DATA_RATE",
                									"BASE_ID"
									              };									              									                                									
                									
                									
                									
              									
                									
//*****************************************
                																		              
									
									
//********************************************************
// menu status niveau 2 (choix des données à afficher)


char *status_menu_1[8] = {
                         "STATUS du BMS",
                         "SOC: ",
						        "courant: ",
                         "Vmin: ",
                         "Vmax: ",
                         "Tmin: ",
                         "Tmax: ",
                         "erreurs: "
                         }; 
                        
char *status_menu_2[2] = {
                            "Entrer le # du module esclave suivi de la touche enter",
                            "Affichage des données du module" 
                         };                        


char *status_menu_3[1] = {
                         "Affichage des données en mode continu pour le module"
                         }; 
                        

						
//**********************************************************
// menu de commandes niveau 2 (choix des données à afficher)	

char *commande_menu_1[2] = {
                             "Équilibrage activé",
                             "Équilibrage désactivé"
                           }; 

char *commande_menu_2[2] = {
                           "Relais fermés",
                           "Relais ouverts",
                           }; 	
                         

                                  

//*************************************************************
//  Functions
//************************************************************* 


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
}


//*****************************************************************************
// skip_lines
//
// Description:   Print a number of blank lines. 
//
//*****************************************************************************
void skip_lines (unsigned int number)
{
   unsigned int i;

   for(i=0; i<number; i++)
      SCIPutChar(0xA);

   SCIPutChar(0xD) ;
}


     
/*
* SCITx: Write data byte to SCIDRL register to transmission. 
* 
*
* Parameters: array[] : vecteur conteant les string à printer
*             index   : # de l'élément à imprimer
*             options : 0 saut de ligne (CR + LF)
*             options : 1 espace entre 2 données
*
* Return : None
*/
void SCITx(char *array[], unsigned int index,unsigned int options)
 {
   
   unsigned int char_pos = 0;

   while (array[index][char_pos] != '\0')
   {
      SCIPutChar(array[index][char_pos]);
      char_pos++;
   }
     
   switch (options)
   {
      case 0: 
          // 1 élément par ligne
          skip_lines(1);
          break; 

       case 1: 
           // Les éléments sont sur une même ligne et espacés par 1 tab
          SCIPutChar(0x20);     //space                       
          break;
     }
 }    
   
   
/*
* show_menu: Affiche un menu 
*            L'usager peut interagir avec le MCU.
*                     
* Parameters:  Tableau de char* contenant les strings à afficher
*                              
*
* Return : 
*/ 
void show_menu(char *menu[], unsigned int nb_elements)
{
   unsigned int i;

   for(i=0; i<nb_elements; i++)
      SCITx(menu, i, 0);

   skip_lines(1);
}



/*
* print_string: Convertie un int en un array de char et print dans le terminal.
* 
* Parameters: int arg: valeur à imprimer stocké dans un entier
*                      type : -0 aucune dimension 		" "
*                             -1 voltage 				"V"
*                             -2 temperature 			"oC"
*                             -3 SOC "State of Charge"  "%"                         
*						      -4 courant 				"A"
*							  -5 délai					"ms"

* Return : Aucun
*
* NOTE: Cette fonction fait juster shooter le data au terminal
*        (i.e ne gère aucun saut de ligne)
*/
void print_string(int arg, int type)
{
   char buffer[20];
   int char_pos = 0;
   int sizeof_char_buffer;

   switch (type)
   {
      case 0:
         //conversion du parametre int en char pour nombre sans décimale
         sizeof_char_buffer = sprintf(buffer,"%d", arg);
         break;
         
      case 1:
         //conversion du parametre int en char pour voltage
         sizeof_char_buffer = sprintf(buffer,"%1.2f V", arg/(float)100);
         break;
         
      case 2:
         //conversion du parametre int en char pour une temperature
         sizeof_char_buffer = sprintf(buffer,"%2.1f oC", arg/(float)10);
         break;
         
      case 3:
         //conversion du parametre int en char pour le SOC (%)
         sizeof_char_buffer = sprintf(buffer,"%3.1f %%", arg/(float)10);
         break;
         
      case 4:
         //conversion du parametre int en char pour un courant
         sizeof_char_buffer = sprintf(buffer,"%d A", arg);
         break;
         
      case 5:
         //conversion du parametre int en char pour un délai en ms
         sizeof_char_buffer = sprintf(buffer,"%d ms", arg);
         break;
   }
   
   for(char_pos=0; char_pos <= sizeof_char_buffer; char_pos++)
      SCIPutChar(buffer[char_pos]);
   
}


/*
* get_cells_data: Affiche les voltage et la température de chaque cellule
*
* Parameters:  int *volt[]: array 2 dimensions contenant les voltages
*			   int *temp[]: array 2 dimensions contenant les températures
*              int numer_mod : numéro du module pour lequel on veut les informations
*              int nb_cell : nombre de cellule par module
* Return : aucun.
*/
void get_cells_data(unsigned int volt[][N_CELL_SLV_MAX], int temp[][N_CELL_SLV_MAX], int numer_mod, int nb_cell)
{
  
   char cbuffer[8]; //buffer pour la conversion en char
   int nb_char;     // nombre de char dans le buffer (<8)
   int char_pos;  // position du char dans la loop d'écriture du *char
   int junk;
   char buf[40];

   unsigned int cell = 0; // cellule pour laquelle on affiche des donnés
   
   for(cell=0; cell < nb_cell; cell++)
   {
      nb_char = sprintf(cbuffer,"cell %d:", cell);

      for(char_pos=0; char_pos <= nb_char; char_pos++)    // envoie dans le SCI un string du type "cell i:"
         SCIPutChar(cbuffer[char_pos]);

      SCIPutChar(0x20);                       //space
      //print_string(volt[numer_mod][cell],1);  // écriture du voltage
      junk = sprintf(buf, formatVolt, (float)volt[numer_mod][cell]/1000.0);
      SCIprintString(buf);
      SCIPutChar(0x20);                       //space   
      //print_string(temp[numer_mod][cell],2);  //écriture de la température
      junk = sprintf(buf, formatTemp, (float)temp[numer_mod][cell]/10.0);
      SCIprintString(buf);
      skip_lines(1);
   }
     
}

//*****************************************************************************
// SCIprintStatus
//
// Description:   Print the status of the BMS. Including:
//                - SOC
//                - Last current measurement
//                - Minimal cell voltage, maximal cell voltage
//                - Minimal cell temperature, maximal cell temperature
//                - Error flags
//
//*****************************************************************************
void SCIprintStatus(void)
{
   int junk;
   char buf[40];

    SCITx(status_menu_1, 0 ,1);  //"STATUS du BMS "
	 skip_lines(1);
	 												
	 SCITx(status_menu_1, 1 ,1);  //"SOC: "
	 junk = sprintf(buf, formatSOC, (float)lastSOC/10.0);
    SCIprintString(buf);
	 skip_lines(1);
	 
	 SCITx(status_menu_1, 2 ,1);  //"courant: "
	 junk = sprintf(buf, formatCurrMa, (float)gLastCurrent/1000.0);
    SCIprintString(buf);
	 skip_lines(1);
	 
	 SCITx(status_menu_1, 3 ,1);  //"Vmin: "
	 junk = sprintf(buf, formatVolt, (float)glowestV/1000.0);
    SCIprintString(buf);
	 skip_lines(1);
	 
	 SCITx(status_menu_1, 4 ,1);  //"Vmax: "
	 junk = sprintf(buf, formatVolt, (float)gmaxV/1000.0);
    SCIprintString(buf);
	 skip_lines(1);
	 
	 SCITx(status_menu_1, 5 ,1);  //"Tmin: "
	 junk = sprintf(buf, formatTemp, (float)glowestT/10.0);
    SCIprintString(buf);
	 skip_lines(1);
	 
	 SCITx(status_menu_1, 6 ,1);  //"Tmax: "
	 junk = sprintf(buf, formatTemp, (float)gmaxT/10.0);
    SCIprintString(buf);
	 skip_lines(1);

	 SCIprintErrors();
	 
	 skip_lines(2);
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

   
   SCIprintString("Errors:\n");

   if(gErrorBits & ERROR_GND_FAULT)
      SCIprintString("Ground fault error\n");  

   if(gErrorBits & ERROR_SLV_COM_TO){
      SCIprintString("Slave communication timeout error for module(s): ");
      for(i=0; i<gMesuresParams.N_MOD; i++) {
         tmp = 1<<i;
         if((gSlaveComState & tmp) == tmp){
            junk = sprintf(buf,"%d ", i);
            SCIprintString(buf);
         }  
      }
      SCIPutChar('\n');
   } 

   if(gErrorBits & ERROR_SLV_COM_DATA)
      SCIprintString("Slave communication data integrity error\n");

   if(gErrorBits & ERROR_OPEN_CONNECT)
      SCIprintString("Cell open connection error\n");

   if(gErrorBits & ERROR_MAX_VOLT)
      SCIprintString("Cell overvoltage error\n");

   if(gErrorBits & ERROR_MIN_VOLT)
      SCIprintString("Cell undervoltage error\n");

   if(gErrorBits & ERROR_MAX_TEMP)
      SCIprintString("Temperature too high error\n");

   if(gErrorBits & ERROR_MIN_TEMP)
      SCIprintString("Temperature too low error\n");

   if(gErrorBits & ERROR_MAX_MEAN_CURR)
      SCIprintString("Maximum mean current error\n");

   if(gErrorBits & ERROR_MAX_PEAK_CURR)
      SCIprintString("Maximum peak current error\n");

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
   if(niveau_3 != 0){
      niveau_3 = 0;
      
   } else if(niveau_2 != 0) {
      niveau_2 = 0;
      	      
   } else {  
      niveau_1 = 0; 
   }
}

/*
void SCIcontinuous(void)
{

   static unsigned char count = 10;
   unsigned int i;
   char buf[160];
   int nChar;
   char* bufPos;
   
   bufPos = buf;

   if(count == 10){
      count = 0;
      for(i=0; i<gMesuresParams.N_CELL; i++){
         nChar = sprintf(bufPos, "CELL%d          ",i);
         bufPos += (nChar-1);  
      }
      sprintf(bufPos, "\n",i);
      SCIprintString(buf);
   }

   bufPos = buf;

   for(i=0; i<gMesuresParams.N_CELL; i++){
      nChar = sprintf(bufPos, "%1.3f/%2.1f      ", (float)gCellVolt[num_module-1][i]/1000.0, (float)gCellTemp[num_module-1][i]/10.0);
      bufPos += (nChar-1);  
   }
   sprintf(bufPos, "\n",i);
   SCIprintString(buf);
   
}
*/

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
		if (niveau_1 == 0 && niveau_2 == 0 && niveau_3 == 0)  															 
		{
			//chaque case permet d'afficher un sous-menu différent à l'écran
			switch (input)
			{
            case 0:    //Root menu
               show_menu(main_menu, 4);
         		skip_lines(1);
         		show_menu(help_string, 4);   
               break;
            case '1':  //affichage menu configuration
               show_menu(configuration_menu, 5);
               niveau_1 = 1; 
               break;
            case '2':  //Affichage menu status
               show_menu(status_menu, 4);
               niveau_1 = 2;
               break;
            case '3': //Affichage menu commande
               show_menu(commande_menu, 7);
               niveau_1 = 3;
               break;
			} 
			
			skip_lines(2);
		}
//menu de configuration (4 sous menus de paramètres à modifier)
		else if (niveau_1 == 1 && niveau_2 == 0 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
			  case 0:    //Root menu
               show_menu(configuration_menu, 5);   
               break;
			  case '1':  //Paramètres de sécurité
					show_menu(configuration_menu_1, 16);			
					niveau_2 = 1;				
					break;
			  case '2':  //Paramètres d'équilibrage
					show_menu(configuration_menu_2, 6);							
					niveau_2 = 2;				
					break;
			  case '3': //Paramètres de mesures
					show_menu(configuration_menu_3, 8);				
					niveau_2 = 3; 				
					break;
			  case '4': //Paramètres de communication
					show_menu(configuration_menu_4, 3);			
					niveau_2 = 4; 								
					break;				
			}
			
			skip_lines(2);
		}	
//sous-menu configuration_menu_1 (liste des paramètres de sécurité (16 choix))
		else if (niveau_1 == 1 && niveau_2 == 1 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
			  case 0:    //Root menu
               show_menu(configuration_menu_1, 16);   
               break;    
               
			  case '1':  //TD_MIN_LIM
				niveau_3 = 1;
				SCITx(config_string, 0 ,1);         //"Modification de "
				SCITx(list_param_securite, 0 ,0);   //nom du param que l'on modifie	
				SCITx(config_string, 1 ,1);         // "Valeurs actuelle du paramètre: "
            junk = sprintf(buf,formatTemp, (float)gSecurityParams.TD_MIN_LIM/10.0);
            SCIprintString(buf); 
				SCITx(config_string, 2 ,0);         //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;             //input mode (sci_interrupt_mode = 1)
				break;
				
			  case '2':  //TD_MAX_LIM
				niveau_3 = 2;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 1 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTemp, (float)gSecurityParams.TD_MAX_LIM/10.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '3': //TD_MIN
				niveau_3 = 3;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 2 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTemp, (float)gSecurityParams.TD_MIN/10.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '4': //TD_MAX
				niveau_3 = 4;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 3 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
		   	junk = sprintf(buf,formatTemp, (float)gSecurityParams.TD_MAX/10.0);
            SCIprintString(buf); 
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
								
			  case '5': //TC_MIN_LIM 
				niveau_3 = 5;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 4 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTemp, (float)gSecurityParams.TC_MIN_LIM/10.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '6': //TC_MAX_LIM
				niveau_3 = 6;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 5 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTemp, (float)gSecurityParams.TC_MAX_LIM/10.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '7': //TC_MIN 
				niveau_3 = 7;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 6 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: " 
		   	junk = sprintf(buf,formatTemp, (float)gSecurityParams.TC_MIN/10.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '8': //TC_MAX 
				niveau_3 = 8;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 7 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTemp, (float)gSecurityParams.TC_MAX/10.0);
            SCIprintString(buf); 
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case '9': //V_MIN_LIM
				niveau_3 = 9;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 8 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
		   	junk = sprintf(buf,formatVolt, (float)gSecurityParams.V_MIN_LIM/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 'q': //V_MAX_LIM 
				niveau_3 = 10;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 9 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatVolt, (float)gSecurityParams.V_MAX_LIM/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 'w': //V_MIN 
				niveau_3 = 11;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 10 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatVolt, (float)gSecurityParams.V_MIN/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 'e': //V_MAX 
				niveau_3 = 12;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 11 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatVolt, (float)gSecurityParams.V_MAX/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 'r': //IC_MAX			  
				niveau_3 = 13;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 12 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatCurr, gSecurityParams.IC_MAX);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 't': //ID_MAX			  
				niveau_3 = 14;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_securite, 13 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatCurr, gSecurityParams.ID_MAX);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
				
			  case 'y': //IDP_MAX			  
				niveau_3 = 15;
				SCITx(config_string, 0 ,1);         //"Modification de "
				SCITx(list_param_securite, 14 ,0);  //nom du param que l'on modifie
				SCITx(config_string, 1 ,1);         // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatCurr, gSecurityParams.IDP_MAX);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);         //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;             //input mode (sci_interrupt_mode =1)				
				break;				
			}
			
			skip_lines(1);
		}		
//sous-menu configuration_menu_2 (liste des paramètres d'équilibrage(4 choix))
		else if (niveau_1 == 1 && niveau_2 == 2 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
            case 0:    //Root menu
               show_menu(configuration_menu_2, 6);   
               break;
			  case '1':  //I_BAL			    
				niveau_3 = 1;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_equilibrage, 0 ,0); //nom du param que l'on modifie							
				SCITx(config_string, 1,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatCurr, gEquiParams.I_BAL);
            SCIprintString(buf);
				SCITx(config_string, 2 ,1);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)
				break;			  
			  case '2':  //V_BAL			  
				niveau_3 = 2;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_equilibrage, 1 ,0);	//nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatVolt, (float)gEquiParams.V_BAL/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
			  case '3': //DV			  
				niveau_3 = 3;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_equilibrage, 2 ,0);	//nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatVolt, (float)gEquiParams.DV/1000.0);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
			  case '4': //MAN_CHARGE			  
				niveau_3 = 4;SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_equilibrage, 3 ,0);	//nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
			   junk = sprintf(buf,"%d", gEquiParams.MAN_CHARGE);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
			  case '5': //DEMO_MODE			  
				niveau_3 = 5;SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_equilibrage, 4 ,0);	//nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
			   junk = sprintf(buf,"%u", gEquiParams.DEMO_MODE);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;					
			}
			skip_lines(1);			
		}
//sous-menu configuration_menu_3 (liste des paramètres de mesure (7 choix))
		else if (niveau_1 == 1 && niveau_2 == 3 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
			  case 0:    //Root menu
               show_menu(configuration_menu_3, 8);   
               break;
			  case '1':  //DT_V
				niveau_3 = 1;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 0 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTime, gMesuresParams.DT_V);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)
				break;
			  
			  case '2':  //DT_VS
				niveau_3 = 2;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 1 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTime, gMesuresParams.DT_VS);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;

			  case '3': //DT_T  			  
				niveau_3 = 3;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 2 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTime, gMesuresParams.DT_T);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;

			  case '4': //DT_TS 			   
				niveau_3 = 4;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 3 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTime, gMesuresParams.DT_TS);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;							
			  case '5': //DT_I			  
				niveau_3 = 5;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 4 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,formatTime, gMesuresParams.DT_I);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
			  case '6': //N_MOD     			   
				niveau_3 = 6;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 5 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,"%u", gMesuresParams.N_MOD);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
        
			  case '7': //N_CELL			  
				niveau_3 = 7;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_mesure, 6 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,"%u", gMesuresParams.N_CELL);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;
	
			}
			skip_lines(1);
		}			  		
//sous-menu configuration_menu_4 (liste des paramètres de communication (2 choix))
		else if (niveau_1 == 1 && niveau_2 == 4 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
			  case 0:    //Root menu
               show_menu(configuration_menu_4, 3);   
               break;			
			  case '1':  //DATA_RATE
				niveau_3 = 1;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_comm, 0 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,"%u", gComParams.DATA_RATE);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)
				break;			  
			  case '2':  //BASE_ID			  
				niveau_3 = 2;
				SCITx(config_string, 0 ,1); //"Modification de "
				SCITx(list_param_comm, 1 ,0); //nom du param que l'on modifie
				SCITx(config_string, 1 ,1); // "Valeurs actuelle du paramètre: "
				junk = sprintf(buf,"%u", gComParams.BASE_ID);
            SCIprintString(buf);
				SCITx(config_string, 2 ,0);  //"Entrez une nouvelle valeur",
				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
				break;		
			}
			skip_lines(1);
		}		
//***************************************
//menu de status (3 choix)
//***************************************
		else if (niveau_1 == 2 && niveau_2 == 0 && niveau_3 == 0)  
		{													  

			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
				case 0:    //Root menu
               show_menu(status_menu, 4);  
               break;
            case '1':  //affichage du STATUS du BMS 
            // soc, vmin, vmax, tmin, tmax, code d'erreur			
               SCIprintStatus(); 
               show_menu(status_menu, 4); 
               break;

            case '2':  //Affichage des tensions et températures pour 1 module								
               niveau_2 = 2;
               SCITx(status_menu_2, 0 ,0); // "Entrer le # du module esclave suivi de la touche enter"
               sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)				
               break;

            case '3': //Affichage des données en mode continu pour 1 module							
               niveau_2 = 3;				
               SCITx(status_menu_2, 0 ,0); // "Entrer le # du module esclave suivi de la touche enter"
               sci_interrupt_mode = 1;	//input mode (sci_interrupt_mode =1)
               // NOTE: Dans le mode assignation, une interruption périodique est lancée
               // juste après la saise du module à afficher en loop.
               break;
				
			}
				
			skip_lines(1);
		}				
//***************************************
//menu de commandes (2 choix)
//***************************************
		else if (niveau_1 == 3 && niveau_2 == 0 && niveau_3 == 0)  
		{													  
			//chaque case permet d'afficher un sous-menu du menu de configuration
			switch (input)
			{
			  case 0:    //Root menu
               show_menu(commande_menu, 7);  
               break;
               
			  case '1':  //Activer équilibrage
			  
			      if(!gActivEqui){
                  if(gEquiParams.DEMO_MODE){
                     niveau_2 = 1;
                     SCIprintString("Entrez la valeur de tension à atteindre (en V): ");
         				sci_interrupt_mode = 1;  //input mode (sci_interrupt_mode =1)
                  } else {
                     gActivEqui = 1;
                  }	
               } else {
                  SCIprintString("Équilibrage déjà activé.\n");
                  skip_lines(2);
		            show_menu(commande_menu, 7);
               }
               break;
				 
				 
			  case '2':  //Désactiver l'équilibrage
			  
               if(gEquiParams.DEMO_MODE){
                  //TODO: envoyer commande can de fin d'equilibrage a l'esclave
                  //On attend la réponse de fin d'équilibrage de tous les esclaves avant
                  //de mettre le flag à 0. Même mécanisme que pour la fin d'équilibrage
                  //"naturelle".
                  CAN0SendCommand(COMMAND_BAL_STOP,0,0x03FF,0);
                  gActivEqui = 0;  //TODO: enlever cette ligne, pour fin de debug
               } else {
                  gActivEqui = 0;   
               }
               skip_lines(1);
               SCITx(commande_menu_1, 1 ,0); // "Équilibrage désactivé"
               skip_lines(2);
		      	show_menu(commande_menu, 7);
               break;
				
						
			  case '3':  //Fermer Relais
               if(!gRelaysClosed){
                  Precharge(PRECHARGE_DELAY);
                  CloseRelays(RELAY_DELAY);
                  gRelaysClosed = 1;
               }
               break;
				
				
			  case '4':  //Ouvrir Relais				
               if(gRelaysClosed)
                  OpenRelays(RELAY_DELAY);
                  gRelaysClosed = 0;
               break;
           
           
           case '5':  //Activer charge				
               if(!gActivCharge){
                  if(gEquiParams.DEMO_MODE){
                     //TODO: vérifier que le chargeur est connecté
                     PITCE_PCE3 = 1;      //Activer l'envoi de commandes au brusa
                     gActivCharge = 1;
                  } else {
                     gActivCharge = 1;   
                  }
               }
               break;
           
           
           case '6':  //Désactiver charge				
               if(gEquiParams.DEMO_MODE){
                  PITCE_PCE3 = 0;            //Arret d'envoi de commandes au Brusa: fin de charge automatique
               }
               gActivCharge = 0;
               SCIprintString("Charge désactivée.\n\n");
		      	show_menu(commande_menu, 7);
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
   char buf[40];
   int junk;
   unsigned int i=0;

	skip_lines(1);
	//Menu configuration -> Assignation des paramètres de sécurité
	if(niveau_1 == 1 && niveau_2 == 1)
	{
	   SCITx(config_string, 3 ,1);  //"Nouvelle valeur du paramètre: "
	
		switch (niveau_3)
		{	
	
			case 1: 
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TD_MIN_LIM = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);						
					break;		
			case 2: 
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TD_MAX_LIM = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 3:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TD_MIN = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);			
					break;
			case 4:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TD_MAX = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 5:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TC_MIN_LIM = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 6:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TC_MAX_LIM = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 7:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TC_MIN = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0); 
					break;
			case 8:
			      iNewParam = (int)(user_input*10);
			      gSecurityParams.TC_MAX = iNewParam;
			      junk = sprintf(buf,formatTemp, (float)iNewParam/10.0);  
					break;
			case 9:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gSecurityParams.V_MIN_LIM = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 10:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gSecurityParams.V_MAX_LIM = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 11:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gSecurityParams.V_MIN = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 12:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gSecurityParams.V_MAX = uiNewParam;
			      junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 13:
			      uiNewParam = (unsigned int)user_input;
			      gSecurityParams.IC_MAX = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam); 
					break;
			case 14:
			      uiNewParam = (unsigned int)user_input;
			      gSecurityParams.ID_MAX = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam); 
					break;
			case 15:
			      uiNewParam = (unsigned int)user_input;
			      gSecurityParams.IDP_MAX = uiNewParam;
			      junk = sprintf(buf,formatCurr, uiNewParam);  
					break; 
		}
		
		SCIprintString(buf);
		skip_lines(2);

    } 
	//Menu configuration -> Assignation des paramètres de d'équilibrage
	else if(niveau_1 == 1 && niveau_2 == 2)
	{
	   SCITx(config_string, 3 ,1);  //"Nouvelle valeur du paramètre: "
	
		switch (niveau_3)
		{
			case 1:
			      uiNewParam = (unsigned int)user_input;
			      gEquiParams.I_BAL = uiNewParam;
					junk = sprintf(buf,formatCurr, uiNewParam);
					break;
			case 2:
			      uiNewParam = (unsigned int)(user_input*1000);
			      gEquiParams.V_BAL = uiNewParam;
					junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0);
					break;
			case 3:
		      	uiNewParam = (unsigned int)(user_input*1000);
			      gEquiParams.DV = uiNewParam;
					junk = sprintf(buf,formatVolt, (float)uiNewParam/1000.0); 
					break;
			case 4:
			      ucNewParam = (unsigned char) user_input;
			      gEquiParams.MAN_CHARGE = ucNewParam;
			      junk = sprintf(buf,"%u", ucNewParam); 
					break;
			case 5:
			      ucNewParam = (unsigned char) user_input;
			      gEquiParams.DEMO_MODE = ucNewParam;
			      junk = sprintf(buf,"%u", ucNewParam); 
					break;
		}
		
		SCIprintString(buf);
		skip_lines(2);	
	} 	   
	//Menu configuration -> Assignation des paramètres de mesure
	else if(niveau_1 == 1 && niveau_2 == 3)
	{
	
	   SCITx(config_string, 3 ,1);  //"Nouvelle valeur du paramètre: "
	
		switch (niveau_3)
		{
			case 1:
			      uiNewParam = (unsigned int)user_input;
			      gMesuresParams.DT_V = uiNewParam;
					junk = sprintf(buf,formatTime, uiNewParam);
					break;
			case 2:
			      uiNewParam = (unsigned int)user_input;
			      gMesuresParams.DT_VS = uiNewParam;
					junk = sprintf(buf,formatTime, uiNewParam); 
					break;
			case 3:
			      uiNewParam = (unsigned int)user_input;
			      gMesuresParams.DT_T = uiNewParam;
					junk = sprintf(buf,formatTime, uiNewParam); ;
					break;
			case 4:
			      uiNewParam = (unsigned int)user_input;
			      gMesuresParams.DT_TS = uiNewParam;
					junk = sprintf(buf,formatTime, uiNewParam); 
					break;
			case 5:
			      uiNewParam = (unsigned int)user_input;
			      gMesuresParams.DT_I = uiNewParam;
					junk = sprintf(buf,formatTime, uiNewParam); 
					break;
			case 6:
			      ucNewParam = (unsigned char)user_input;
			      gMesuresParams.N_MOD = ucNewParam;
					junk = sprintf(buf,"%u", ucNewParam); 
					break;
					
			case 7:
			      ucNewParam = (unsigned char)user_input;
			      gMesuresParams.N_CELL = ucNewParam;
					junk = sprintf(buf,"%u", ucNewParam); 
					break;						
		}
		
		SCIprintString(buf);
		skip_lines(2);	
	} 		
	//Menu configuration -> Assignation des paramètres de communication
	else if(niveau_1 == 1 && niveau_2 == 4)
	{
	
	   SCITx(config_string, 3 ,1);  //"Nouvelle valeur du paramètre: "
	   
		switch (niveau_3)
		{
			case 1:
			      ucNewParam = (unsigned char)user_input;
			      gComParams.DATA_RATE = ucNewParam;
					junk = sprintf(buf,"%u", ucNewParam); 			
					break;
			case 2:
		      	uiNewParam = (unsigned int)user_input;
			      gComParams.BASE_ID = uiNewParam;
					junk = sprintf(buf,"%u", uiNewParam); 
					break;
		}
		
		SCIprintString(buf);
		skip_lines(2);					
	}
// Menu status -> sélection du module à afficher dans le terminal
	else if(niveau_1 == 2)
	{
	
	   num_module = (unsigned int) user_input; //assignation du nombre de module
	
		switch (niveau_2)
		{
			case 2: 
            SCITx(status_menu_2, 1 ,1);  //"Affichage des données du module " 
            junk = sprintf(buf,"%u", num_module);
            SCIprintString(buf);
            skip_lines(1);			
            get_cells_data(gCellVolt, gCellTemp, num_module - 1 ,gMesuresParams.N_CELL );			
				break;
			
			case 3:
            SCITx(status_menu_3, 0 ,1);  //"Affichage des données en mode continu pour le module " 
            junk = sprintf(buf,"%u", num_module);
            SCIprintString(buf);
            skip_lines(1);	 
            PITCE_PCE3 = 1; //activation de l'interrupt, le mode continu pourra
            sci_interrupt_mode = 2;
            break;          //être interrompu en appuyant sur la touche backspace. 				
		}
	}
	//Menu commandes
	else if(niveau_1 == 3){
  		switch (niveau_2)
		{
			case 1:
			 
		      uiNewParam = (unsigned int)(user_input*1000);      //Balance threshold
            junk = sprintf(buf,"Valeur entrée: %u mV\n\n", uiNewParam);
            SCIprintString(buf);
            
		      if((uiNewParam < gSecurityParams.V_MAX_LIM) && (uiNewParam > gSecurityParams.V_MIN_LIM)) {
		      
		         //Envoi de la commande d'équilibration
		         CAN0SendCommand(COMMAND_BAL,0,0x03FF,uiNewParam);
		         
		         //On met les flags d'équilibration des modules à 1
		         for(i=0; i<gMesuresParams.N_MOD; i++)
		            gSlaveEquiStatus = gSlaveEquiStatus | (1<<i);
		         
		         //on active le flag interne d'équilibration     
		         gActivEqui = 1;  
		         
		      } else {
		         SCIprintString("Invalid value entered.\n\n\n");   
		      }
				break;
		} 
	   
	}
			
}
