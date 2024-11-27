
//void print_avg(int window_size) {
//    float avg;
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//    for (int i = 0; i < NUM_SENSORS; i++) {
//        avg = 0;
//        printf("sensor %d {", i + 1);
//        for(int j = 0; j < window_size; j++) {
//            avg += averages[i][j];
//            printf(" %0.2f", averages[i][j]);
//        }
//        avg /= window_size;
//        printf(" } -- avg =  %0.2f\n", avg);
//    }
//    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//}
#include "../include/output.h"


void print_avg(const float* averages, const int window_size) {

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    for (int i = 0; i < NUM_SENSORS; i++) {

        // Scale the average to fit within the bar's max length
        int bar_length = (int)((averages[i] / MAX_SENSOR_VALUE) * MAX_BAR_LENGTH);

        // Print sensor data and moving average
        printf("Sensor %d: Moving Average = %5.2f ", i + 1, averages[i]);

        // Print the bar graph
        printf("| ");
        for (int j = 0; j < bar_length; j++) {
            printf("█");  // Print one block for each unit in the scaled bar
        }
        printf("\n\n");
    }

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
