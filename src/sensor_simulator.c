#include <stdlib.h>
#include <unistd.h>

#include "../include/sensor_simulator.h"


float generate_sensor_data() {
    return ((float)rand() / (float)(RAND_MAX)) * 100.0f;
}

void read_sensors(uint8_t sensors_enabled){
    float data;

    for (int i = 0; i < NUM_SENSORS; i++) {
        // check if current sensor is enabled
        if (sensors_enabled & (1<<i)) {
            data = generate_sensor_data();
            shared_memory.sensor_data[i] = data;
        }
    }
}

void *simulate_sensor_data(void* arg) {
    sensor_params_t *params = (sensor_params_t *)arg;
    uint8_t sensors_enabled = params->sensors_enabled;
    int sampling_rate_ms = params->sampling_rate_ms;

    srand(time(NULL));
    while (1) {
        pthread_mutex_lock(&shared_memory.mutex);

        // critical zone
        if(shared_memory.interrupt_sensors) {
            pthread_mutex_unlock(&shared_memory.mutex);
            break;
        }

        read_sensors(sensors_enabled);
        shared_memory.sensors_updated = 1;

        // end critical zone
        pthread_mutex_unlock(&shared_memory.mutex);

        // sleep to maintain sampling rate
        msleep(sampling_rate_ms);
    }

    pthread_exit(0);
}

int msleep(unsigned long time_ms) {
        return usleep(1000*time_ms);
}
