/*
 * display_task.h
 *
 * Handles all OLED display rendering based on current system state.
 * Includes display initialization and toggling between unit modes.
 *
 * Created on: Mar 12, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_DISPLAY_TASK_H_
#define INC_DISPLAY_TASK_H_

#include <stdint.h>
#include <stdio.h>

// Initializes the OLED display (called once at startup)
void display_task_init(void);

// Updates the display with appropriate screen content
void display_task_execute(void);

// Toggles between unit display modes (km/yd, %, raw)
void display_toggle(void);

#endif /* INC_DISPLAY_TASK_H_ */
