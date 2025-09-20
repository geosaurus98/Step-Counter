/*
 * joystick_math.c
 *
 * Implements percentage scaling and direction detection for joystick inputs.
 *
 * Created on: Mar 20, 2025
 * Author: eaz11 & gjo77
 */

#include "joystick_math.h"

// Maps X-axis ADC value to movement percentage
uint8_t calculate_x_percentage(uint16_t adc_value)
{
    uint8_t percentage;

    if (adc_value > X_CENTRE) {
        percentage = (uint8_t)(((adc_value - X_CENTRE) * 100) / (X_MAX - X_CENTRE));  // Right
    } else {
        percentage = (uint8_t)(((X_CENTRE - adc_value) * 100) / (X_CENTRE - X_MIN));  // Left
    }

    // Clamp small jitters and extreme values
    if (percentage < 15) return 0;
    if (percentage > 50) return 100;

    return percentage;
}

// Maps Y-axis ADC value to movement percentage
uint8_t calculate_y_percentage(uint16_t adc_value)
{
    uint8_t percentage;

    if (adc_value > Y_CENTRE) {
        percentage = (uint8_t)(((adc_value - Y_CENTRE) * 100) / (Y_MAX - Y_CENTRE));  // Down
    } else {
        percentage = (uint8_t)(((Y_CENTRE - adc_value) * 100) / (Y_CENTRE - Y_MIN));  // Up
    }

    // Clamp small jitters and extreme values
    if (percentage < 10) return 0;
    if (percentage > 90) return 100;

    return percentage;
}

// Maps potentiometer ADC value to 0–100% range
uint8_t calculate_potentiometer_percentage(uint16_t adc_value)
{
    if (adc_value <= POTENT_MIN) return 0;
    if (adc_value >= POTENT_MAX) return 100;

    return (uint8_t)(((uint32_t)(adc_value - POTENT_MIN) * 100) / (POTENT_MAX - POTENT_MIN));
}

// Returns X-axis movement direction
const char* get_x_direction(uint16_t adc_x)
{
    if (adc_x < X_CENTRE - CENTRE_THRESHOLD) {
        return "Right";
    } else if (adc_x > X_CENTRE + CENTRE_THRESHOLD) {
        return "Left";
    }
    return "Rest";
}

// Returns Y-axis movement direction
const char* get_y_direction(uint16_t adc_y)
{
    if (adc_y < Y_CENTRE - CENTRE_THRESHOLD) {
        return "Up";
    } else if (adc_y > Y_CENTRE + CENTRE_THRESHOLD) {
        return "Down";
    }
    return "Rest";
}
