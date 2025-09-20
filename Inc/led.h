/*
 * led.h
 *
 * Controls LEDs to visually represent goal progress.
 * One LED turns on for every 25% of goal completion.
 * When below 25%, DS3 uses PWM to show partial completion.
 *
 * Created on: May 20, 2025
 * Author: eaz11 & gjo77
 */

#ifndef INC_LED_H_
#define INC_LED_H_

// Initializes LED PWM and state
void LED_init(void);

// Updates LED state based on current goal progress
void LED_execute(void);

#endif /* INC_LED_H_ */
