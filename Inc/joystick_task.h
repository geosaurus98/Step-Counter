/*
 * joystick_task.h
 *
 * Manages ADC sampling and interaction logic for joystick and rotary potentiometer.
 * Detects click-hold input for goal setting and upward movement to toggle display mode.
 *
 * Created on: Mar 13, 2025
 * Author: eaz11 & gjo77
 */

#ifndef JOYSTICK_TASK_H_
#define JOYSTICK_TASK_H_

#include "adc.h"
#include <stdbool.h>
#include <stdint.h>

// Indices into the raw_adc array returned by joystick_get_values()
#define ADC_IDX_POT  0
#define ADC_IDX_Y    1
#define ADC_IDX_X    2

// Initializes ADC peripheral for joystick input
void joystick_init(void);

// Starts DMA sampling and handles click detection (long vs short press)
void joystick_task_execute(void);

// Returns latest raw ADC readings [Pot, Y, X]
uint16_t* joystick_get_values(void);

// Checks if joystick is pushed up far enough to toggle display mode
void check_for_display_toggle(void);

#endif /* JOYSTICK_TASK_H_ */
