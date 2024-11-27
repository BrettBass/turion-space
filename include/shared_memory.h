#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>
#include <pthread.h>

// Constants
#define NUM_SENSORS 6

typedef struct {
    float sensor_data [NUM_SENSORS];
    bool interrupt_sensors;
    bool sensors_updated;
    pthread_mutex_t mutex;
} shared_memory_t;

#endif // SENSOR_SIMULATOR_H
