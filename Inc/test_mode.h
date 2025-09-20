/*
 * test_mode.h
 *
 * Provides joystick-controlled test mode for manually simulating step count changes.
 * Allows increasing or decreasing step count based on joystick Y-direction and displacement.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#ifndef TEST_MODE_H
#define TEST_MODE_H

#include <stdint.h>
#include <stdbool.h>

// Executes test mode logic (called periodically)
void test_mode_execute(void);

// Toggles test mode on/off
void test_mode_toggle(void);

// Applies step change based on joystick Y input (Up = +, Down = -)
void test_mode_update_stepcount(uint16_t adc_y);

// Returns whether test mode is currently active
bool check_test_mode(void);

#endif // TEST_MODE_H
