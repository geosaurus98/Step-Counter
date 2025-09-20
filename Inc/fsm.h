/*
 * fsm.h
 *
 * Finite State Machine for screen navigation using joystick X-axis.
 * Handles screen transitions between steps, distance, and goal progress.
 *
 * Created on: Apr 3, 2025
 * Author: eaz11 & gjo77
 */

#ifndef FSM_H
#define FSM_H

#include <stdint.h>
#include <stdbool.h>

#define COOLDOWN_TICKS 2 // Joystick input cooldown to prevent rapid flipping

// Display states (cyclical)
typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_GOAL_PROGRESS,
    DISPLAY_DISTANCE,
    NUM_DISPLAY_STATES  // Not a real state; used to wrap around
} display_state_t;

// Initializes the FSM to a default state
void fsm_init(void);

// Updates the FSM based on joystick X input (ignored during test or goal-set mode)
void fsm_update(uint16_t adc_x, bool test_mode);

// Returns the current screen state
display_state_t fsm_get_current_state(void);

#endif // FSM_H
