#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <stdint.h>

// Constants
#define NUM_SENSORS 6
#define DATA_SIZE 16
#define PIPE_PAYLOAD_SIZE sizeof(float) * NUM_SENSORS

// Extern declaration for the global sensor buffer
extern float sensor_buffer[NUM_SENSORS];

float generate_sensor_data(void);
void read_sensors(uint8_t sensors_enabled);
void simulate_sensor_data(uint8_t sensors_enabled, int sampling_rate_ms, int p);

#endif // SENSOR_SIMULATOR_H
