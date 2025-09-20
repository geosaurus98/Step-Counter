/*
 * button_task.h
 *
 * Public interface for button input handling logic.
 * Includes support for double-press tracking.
 *
 * Created on: Mar 12, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_BUTTON_TASK_H_
#define INC_BUTTON_TASK_H_

#include <stdint.h>
#include <stdbool.h>

#define DOUBLE_PRESS_THRESHOLD_MS 500

typedef struct {
    uint32_t lastPressTime;
    uint8_t pressCount;
} DoublePressTracker;

// Executes button logic for all directions
void button_task_execute(void);

#endif /* INC_BUTTON_TASK_H_ */
