#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H



void manage_voltages(void);
void manage_temperatures(void);
void manage_LED(void);
void manage_CAN(void);
void manage_equilibration(void);

void updateInputs(void);

void checkForBalancedCells(uint16*, uint16[], uint16, uint16);
void turnOffLEDs(void);

void turn_ON_ADC(void);
void turn_OFF_ADC(void);
void start_ADC_conversion(void);

#endif // MANAGER_H
