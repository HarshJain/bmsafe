#ifndef TIMERS_H
#define TIMERS_H


void initMicroTimers(unsigned char mt0, unsigned char mt1);
void initCanTxTimer(unsigned int pit0);
void initADCtimer(unsigned int pit1);


#endif