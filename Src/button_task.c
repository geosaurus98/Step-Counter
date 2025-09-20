/*
 * button_task.c
 *
 * Handles all button input and associated actions:
 * - UP button increments step count (unless setting goal)
 * - DOWN button toggles serial on single press, test mode on double press
 *
 * Created on: Mar 12, 2025
 * Author: eaz11 & gjo77
 */

#include "button_task.h"
#include "serial.h"
#include "buttons.h"
#include "rgb.h"
#include "stm32c0xx_hal.h"
#include "app.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "step_detection.h"

static DoublePressTracker downTracker = {0};

// Internal helper functions
static bool check_double_press(DoublePressTracker *tracker);
static void handle_up_button(void);
static void handle_down_button(void);
static void handle_right_button(void);
static void handle_left_button(void);

// Returns true if button was double-pressed within threshold time
static bool check_double_press(DoublePressTracker *tracker)
{
    uint32_t currentTime = HAL_GetTick();

    if (currentTime - tracker->lastPressTime < DOUBLE_PRESS_THRESHOLD_MS) {
        tracker->pressCount++;
        tracker->lastPressTime = currentTime;

        if (tracker->pressCount == 2) {
            tracker->pressCount = 0;
            return true;
        }
    } else {
        tracker->pressCount = 1;
        tracker->lastPressTime = currentTime;
    }

    return false;
}

// Handles UP button logic (step increment)
static void handle_up_button(void)
{
    if (buttons_checkButton(UP) == PUSHED && !check_set_goal_state()) {
        increment_stepcount_button();
    }
}

// Handles DOWN button logic (serial toggle + test mode double press)
static void handle_down_button(void)
{
    if (buttons_checkButton(DOWN) == PUSHED && !check_set_goal_state()) {
        if (check_double_press(&downTracker)) {
            test_mode_toggle();
        }
        serial_toggle();
    }
}

// Placeholder for RIGHT button
static void handle_right_button(void)
{
    if (buttons_checkButton(RIGHT) == PUSHED) {
        // Future functionality
    }
}

// Placeholder for LEFT button
static void handle_left_button(void)
{
    if (buttons_checkButton(LEFT) == PUSHED) {
        // Future functionality
    }
}

// Executes all button logic each cycle
void button_task_execute(void)
{
    handle_up_button();
    handle_down_button();
    handle_right_button();
    handle_left_button();
}
