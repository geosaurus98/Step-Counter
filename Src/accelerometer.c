/*
 * accelerometer.c
 *
 * Handles raw accelerometer readings, applies orientation-based offsets,
 * performs filtering, and computes squared magnitude.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#include "accelerometer.h"
#include "imu_lsm6ds.h"

#define ORIENTATION_THRESHOLD 16000

// Module-scoped filters and latest result
static AveragingFilter x_filter, y_filter, z_filter;
static FilteredAcceleration latest_filtered_data;

// Initializes an averaging filter with zeros
void filter_init(AveragingFilter *filter) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        filter->buffer[i] = 9310;  // Pre-fill buffer for consistent initial output
    }
    filter->index = 0;
}

// Returns average of buffer after inserting new value
int16_t filter_apply(AveragingFilter *filter, int16_t new_value) {
    filter->buffer[filter->index] = new_value;
    filter->index = (filter->index + 1) % BUFFER_SIZE;

    int32_t sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sum += filter->buffer[i];
    }
    return (int16_t)(sum / BUFFER_SIZE);
}

// Computes squared magnitude without sqrt for performance
uint64_t magnitude_squared(int16_t x, int16_t y, int16_t z) {
    return (int64_t)x * x + (int64_t)y * y + (int64_t)z * z;
}

// Reads 16-bit signed acceleration value from register pair
int16_t get_acceleration_axis(uint8_t low_reg, uint8_t high_reg) {
    uint8_t low = imu_lsm6ds_read_byte(low_reg);
    uint8_t high = imu_lsm6ds_read_byte(high_reg);
    return (int16_t)((high << 8) | low);
}

// Hardware and filters init
void accelerometer_init(void) {
    imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
    filter_init(&x_filter);
    filter_init(&y_filter);
    filter_init(&z_filter);
}

// Main accelerometer logic: read, adjust, filter, compute magnitude
FilteredAcceleration accelerometer_execute(void) {
    int16_t ax = get_acceleration_axis(OUTX_L_XL, OUTX_H_XL);
    int16_t ay = get_acceleration_axis(OUTY_L_XL, OUTY_H_XL);
    int16_t az = get_acceleration_axis(OUTZ_L_XL, OUTZ_H_XL);

    int16_t x_offset = 0, y_offset = 0, z_offset = 0;

    if (ax > ORIENTATION_THRESHOLD) {
        x_offset = 100; y_offset = -70; z_offset = -500;
    } else if (ax < -ORIENTATION_THRESHOLD) {
        x_offset = 100; z_offset = 300;
    } else if (ay > ORIENTATION_THRESHOLD) {
        x_offset = 500; y_offset = -200; z_offset = 600;
    } else if (ay < -ORIENTATION_THRESHOLD) {
        x_offset = -65; z_offset = -225;
    } else if (az > ORIENTATION_THRESHOLD) {
        x_offset = 150; y_offset = -110;
    } else {
    	x_offset = 0; y_offset = 0; z_offset = 0;
    }

    ax += x_offset;
    ay += y_offset;
    az += z_offset;

    int16_t fx = filter_apply(&x_filter, ax);
    int16_t fy = filter_apply(&y_filter, ay);
    int16_t fz = filter_apply(&z_filter, az);

    latest_filtered_data = (FilteredAcceleration){
        .acc_x_filtered = fx,
        .acc_y_filtered = fy,
        .acc_z_filtered = fz,
        .magnitude_square = magnitude_squared(fx, fy, fz)
    };

    return latest_filtered_data;
}

FilteredAcceleration accelerometer_get_latest(void) {
    return latest_filtered_data;
}
