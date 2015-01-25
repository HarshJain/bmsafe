#include "Display.h"

#include <hidef.h>
#include "defines.h"
#include "derivative.h"
#include "relays.h"

// AFFICHAGE 7 segments
void UpdateErrorDisplay()
{
	 
	if(gFlags.errorState)
	{
		//Affichage du numéro de l'erreur selon l'erreur
		if(gError.slaveTimeout)
			display(SlaveTimeout_ErrorCode);
		else if(gError.cellMaxTemp)
			display(CellMaxTemp_ErrorCode);
		else if(gError.cellMaxVolt)
			display(CellMaxVolt_ErrorCode);
		else if(gError.cellMinVolt)
			display(CellMinVolt_ErrorCode);
		else if(gError.cellMinTemp)
			display(CellMinTemp_ErrorCode);
		else if((!gFlags.drive_online && !gFlags.charger_online) || (gFlags.drive_online && gFlags.charger_online))
			display('H');
		else if(!gFlags.TCS_online && !gFlags.charger_online)
			display('L');
		else
			display(GenericErrorState_ErrorCode);	
  }
	else if(gFlags.ImdError)
		display(ImdError_ErrorCode);
	else if(gFlags.cellHighTemp)
        display(CellHighTemp_WarningCode);
  else if(gFlags.cellLowVolt)
      display(CellLowVolt_WarningCode);
  else if(gFlags.cellHighVolt)
      display(CellHighVolt_WarningCode);
  else if(gFlags.cellLowTemp)
      display(CellLowTemp_WarningCode);
	else if(CLOSED == gRelayState)
		display(RelaysCloses_MessageCode);
  else
      display(NoError_MessageCode);
	
	// Point du 7 segment
	if(gFlags.interlockClosed)
        display(InterlockClosed_MessageCode);
	else
		display(InterlockOpen_MessageCode); 
}


// ----------------- LED sur le tableau de bord -----------------
void UpdateDashLed()
{
  	static uint16 flashSequence;
	  
		if(flashSequence > FLASH_LED_PERIOD)
		{
			flashSequence = 0;
		}
	   
		if(!gFlags.interlockClosed)
		{
			flashSequence++;
			//Clignotte si l'interlock est ouvert
			if(flashSequence < (FLASH_LED_PERIOD / 2))
				WARNING_DASH_LED = 0; //On éteint
			else
				WARNING_DASH_LED = 1; //On allume
		}
		else if (gFlags.errorState)
			WARNING_DASH_LED = 1; //Allumée en cas d'erreur
		else
			WARNING_DASH_LED = 0; //Éteinte
}


// ***********************************************************************
// Modifie l'affichage de l'afficheur 7 segments selon le paramÃ¨tre donnÃ©
//	ParamÃ¨tre :
//	0 Ã  9 affiche les chiffres de 0 Ã  9
//  A,E,C : affiche la lettre correspondante
//  '.' : allume le point
//  ',' : Ã©teint le point
// ***********************************************************************

void display(int16 no)
{
	switch(no)
	{
		case 0:
		// Affichage de 0
			SEGMENT_A = 0; // LED off = 1
			SEGMENT_B = 0; // LED on = 0
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 1;
			break;
	
		case 1:
		// Affichage de 1
			SEGMENT_A = 1;
			SEGMENT_B = 0;
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 1;
			break;
			
		case 2:
		// Affichage de 2
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
			break;
			
		case 3:
		// Affichage de 3
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
			break;
			
		case 4:
		//Affichage de 4
           SEGMENT_A = 1;
           SEGMENT_B = 0; 
           SEGMENT_C = 0;
           SEGMENT_D = 1;
           SEGMENT_E = 1;
           SEGMENT_F = 0;
           SEGMENT_G = 0;
			break;
			
		case 5:
		//Affichage de 5
           SEGMENT_A = 0;
           SEGMENT_B = 1; 
           SEGMENT_C = 0;
           SEGMENT_D = 0;
           SEGMENT_E = 1;
           SEGMENT_F = 0;
           SEGMENT_G = 0;
			break;
			
		case 6:
		//Affichage de 6
			SEGMENT_A = 0;
			SEGMENT_B = 1; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 7:
		//Affichage de 7
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 1;
			break;
			
		case 8:
		//Affichage de 8
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 9:
		//Affichage de 9
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
		
		case 'A': // no = 65
		//Affichage de la lettre A
			SEGMENT_A = 0;
			SEGMENT_B = 0; 
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
		
		case 'C': //no = 67
		//Affichage de la lettre C
           SEGMENT_A = 0;
           SEGMENT_B = 1; 
           SEGMENT_C = 1;
           SEGMENT_D = 0;
           SEGMENT_E = 0;
           SEGMENT_F = 0;
           SEGMENT_G = 1;
			break;
			
		case 'E': //no = 69
		//Affichage de E
			SEGMENT_A = 0;
			SEGMENT_B = 1;
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 'F': // no = 70
			SEGMENT_A = 0;
			SEGMENT_B = 1;
			SEGMENT_C = 1;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 'P':	// no = 80
		// Affichage de P
			SEGMENT_A = 0;
			SEGMENT_B = 0;
			SEGMENT_C = 1;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case '.': // no = 46
			DOT = 0;
			break;
			
		case ',': // no = 44
			DOT = 1;
			break;	

		case 'H': // no = ?
		// Affichage de h
			SEGMENT_A = 1;
			SEGMENT_B = 0;
			SEGMENT_C = 0;
			SEGMENT_D = 1;
			SEGMENT_E = 0;
			SEGMENT_F = 0;
			SEGMENT_G = 0;
			break;
			
		case 'L': // no = 76
		//Affichage de L
           SEGMENT_A = 1;
           SEGMENT_B = 1; 
           SEGMENT_C = 1;
           SEGMENT_D = 0;
           SEGMENT_E = 0;
           SEGMENT_F = 0;
           SEGMENT_G = 1;
		   break;
			
		default:
		//Affichage de trois bandes horizontales
			SEGMENT_A = 0;
			SEGMENT_B = 1; 
			SEGMENT_C = 1;
			SEGMENT_D = 0;
			SEGMENT_E = 1;
			SEGMENT_F = 1;
			SEGMENT_G = 0;
	}
	
}