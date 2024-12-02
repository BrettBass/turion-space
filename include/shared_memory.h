/**
 * @file shared_memory.h
 * @brief Header file for shared memory structure used as an IPC between threads
 *
 * The shared memory structure `shared_memory_t` is used to store:
 * - A shutdown flag that allows the main thread to signal all other threads to
 *   clean up for a graceful shutdown
 * - An array of sensor data to pass sensor "readings"
 * - A bitmask to indicate what indicies in the array have been updated
 * - A mutex to ensure thread safety
 *
 * @author Brett Bass
 */
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>

// Constants
#define NUM_SENSORS 6

typedef struct {
    atomic_bool shutdown;
    float sensor_data [NUM_SENSORS];
    int sensors_update_mask;
    pthread_mutex_t mutex;
} shared_memory_t;

#endif // SENSOR_SIMULATOR_H
