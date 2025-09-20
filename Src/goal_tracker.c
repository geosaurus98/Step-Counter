/*
 * goal_tracker.c
 *
 * Controls step goal value, entry/exit from goal setting mode,
 * and calculates progress percentage. Interfaces with potentiometer,
 * FSM, step count, and buzzer logic.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#include "goal_tracker.h"
#include "step_detection.h"
#include "joystick_task.h"
#include "joystick_math.h"
#include "test_mode.h"
#include "fsm.h"
#include "buzzer.h"
#include "ssd1306.h"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

#define MAX_GOAL_VALUE        15000
#define MIN_GOAL_VALUE         500

// -----------------------------------------------------------------------------
// State Variables
// -----------------------------------------------------------------------------

static bool set_goal_state = false;
static uint16_t goal = 1000;
static uint16_t prev_goal = 1000;

static bool longpress = false;
static bool shortpress = false;

// -----------------------------------------------------------------------------
// Public Functions
// -----------------------------------------------------------------------------

// Scales potentiometer value to a valid goal range
void potentiometer_update_stepcount(uint16_t potent) {
    uint8_t percent = calculate_potentiometer_percentage(potent);
    uint16_t new_goal = percent * MAX_GOAL_VALUE / 100;
    goal = (new_goal < MIN_GOAL_VALUE) ? MIN_GOAL_VALUE : new_goal;
}

// Handles goal setting mode entry/exit logic based on long/short press
void goal_set_mode(void)
{
    if (set_goal_state)
    {
        if (longpress) {
            steps_exit_goal_setting();
            longpress_toggle();
        } else if (shortpress) {
            goal = prev_goal;
            steps_exit_goal_setting();
            shortpress_toggle();
        }
    }
    else if (longpress)
    {
        steps_enter_goal_setting();
        longpress_toggle();
    }

    shortpress = false;
}

// Called when entering goal setting mode; saves current goal
void steps_enter_goal_setting(void)
{
    prev_goal = goal;
    set_goal_mode_toggle();
}

// Called when exiting goal setting mode; enforces that step count ≤ goal
void steps_exit_goal_setting(void)
{
    set_goal_mode_toggle();
    if (get_steps() > goal) {
        set_step_count(goal);
    }
}

// Returns current goal value
uint16_t get_goal(void) {
    return goal;
}

// Returns goal progress as a percentage (0–100)
uint8_t get_goal_progress_percentage(void) {
    if (goal == 0) return 0;
    if (get_steps() >= goal) return 100;
    return (get_steps() * 100) / goal;
}

// Toggles goal setting mode flag
void set_goal_mode_toggle(void) {
    set_goal_state = !set_goal_state;
}

// Toggles long press state
void longpress_toggle(void) {
    longpress = !longpress;
}

// Toggles short press state
void shortpress_toggle(void) {
    shortpress = !shortpress;
}

// Returns true if currently in goal setting mode
bool check_set_goal_state(void) {
    return set_goal_state;
}
