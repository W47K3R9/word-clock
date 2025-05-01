/**
 * Author: Lucas Scheidt
 * Date: 12.04.25
 * 
 * Description: 
 */

#include "../lib/time_translation/time_translation.h"
#include "../lib/utils/pretty_printer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    uint32_t test_array[10] = {0,0,0,0,0,0,0,0,0,0};
    char* hour_char = argv[1];
    char* min_char = argv[2];
    char *endptr1;
    char *endptr2;
    long int hour, min;
    HourAndMinute the_time;
    // Convert the first command-line argument (string) to a long integer
    if (argc == 3)
    {
        hour = strtol(hour_char, &endptr1, 10);
        min = strtol(min_char, &endptr2, 10);
    	the_time.hour = (uint8_t)hour;
        the_time.minute = (uint8_t)min;
    }
    else
    {
        printf("Using default time 17:10");
        the_time.hour = 17;
        the_time.minute=10;
    }

    translate_time_to_led_positions(&the_time, test_array);
    for (uint8_t pos = 0; pos < WORD_CLOCK_MAX_ROWS; ++pos)
    {
        printf("Row %d: ", pos);
        uint32_to_binary(test_array[pos]);
    }
    return 0;
}