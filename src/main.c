#include "../include/utils.h"
#include "../include/sensor_simulator.h"
#include "../include/moving_average.h"
#include "../include/shared_memory.h"


pthread_t sensor_thread, moving_average_thread;
shared_memory_t shared_memory;
float *sensor_buffer[NUM_SENSORS];

void cleanup();

int main(int argc, char *argv[]) {
    sensor_params_t sensor_params;
    moving_average_params_t moving_average_params;

    parse_arguments(argc, argv, &sensor_params, &moving_average_params);
    initialize_buffers(sensor_buffer, moving_average_params.window_size, sensor_params.sensors_enabled);

    moving_average_params.sensor_buffer = sensor_buffer;
    moving_average_params.start_time = time(NULL);

    shared_memory.sensors_update_mask = 0;
    atomic_store(&shared_memory.interrupt_sensors, 0);
    pthread_mutex_init(&shared_memory.mutex, NULL);

    pthread_create(&moving_average_thread, NULL,moving_average, &moving_average_params);
    pthread_create(&sensor_thread, NULL, simulate_sensor_data, &sensor_params);

    pthread_join(moving_average_thread, NULL);
    pthread_join(sensor_thread, NULL);

    cleanup();
    return 0;
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
