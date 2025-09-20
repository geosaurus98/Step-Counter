/*
 * led.c
 *
 * Visual indicator for goal progress using three RGB LEDs and one PWM-controlled LED.
 * - Each 25% progress activates another full-brightness LED
 * - Below 25%, a fourth LED (DS3) shows partial progress via PWM brightness
 *
 * Created on: May 20, 2025
 * Author: eaz11 & gjo77
 */

#include "led.h"
#include "rgb.h"
#include "goal_tracker.h"
#include "pwm.h"
#include "tim.h"

void LED_init(void) {
    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, 0); // DS3 initially off
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}

void LED_execute(void)
{
    uint8_t progress = get_goal_progress_percentage(); // 0–100%
    uint8_t duty_cycle_level = 0;

    // Turn off all RGB LEDs initially
    rgb_led_off(RGB_RIGHT);
    rgb_led_off(RGB_DOWN);
    rgb_led_off(RGB_LEFT);

    // LED logic:
    // DS3: Gradual brightness for 0–24%
    // RGB_RIGHT: ≥50%
    // RGB_DOWN:  ≥75%
    // RGB_LEFT: 100%

    if (progress == 0) {
        duty_cycle_level = 0;
    } else if (progress < 25) {
        duty_cycle_level = (progress * 100) / 25; // Scale 0–24% to 0–100 brightness
    } else {
        duty_cycle_level = 100;

        if (progress >= 50) rgb_led_on(RGB_RIGHT);
        if (progress >= 75) rgb_led_on(RGB_DOWN);
        if (progress == 100) rgb_led_on(RGB_LEFT);
    }

    pwm_setDutyCycle(&htim2, TIM_CHANNEL_3, duty_cycle_level); // DS3
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}
