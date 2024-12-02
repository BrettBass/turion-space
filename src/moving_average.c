#include "../include/moving_average.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdatomic.h>


void* moving_average(void* arg){

    moving_average_params_t *params = (moving_average_params_t *)arg;
    moving_average_t moving_averages[NUM_SENSORS] = {0};

    int sensor_mask = 0;

    printf("Filling Buffer...\n");

    while ( !atomic_load(&shared_memory.shutdown) ){
        pthread_mutex_lock(&shared_memory.mutex);
        // ~~~~~~~~~~~~~~~ CRITICAL ZONE START ~~~~~~~~~~~~~~~

        // check for sensor updates
        if (shared_memory.sensors_update_mask > 0) {

            // sensors are nolonger up to date
            sensor_mask = shared_memory.sensors_update_mask;
            shared_memory.sensors_update_mask = 0;

            // update read index and sesnor update flag
            for (int i = 0; i < NUM_SENSORS; i++) {
                if ( !((sensor_mask >> i) & 1) ) continue;
                update_buffer(params->sensor_buffer, moving_averages, sensor_mask, moving_averages[i].read_index, params->window_size);

                if (moving_averages[i].read_index >= params->window_size-1)
                    moving_averages[i].buffer_full = true;

                if (moving_averages[i].buffer_full)
                    print_avg(moving_averages[i].average, params->window_size, i);

                moving_averages[i].read_index = (moving_averages[i].read_index + 1) % params->window_size;
            }
        }

        // ~~~~~~~~~~~~~~~ CRITICAL ZONE END ~~~~~~~~~~~~~~~~
        pthread_mutex_unlock(&shared_memory.mutex);

    }

    pthread_exit(0);
}
void update_buffer(float** sensor_buffer, moving_average_t* moving_averages, int sensor_index, int read_index, int window_size){
    float element_weight;
    for(int i = 0; i < NUM_SENSORS; i++) {
        if ( !((sensor_index >> i) & 1) ) continue;
        element_weight = shared_memory.sensor_data[i]/window_size;
        moving_averages[i].average += element_weight - sensor_buffer[i][read_index];
        sensor_buffer[i][read_index] = element_weight;
    }
}
