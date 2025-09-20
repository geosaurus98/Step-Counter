/*
 * accelerometer.h
 *
 * Interface for accelerometer processing module.
 *
 * Created on: May 8, 2025
 * Author: eaz11 & gjo77
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

#define BUFFER_SIZE 20

// Holds a circular buffer of recent values for averaging
typedef struct {
    int16_t buffer[BUFFER_SIZE];
    uint8_t index;
} AveragingFilter;

// Holds filtered acceleration values and their squared magnitude
typedef struct {
    int16_t acc_x_filtered;
    int16_t acc_y_filtered;
    int16_t acc_z_filtered;
    uint64_t magnitude_square;
} FilteredAcceleration;

// Initializes accelerometer and filtering
void accelerometer_init(void);

// Reads and filters accelerometer values; returns result
FilteredAcceleration accelerometer_execute(void);

// Returns the most recent filtered result
FilteredAcceleration accelerometer_get_latest(void);

// Initializes an averaging filter
void filter_init(AveragingFilter *filter);

// Adds a new value to the filter and returns the average
int16_t filter_apply(AveragingFilter *filter, int16_t new_value);

// Computes squared magnitude of a 3D vector
uint64_t magnitude_squared(int16_t x, int16_t y, int16_t z);

// Reads a 16-bit signed axis value from register pair
int16_t get_acceleration_axis(uint8_t low_reg, uint8_t high_reg);

#endif /* ACCELEROMETER_H_ */

