/*
 * buzzer.c
 *
 * Plays a celebratory tune when the step goal is reached.
 * Uses non-blocking state machine with timer-driven melody playback.
 *
 * Created on: Apr 4, 2025
 * Author: eaz11 & gjo77
 */

#include "buzzer.h"
#include "tim.h"
#include "goal_tracker.h"
#include "test_mode.h"
#include "step_detection.h"
#include "ssd1306.h"

#include <stdint.h>
#include <stdbool.h>

// Notes (frequencies in Hz)
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  261
#define NOTE_D4  294
#define NOTE_E4  329
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define REST     0

// Melody and note durations
static const uint16_t melody[] = {
    NOTE_E4, NOTE_E4, REST, NOTE_E4, REST, NOTE_C4, NOTE_E4, REST,
    NOTE_G4, REST, REST, NOTE_G3
};

static const uint16_t durations[] = {
    150, 150, 100, 150, 100, 150, 150, 100, 300, 200, 100, 200
};

static const uint8_t melody_length = sizeof(melody) / sizeof(melody[0]);

// Melody state
static bool tune_played = false;
static bool melody_active = false;
static uint32_t next_note_time = 0;
static uint8_t current_note_index = 0;

// Sets buzzer frequency using TIM16 PWM
void set_buzzer_freq(uint16_t freq) {
    uint32_t timer_clk = HAL_RCC_GetPCLK1Freq();
    uint32_t period = (timer_clk / freq) - 1;

    __HAL_TIM_SET_AUTORELOAD(&htim16, period);
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, period / 2);
    __HAL_TIM_SET_PRESCALER(&htim16, 0);
    HAL_TIM_GenerateEvent(&htim16, TIM_EVENTSOURCE_UPDATE);
}

// Non-blocking state machine to play melody
void buzzer_execute(void) {
    uint16_t steps = get_steps();
    uint16_t goal = get_goal();

    if (steps >= goal && !tune_played && !check_set_goal_state()) {
        tune_played = true;
        melody_active = true;
        current_note_index = 0;
        next_note_time = HAL_GetTick(); // Start immediately
    } else if (steps < goal) {
        tune_played = false; // Reset if goal is no longer reached
    }

    if (melody_active && HAL_GetTick() >= next_note_time) {
        if (current_note_index < melody_length) {
            uint16_t note = melody[current_note_index];
            uint16_t duration = durations[current_note_index];

            if (note == REST) {
                buzzer_stop();
            } else {
                set_buzzer_freq(note);
                buzzer_start();
            }

            next_note_time = HAL_GetTick() + duration;
            current_note_index++;
        } else {
            buzzer_stop();
            melody_active = false;
        }
    }
}

// Starts buzzer using current timer settings
void buzzer_start(void) {
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, __HAL_TIM_GET_AUTORELOAD(&htim16) / 2);
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
}

// Stops buzzer output
void buzzer_stop(void) {
    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
}
