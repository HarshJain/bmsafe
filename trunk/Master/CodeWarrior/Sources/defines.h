#ifndef DEFINES_H
#define DEFINES_H


#define ID_500          0xA0000000
#define ID_780          0xF0000000

#define N_MOD_MAX       10                          //Maximum number of slaves modules
#define N_CELL_SLV_MIN   4                          //Minimum number of cells per board
#define N_CELL_SLV_MAX  10                          //Maximum number of cells per board
#define N_CELL_MAX      N_MOD_MAX*N_CELL_SLV_MAX    //Maximum number of total cells
#define N_CURR_MSR      100                         //Number of current measurements after which the SOC is calculated
#define RELAY_DELAY     100                         //[ms] Time between the opening or closing of 2 relays
#define PRECHARGE_DELAY 5000                        //[ms] Time of precharge
#define DISCHARGE_DELAY 15000                       //[ms] Time of discharge
#define K_HALL1         18                          //Low range hall effet sensor gain
#define O_HALL1         2048                        //Low range hall effet sensor offset en mV
#define K_HALL2         210                         //High range hall effet sensor gain
#define O_HALL2         2048                        //High range hall effet sensor offset en mV 


//----------------
// User interface
//----------------

#define USER_INT_SPEED  52         //1: pour simulation, 52: hardware (9600 bps) 

//----------------
// Operating modes
//----------------

#define NORMAL_MODE     0
#define CHARGE_MODE     1
#define STAND_BY_MODE   2
#define ERROR_MODE      3


//----------------------
// BMS error codes masks
//----------------------

#define ERROR_GND_FAULT       0x1
#define ERROR_SLV_COM_TO      0x2
#define ERROR_SLV_COM_DATA    0x4
#define ERROR_OPEN_CONNECT    0x8
#define ERROR_MAX_VOLT        0x10
#define ERROR_MIN_VOLT        0x20
#define ERROR_MAX_TEMP        0x40
#define ERROR_MIN_TEMP        0x80
#define ERROR_MAX_MEAN_CURR   0x100
#define ERROR_MAX_PEAK_CURR   0x200


//------------------
// CAN
//------------------

#define COM_IDLE_COUNT_MAX    10       //Number of com timeout timer cycles without
                                       //communication from a slave before the error flag is set

//------------------
// CAN Error codes
//------------------

#define NO_ERR                0x00
#define ERR_BUFFER_FULL       0x80


//------------------
// CAN Commands
//------------------
#define COMMAND_BAL             0x03
#define COMMAND_BAL_STOP        0x04
#define COMMAND_BAL_DONE        0x05
#define BROADCAST_SLV           0x3F
#define MASTER                  0x4F


#endif //DEFINES_H