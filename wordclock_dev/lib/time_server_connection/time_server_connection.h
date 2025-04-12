/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 * 
 * Description: 
 */

#ifndef TIME_SERVER_CONNECTION_H
#define TIME_SERVER_CONNECTION_H
#include <stddef.h>
#include "time_translation.h"

void get_current_time_with_description(char* date_time, size_t buffer_size, HourAndMinute* hour_minute);
void get_current_time(HourAndMinute* hour_minute);

void set_system_time();

#endif // TIME_SERVER_CONNECTION_H
