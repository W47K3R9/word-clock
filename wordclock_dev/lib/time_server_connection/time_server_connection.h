/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 * 
 * Description: 
 */

#ifndef TIME_SERVER_CONNECTION_H
#define TIME_SERVER_CONNECTION_H
#include <stddef.h>

void get_current_time(char* date_time, size_t buffer_size, int* t_min_and_hour);

void set_system_time();

#endif // TIME_SERVER_CONNECTION_H
