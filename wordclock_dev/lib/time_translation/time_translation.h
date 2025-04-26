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

/// @brief Horizontal rows of the word clock.
#define WORD_CLOCK_MAX_ROWS 10
/// @brief 25 Bits need to be written for a word.
/// 0000 00rr rrrr rrrr rrrr rrrr rrrr rrr0
/// r means occupied by a register
/// -> needs to be shifted 31 times, to the mostly last bit
#define WORD_CLOCK_WORD_LENGTH 25

typedef struct HourAndMinute
{
  uint8_t hour;
  uint8_t minute;
} HourAndMinute;

void translate_time_to_led_positions(const HourAndMinute* hour_minute, uint32_t* t_rows_to_write);

#endif // TIME_TRANSLATION_H

