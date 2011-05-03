#ifndef TIMERS_H
#define TIMERS_H


void initMicroTimers(unsigned char mt0, unsigned char mt1);
void initADCtimer(unsigned int pit2);
void initComTimer(unsigned int pit1);
void initDelayTimer(unsigned int pit0);


#endif