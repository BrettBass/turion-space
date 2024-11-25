#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
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

shared_memory_t *shared_memory;

void read_sensor_data(shared_memory_t* shared_memory);
void shutdown_handler(int sig);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms> <runtime_sec>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the sensor mask from the hexadecimal string (e.g., 0x3F)
    uint8_t sensors_enabled = (uint8_t)strtol(argv[1], NULL, 16); // Parse hex value

    // Parse the sampling rate (in milliseconds)
    int sampling_rate_ms = atoi(argv[2]);

    if (sampling_rate_ms <= 0) {
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

    int pid;

    start_time = time(NULL);


    int key = 1;
    int id = shmget(key, sizeof(shared_memory_t), IPC_CREAT | 0644);
    shared_memory = (shared_memory_t *) shmat(id, NULL, 0);
    shared_memory->sensors_updated = 0;
    shared_memory->interrupt_sensors = 0;

    pthread_mutex_init(&shared_memory->mutex, NULL);

    if ((pid = fork()) > 0) {
        child_pid = pid;
        // child process: Simulate sensor data and write it to the pipe
        simulate_sensor_data(sensors_enabled, shared_memory, sampling_rate_ms);
        //kill(child_pid, SIGTERM);
        //close(p[1]);
    } else if (pid == 0){
        current_time = time(NULL);
        read_sensor_data(shared_memory);
    } else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }


    return 0;
}

void read_sensor_data(shared_memory_t* shared_memory){

    while (difftime(current_time, start_time) < runtime_sec) {
        time(&current_time);
        pthread_mutex_lock(&shared_memory->mutex);

        // critical zone
        if (shared_memory->sensors_updated) {

            for(int i = 0; i < NUM_SENSORS; i++) {
		        printf("sensor %d: %0.2f\n", i + 1, shared_memory->sensor_buffer[i]);
            }

        shared_memory->sensors_updated = 0;
        }

        pthread_mutex_unlock(&shared_memory->mutex);

        // prevent spinning? doesn't seemed needed
        //usleep(500);
    }

    pthread_mutex_lock(&shared_memory->mutex);

    shared_memory->interrupt_sensors = 1;

    pthread_mutex_unlock(&shared_memory->mutex);

}

void shutdown_handler(int sig) {

    printf("\nReceived signal %d\n pid: %d\n ppid: %d\nshutting down...\n", sig, getpid(), getppid());

    pthread_mutex_unlock(&shared_memory->mutex);
    pthread_mutex_destroy(&shared_memory->mutex);

    if (child_pid != 0) {
        printf("Parent: %d -- Terminating child process %d\n" , getppid(), child_pid);
    }
    exit(EXIT_SUCCESS);
}
