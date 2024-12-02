#include "../include/utils.h"
#include "../include/sensor_simulator.h"
#include "../include/moving_average.h"
#include "../include/shared_memory.h"
#include <time.h>


pthread_t sensor_simulation_thread, moving_average_thread;
shared_memory_t shared_memory;

int main(int argc, char *argv[]) {
    time_t start_time = time(NULL);
    int runtime_sec;

    sensor_params_t sensor_params;
    moving_average_params_t moving_average_params;

    parse_arguments(argc, argv, &sensor_params, &moving_average_params, &runtime_sec);
    initialize_buffers(moving_average_params.sensor_weights, moving_average_params.window_size, sensor_params.sensors_enabled);

    // initialize required values
    shared_memory.sensors_update_mask = 0; //set udpated sensors to NONE
    atomic_store(&shared_memory.shutdown, false);
    pthread_mutex_init(&shared_memory.mutex, NULL);

    // create threads for simulation and calculations
    pthread_create(&moving_average_thread, NULL,moving_average, &moving_average_params);
    pthread_create(&sensor_simulation_thread, NULL, simulate_sensors, &sensor_params);

    // begin shutdown once runtime is met
    while (difftime(time(NULL), start_time) < runtime_sec);
    atomic_store(&shared_memory.shutdown, true);

    pthread_join(moving_average_thread, NULL);
    pthread_join(sensor_simulation_thread, NULL);

    cleanup(sensor_params.sensors_enabled, moving_average_params.sensor_weights);
    return 0;
}
