/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 * 
 * Description: Create a translation between a current set of mm:hh and produce a bit field out of it.
 *
 */

#ifndef TIME_TRANSLATION_H
#define TIME_TRANSLATION_H

#include "stdint.h"

void translate_time_to_led_positions(int t_hour, int t_minute, uint32_t* t_rows_to_write);

#endif // TIME_TRANSLATION_H

