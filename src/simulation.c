#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "../include/sensor_simulator.h"

void read_sensor_data(int p){
    int nbytes;
    float inbuf [NUM_SENSORS];

	while ((nbytes = read(p, &inbuf, PIPE_PAYLOAD_SIZE) > 0)){
        for(int i = 0; i < NUM_SENSORS; i++)
		    printf("sensor %d: %0.2f\n", i + 1, inbuf[i]);
    }
	if (nbytes != 0) {
        fprintf(stderr, "Error reading pipe at line %d\n", __LINE__);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <sensor_mask_hex> <sampling_rate_ms>\n", argv[0]);
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

    int p[2], pid;

    if (pipe(p) < 0) {
        fprintf(stderr, "Error creating pipe: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) > 0) {
        // Parent process: Simulate sensor data and write it to the pipe
        simulate_sensor_data(sensors_enabled, sampling_rate_ms, p[1]);
    } else {
        // Child process: Read and display the sensor data
        read_sensor_data(p[0]);
    }

    return 0;
}
