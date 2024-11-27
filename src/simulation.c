#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include "../include/utils.h"
#include "../include/sensor_simulator.h"
#include "../include/moving_average.h"


pthread_t sensor_thread, moving_average_thread;
shared_memory_t shared_memory;
float *sensor_buffer[NUM_SENSORS];

void shutdown_handler(int sig);
void cleanup();
void parse_arguments(int argc, char* argv[], sensor_params_t* sensor_params, moving_average_params_t* ma);

int main(int argc, char *argv[]) {
    sensor_params_t sensor_params;
    moving_average_params_t moving_average_params;

    parse_arguments(argc, argv, &sensor_params, &moving_average_params);
    initialize_buffers(sensor_buffer, moving_average_params.window_size);

    moving_average_params.sensor_buffer = sensor_buffer;
    moving_average_params.start_time = time(NULL);

    shared_memory.sensors_updated = 0;
    shared_memory.interrupt_sensors = 0;
    pthread_mutex_init(&shared_memory.mutex, NULL);

    pthread_create(&sensor_thread, NULL, simulate_sensor_data, &sensor_params);
    pthread_create(&moving_average_thread, NULL,moving_average, &moving_average_params);

    pthread_join(sensor_thread, NULL);
    pthread_join(moving_average_thread, NULL);

    cleanup();
    return 0;
}


void cleanup() {
    // Free sensor buffers
    for (int i = 0; i < NUM_SENSORS; i++) {
        free(sensor_buffer[i]);
    }

    pthread_mutex_lock(&shared_memory.mutex);
    pthread_mutex_unlock(&shared_memory.mutex);
    pthread_mutex_destroy(&shared_memory.mutex);

    printf("Cleanup complete.\n");
}
void parse_arguments(int argc, char* argv[], sensor_params_t* sensor_params, moving_average_params_t* ma_params) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms> <runtime_sec> <window size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the sensor mask from the hexadecimal string (e.g., 0x3F)
    sensor_params->sensors_enabled = (uint8_t)strtol(argv[1], NULL, 16); // Parse hex value

    // Parse the sampling rate (in milliseconds)
    sensor_params->sampling_rate_ms = atoi(argv[2]);

    if (sensor_params->sampling_rate_ms <= 0) {
        fprintf(stderr, "Invalid sampling rate. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    // Parse the runtime (in seconds)
    ma_params->runtime_sec = atoi(argv[3]);
    if (ma_params->runtime_sec <= 0) {
        fprintf(stderr, "Invalid runtime. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    ma_params->window_size = atoi(argv[4]);

    if (ma_params->window_size <= 0) {
        fprintf(stderr, "Invalid window siez. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
}
