#include "../include/output.h"



void print_avg(float average, const int window_size, int index) {

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    // Scale the average to fit within the bar's max length
    int bar_length = (int)((average / MAX_SENSOR_VALUE) * MAX_BAR_LENGTH);

    // Print sensor data and moving average
    printf("Sensor %d: Moving Average = %5.2f ", index + 1, average);

    // Print the bar graph
    printf("| ");
    for (int j = 0; j < bar_length; j++) {
        printf("█");  // Print one block for each unit in the scaled bar
    }
    printf("\n\n");

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
