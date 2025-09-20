/*
 * test_mode.c
 *
 * Allows joystick-based step manipulation for testing purposes.
 * Adjusts step count up/down in proportion to joystick Y-displacement.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#include "test_mode.h"
#include "step_detection.h"
#include "goal_tracker.h"
#include "joystick_task.h"
#include "joystick_math.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

#define MAX_STEP_CHANGE_PER_TICK 15  // Maximum step change as % of goal
#define HOLD_TIME_MS             1000

static bool test_mode = false;

// -----------------------------------------------------------------------------
// Test Mode Logic
// -----------------------------------------------------------------------------

// Updates the step count based on joystick Y-axis position
void test_mode_update_stepcount(uint16_t adc_y) {
    uint8_t percent = calculate_y_percentage(adc_y);
    const char* direction = get_y_direction(adc_y);

    uint16_t goal = get_goal();
    uint16_t current = get_steps();

    if (percent >= 30 && direction) {
        // Scale step change to joystick force and goal size
        uint16_t max_step_change = (goal * MAX_STEP_CHANGE_PER_TICK) / 1000;
        uint8_t step_delta = (percent * max_step_change) / 100;

        if (strcmp(direction, "Up") == 0) {
            uint16_t new_count = (current + step_delta > goal) ? goal : current + step_delta;
            set_step_count(new_count);
        } else if (strcmp(direction, "Down") == 0) {
            uint16_t new_count = (step_delta > current) ? 0 : current - step_delta;
            set_step_count(new_count);
        }
    }
}

// Called every tick to process test mode behavior
void test_mode_execute(void) {
    if (test_mode && !check_set_goal_state()) {
        uint16_t* adc_values = joystick_get_values();
        uint16_t adc_y = adc_values[1];
        test_mode_update_stepcount(adc_y);
    }
}

// Toggles test mode on/off
void test_mode_toggle(void) {
    test_mode = !test_mode;
}

// Returns test mode status
bool check_test_mode(void) {
    return test_mode;
}
