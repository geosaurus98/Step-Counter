/*
 * joystick_math.h
 *
 * Utility functions for interpreting joystick and potentiometer ADC values.
 * Converts raw values to direction and percentage movement.
 *
 * Created on: Mar 20, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_JOYSTICK_MATH_H_
#define INC_JOYSTICK_MATH_H_

#include <stdint.h>

// Joystick and potentiometer ADC calibration
#define X_CENTRE        2185
#define Y_CENTRE        2265
#define X_MIN            190
#define X_MAX           4085
#define Y_MIN            295
#define Y_MAX           4085
#define POTENT_MIN       125
#define POTENT_MAX      4095
#define CENTRE_THRESHOLD 100

// Returns X-axis movement percentage (0–100%)
uint8_t calculate_x_percentage(uint16_t adc_value);

// Returns Y-axis movement percentage (0–100%)
uint8_t calculate_y_percentage(uint16_t adc_value);

// Returns potentiometer percentage (0–100%) from raw ADC
uint8_t calculate_potentiometer_percentage(uint16_t adc_value);

// Returns direction based on X-axis value ("Left", "Right", "Rest")
const char* get_x_direction(uint16_t adc_x);

// Returns direction based on Y-axis value ("Up", "Down", "Rest")
const char* get_y_direction(uint16_t adc_y);

#endif /* INC_JOYSTICK_MATH_H_ */
