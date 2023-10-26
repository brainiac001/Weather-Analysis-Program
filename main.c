#include <stdio.h>
#include <stdlib.h>
#include "rainfall.h"
#include "TestTotalRainfall.h"


// Switch constants for main menus
#define LOAD_DATA 1
#define DISPLAY_DATA 2
#define ANALYSE_DATA 3
#define EXIT 4

// Switch constants for Analysis sub-menu
#define SINGLE_YEAR_STATS 1
#define MULTI_YEAR_STATS 2
#define SORTED_LIST 3
#define MONTH_EXTREMES 4
#define VARIANCE_FROM_AVERAGE 5
#define SUB_EXIT 6

// Global constants
#define MAX_ROWS 25                  // Rows in array - one row per year of data
#define MAX_COLS 13                  // Columns in array (year + 12 months = 13)
#define DATA_FILE "rainfalldata.txt" // Data file name

// Boolean constants
#define TRUE 1
#define FALSE 0

// Structure to hold statistics for a year.
struct Single_Year_Stats
{
    int year;        // Year for stats
    int min_rain;    // Min rainfall for year
    int min_monthNo; // Month number for minimum
    int max_rain;    // Max rainfall for the year
    int max_monthNo; // Month number for maximum
    int total_rain;  // Total annual rainfall
    float mean_rain; // Mean monthly rainfall
};

// Function to validate the menu input from stdin
int check_input(int min, int max)
{
    int input;
    char ch;

    // Check valid assignment and range
    while (scanf("%d", &input) != 1 || (input < min || input > max))
    {
        while ((ch = getchar()) != '\n')
            ; // Flush the buffer
        printf("*** Invalid data: Enter a value between %d and %d: ", min, max);
    }

    return input;
}

// Function to read the data into an array
int read_data(int the_data[MAX_ROWS][MAX_COLS])
{
    int *p = &the_data[0][0]; // Pointer to the start of the array
    int data_count = 0;       // Count of data items read

    // Check if the file exists and open it in read mode
    FILE *fptr = fopen(DATA_FILE, "r");

    if (fptr == NULL)
    {
        printf("\n*** Error: Not able to open the file.\n");
        exit(0);
    }
    else
    {
        printf("\nReading file\n");
        int number; // Input variable for readability

        while (fscanf(fptr, "%d,", &number) != EOF)
        {
            *p = number; // Put the number in the next element of the array
            p++;         // Point to the next element of the array
            data_count++;
        }

        fclose(fptr);
    }

    return data_count;
}

// Function to print the data in the array to stdout
void print_data(int the_data[MAX_ROWS][MAX_COLS], int data_count)
{
    int *p = &the_data[0][0]; // Pointer to the start of the array

    printf("Year Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ");

    for (int i = 0; i < data_count; i++)
    {
        if ((i % MAX_COLS == 0))
        {
            printf("\n");
        } // New line for each year's data

        printf("%4d", *p);
        p++;
    }

    printf("\n-----------------\n");
}

// Function to save the data in the array to a file
void save_data(int the_data[MAX_ROWS][MAX_COLS], int data_count)
{
    int *p = &the_data[0][0]; // Pointer to the start of the array
    FILE *fptr;
    fptr = fopen(DATA_FILE, "w"); // Open a file in writing mode

    if (fptr == NULL)
    { // Check for success
        printf("\n*** Error: Not able to open the file.\n");
        exit(0);
    }

    // Loop through all data and save it in a text file with new lines at the end of each year
    for (int i = 0; i < data_count; i++)
    {
        if ((i % MAX_COLS == 0))
        {
            fprintf(fptr, "\n");
        }

        fprintf(fptr, "%4d", *p);
        p++;
    }

    fclose(fptr);
}

// Function to input and validate a single year within the range of data 
int get_valid_year(int the_data[MAX_ROWS][MAX_COLS], int data_count)
{
    int years = data_count / MAX_COLS;     // Find how many full years' data are in the array
    int min_year = the_data[0][0];         // First year is the first element in the array
    int max_year = the_data[years - 1][0]; // Last full year
    printf("Enter a year between %d and %d and press Enter: ", min_year, max_year);
    int the_year;
    scanf("%d", &the_year);

    while (the_year < min_year || the_year > max_year)
    { // Loop until the year is in range
        printf(" \n *** Invalid year %d ***\n", the_year);
        printf("Enter a year between %d and %d and press Enter: ", min_year, max_year);
        scanf("%d", &the_year);
    }

    return the_year;
}

// Function to return total rainfall for a given year
int total_rainfall(int year, int the_data[MAX_ROWS][MAX_COLS])
{
    int r = year - the_data[0][0]; // Get the row index in the array for the chosen year
    int running_total = 0;         // Total rainfall

    for (int m = 1; m < MAX_COLS; m++)
    { // Sum data in this row
        running_total = running_total + the_data[r][m];
    }

    return running_total;
}

