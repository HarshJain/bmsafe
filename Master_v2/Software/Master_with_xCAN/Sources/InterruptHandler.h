/*
  Middle man between interruption thrown by a specific mcu and the generic code
*/

#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H

void HandleSafetyCheck_1kHz(void);	// 1 kHz timer for safety check

void HandleTimer_100Hz(void);	// 100 Hz timer for different purpose
void HandleTimer_1kHz(void);	// 1 kHz timer for different purpose

#endif INTERRUPTHANDLER_H