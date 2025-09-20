/*
 * step_detection.c
 *
 * Core logic for tracking user steps based on accelerometer magnitude.
 * Uses thresholds to detect discrete step events, with support for test mode and button-based input.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#include "step_detection.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "joystick_task.h"
#include "joystick_math.h"
#include "accelerometer.h"
#include "fsm.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

#define STEP_LENGTH_CM          90  // Distance per step in cm
#define LOWER_THRESHOLD        225000000
#define UPPER_THRESHOLD        305000000

// -----------------------------------------------------------------------------
// State
// -----------------------------------------------------------------------------

static bool step_detected = false;
static uint16_t step_count = 0;

// -----------------------------------------------------------------------------
// Internal Utility Functions
// -----------------------------------------------------------------------------

// Common increment logic used for both test mode and normal steps
void increment_stepcount_common(uint16_t increment_value) {
    if (check_test_mode()) {
        uint16_t goal = get_goal();
        if (step_count < goal) {
            step_count += increment_value;
            if (step_count > goal) {
                step_count = goal;
            }
        }
    } else {
        step_count += increment_value;
    }
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

void increment_stepcount_button(void) {
    increment_stepcount_common(BUTTON_STEP_INCREMENT);
}

void increment_stepcount(void) {
    increment_stepcount_common(1);
}

void set_step_count(uint16_t new_count) {
    step_count = new_count;
}

uint16_t get_distance_yards(void) {
    return (step_count * STEP_LENGTH_CM) / 91.44;
}

uint16_t get_distance_metres(void) {
    return (step_count * STEP_LENGTH_CM) / 100;
}

uint16_t get_steps(void) {
    return step_count;
}

// Main step processing loop, runs periodically
void steps_task_execute(void) {
	static bool initialized = false;
	static uint32_t skip_start = 0;
	if (!initialized) {
	    if (skip_start == 0) skip_start = HAL_GetTick();
	    if (HAL_GetTick() - skip_start < 500) return; // skip first 500ms
	    initialized = true;
	}


    uint16_t* adc_values = joystick_get_values();
    uint16_t adc_x = adc_values[2];
    uint16_t potent = adc_values[0];
    bool test_mode = check_test_mode();

    if (!test_mode && !check_set_goal_state()) {
        // Update display and goal states
        fsm_update(adc_x, test_mode);
        check_for_display_toggle();
        goal_set_mode();

        // Get filtered magnitude from accelerometer
        FilteredAcceleration data = accelerometer_get_latest();
        uint32_t magnitude_square = data.magnitude_square;

        // Step detection logic using hysteresis-style thresholds
        if ((magnitude_square > UPPER_THRESHOLD && !step_detected) ||
            (magnitude_square < LOWER_THRESHOLD && !step_detected)) {
            increment_stepcount();
            step_detected = true;
        } else if (magnitude_square >= LOWER_THRESHOLD && magnitude_square <= UPPER_THRESHOLD) {
            step_detected = false; // Reset step window
        }
    }

    // Goal-setting mode logic (joystick + potentiometer)
    if (check_set_goal_state() && !test_mode) {
        potentiometer_update_stepcount(potent);
        goal_set_mode();
    }
}
