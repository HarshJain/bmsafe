
/*! \file Display.h
 * 	Contain necessary function to share state or error information to an user: 7 segment display and dashboard LED control.
 */


#ifndef DISPLAY_H
#define DISPLAY_H

#include "defines.h"

/*!	\brief	Display a symbol on the 7 segment LED display
 *
 *	The parameter to be display can be a number from 0 to 9 or some letters in char type
 *	(ex : 'C', 'E','F','P', ...). See the implementation of the function to see all possible
 *	values of \a no .
 *
 * 	\param[in] no Symbol to display.
 */
void display(int16 no);

/*!	\brief	Updates the 7 segment LED display.
 *
 *	Updates the 7 segment LED display. The symbol displayed depends uppon the errors, warnings and
 *  state of the system. This function is called periodically.
 */

void UpdateErrorDisplay(void);

/*!	\brief	Updates the 7 segment LED display.
 *
 *	Updates the dashboard BMS LED display.
 *  This function is called periodically.
 */
void UpdateDashLed(void);

#endif