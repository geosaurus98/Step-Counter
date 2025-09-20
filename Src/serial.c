/*
 * serial.c
 *
 * UART debug output for step counter project.
 * Streams raw ADC and acceleration data when serial output is toggled on.
 *
 * Created on: Mar 19, 2025
 * Author: eaz11 & gjo77
 */

#include "serial.h"
#include "usart.h"
#include "joystick_task.h"
#include "accelerometer.h"
#include <stdio.h>

static bool serial_on = false;  // Serial toggle state

// Enables/disables UART data output
void serial_toggle(void) {
    serial_on = !serial_on;
}

// Outputs joystick and filtered acceleration data to UART
void serial_task_execute(void) {
    if (!serial_on) return;

    char uart_buffer[128];

    // Retrieve raw joystick/potentiometer ADC readings
    //uint16_t* adc_values = joystick_get_values();
    //uint16_t adc_x = adc_values[2];
    //uint16_t adc_y = adc_values[1];
    //uint16_t potent = adc_values[0]; // Not printed here but could be included

    // Retrieve filtered accelerometer data
    FilteredAcceleration data = accelerometer_get_latest();

    // Format data for UART transmission
    int len = snprintf(uart_buffer, sizeof(uart_buffer),
        ">ACC_X:%d,ACC_Y:%d,ACC_Z:%d,MAG:%llu\r\n",
        data.acc_x_filtered, data.acc_y_filtered, data.acc_z_filtered,
        data.magnitude_square);

    // Send over USART2
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buffer, len, HAL_MAX_DELAY);
}
