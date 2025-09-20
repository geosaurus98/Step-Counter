/*
 * fsm.c
 *
 * Screen state management for UI navigation.
 * Changes display screen in response to joystick left/right input.
 *
 * Created on: Apr 3, 2025
 * Author: eaz11 & gjo77
 */

#include "fsm.h"
#include "joystick_math.h"
#include "joystick_task.h"
#include "goal_tracker.h"
#include <string.h> // For strcmp

// Current state of the display screen
static display_state_t current_display_state;

void fsm_init(void) {
    current_display_state = DISPLAY_STEPS;
}

void fsm_update(uint16_t adc_x, bool test_mode) {
    static uint8_t cooldown_timer = 0;

    if (test_mode || check_set_goal_state())
        return;

    if (cooldown_timer > 0) {
        cooldown_timer--;
        return;
    }

    uint8_t x_percent = calculate_x_percentage(adc_x);
    const char* direction = get_x_direction(adc_x);

    if (x_percent >= 75) {
        if (strcmp(direction, "Right") == 0) {
            current_display_state = (current_display_state + 1) % NUM_DISPLAY_STATES;
            cooldown_timer = COOLDOWN_TICKS;
        } else if (strcmp(direction, "Left") == 0) {
            if (current_display_state == 0) {
                current_display_state = NUM_DISPLAY_STATES - 1;
            } else {
                current_display_state--;
            }
            cooldown_timer = COOLDOWN_TICKS;
        }
    }
}

display_state_t fsm_get_current_state(void) {
    return current_display_state;
}
