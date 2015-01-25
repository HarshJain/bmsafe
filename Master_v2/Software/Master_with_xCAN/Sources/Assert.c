#include "Assert.h"

#include <hidef.h>            /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "terminal_code.h"
#include "defines.h"
#include "termio.h"
#include "common.h"
#include "charger.h"

//------------- INSPECTION DES TENSIONS EXTRÊMES ------------- 
void AssertSafeVoltage()
{
   static uint16 delayMinVolt = 0;
     
   gFlags.cellLowVolt = *gLowestCellVoltage < gParams.lowCellVoltage;
   gError.cellMinVolt = *gLowestCellVoltage < gParams.minCellVoltage;
   gFlags.cellHighVolt = *gHighestCellVoltage > gParams.highCellVoltage;
   gError.cellMaxVolt = *gHighestCellVoltage > gParams.maxCellVoltage;
   
   
   //----------- PATCH : Délai avant de déclencher une erreur cellMinVolt
   // L'accélération du véhicule provoque une diminution momentanée de la tension aux bornes des cellues du au courant sortant. Il ne faut pas
   // que le BMS tombe en erreur lors d'une accélération. On ajoute donc un délai.
   
   if(gError.cellMinVolt)
   {
		if(delayMinVolt < DELAI_CELLMINVOLT)
		{
			gError.cellMinVolt = 0; //on ignore l'erreur
			delayMinVolt++;
		}
   }
   else
   {
		delayMinVolt = 0; //on remet le compteur à 0
   }
}

//------------- INSPECTION DES TEMPÉRATURES EXTRÊMES ------------- 
void AssertSafeTemperature() 
{
   if(gCharging_state != NOT_CHARGING) {
      gError.cellMinTemp = *gLowestCellTemp < gParams.minDischargeCellTemp;
      gFlags.cellLowTemp = *gLowestCellTemp < gParams.lowDischargeCellTemp;
      gError.cellMaxTemp = *gHighestCellTemp > gParams.maxDischargeCellTemp;
      gFlags.cellHighTemp = *gHighestCellTemp > gParams.highDischargeCellTemp;
   } else {
      gError.cellMinTemp = *gLowestCellTemp < gParams.minChargeCellTemp;
      gFlags.cellLowTemp = *gLowestCellTemp < gParams.lowChargeCellTemp;
      gError.cellMaxTemp = *gHighestCellTemp > gParams.maxChargeCellTemp;
      gFlags.cellHighTemp = *gHighestCellTemp > gParams.highChargeCellTemp;
   }
}

//------------- VÉRIFICATION DES DÉLAIS DE COMMUNICATION -------------
void AssertCommunicationWithSlave()
{
   uint16 tmp;
   uint8 i; 
   for(i=0; i<N_MOD; ++i) {
      tmp = (1<<i);
      if(idleCount[i] >= COM_IDLE_COUNT_MAX) {
         gSlaveComState = gSlaveComState | tmp;    // Il y a un slave de déconnecté
         gError.slaveTimeout = 1;
      } else {
         gSlaveComState = gSlaveComState & ~tmp;
      }
   }

   if(!gSlaveComState)
      gError.slaveTimeout = 0;   //Il n'y a plus de retard dans les comm.
}

//---------------- INSPECTION DE LA MESURE DE COURANT ----------------
void AssertSafeCurrent() 
{
     //long int highCurrent, maxCurrent;
 // if(!gFlags.charging) {
    // //En décharge
    // highCurrent = gParams.highPeakDischargeCurrent;
    // maxCurrent = gParams.maxPeakDischargeCurrent;
    // gFlags.highPeakCurrent = (gCurrentFlow_mA > highCurrent) || (gCurrentFlow_mA < -highCurrent);
    // gError.maxPeakCurrent = (gCurrentFlow_mA > maxCurrent) || (gCurrentFlow_mA < -maxCurrent);
      
 // } else {
    // //En charge
    // maxCurrent = gParams.maxMeanChargeCurrent;
    // gError.maxPeakCurrent = (gCurrentFlow_mA > maxCurrent) || (gCurrentFlow_mA < -maxCurrent);
 //}                     
 
  gError.maxPeakCurrent = 0;
  gFlags.highPeakCurrent = 0;
  gError.maxMeanCurrent = 0;   //TODO: quoi faire avec le courant moyen?
}