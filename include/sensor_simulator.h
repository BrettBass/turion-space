#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

//#include <bits/pthreadtypes.h>
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

float generate_sensor_data(void);
void read_sensors(uint8_t sensors_enabled, shared_memory_t* shared_memory);
void simulate_sensor_data(uint8_t sensors_enabled, shared_memory_t* shared_memory, int sampling_rate_ms);

#endif // SENSOR_SIMULATOR_H
