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

// Address space accessed by multiple threads, see shared_memory.h for more information
extern shared_memory_t shared_memory;

/**
 * @brief This stucture used to manage the circular buffers containing sensor reading weights
 *        along with storing the current moving average
 *
 * This structure tracks the moving average state for each individual sensor. It includes information
 * about whether the the average is fit to be displayed based on the initial fillup of the circular buffer,
 * the index of the least recently used (LRU) element weight, and the calculated average value for the sensor.
 *
 * The structure helps manage the state of each sensors' collected data as it undergoes moving average calculations
 * over a rolling window.
 */
typedef struct {
    bool buffer_full;       // Flag indicating when the initial filling circular
    int lru_index;          // Index of the least recently used (LRU) element
    float average;          // The current moving average value for the sensor
} moving_average_t;

/**
 * @brief Structure that holds the parameters used to parsed data from the main thread to current during intial startup
 *
 * This structure contains user defined parameters used for calculating the moving average of sensor data. It includes
 * the window size (percision value when calculating average) and a dynamically allocated array used to store sensor
 * weights
 *
 * NOTE: All sensors produce equal weights, of sensor_reading/window_size
 *
 * The structure helps pass configurationg data during the creation of this thread.
 */
typedef struct {
    int window_size;                        // window size of how many readings we want to store for average calculation
    float *sensor_weights[NUM_SENSORS];     // sensor readings weight stored as sensor_reading/window_size
} moving_average_params_t;

/**
 * @brief Calculates and manages moving averages of sensor data.
 *
 * This function calculates the moving averages on an interrupt based system for enabled sensors.
 * Weighted sensor values are stored in a management buffered of a specified window size.
 * The function will wait for the buffers to fill before printing results. Once buffers are filled
 * stored weights will be repalced on an LRU based system maintaining a window of data in which
 * we use to track average.
 *
 * The function utilizes a mutex to synchronize access to shared memory ensuring thread safety
 * along with an atomic variable to indicate a shutdown request
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
