#include "../include/utils.h"
#include "../include/sensor_simulator.h"


float generate_sensor_data() {
    return ((float)rand() / (float)(RAND_MAX)) * 100.0f;
}

void sensor_periodic_handler(union sigval sv){
    int sensor_index = sv.sival_int;

    // Update sensor
    pthread_mutex_lock(&shared_memory.mutex);
    // ~~~~~~~~~~~~~~~~~~ CRITIAL ZONE START ~~~~~~~~~~~~~~~~~~

    shared_memory.sensor_data[sensor_index] = generate_sensor_data();
    shared_memory.sensors_update_mask = (1 << sensor_index);

    // ~~~~~~~~~~~~~~~~~~ CRITIAL ZONE END   ~~~~~~~~~~~~~~~~~~
    pthread_mutex_unlock(&shared_memory.mutex);
}

void create_sensor_timers(timer_t timer_ids[], sensor_params_t *params) {
    // 0 non-used fields
    struct sigevent sev = {0};
    struct itimerspec its = {0};

    for (int i = 0; i < NUM_SENSORS; i++) {
        // skip disabled sensors
        if ( !(params->sensors_enabled & (1 << i)) ) continue;

        // create timer
        sev.sigev_notify_function = sensor_periodic_handler;
        sev.sigev_notify = SIGEV_THREAD; // thread awareness
        sev.sigev_signo = SIGRTMIN; // first realtime signal
        sev.sigev_value.sival_int = i; // which sensor triggers signal

        if (timer_create(CLOCK_REALTIME, &sev, &timer_ids[i]) == -1)
            die("Error: timer_create");


        // set timing interval
        its.it_value.tv_sec = params->sampling_rates_ms[i] / 1000;
        its.it_value.tv_nsec = (params->sampling_rates_ms[i] % 1000) * 1000000;
        its.it_interval = its.it_value; // repeat

        if (timer_settime(timer_ids[i], 0, &its, NULL) == -1)
            die("Error: timer_settime");
    }
}
void delete_sensor_timers(timer_t timer_ids[], int sensors_mask) {

    for (int i = 0; i < NUM_SENSORS; i++) {
        // skip disabled sensors
        if ( !(sensors_mask & (1 << i)) ) continue;

        timer_delete(timer_ids[i]);
    }
}

void *simulate_sensors(void* arg) {
    sensor_params_t *params = (sensor_params_t *)arg;
    timer_t timer_ids[NUM_SENSORS];

    create_sensor_timers(timer_ids, params);

    while ( !atomic_load(&shared_memory.shutdown) );

    pthread_mutex_unlock(&shared_memory.mutex);

    delete_sensor_timers(timer_ids, params->sensors_enabled);

    pthread_exit(0);
}
