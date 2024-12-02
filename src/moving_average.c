#include "../include/moving_average.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdatomic.h>


void* moving_average(void* arg){

    moving_average_t average_managers[NUM_SENSORS] = {0};
    moving_average_params_t *params = (moving_average_params_t *)arg;

    float **sensor_weights = params->sensor_weights;
    int window_size = params->window_size;

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

                update_averages(sensor_weights, average_managers, sensor_mask, window_size);

                // when enough weights are gathered for the window
                // enable printing
                if (average_managers[i].lru_index >= window_size-1)
                    average_managers[i].buffer_full = true;

                if (average_managers[i].buffer_full)
                    print_avg(average_managers[i].average, window_size, i);
            }
        }

        // ~~~~~~~~~~~~~~~ CRITICAL ZONE END ~~~~~~~~~~~~~~~~
        pthread_mutex_unlock(&shared_memory.mutex);
    }

    pthread_exit(0);
}
void update_averages(float** sensor_weights, moving_average_t* average_managers, int update_mask, int window_size){
    float element_weight;
    for(int i = 0; i < NUM_SENSORS; i++) {
        if ( !((update_mask >> i) & 1) ) continue;

        // Calculate weight
        element_weight = shared_memory.sensor_data[i]/window_size;

        // update average by subtracting LRU weigth with new weigth
        average_managers[i].average += element_weight - sensor_weights[i][average_managers[i].lru_index];

        // replace LRU weigth w/ new
        sensor_weights[i][average_managers[i].lru_index] = element_weight;

        // updates LRU position
        average_managers[i].lru_index = (average_managers[i].lru_index + 1) % window_size;
    }
}
