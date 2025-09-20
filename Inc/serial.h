/*
 * serial.h
 *
 * Provides UART-based debug output for joystick and acceleration data.
 * Can be toggled on/off at runtime.
 *
 * Created on: Mar 19, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include <stdint.h>
#include <stdbool.h>

// Toggles serial debug output on/off
void serial_toggle(void);

// If enabled, sends joystick and acceleration data via UART
void serial_task_execute(void);

#endif /* INC_SERIAL_H_ */
