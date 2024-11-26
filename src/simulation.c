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

int runtime_sec = -1;
time_t start_time;
time_t current_time;

pthread_t sensor_thread, read_thread;
shared_memory_t shared_memory;
float *sensor_buffer[NUM_SENSORS];

void* read_sensor_data(void* arg);
void shutdown_handler(int sig);
void cleanup();
void parse_arguments(int argc, char* argv[], simulation_params_t *params, int* window_size);
void print_avg(int window_size);

int main(int argc, char *argv[]) {
    simulation_params_t params;
    int window_size;

    parse_arguments(argc, argv, &params, &window_size);
    initialize_buffers(sensor_buffer, window_size);

    start_time = time(NULL);
    shared_memory.sensors_updated = 0;
    shared_memory.interrupt_sensors = 0;
    pthread_mutex_init(&shared_memory.mutex, NULL);

    pthread_create(&sensor_thread, NULL, simulate_sensor_data, &params);
    pthread_create(&read_thread, NULL, read_sensor_data, &window_size);

    pthread_join(sensor_thread, NULL);
    pthread_join(read_thread, NULL);

    cleanup();
    return 0;
}

void* read_sensor_data(void* arg){

    int window_size = *(int *)arg;
    int read_index = 0;
    int buffer_occupancy = 1;
    bool ready = 0;

    printf("Filling Buffer...\n");

    while (difftime(current_time, start_time) < runtime_sec) {

        time(&current_time);
        pthread_mutex_lock(&shared_memory.mutex);

        // critical zone
        if (shared_memory.sensors_updated) {

            for(int i = 0; i < NUM_SENSORS; i++) {
                sensor_buffer[i][read_index] = shared_memory.sensor_data[i];
            }

        ready = 1;
        read_index = (read_index + 1) % window_size;
        shared_memory.sensors_updated = 0;
        }

        pthread_mutex_unlock(&shared_memory.mutex);


        if(ready){
            if (buffer_occupancy < window_size) {
                buffer_occupancy++;
            } else {
                print_avg(window_size);
            }
            ready = 0;
        }

        // prevent spinning? doesn't seem needed
        //usleep(500);
    }

    pthread_mutex_lock(&shared_memory.mutex);

    shared_memory.interrupt_sensors = 1;

    pthread_mutex_unlock(&shared_memory.mutex);

    pthread_exit(0);
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
void parse_arguments(int argc, char* argv[], simulation_params_t *params, int* window_size) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms> <runtime_sec> <window size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the sensor mask from the hexadecimal string (e.g., 0x3F)
    params->sensors_enabled = (uint8_t)strtol(argv[1], NULL, 16); // Parse hex value

    // Parse the sampling rate (in milliseconds)
    params->sampling_rate_ms = atoi(argv[2]);

    if (params->sampling_rate_ms <= 0) {
        fprintf(stderr, "Invalid sampling rate. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    // Parse the runtime (in seconds)
    runtime_sec = atoi(argv[3]);
    if (runtime_sec <= 0) {
        fprintf(stderr, "Invalid runtime. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    *window_size = atoi(argv[4]);

    if (window_size <= 0) {
        fprintf(stderr, "Invalid window siez. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
}

//void print_avg(int window_size) {
//    float avg;
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//    for (int i = 0; i < NUM_SENSORS; i++) {
//        avg = 0;
//        printf("sensor %d {", i + 1);
//        for(int j = 0; j < window_size; j++) {
//            avg += sensor_buffer[i][j];
//            printf(" %0.2f", sensor_buffer[i][j]);
//        }
//        avg /= window_size;
//        printf(" } -- avg =  %0.2f\n", avg);
//    }
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//}
#define MAX_BAR_LENGTH 50  // Max number of characters for the bar (you can adjust this)
#define MAX_SENSOR_VALUE 100.0  // The max value to scale sensor data to

void print_avg(int window_size) {
    float avg;
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    for (int i = 0; i < NUM_SENSORS; i++) {
        avg = 0;

        // Calculate the average value for this sensor
        for (int j = 0; j < window_size; j++) {
            avg += sensor_buffer[i][j];
        }
        avg /= window_size;

        // Scale the average to fit within the bar's max length
        int bar_length = (int)((avg / MAX_SENSOR_VALUE) * MAX_BAR_LENGTH);

        // Print sensor data and moving average
        printf("Sensor %d: Moving Average = %5.2f ", i + 1, avg);

        // Print the bar graph
        printf("| ");
        for (int j = 0; j < bar_length; j++) {
            printf("█");  // Print one block for each unit in the scaled bar
        }
        printf("\n\n");
    }

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    //printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
