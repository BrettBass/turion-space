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
