/*
  Contain function that assert the safety / good working of different component of the vehicle. 
  If an error or a dangerous condition is detected an error flag is raised 
*/

#ifndef ASSERT_H
#define ASSERT_H



void AssertSafeVoltage(void);
void AssertSafeTemperature(void);
void AssertCommunicationWithSlave(void);
void AssertSafeCurrent(void);

#endif