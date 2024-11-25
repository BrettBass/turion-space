#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include "../include/sensor_simulator.h"

int runtime_sec = -1;
time_t start_time;
time_t current_time;
pid_t child_pid, wpid;
int status;

pthread_t sensor_thread, read_thread;

float *sensor_buffer[NUM_SENSORS];

shared_memory_t shared_memory;
volatile sig_atomic_t shutdown_requested = 0;

void* read_sensor_data(void* arg);
void shutdown_handler(int sig);
void cleanup();

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms> <runtime_sec> <window size>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    simulation_params_t params;
    // Parse the sensor mask from the hexadecimal string (e.g., 0x3F)
    params.sensors_enabled = (uint8_t)strtol(argv[1], NULL, 16); // Parse hex value

    // Parse the sampling rate (in milliseconds)
    params.sampling_rate_ms = atoi(argv[2]);

    if (params.sampling_rate_ms <= 0) {
        fprintf(stderr, "Invalid sampling rate. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    // Parse the runtime (in seconds)
    runtime_sec = atoi(argv[3]);
    if (runtime_sec <= 0) {
        fprintf(stderr, "Invalid runtime. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    int window_size = atoi(argv[4]);

    if (window_size <= 0) {
        fprintf(stderr, "Invalid window siez. It must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensor_buffer[i] = malloc(window_size * sizeof(float));  // Allocate memory for each sensor's buffer
        if (!sensor_buffer[i]) {
            perror("Failed to allocate memory for a sensor buffer");
            // Free already allocated buffers before exiting
            for (int j = 0; j < i; j++) {
                free(sensor_buffer[j]);
            }
            exit(EXIT_FAILURE);
        }
    }

    struct sigaction sa;
    sa.sa_handler = shutdown_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);


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

//void read_sensor_data(shared_memory_t* shared_memory){
void* read_sensor_data(void* arg){

    int window_size = *(int *)arg;
    float avg;
    int read_index = 0;
    int buffer_occupancy = 1;
    bool ready = 0;

    while (difftime(current_time, start_time) < runtime_sec) {

        if (shutdown_requested) {
            printf("Shutdown requested. Exiting read loop...\n");
            cleanup();
            break;
        }
        time(&current_time);
        pthread_mutex_lock(&shared_memory.mutex);

        // critical zone
        if (shared_memory.sensors_updated) {

            for(int i = 0; i < NUM_SENSORS; i++) {
		        //printf("sensor %d: %0.2f\n", i + 1, shared_memory.sensor_data[i]);

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
		        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                for (int i = 0; i < NUM_SENSORS; i++) {
                    avg = 0;
		            printf("sensor %d {", i + 1);
                    for(int j = 0; j < window_size; j++) {
                        avg += sensor_buffer[i][j];
		                printf(" %0.2f", sensor_buffer[i][j]);
                    }
                    avg /= window_size;
		            printf(" } -- avg =  %0.2f\n", avg);
                }
		        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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

void shutdown_handler(int sig) {

    printf("\nReceived signal %d\n pid: %d\n ppid: %d\nshutting down...\n", sig, getpid(), getppid());
    if (sig == SIGTERM) {
        cleanup();
        exit(EXIT_SUCCESS);
    }

    shutdown_requested = 1;
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
