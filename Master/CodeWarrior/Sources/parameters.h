#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "defines.h"


//*****************************
//  BMS parameters structures 
//*****************************

struct security_params_t
{
   int TD_MIN_LIM;            // [dixieme oC] Minimal operating temperature in normal mode
   int TD_MAX_LIM;            // [dixieme oC] Maximal operating temperature in normal mode
   int TD_MIN;                // [dixieme oC] Low threshold operating temperature in normal mode
   int TD_MAX;                // [dixieme oC] High threshold operating temperature in normal mode
   int TC_MIN_LIM;            // [dixieme oC] Minimal operating temperature in charge mode
   int TC_MAX_LIM;            // [dixieme oC] Maximal operating temperature in charge mode
   int TC_MIN;                // [dixieme oC] Low threshold operating temperature in charge mode
   int TC_MAX;                // [dixieme oC] High threshold operating temperature in charge mode
   unsigned int V_MIN_LIM;    // [mV] Minimal cell voltage
   unsigned int V_MAX_LIM;    // [mV] Maximal cell voltage
   unsigned int V_MIN;        // [mV] Low threshold cell voltage
   unsigned int V_MAX;        // [mV] High threshold cell voltage
   unsigned int IC_MAX;       // [A] Maximal charge current
   unsigned int ID_MAX;       // [A] Maximal mean discharge current
   unsigned int IDP_MAX;      // [A] Maximal peak discharge current
                     
};

typedef struct security_params_t sec_params_t;


struct equilibrating_params_t
{
   unsigned int I_BAL;              // [A] Balancing current
   unsigned int V_BAL;              // [mV] Balancing voltage
   unsigned int DV;                 // [mV] Maximal error on the cell voltages when balancing
   unsigned char MAN_CHARGE;        // 0: automatic charge when charger is detected, 1: mode is forced to charge. Manual charge, equilibration and relay control possible.
   unsigned char DEMO_MODE;         // 1: No automatic detection of errors
   
};

typedef struct equilibrating_params_t equi_params_t;


struct mesures_params_t
{
   unsigned int DT_V;               // [ms] Cell voltages acquisition period in normal mode
   unsigned int DT_VS;              // [ms] Cell voltages acquisition period in stand-by mode
   unsigned int DT_T;               // [ms] Cell temperature acquisition period in normal mode
   unsigned int DT_TS;              // [ms] Cell temperature acquisition period in stand-by mode
   unsigned int DT_I;               // [ms] Current acquisition period in normal mode
   unsigned char N_MOD;             // Number of connected slave modules
   unsigned char N_CELL;            // Number of slaves connected to each module

   
};

typedef struct mesures_params_t mes_params_t;


struct communications_params_t
{
   unsigned char DATA_RATE;         // CAN bus transfer rate. 0: 125 kbps, 1: 250 kbps, 2: 500 kpbs, 3: 1 Mbps
   unsigned int BASE_ID;            // BMS base CAN ID
   
   
};

typedef struct communications_params_t com_params_t;


//*************************
//  Variables declaration
//*************************

extern sec_params_t gSecurityParams;
extern equi_params_t gEquiParams;
extern mes_params_t gMesuresParams;
extern com_params_t gComParams;


#endif   //PARAMETERS_H