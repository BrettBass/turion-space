#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "shared_memory.h"

extern shared_memory_t shared_memory;

typedef struct {
    uint8_t sensors_enabled; // Bitmask of which sensors are enabled
    int sampling_rate_ms;    // Sampling rate in milliseconds
} sensor_params_t;

float generate_sensor_data(void);
void read_sensors(uint8_t sensors_enabled);
void *simulate_sensor_data(void* arg);
int msleep(unsigned long time_ms);

#endif // SENSOR_SIMULATOR_H
