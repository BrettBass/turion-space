#include "../include/utils.h"
#include "../include/shared_memory.h"
#include "../include/moving_average.h"
#include <stdio.h>
#include <getopt.h>
#include <string.h>

void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void cleanup(int mask, float** alloc_buffers) {
    // Free sensor buffers
    for (int i = 0; i < NUM_SENSORS; i++) {
        if ( !(mask & (1 << i)) ) continue;
        free(alloc_buffers[i]);
    }

    pthread_mutex_lock(&shared_memory.mutex);
    pthread_mutex_unlock(&shared_memory.mutex);
    pthread_mutex_destroy(&shared_memory.mutex);

    printf("Cleanup complete.\n");
}

void initialize_buffers(float* buffer[], int window_size, int sensor_mask) {
    for (int i = 0; i < NUM_SENSORS; i++) {
        if ( !(sensor_mask & (1 << i)) ) continue;

        buffer[i] = malloc(window_size * sizeof(float));
        if (!buffer[i]) {
            // Free already allocated buffers before exiting
            for (int j = 0; j < i; j++) {
                free(buffer[j]);
            }
            die("Error: Failed to allocate memory for sensor buffer\n");
        }
    }
}

// Function to validate that a positive integer is provided
int parse_positive_int(const char *str) {
    char *endptr;
    long value = strtol(str, &endptr, 10);

    if (*endptr != '\0' || value <= 0) {
        return -1; // Invalid input
    }
    return (int)value;
}
// Function to print help message
void print_help(const char* program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Simulate sensor data with moving average processing.\n\n");

    printf("Options:\n");
    printf("  -s, --sensors <sensor_mask_hex>   Specify the sensor mask in hex, 6 bits (e.g., 21 for 0x21, sensors 6 and 1)(default=3F)\n");
    printf("  -r, --rate <r1,r2,...>            Specify comma-separated sampling rates (milliseconds) for enabled sensors.(default=100)\n");
    printf("  -w, --window <window_size>        Specify the window size for moving average.(default=5)\n");
    printf("  -t, --runtime <runtime_sec>       Specify the runtime in seconds. (default=3)\n");
    printf("  -h, --help                        Display this help and exit.\n");

    printf("\nExamples:\n");
    printf("  %s -s 21 -r 100,200 -w 5 -t 2\n", program_name);
    printf("  %s --sensors 21 --rate 100,200 --window 5 --runtime 2\n", program_name);
    printf("\nThis program simulates sensor data with configurable sampling rates, window size, and runtime.\n");
    printf("It calculates a moving average over the specified window size and outputs the results for each enabled sensor.\n");
}
// Function to parse command line arguments
void parse_arguments(int argc, char* argv[], sensor_params_t* sensor_params, moving_average_params_t* ma_params, int* runtime_sec) {
    int opt;
    int sensor_count = 0;

    // Default values for sensor parameters
    sensor_params->sensors_enabled = 0x3F;
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensor_params->sampling_rates_ms[i] = 100;
    }
    ma_params->window_size = 5;

    static struct option long_options[] = {
        {"sensors", required_argument, NULL, 's'},
        {"rates",   required_argument, NULL, 'r'},
        {"window",  required_argument, NULL, 'w'},
        {"runtime", required_argument, NULL, 't'},
        {"help",    no_argument,       NULL, 'h'},
        {0, 0, 0, 0}  // End of options
    };

    // Flag to ensure -s is parsed before -r
    int mask_parsed = 0;
    char* sampling_rates_token;

    // Process command line arguments
    while ((opt = getopt_long(argc, argv, "hs:r:w:t:", long_options, NULL)) != -1) {
        switch (opt) {
            case 's':  // sensor mask (hex)
                sensor_params->sensors_enabled = (uint8_t)strtol(optarg, NULL, 16) & 0x3F;
                mask_parsed = 1; // Mark that mask has been parsed
                sensor_count = __builtin_popcount(sensor_params->sensors_enabled);
                break;
            case 'r':  // sampling rate for enabled sensors
                if (!mask_parsed) {
                    die("You must provide the sensor mask (-s) before the sampling rates (-r).");
                }
                sampling_rates_token = strtok(optarg, ",");
                for (int i = 0; i < NUM_SENSORS; i++) {
                    if ( !(sensor_params->sensors_enabled & (1<<i)) ) continue;

                    sensor_params->sampling_rates_ms[i] = parse_positive_int(sampling_rates_token);

                    if (sensor_params->sampling_rates_ms[i] == -1)
                        die("Invalid sampling rate. It must be a positive integer.");

                    sensor_count--;
                    sampling_rates_token = strtok(NULL, ",");

                    if (sampling_rates_token == NULL) {
                        if(sensor_count > 0)
                            die("Invalid amount of sampling rates");

                        break;
                    }
                }
                break;
            case 'w':  // window size for moving average
                ma_params->window_size = parse_positive_int(optarg);
                if (ma_params->window_size == -1) {
                    die("Invalid window size. It must be a positive integer.");
                }
                break;
            case 't':
                *runtime_sec = parse_positive_int(optarg);
                if (*runtime_sec == -1)
                    die("Invalid runtime argument\n");
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else
                    fprintf (stderr, "Try '%s --help' for more information.\n", argv[0]);
                die("");
            case 'h':
            default:
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
        }
    }

}
