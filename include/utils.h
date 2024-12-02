/**
 * @file utils.h
 * @brief Utility functions either used in main or all over the program
 *
 * This file contains utility functions that support the main application, such as memory management,
 * handling command-line arguments, and initializations.
 *
 * These utilities simplify are ment to modulate aspects of code to imporove readability and teardown
 * of the sensor simulation program.
 *
 * @author Brett Bass
 */
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "sensor_simulator.h"
#include "moving_average.h"

/**
 * @brief This function prints an error message to `stderr` and exits the program with a failure status.
 *
 * @param msg Error message
 */
void die(const char *msg);

/**
 * @brief Cleans up allocated resources and mutexes.
 *
 * @param mask The bitmask indicating which sensors were enabled, used to free corresponding buffers.
 * @param alloc_buffers The array of sensor buffers to free.
 */
void cleanup(int mask, float** alloc_buffers);


/**
 * @brief Initializes sensor buffers with the specified window size.
 *
 * @param sensor_buffer The array of pointers to sensor buffer to initialize.
 * @param window_size The size of the moving average window for each sensor.
 * @param sensor_mask The bitmask indicating which sensors are enabled.
 */
void initialize_buffers(float* buffer[], int window_size, int sensor_mask);

/**
 * @brief Parses a string to extract a positive integer. returns -1 on failure
 *
 * @param str The string to parse.
 * @return The parsed positive integer, or -1 if the string is invalid.
 */
int parse_positive_int(const char *str);

/**
 * @brief Prints the help message for the program.
 *
 * Inspired by cat command written by Torbjorn Granlund and Richard M. Stallman.
 *
 * @param program_name The name of the program to display in the usage instructions.
 * @return None
 */
void print_help(const char* program_name);

/**
 * @brief Parses the command-line arguments.
 *
 * This function processes the command-line arguments using `getopt_long` to set user configuratble values
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param sensor_params Pointer to sensor parameters.
 * @param ma_params Pointer to moving average parameters.
 * @param runtime_sec Pointer to main processes runtime variable.
 * @return None
 */
void parse_arguments(int argc, char* argv[], sensor_params_t* sensor_params, moving_average_params_t* ma_params, int* runtime_sec);
#endif // UTILS_H
