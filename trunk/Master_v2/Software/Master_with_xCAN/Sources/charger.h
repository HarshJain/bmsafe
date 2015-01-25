
#ifndef CHARGER_H
#define CHARGER_H

#include "defines.h"

#define MAINS_CURRENT_MAXIMUM	15	// [A] Maximum mains current (input current)
#define MAXIMUM_OUTPUT_CURRENT	20	// [A] Maximum output current (fed to the battery)
#define MAXIMUM_OUTPUT_VOLTAGE	300	// [V] Maximum output voltage


#define CHARGER_SEND_MSG_PERIOD_MS	100 	// Period for sending messages when in charge mode (must be less than 300 ms)

// Charging step 1 : Constant current charging (CC) until a cell reaches a certain voltage
#define CHARGE_CURRENT_STEP1	10			// [A]
#define STOP_VOLTAGE_STEP1		4000		// [mV] Cell voltage to reach before going to next step

// Charging step 2 : Constant voltage charging (CV) until the charging current reaches a certain value
#define CHARGE_VOLTAGE_STEP2 	292			// [V] Total pack voltage charging
#define STOP_CURRENT_STEP2		2000		// [mA] Current to reach before goint to next step

// CAN data structure for control command of the NLG5 charger
typedef struct {
	// First byte
	uint8 can_enable	:1;		// CAN enable (1 = CAN enable)
	uint8 clear_error	:1;		// Clear error latch commnad (1 = clear error latch)
	uint8 ventilation	:1;		// Control pilot ventilation request (0 = No Ventilation, 1 = Ventilation)
	uint8				:5;		// Padding

	// Following bytes
	uint16 mains_max_current;
	uint16 output_voltage;		// Output voltage command
	uint16 output_current;		// Output current command
} ChargingParameters;

typedef enum{
	NOT_CHARGING = 0,
	STEP1 = 1,			// Current charging
	STEP2 = 2,			// Voltage charging
	STOP_CHARGING,		// Charging is done, we stop charging
	CHARGING_DONE		// Charging is done, we can open the relays
} Charging_State;

// NLG5 Charger's status bits
typedef struct{
	// Byte 1
	unsigned power_enable				: 1;
	unsigned error_latch				: 1;
	unsigned limit_warning				: 1;
	unsigned fan_active					: 1;
	unsigned europe_mains				: 1;
	unsigned usa_mains_level_1			: 1;
	unsigned usa_mains_level_2			: 1;
	unsigned control_pilot_detected		: 1;
	
	// Byte 2
	unsigned bypass_detection_1					: 1;
	unsigned bypass_detection_2					: 1;
	unsigned limitation_by_output_voltage		: 1;
	unsigned limitation_by_output_current		: 1;
	unsigned limitation_by_mains_current		: 1;
	unsigned limitation_by_power_indicator		: 1;
	unsigned limitation_by_control_pilot		: 1;
	unsigned limitation_by_NLG5_max_power		: 1;
	
	// Byte 3
	unsigned limitation_by_NLG5_max_mains_current			: 1;
	unsigned limitation_by_NLG5_max_output_current			: 1;
	unsigned limitation_by_NLG5_max_output_voltage			: 1;
	unsigned limitation_by_temperature_capacitors_prim		: 1;
	unsigned limitation_by_temperature_power_stage			: 1;
	unsigned limitation_by_temperature_diodes				: 1;
	unsigned limitation_by_temperature_transformer			: 1;
	unsigned limitation_by_battery_temperature				: 1;
} Charger_Status;

// NLG5 Charger's actual 1 values
typedef struct{
	uint16 mains_current;		// Charger mains (intput) current (0-50 A) (resolution : 0.01 A)
	uint16 mains_voltage;		// Charger mains (intput) voltage (0-500 V) (res : 0.1 V)
	uint16 output_voltage;		// Charger output voltage (0-1000 V) (res : 0.1 V)
	uint16 output_current;		// Charger output current (0-150 Amps) (res: 0.01 A)
} Charger_Actual_1;

// NLG5 Charger's errors
typedef struct{	
	unsigned output_overvoltage : 1;
	unsigned mains_overvoltage_2 : 1;
	unsigned mains_overvoltage_1 : 1;
	unsigned power_stage_short_circuit_condition : 1;
	unsigned plausibility_output_voltage_measurement : 1;
	unsigned plausibility_mains_voltage_measurement : 1;
	unsigned output_fuse_defect : 1;
	unsigned mains_fuse_defect : 1;
	unsigned battery_polarity : 1;
	unsigned temp_sensor_capacitors_prim : 1;
	unsigned temp_sensor_power_stage_prim : 1;
	unsigned temp_sensor_diodes : 1;
	unsigned temp_sensor_transformer : 1;
	unsigned temp_sensor_extern_1 : 1;
	unsigned temp_sensor_extern_2 : 1;
	unsigned temp_sensor_extern_3 : 1;
	unsigned flash_check_sum_failure : 1;
	unsigned nvsram_check_sum_failure : 1;
	unsigned eeprom_sys_check_sum_failure : 1;
	unsigned eeprom_pow_check_sum_failure : 1;
	unsigned watchdog_internal : 1;
	unsigned initialization : 1;
	unsigned can_timeout : 1;
	unsigned can_off : 1;
	unsigned can_transmit : 1;
	unsigned can_receive : 1;
	unsigned shutdown_threshold_battery_temperature : 1;
	unsigned shutdown_threshold_battery_voltage : 1;
	unsigned shutdown_threshold_battery_ampere_hours : 1;
	unsigned shutdown_threshold_charging_time : 1;
	unsigned : 2;	// Padding
} Charger_Errors;

// NLG5 Charger's warnings
typedef struct{
	unsigned power_limitation_by_low_mains_voltage : 1;
	unsigned power_limitation_by_low_battery_voltage : 1;
	unsigned power_limitation_by_internal_overtemperature : 1;
	unsigned command_value_out_of_range : 1;
	unsigned : 4; // Padding
} Charger_Warnings;

// Structure containing substructures containing charger's parameters and values
typedef struct{
	Charger_Status		status;		// Charger status
	Charger_Errors 		errors;		// Charger errors
	Charger_Warnings 	warnings;	// Charger warnings
	Charger_Actual_1 	actual_1;	// Charger actual values 1
} Charger_State; // Do not mix up with Charging_State

extern ChargingParameters gChargingParameters;	// Contains the control parameters for the current charging session

extern Charging_State gCharging_state;			// Charging state (do not mix with charger_state)
extern Charger_State NLG5_state;			// NLG5 Charger state (do not mix with gCharging_state)

void charger_enableVentilation(uint8 enable);	// Enable or disable ventilation
void charger_setOutputParameters(uint16 output_voltage, uint16 output_current);	// set output voltage and current
void charger_sendControlParameters(ChargingParameters* charging_parameters);	// send control message to the charger by CAN
void charger_clearErrorLatch(void);	// Clear charger error latch

void manage_charging(void);
void AssertSafeCharging(void);

void charger_updateStatus(uint8 *data, uint8 length);
void charger_updateErrors(uint8 *data, uint8 length);
void charger_updateAcutals(uint8 *data, uint8 length);

#endif // CHARGER_H