#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "shared_memory.h"

// Address space accessed by multiple threads, see shared_memory.h for more information
extern shared_memory_t shared_memory;

typedef struct {
    uint8_t sensors_enabled;                // Bitmask of which sensors are enabled
    int sampling_rates_ms[NUM_SENSORS];      // Sampling rate in milliseconds
} sensor_params_t;

float generate_sensor_data(void);
void read_sensors(uint8_t sensors_enabled);
void *simulate_sensor_data(void* arg);
void create_sensor_timers(timer_t timer_ids[], sensor_params_t *params);
void delete_sensor_timers(timer_t timer_ids[], sensor_params_t *params);

#endif // SENSOR_SIMULATOR_H
