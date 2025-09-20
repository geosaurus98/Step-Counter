/*
 * app.c
 *
 * Main application scheduler for step counter firmware.
 * Handles task initialization and periodic execution.
 *
 * Created on: Mar 13, 2025
 * Author: eaz11 & gjo77
 */

#include "app.h"
#include "rgb.h"
#include "buttons.h"
#include "button_task.h"
#include "display_task.h"
#include "joystick_task.h"
#include "serial.h"
#include "led.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "step_detection.h"
#include "buzzer.h"
#include "accelerometer.h"
#include "tim.h"
#include "fsm.h"

// Stores next execution time for each task
static uint32_t taskButtonNextRun = 0;
static uint32_t taskDisplayNextRun = 0;
static uint32_t taskJoystickNextRun = 0;
static uint32_t taskSerialNextRun = 0;
static uint32_t taskStepNextRun = 0;
static uint32_t taskTestNextRun = 0;
static uint32_t taskBuzzerNextRun = 0;
static uint32_t taskAccelerometerNextRun = 0;
static uint32_t taskLEDNextRun = 0;

void app_main(void)
{
    // Set next run times relative to current tick
    uint32_t now = HAL_GetTick();
    taskButtonNextRun        = now + TASK_BUTTON_PERIOD_TICKS;
    taskDisplayNextRun       = now + TASK_DISPLAY_PERIOD_TICKS;
    taskJoystickNextRun      = now + TASK_JOYSTICK_PERIOD_TICKS;
    taskSerialNextRun        = now + TASK_SERIAL_PERIOD_TICKS;
    taskStepNextRun          = now + TASK_STEP_PERIOD_TICKS;
    taskTestNextRun          = now + TASK_TEST_PERIOD_TICKS;
    taskBuzzerNextRun        = now + TASK_BUZZER_PERIOD_TICKS;
    taskAccelerometerNextRun = now + TASK_ACCELEROMETER_PERIOD_TICKS;
    taskLEDNextRun           = now + TASK_LED_PERIOD_TICKS;

    // Initialize all system modules
    buttons_init();
    display_task_init();
    rgb_colour_all_on();
    joystick_init();
    LED_init();
    accelerometer_init();
    fsm_init();

    while (1)
    {
        buttons_update(); // Must be called frequently to detect button events
        uint32_t ticks = HAL_GetTick();

        if (ticks > taskButtonNextRun) {
            button_task_execute();
            taskButtonNextRun += TASK_BUTTON_PERIOD_TICKS;
        }
        if (ticks > taskDisplayNextRun) {
            display_task_execute();
            taskDisplayNextRun += TASK_DISPLAY_PERIOD_TICKS;
        }
        if (ticks > taskJoystickNextRun) {
            joystick_task_execute();
            taskJoystickNextRun += TASK_JOYSTICK_PERIOD_TICKS;
        }
        if (ticks > taskSerialNextRun) {
            serial_task_execute();
            taskSerialNextRun += TASK_SERIAL_PERIOD_TICKS;
        }
        if (ticks > taskStepNextRun) {
            steps_task_execute();
            taskStepNextRun += TASK_STEP_PERIOD_TICKS;
        }
        if (ticks > taskTestNextRun) {
            test_mode_execute();
            taskTestNextRun += TASK_TEST_PERIOD_TICKS;
        }
        if (ticks > taskBuzzerNextRun) {
            buzzer_execute();
            taskBuzzerNextRun += TASK_BUZZER_PERIOD_TICKS;
        }
        if (ticks > taskAccelerometerNextRun) {
            accelerometer_execute();
            taskAccelerometerNextRun += TASK_ACCELEROMETER_PERIOD_TICKS;
        }
        if (ticks > taskLEDNextRun) {
            LED_execute();
            taskLEDNextRun += TASK_LED_PERIOD_TICKS;
        }
    }
}
