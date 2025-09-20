/*
 * display_task.c
 *
 * Manages OLED content rendering based on UI state.
 * Handles test mode, goal setting, and main display states.
 *
 * Created on: Mar 12, 2025
 * Author: eaz11 & gjo77
 */

#include "display_task.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "joystick_task.h"
#include "joystick_math.h"
#include "fsm.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "step_detection.h"
#include <string.h>

// --- Local Prototypes ---
static void display_draw_test_mode(void);
static void display_draw_set_goal(void);
static void display_draw_main_screen(void);
static uint8_t calculate_percent(uint16_t value, uint16_t goal);
static void format_steps(char *buf, size_t size);
static void format_distance(char *buf, size_t size);
static void format_progress(char *buf, size_t size);

// Display mode toggle flag (true = percentage/km, false = raw/yd)
bool display_mode;

// --- Public Functions ---

void display_task_init(void) {
    ssd1306_Init();
    display_mode = false;
}

void display_task_execute(void) {
    ssd1306_Fill(Black);

    if (check_test_mode())
        display_draw_test_mode();
    else if (check_set_goal_state())
        display_draw_set_goal();
    else
        display_draw_main_screen();

    ssd1306_UpdateScreen();
}

void display_toggle(void) {
    display_mode = !display_mode;
}

// --- Private Drawing Functions ---

static void display_draw_test_mode(void) {
    char buf[20];

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("=== TEST MODE ===", Font_7x10, White);

    snprintf(buf, sizeof(buf), "Steps RN: %u", get_steps());
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString(buf, Font_7x10, White);

    snprintf(buf, sizeof(buf), "Goal:  %u", get_goal());
    ssd1306_SetCursor(0, 24);
    ssd1306_WriteString(buf, Font_7x10, White);

    ssd1306_SetCursor(0, 40);
    ssd1306_WriteString("Use joystick to test", Font_6x8, White);
}

static void display_draw_set_goal(void) {
    char buf[20];

    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Set Step Goal:", Font_7x10, White);

    snprintf(buf, sizeof(buf), "%u/%u", get_steps(), get_goal());
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString(buf, Font_11x18, White);
}

static void display_draw_main_screen(void) {
    char buf[20];
    display_state_t current_display_state = fsm_get_current_state();

    switch (current_display_state) {
        case DISPLAY_STEPS:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Steps:", Font_7x10, White);
            format_steps(buf, sizeof(buf));
            break;

        case DISPLAY_DISTANCE:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Distance:", Font_7x10, White);
            format_distance(buf, sizeof(buf));
            break;

        case DISPLAY_GOAL_PROGRESS:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Goal Progress:", Font_7x10, White);
            format_progress(buf, sizeof(buf));

            if (!display_mode) {
                char steps_buf[20];
                snprintf(steps_buf, sizeof(steps_buf), "Steps");
                ssd1306_SetCursor(0, 36);
                ssd1306_WriteString(steps_buf, Font_11x18, White);
            }
            break;

        default:
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Invalid state", Font_6x8, White);
            break;
    }

    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString(buf, Font_11x18, White);
}

// --- Private Formatting Functions ---

static uint8_t calculate_percent(uint16_t value, uint16_t goal) {
    return (goal == 0) ? 0 : (value * 100) / goal;
}

static void format_steps(char *buf, size_t size) {
    if (display_mode)
        snprintf(buf, size, "%u%%", calculate_percent(get_steps(), get_goal()));
    else
        snprintf(buf, size, "%u steps", get_steps());
}

static void format_distance(char *buf, size_t size) {
    if (display_mode)
        snprintf(buf, size, "%u yd", get_distance_yards());
    else {
        uint16_t metres = get_distance_metres();
        snprintf(buf, size, "%u.%03u km", metres / 1000, metres % 1000);
    }
}

static void format_progress(char *buf, size_t size) {
    if (display_mode)
        snprintf(buf, size, "%u%%", calculate_percent(get_steps(), get_goal()));
    else
        snprintf(buf, size, "%u/%u", get_steps(), get_goal());
}
