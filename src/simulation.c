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

shared_memory_t shared_memory;
volatile sig_atomic_t shutdown_requested = 0;

void* read_sensor_data();
void shutdown_handler(int sig);
void cleanup();

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms> <runtime_sec>\n", argv[0]);
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

    struct sigaction sa;
    sa.sa_handler = shutdown_handler;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);


    start_time = time(NULL);

    shared_memory.sensors_updated = 0;
    shared_memory.interrupt_sensors = 0;

    pthread_mutex_init(&shared_memory.mutex, NULL);

    pthread_create(&sensor_thread, NULL, simulate_sensor_data, &params);
    pthread_create(&read_thread, NULL, read_sensor_data, NULL);

    pthread_join(sensor_thread, NULL);
    pthread_join(read_thread, NULL);
    cleanup();


    return 0;
}

//void read_sensor_data(shared_memory_t* shared_memory){
void* read_sensor_data(){

    while (difftime(current_time, start_time) < runtime_sec) {

        if (shutdown_requested) {
            printf("Shutdown requested. Exiting read loop...\n");
            break;
        }
        time(&current_time);
        pthread_mutex_lock(&shared_memory.mutex);

        // critical zone
        if (shared_memory.sensors_updated) {

            for(int i = 0; i < NUM_SENSORS; i++) {
		        printf("sensor %d: %0.2f\n", i + 1, shared_memory.sensor_buffer[i]);
            }

        shared_memory.sensors_updated = 0;
        }

        pthread_mutex_unlock(&shared_memory.mutex);

        // prevent spinning? doesn't seemed needed
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
    printf("Cleanup started. child pid: %d\n", child_pid);
    //if (shared_memory != NULL) {
        // Clean up shared memory and mutex
        pthread_mutex_lock(&shared_memory.mutex);
        pthread_mutex_unlock(&shared_memory.mutex);
        pthread_mutex_destroy(&shared_memory.mutex);

        //shmdt(shared_memory);  // Detach shared memory
     //}

     //if (child_pid != getpid() && child_pid != 0) {
     //    printf("Parent: %d -- Terminating child process %d\n", getppid(), child_pid);
     //    kill(child_pid, SIGTERM);  // Terminate child process if necessary
     //    waitpid(child_pid, &status, 0);  // Wait for the child to terminate
     //}

        printf("Cleanup complete.\n");
}
