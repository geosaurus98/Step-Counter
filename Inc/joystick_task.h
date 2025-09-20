/*
 * joystick_task.h
 *
 * Manages ADC sampling and interaction logic for joystick and rotary potentiometer.
 * Detects click-hold input for goal setting and upward movement to toggle display mode.
 *
 * Created on: Mar 13, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_JOYSTICK_TASK_H_
#define INC_JOYSTICK_TASK_H_

#include "adc.h"
#include <stdbool.h>
#include <stdint.h>

// Initializes ADC peripheral for joystick input
void joystick_init(void);

// Starts DMA sampling and handles click detection (long vs short press)
void joystick_task_execute(void);

// Returns latest raw ADC readings [Y, X, Potentiometer]
uint16_t* joystick_get_values(void);

// Toggles between goal-setting and normal mode (optional external control)
void joystick_toggle(void);

// Checks if joystick is pushed up far enough to toggle display mode
void check_for_display_toggle(void);

#endif /* INC_JOYSTICK_TASK_H_ */
