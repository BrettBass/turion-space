#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

// Constants
#define NUM_SENSORS 6
#define DATA_SIZE 16
#define PIPE_PAYLOAD_SIZE sizeof(float) * NUM_SENSORS

// Extern declaration for the global sensor buffer
extern float sensor_buffer[NUM_SENSORS];

typedef struct {
    float sensor_buffer [NUM_SENSORS];
    bool interrupt_sensors;
    bool sensors_updated;
    pthread_mutex_t mutex;
} shared_memory_t;

extern shared_memory_t shared_memory;

typedef struct {
    uint8_t sensors_enabled; // Bitmask of which sensors are enabled
    int sampling_rate_ms;    // Sampling rate in milliseconds
} simulation_params_t;

float generate_sensor_data(void);
void read_sensors(uint8_t sensors_enabled);
void *simulate_sensor_data(void* arg);

#endif // SENSOR_SIMULATOR_H