// Function for Single Year Statistics
void single_year_statistics(int rainfall[MAX_ROWS][MAX_COLS], int data_count, char month_names[12][4])
{
    int the_year = get_valid_year(rainfall, data_count);
    int r = the_year - rainfall[0][0];
    int min = 1000, max = 0, sum = 0, min_month = 0, max_month = 0;
    struct Single_Year_Stats single_year_results;

    printf("\n***** Single year statistics for year %d *****\n", the_year);
    printf("Year    Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec\n");

    for (int m = 1; m < MAX_COLS; m++)
    {
        printf("%4d ", rainfall[r][m]);

        if (rainfall[r][m] < min)
        {
            min = rainfall[r][m];
            min_month = m;
        }

        if (rainfall[r][m] > max)
        {
            max = rainfall[r][m];
            max_month = m;
        }

        sum += rainfall[r][m];
    }

    single_year_results.total_rain = sum;
    single_year_results.mean_rain = (float)sum / 12;
    single_year_results.min_rain = min;
    single_year_results.min_monthNo = min_month;
    single_year_results.max_rain = max;
    single_year_results.max_monthNo = max_month;

    printf("\n\nMinimum %d mm rainfall in month %s\n", single_year_results.min_rain, month_names[min_month - 1]);
    printf("Maximum %d mm rainfall in month %s\n", single_year_results.max_rain, month_names[max_month - 1]);
    printf("Mean monthly rainfall: %.1f mm\nTotal annual rainfall: %d mm\n\n", single_year_results.mean_rain, single_year_results.total_rain);
}

// Function for Multi-Year Statistics
void multi_year_statistics(int rainfall[MAX_ROWS][MAX_COLS], int data_count)
{
    int year1 = get_valid_year(rainfall, data_count);
    int year2 = get_valid_year(rainfall, data_count);
    int year, max_year, min_year;
    int min = 10000, max = 0;
    int multi_year_total = 0;
    float mean;

    if (year2 <= year1)
    {
        printf("\n***Error: the second year must be greater than the first year\n");
        exit(0);
    }

    printf("Enter a range of years to analyze: %d to %d\n", year1, year2);
    printf("Total Rainfall by year\n");

    for (year = year1; year <= year2; year++)
    {
        int sum = total_rainfall(year, rainfall);
        multi_year_total += sum;

        if (sum < min)
        {
            min = sum;
            min_year = year;
        }

        if (sum > max)
        {
            max = sum;
            max_year = year;
        }

        printf("%d %dmm\n", year, sum);
    }

    printf("Minimum %d mm rainfall in year %d\n", min, min_year);
    printf("Maximum %d mm rainfall in year %d\n", max, max_year);
    mean = (float)multi_year_total / (year2 - year1 + 1);
    printf("Mean annual rainfall: %.1f mm\n", mean);
}

// Function for Sorted List
void sorted_list(int rainfall[MAX_ROWS][MAX_COLS], int data_count)
{
    int year_rainfall[MAX_ROWS][2];   // To hold year and annual rainfall
    int r = 0;
    int year1 = get_valid_year(rainfall, data_count);
    int year2 = get_valid_year(rainfall, data_count);

    if (year2 <= year1)
    {
        printf("\n***Error: the second year must be greater than the first year\n");
        exit(0);
    }

    int num_years = year2 - year1 + 1;

    // Calculate total rainfall and populate the array
    for (int year = year1; year <= year2; year++)
    {
        year_rainfall[r][0] = year;
        year_rainfall[r][1] = total_rainfall(year, rainfall);
        r++;
    }

    // Sort the rainfall data (insertion sort)
    for (int i = 1; i < num_years; i++)
    {
        int key = year_rainfall[i][1];
        int year = year_rainfall[i][0];
        int j = i - 1;

        while (j >= 0 && year_rainfall[j][1] > key)
        {
            year_rainfall[j + 1][1] = year_rainfall[j][1];
            year_rainfall[j + 1][0] = year_rainfall[j][0];
            j = j - 1;
        }

        year_rainfall[j + 1][1] = key;
        year_rainfall[j + 1][0] = year;
    }

    printf("A sorted list of years and total rainfall between %d and %d:\n", year1, year2);
    for (int i = 0; i < num_years; i++)
    {
        printf("Year = %d, Rainfall = %d\n", year_rainfall[i][0], year_rainfall[i][1]);
    }
}

