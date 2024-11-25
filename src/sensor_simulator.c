#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

//#include <signal.h>
//#include <errno.h>

#include "../include/sensor_simulator.h"


// buffered sensor data, initalized to 0 for disabled sensors
//float sensor_buffer [NUM_SENSORS] = { 0 };


float generate_sensor_data() {
    return ((float)rand() / (float)(RAND_MAX)) * 100.0f;
}

void read_sensors(uint8_t sensors_enabled, shared_memory_t* shared_memory){
    float data;

    for (int i = 0; i < NUM_SENSORS; i++) {
        // check if current sensor is enabled
        if (sensors_enabled & (1<<i)) {
            data = generate_sensor_data();
            shared_memory->sensor_buffer[i] = data;
        }
    }
}

void simulate_sensor_data(uint8_t sensors_enabled, shared_memory_t* shared_memory, int sampling_rate_ms) {
    srand(time(NULL));
    while (1) {
        //pthread_mutex_lock(&shared_memory->mutex);
        while(pthread_mutex_lock(&shared_memory->mutex) != 0);

        // critical zone
        if(shared_memory->interrupt_sensors) {
            pthread_mutex_unlock(&shared_memory->mutex);
            break;
        }

        read_sensors(sensors_enabled, shared_memory);
        shared_memory->sensors_updated = 1;

        // end critical zone
        pthread_mutex_unlock(&shared_memory->mutex);

        // sleep to maintain sampling rate
        usleep(1000*sampling_rate_ms);
    }

    printf("Dying child\n");

    // Close pipe
    //if (close(p) == -1) {
    //    // Print error with line number
    //    fprintf(stderr, "Error closing pipe at line %d: %s\n", __LINE__, strerror(errno));
    //    exit(EXIT_FAILURE);
    //}
    printf("Child process terminating gracefully...\n");

    //kill(getpid(), SIGTERM);
}

