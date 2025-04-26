/**
 * Author: Lucas Scheidt
 * Date: 04.10.24
 * 
 * Description: Creates the desired LED patterns row by row.
 */

#ifndef SEQUENTIAL_TIME_DISPLAY_H
#define SEQUENTIAL_TIME_DISPLAY_H

void setup_pins_to_main_shift_register();

void activate_screen();

void display_time(void* t_rows_to_display);

#endif
