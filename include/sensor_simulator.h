#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <bits/types/sigevent_t.h>
#include "shared_memory.h"

// Address space accessed by multiple threads, see shared_memory.h for more information
extern shared_memory_t shared_memory;

/**
 * @brief Structure that holds the parameters used to parsed data from the main thread to current during intial startup
 *
 * This structure contains user defined parameters specifing which sensors are enabled with a bitmask, and their
 * corresponding sampling rates in milliseconds
 *
 * The structure helps pass configurationg data during the creation of this thread.
 */
typedef struct {
    uint8_t sensors_enabled;                // Bitmask of which sensors are enabled
    int sampling_rates_ms[NUM_SENSORS];     // Sampling rate in milliseconds
} sensor_params_t;

 /**
 * @brief Simulates sensor data by creating and managing timers for each enabled sensor.
 *
 * This function uses the `create_sensor_timers` function, to create the timer interrupts
 * and then continuously waits for the shutdown signal. When the main thread signals a shutdown
 * via the shutdown flag is set, the function cleans up using `delete_sensor_timers` and exits
 *
 * @param arg Pointer to `sensor_params_t` structure containing sensor configurations.
 *
 * @return None
 */
void *simulate_sensors(void* arg);

/**
 * @brief Generates simulated sensor data using rand.
 *
 * @return A simulated sensor data value as a floating-point number.
 */
float generate_sensor_data(void);

/**
 * @brief handler sensor data updates, inspired by interrupt handlers used in embedded systems
 *
 * This function is triggered periodically by a timer for each enabled sensor at the specified sampling rate.
 * It updates the sensor data in the shared memory and marks the sensor as updated
 *
 * @param sv Union `sigval` containing the sensor index (`sival_int`) to identify which sensor
 *           triggered the handler.
 *
 * @return None
 */
void sensor_periodic_handler(union sigval sv);

/**
 * @brief Creates timers for each enabled sensor.
 *
 * This function sets up periodic timers for each enabled sensor using the sampling rate
 * provided in `sensor_params_t`. Each timer triggers a signal handler (`sensor_periodic_handler`)
 * at the specified intervals.
 *
 * @param timer_ids Array of `timer_t` to store the created timers.
 * @param params Pointer to the `sensor_params_t` containing sensor configurations (sampling rate & mask).
 *
 * @return None
 */
void create_sensor_timers(timer_t timer_ids[], sensor_params_t *params);

/**
 * @brief Deletes timers for each enabled sensor.
 *
 * @param timer_ids
 * @param sensors_mask mask of which indicies of 'timer_ids' are initialized
 *
 * @return None
 */
void delete_sensor_timers(timer_t timer_ids[], int sensors_mask);

#endif // SENSOR_SIMULATOR_H
