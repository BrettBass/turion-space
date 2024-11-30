#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>

// Constants
#define NUM_SENSORS 6

typedef struct {
    float sensor_data [NUM_SENSORS];
    atomic_bool interrupt_sensors;
    int sensors_update_mask;
    pthread_mutex_t mutex;
} shared_memory_t;

#endif // SENSOR_SIMULATOR_H
