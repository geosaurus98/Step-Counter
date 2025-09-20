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

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdint.h>
#include <stdbool.h>

// Call from main loop to run buzzer state machine
void buzzer_execute(void);

// Starts melody playback (only triggers once when called)
void buzzer_play_tune(void);  // (Not used but left for optional expansion)

// Internal-use only: set frequency of buzzer PWM
void set_buzzer_freq(uint16_t freq);

// Manual control (optional external use)
void buzzer_start(void);
void buzzer_stop(void);

#endif /* INC_BUZZER_H_ */
