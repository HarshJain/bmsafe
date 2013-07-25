#ifndef RELAYS_H
#define RELAYS_H

//Nom des relais
#define MCR1_HVN        PORTA_PA0
#define MCR2_P2P        PORTA_PA1
#define MCR3_P1N        PORTA_PA3
#define MCR4_HVP        PORTA_PA4
#define MCR5_PRE        PORTA_PA5
#define DISCH_RLY       PORTA_PA2

//États des relais
#define RLY_OPEN    0
#define RLY_CLOSED  1
#define DISCH_RLY_OPEN    1
#define DISCH_RLY_CLOSED  0

//Temps
#define RELAY_DELAY         100            //[ms] Time between the opening or closing of 2 relays
#define PRECHARGE_DELAY     8000           //[ms] Time of precharge
#define DISCHARGE_DELAY     15000          //[ms] Time of discharge. Le circuit de décharge ne devrait pas être actif

#define START_SEQUENCE        0
#define CONTINUE_SEQUENCE     1

//Déclaration de fonctions
void OpenRelays(void);
void CloseRelays(unsigned char option);
void delayRelayMs(unsigned int numMs);
     
//Variables
extern unsigned int gTimerLimit;
extern unsigned int gClosingRelays;
     
#endif RELAYS_H
