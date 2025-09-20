/*
 * joystick_task.c
 *
 * Handles joystick ADC readings and button press detection.
 * Detects click duration for entering/exiting goal-setting mode.
 * Detects upward motion to toggle between unit display modes.
 *
 * Created on: Mar 13, 2025
 * Author: eaz11 & gjo77
 */

#include "joystick_task.h"
#include "joystick_math.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "step_detection.h"
#include "fsm.h"
#include "display_task.h"

#include <string.h>

#define HOLD_TIME_MS              1000  // Time required for a long press
#define DISPLAY_TOGGLE_THRESHOLD    90  // Minimum upward joystick % to allow toggle

static uint16_t raw_adc[3];  // Format: [Y, X, Potentiometer]

// Returns the current raw ADC values (from DMA): [Y, X, Pot]
uint16_t* joystick_get_values(void) {
    return raw_adc;
}

// Initializes ADC hardware (ADC1 + DMA already configured in STM32Cube)
void joystick_init(void) {
    HAL_ADC_Init(&hadc1);
}

// Starts ADC sampling and processes joystick button click for goal setting mode
void joystick_task_execute(void) {
    static uint32_t press_start_time = 0;
    static bool click_in_progress = false;
    static bool longpress_detected = false;

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)raw_adc, 3);

    if (!check_test_mode() && fsm_get_current_state() == DISPLAY_GOAL_PROGRESS) {
        if (HAL_GPIO_ReadPin(JOYSTICK_CLICK_GPIO_Port, JOYSTICK_CLICK_Pin) == GPIO_PIN_SET) {
            if (!click_in_progress) {
                press_start_time = HAL_GetTick();
                click_in_progress = true;
                longpress_detected = false;
            } else if (!longpress_detected && (HAL_GetTick() - press_start_time >= HOLD_TIME_MS)) {
                longpress_toggle();
                longpress_detected = true;
            }
        } else {
            if (click_in_progress && !longpress_detected) {
                shortpress_toggle();
            }
            press_start_time = 0;
            click_in_progress = false;
            longpress_detected = false;
        }
    }
}

// Detects "Up" joystick movement and toggles display units if movement is significant
void check_for_display_toggle(void) {
    static bool toggle_locked = false;

    uint16_t* adc_values = joystick_get_values();
    uint16_t adc_y = adc_values[1];  // Y is at index 1

    uint8_t y_percent = calculate_y_percentage(adc_y);
    const char* direction = get_y_direction(adc_y);

    if (y_percent >= DISPLAY_TOGGLE_THRESHOLD && strcmp(direction, "Up") == 0) {
        if (!toggle_locked) {
            display_toggle();
            toggle_locked = true;
        }
    } else {
        toggle_locked = false; // Reset lock when joystick returns to neutral
    }
}
