/*
 * buzzer.h
 *
 * Interface for buzzer control module:
 * - Plays melody when step goal is reached
 * - Provides manual PWM control if needed
 *
 * Created on: Apr 4, 2025
 * Author: eaz11 & gjo77
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>
#include <stdbool.h>

// Call from main loop to run buzzer state machine
void buzzer_execute(void);

// Manual control (optional external use)
void buzzer_start(void);
void buzzer_stop(void);

#endif /* BUZZER_H_ */
