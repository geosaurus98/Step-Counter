/*
 * goal_tracker.h
 *
 * Interface for setting and tracking step goal progress.
 * Includes potentiometer scaling, long/short press handling, and goal enforcement.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#ifndef GOAL_TRACKER_H_
#define GOAL_TRACKER_H_

#include <stdint.h>
#include <stdbool.h>

// Updates goal value based on potentiometer position
void potentiometer_update_stepcount(uint16_t potent);

// Manages entry/exit of goal-setting state based on long/short presses
void goal_set_mode(void);

// Goal state toggles (driven by user input)
void longpress_toggle(void);
void shortpress_toggle(void);

// Gets the current goal value
uint16_t get_goal(void);

// Gets percentage progress toward goal
uint8_t get_goal_progress_percentage(void);

// Returns true if currently in goal-setting state
bool check_set_goal_state(void);

#endif /* GOAL_TRACKER_H_ */
