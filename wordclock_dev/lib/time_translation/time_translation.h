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

/// @brief variables to determine the qlock row which will sequentially be turned on.
#define QLOCK_ROW_0 1 << 24
#define QLOCK_ROW_1 1 << 23
#define QLOCK_ROW_2 1 << 22
#define QLOCK_ROW_3 1 << 21
#define QLOCK_ROW_4 1 << 20
#define QLOCK_ROW_5 1 << 19
#define QLOCK_ROW_6 1 << 18
#define QLOCK_ROW_7 1 << 17
#define QLOCK_ROW_8 1 << 16
#define QLOCK_ROW_9 1 << 15
#define MAX_ROWS 10

/// @brief variable for the qlock words to be displayed (horizontal positions of corresponding LED's)
// clang-format off
#define ES_IST  0b00100011111 << 4
#define EINS    0b00001111111 << 4
#define ZWEI    0b11111110000 << 4
#define DREI    0b00001111111 << 4
#define VIER    0b11111110000 << 4
#define FUENF   0b11111110000 << 4
#define SECHS   0b00000111111 << 4
#define SIEBEN  0b00000011111 << 4
#define ACHT    0b11111110000 << 4
#define NEUN    0b11100001111 << 4
#define ZEHN    0b00001111111 << 4
#define ELF     0b11111000111 << 4
#define ZWOELF  0b11111100000 << 4
#define ZWANZIG 0b11110000000 << 4
#define VIERTEL 0b11110000000 << 4
#define HALB    0b00001111111 << 4
#define VOR     0b00011111111 << 4
#define NACH    0b11111110000 << 4
#define UHR     0b11111111000 << 4
// clang-format on

/// @brief bits for one column / row / minute combination to be displayed
#define QLOCK_WORD_LENGTH 25

void translate_time_to_led_positions(int t_hour, int t_minute, uint32_t* t_rows_to_write);

#endif // TIME_TRANSLATION_H

