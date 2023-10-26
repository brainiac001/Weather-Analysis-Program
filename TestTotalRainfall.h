#include <stdio.h>
#include <stdlib.h>
#include "rainfall.h"

// Test the total_rainfall function
void test_total_rainfall()
{
    // Test data
    int rainfall_data[MAX_ROWS][MAX_COLS] = {
        {2019, 40, 45, 42, 50, 62, 68, 75, 79, 82, 88, 72, 45},  // Year 2019
        {2020, 30, 28, 35, 45, 55, 60, 72, 78, 80, 85, 65, 38}, // Year 2020
        {2021, 25, 32, 40, 50, 60, 70, 75, 80, 85, 90, 70, 40} // Year 2021
        //  more test data can be added here as needed
    };

    int data_count = 3; // Number of data points in the test data

    // Test years 
    int test_years[] = {2021, 2020, 2019};

    // Expected total rainfall for the test years
    int expected_results[] = {717, 671, 748};

    for (int i = 0; i < data_count; i++)
    {
        int test_year = test_years[i];
        int expected_total_rainfall = expected_results[i];

        // Calculate actual result
        int actual_total_rainfall = total_rainfall(test_year, rainfall_data);

        // Check if the test passed
        if (actual_total_rainfall == expected_total_rainfall)
        {
            printf("Test passed for year %d: Total rainfall is %d mm.\n", test_year, actual_total_rainfall);
        }
        else
        {
            printf("Test failed for year %d: Expected %d mm, but got %d mm.\n", test_year, expected_total_rainfall, actual_total_rainfall);
        }
    }
}
