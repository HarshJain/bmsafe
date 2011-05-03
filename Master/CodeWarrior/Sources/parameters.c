#include "parameters.h"

//----------------------------
// BMS parameters definitions
//----------------------------

sec_params_t gSecurityParams = {
                                   -1000,  //TD_MIN_LIM  
                                    1000,  //TD_MAX_LIM
                                   -1000,  //TD_MIN
                                    1000,  //TD_MAX
                                   -1000,  //TC_MIN_LIM
                                    1000,  //TC_MAX_LIM
                                   -1000,  //TC_MIN
                                    1000,  //TC_MAX
                                       0,  //V_MIN_LIM
                                   10000,  //V_MAX_LIM
                                       0,  //V_MIN
                                   10000,  //V_MAX
                                     500,  //IC_MAX
                                     500,  //ID_MAX
                                     500   //IDP_MAX
                                };
                                
equi_params_t gEquiParams = {
                                    100,  //I_BAL
                                    410,  //V_BAL
                                    100,  //DV
                                    1,    //MAN_CHARGE
                                    1     //DEMO_MODE
                             };
                             
mes_params_t gMesuresParams = {
                                    1000,    //DT_V
                                    10000,   //DT_VS
                                    2000,    //DT_T
                                    10000,   //DT_TS
                                    15,      //DT_I
                                    2,       //N_MOD
                                    10       //N_CELL

                              };
                              
com_params_t gComParams = {
                              0,    //DATA_RATE
                              0     //BASE_ID
                           };
                           
                           
                           
/*  Default parameters

sec_params_t gSecurityParams = {
                                    -40,  //TD_MIN_LIM  
                                    100,  //TD_MAX_LIM
                                    -30,  //TD_MIN
                                     80,  //TD_MAX
                                    -40,  //TC_MIN_LIM
                                     90,  //TC_MAX_LIM
                                    -30,  //TC_MIN
                                     70,  //TC_MAX
                                    200,  //V_MIN_LIM
                                    420,  //V_MAX_LIM
                                    220,  //V_MIN
                                    400,  //V_MAX
                                     10,  //IC_MAX
                                    300,  //ID_MAX
                                    350   //IDP_MAX
                                };
                                
equi_params_t gEquiParams = {
                                    100,  //I_BAL
                                    410,  //V_BAL
                                    100,  //DV
                                    1     //MAN_CHARGE
                             };
                             
mes_params_t gMesuresParams = {
                                    1000,    //DT_V
                                    10000,   //DT_VS
                                    2000,    //DT_T
                                    10000,   //DT_TS
                                    15,      //DT_I
                                    1,       //N_MOD
                                    10       //N_CELL

                              };
                              
com_params_t gComParams = {
                              0,    //DATA_RATE
                              0     //BASE_ID
                           };
*/