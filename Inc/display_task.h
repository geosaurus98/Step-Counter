/*
 * display_task.h
 *
 * Handles all OLED display rendering based on current system state.
 * Includes display initialization and toggling between unit modes.
 *
 * Created on: Mar 12, 2025
 * Author: eaz11 & gjo77
 */

#ifndef DISPLAY_TASK_H_
#define DISPLAY_TASK_H_

// Initializes the OLED display (called once at startup)
void display_task_init(void);

// Updates the display with appropriate screen content
void display_task_execute(void);

// Toggles between display unit modes (counts/km/steps-per-goal vs. %/yards/%)
void display_toggle(void);

#endif /* DISPLAY_TASK_H_ */
