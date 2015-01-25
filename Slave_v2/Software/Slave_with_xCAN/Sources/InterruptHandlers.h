
#ifndef INT_HANDLER_H
#define INT_HANDLER_H

#include "Defines.h"

extern uint32 timeRef_lastMasterMessage;


void HandleTimer_10Hz(void);
void HandleTimer_1kHz(void);

void UpdateTimeDependentFlags(void);



#endif