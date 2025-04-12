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

int main()
{
    uint32_t test_array[10] = {0,0,0,0,0,0,0,0,0,0};
    HourAndMinute the_time = {.hour = 12, .minute = 18};
    translate_time_to_led_positions(&the_time, test_array);
    for (uint8_t pos = 0; pos < WORD_CLOCK_MAX_ROWS; ++pos)
    {
        printf("Row %d: ", pos);
        uint32_to_binary(test_array[pos]);
    }
    return 0;
}