// Function for Month Extremes
void month_extremes(int rainfall[MAX_ROWS][MAX_COLS], int data_count, char month_names[12][4])
{
    int year1 = get_valid_year(rainfall, data_count);
    int year2 = get_valid_year(rainfall, data_count);
    int min = 1000, max = 0;
    int min_month = 0, max_month = 0;
    int min_year = 0, max_year = 0;
    int r = year1 - rainfall[0][0];

    if (year2 <= year1)
    {
        printf("\n***Error: the second year must be greater than the first year\n");
        exit(0);
    }

    for (int year = year1; year <= year2; year++)
    {
        for (int m = 1; m < MAX_COLS; m++)
        {
            if (rainfall[r][m] < min)
            {
                min = rainfall[r][m];
                min_month = m;
                min_year = year;
            }

            if (rainfall[r][m] > max)
            {
                max = rainfall[r][m];
                max_month = m;
                max_year = year;
            }
        }
        r++;
    }

    printf("For a given range, the maximum and minimum monthly rainfall with year/month:\n");
    printf("Minimum %d mm rainfall in month %s of %d\n", min, month_names[min_month - 1], min_year);
    printf("Maximum %d mm rainfall in month %s of %d\n", max, month_names[max_month - 1], max_year);
}

// Function for Variance from Average
void variance_from_average(int rainfall[MAX_ROWS][MAX_COLS], int data_count)
{
    int the_year = get_valid_year(rainfall, data_count);
    int the_year_total_rainfall = total_rainfall(the_year, rainfall);
    int year1 = rainfall[0][0];
    int year2 = rainfall[(data_count / MAX_COLS) - 1][0];
    int multi_year_total = 0;

    for (int year = year1; year <= year2; year++)
    {
        multi_year_total += total_rainfall(year, rainfall);
    }

    float long_term_average_rainfall = (float)multi_year_total / (float)(year2 - year1 + 1);

    printf("Variance of rainfall in year %d from the long-term average:\n", the_year);
    printf("The long-term average rainfall from %d to %d is: %.1f mm\n", year1, year2, long_term_average_rainfall);
    printf("The Rainfall in year %d is %d mm, which is %.1f %% of the long-term average\n", the_year, the_year_total_rainfall, 100.0 * (float)the_year_total_rainfall / long_term_average_rainfall);
}

// Main Program
int main()
{
    /*
        // Run the test for total rainfall
        printf("Testing the total_rainfall function...\n");
        test_total_rainfall(); 
    */

    char month_names[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int data_count = 0; // Global variable for the number of data points (year + monthly rainfall)
    int rainfall[MAX_ROWS][MAX_COLS]; // Each row represents a year. Format [Year, Jan, Feb... Dec]

    int choice = 0; // Menu selection variable

    while (choice != EXIT)
    {
        // Menu display
        printf("Rainfall Analysis Menu\n\n");
        printf("1. Load Data from file\n2. Display Data\n3. Analyze Data\n4. Exit\n");
        printf("Enter your choice:");
        choice = check_input(LOAD_DATA, EXIT); // Validate input

        switch (choice)
        {
            case LOAD_DATA:
                printf("> Load Data\n");
                data_count = read_data(rainfall);
                break;
            case DISPLAY_DATA:
                printf("> Display Data \n");

                if (data_count == 0)
                {
                    printf("\n*** No data loaded *** \n\n");
                }
                else
                {
                    print_data(rainfall, data_count);
                }

                break;
            case ANALYSE_DATA:; // Analysis options (sub-menu)
                 if (data_count == 0)
                {
                    printf("\n*** No data loaded *** \n\n");
                }else{
 int analysis_choice = 0; // Switch variable

                while (analysis_choice != SUB_EXIT)
                {
                    // Sub-menu
                    printf("\n***** Analyze Data *****\n\n Choose one of the following options:\n");
                    printf("1. Single year statistics\n2. Multi-year statistics\n3. Sorted list in range of years\n");
                    printf("4. Extremes of monthly rainfall in a period\n5. Given year variance from long term mean\n6. Exit\n");
                    printf("\nEnter your choice:");
                    analysis_choice = check_input(SINGLE_YEAR_STATS, SUB_EXIT); // Validate input

                    switch (analysis_choice)
                    {
                        case SINGLE_YEAR_STATS:
                            single_year_statistics(rainfall, data_count, month_names);
                            break;
                        case MULTI_YEAR_STATS:
                            multi_year_statistics(rainfall, data_count);
                            break;
                        case SORTED_LIST:
                            sorted_list(rainfall, data_count);
                            break;
                        case MONTH_EXTREMES:
                            month_extremes(rainfall, data_count, month_names);
                            break;
                        case VARIANCE_FROM_AVERAGE:
                            variance_from_average(rainfall, data_count);
                            break;
                        case SUB_EXIT:
                            printf("\n Analysis menu Exit\n");
                            break;
                        default: // Should never get here
                            break;
                    }
                }
                }
               
                break;
            case EXIT:
                printf("Exit\n");
                break;
            default: // Should never get here
                break;
        }
    }

    return 0;
}
