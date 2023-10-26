#ifndef RAINDFALL_H
#define RAINDFALL_H

#define MAX_ROWS 25                  // Rows in array - one row per year of data
#define MAX_COLS 13                  // Columns in array (year + 12 months = 13)

int total_rainfall(int year, int the_data[MAX_ROWS][MAX_COLS]);

#endif
