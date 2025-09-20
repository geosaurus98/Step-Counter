/*
 * step_detection.h
 *
 * Provides core logic for counting steps based on accelerometer data.
 * Also calculates travelled distance in metres/yards and manages step updates.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#ifndef STEP_DETECTION_H_
#define STEP_DETECTION_H_

#include <stdint.h>
#include <stdbool.h>

// Step update limits
#define BUTTON_STEP_INCREMENT    10
#define MAX_STEP_CHANGE_PER_TICK 15

// Thresholds for goal setting and input hold detection
#define MAX_GOAL_VALUE           15000
#define MIN_GOAL_VALUE            500
#define HOLD_TIME_MS             1000

// Called each loop cycle to process acceleration data and update step count
void steps_task_execute(void);

// Increments step count by 1 (used in motion-based detection)
void increment_stepcount(void);

// Increments step count by BUTTON_STEP_INCREMENT (used in SW1 press)
void increment_stepcount_button(void);

// Overrides step count with a specific value (used in goal logic)
void set_step_count(uint16_t new_count);

// Gets distance in yards (based on step count)
uint16_t get_distance_yards(void);

// Gets distance in metres (based on step count)
uint16_t get_distance_metres(void);

// Returns the current step count
uint16_t get_steps(void);

#endif /* STEP_DETECTION_H_ */
