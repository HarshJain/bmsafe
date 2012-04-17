#ifndef RELAYS_H
#define RELAYS_H

 void OpenRelays(unsigned int delay);
 void CloseRelays(unsigned int delay);
 void SingleCloseRelays(int relay);
 void Precharge(unsigned int time);
 void PeriphInit(void);
 void DontDischarge(void);
 void Discharge(unsigned int time);

     
#endif RELAYS_H
