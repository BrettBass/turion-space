/**
 * @file moving_average.h
 * @brief Header file for the Moving Average module.
 *
 * This file defines the structures and function prototypes used for calculating and updating the
 * moving averages of sensor data. It provides functionality for managing of sensor data stored
 * as a weigrt in a circular buffer. Calculating the moving averages over a defined window size
 * The moving averages are computed in a separate thread to simulate real-time analysis of
 * sensor data.
 *
 * @author Brett Bass
 *
 */

#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "shared_memory.h"
#include "output.h"

// Address space accessed by multiple threads
extern shared_memory_t shared_memory;

typedef struct {
    bool buffer_full; // Buffer full flags for each sensor
    int lru_index;    // lru indices for each sensor
    float average;   // Average values for each sensor
} moving_average_t;

typedef struct {
    int window_size;
    float *sensor_weights[NUM_SENSORS];
} moving_average_params_t;

/**
 * @brief Calculates and manages moving averages of sensor data.
 *
 * This function calculates the moving averages on an interrupt based systen for enabled sensors.
 * Weighted sensor values are stored in a management buffered of a specified window size.
 * The function will wait for the buffers to fill before printing results. Once buffers are filled
 * stored weights will be repalced on an LRU based system maintaining a window of data in which
 * we use to track average.
 *
 * The function utilizes a mutex to synchronize access to shared memory ensuring thread safety
 *
 * @param arg A pointer to the `moving_average_params_t` structure, which contains a buffer used
 *            our weighted sensor data and the window size for specifing the buffers length.
 *
 * @return None
 */
void* moving_average(void* arg);

/**
 * @brief Updates the weighted sensor data buffer along with their current average.
 *
 * This function updates the weighted buffer for each sensor and adjusts the moving average values
 * based on new sensor readings. It calculates the new weighted value for each updated sensor based
 * on an update mask. Effectively adjusting the moving average by adding the weight of the new data
 * and removing the weight of the least-recently used (LRU) data in the buffer.
 *
 * The function ensures that only the sensors indicated by the `update_mask` are processed,
 * and it updates the moving averages accordingly.
 *
 * @param sensor_weights A pointer to an array of buffers for the sensor data.
 * @param average_managers A pointer to an array of `moving_average_t` structures containing
 *                         the current moving averages and buffer states for each sensor.
 * @param update_mask A bitmask indicating which sensors have triggered an updated.
 * @param window_size User defined percision window used to calculate the updated weight
 *
 * @return None
 */
void update_averages(float** sensor_weights, moving_average_t* average_managers, int update_mask, int window_size);

#endif // MOVING_AVERAGE_H
