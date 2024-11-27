#include "../include/moving_average.h"

void* moving_average(void* arg){

    moving_average_params_t *params = (moving_average_params_t *)arg;
    int read_index = 0;
    bool buffer_full = 0;
    bool ready = 0;
    time_t current_time;
    float averages[NUM_SENSORS] = { 0 };

    printf("Filling Buffer...\n");

    do {
        time(&current_time);

        pthread_mutex_lock(&shared_memory.mutex);
        // ~~~~~~~~~~~~~~~ CRITICAL ZONE START ~~~~~~~~~~~~~~~

        // check for sensor updates
        if (shared_memory.sensors_updated) {

            // update circular buffer with new sensor data
            update_buffers(params->sensor_buffer, averages, read_index, params->window_size);

            // Mark data as processed
            ready = 1;

            // sensors are nolonger up to date
            shared_memory.sensors_updated = 0;

            // update read index and sesnor update flag
            read_index = (read_index + 1) % params->window_size;

            if (read_index >= params->window_size-1)
                buffer_full = true;
        }

        // ~~~~~~~~~~~~~~~ CRITICAL ZONE END ~~~~~~~~~~~~~~~~
        pthread_mutex_unlock(&shared_memory.mutex);

        // check buffer and processed statuses
        if(ready && buffer_full){
            print_avg((const float*) averages, params->window_size);
            ready = 0;
        }
    } while (difftime(current_time, params->start_time) < params->runtime_sec);

    pthread_mutex_lock(&shared_memory.mutex);
    // ~~~~~~~~~~~~~~~ CRITICAL ZONE START ~~~~~~~~~~~~~~~

    shared_memory.interrupt_sensors = 1;

    // ~~~~~~~~~~~~~~~ CRITICAL ZONE START ~~~~~~~~~~~~~~~
    pthread_mutex_unlock(&shared_memory.mutex);

    pthread_exit(0);
}
void update_buffers(float** sensor_buffer, float* averages, int read_index, int window_size){
    float element_weight;
    for(int i = 0; i < NUM_SENSORS; i++) {
        element_weight = shared_memory.sensor_data[i]/window_size;
        averages[i] += element_weight - sensor_buffer[i][read_index];
        sensor_buffer[i][read_index] = element_weight;
    }
}
