/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 *
 * Description:
 *
 */

#include "time_translation.h"
#include <assert.h>

/// @brief variables to determine the clock row which will sequentially be turned on.
static const uint32_t WORD_CLOCK_ROW_0 = 1 << 24;
static const uint32_t WORD_CLOCK_ROW_1 = 1 << 23;
static const uint32_t WORD_CLOCK_ROW_2 = 1 << 22;
static const uint32_t WORD_CLOCK_ROW_3 = 1 << 21;
static const uint32_t WORD_CLOCK_ROW_4 = 1 << 20;
static const uint32_t WORD_CLOCK_ROW_5 = 1 << 19;
static const uint32_t WORD_CLOCK_ROW_6 = 1 << 18;
static const uint32_t WORD_CLOCK_ROW_7 = 1 << 17;
static const uint32_t WORD_CLOCK_ROW_8 = 1 << 16;
static const uint32_t WORD_CLOCK_ROW_9 = 1 << 15;

/// @brief variable for the clock words to be displayed (horizontal positions of corresponding LED's)
// clang-format off
// format:
//   n.a.  |     row     |    column   | min
// 0000 000 0 0000 0000 0 000 0000 0000 0000

static const uint32_t COLUMN_MINUTE_MASK = 0x7FFF; // masks all colums and the minutes because they are low active.
static const uint32_t ROW_MASK = 0x1FF8000;

static const uint32_t SHOW_ES_IST     = 0b11011100000 << 4;
static const uint32_t SHOW_FIRST_FOUR = 0b11110000000 << 4;
static const uint32_t SHOW_LAST_FOUR  = 0b00000001111 << 4;
static const uint32_t SHOW_TEST       = 0b00000011111 << 4;
static const uint32_t SHOW_SECHS      = 0b11111000000 << 4;
static const uint32_t SHOW_SIEBEN     = 0b11111100000 << 4;
static const uint32_t SHOW_NEUN       = 0b00011110000 << 4;
static const uint32_t SHOW_ELF        = 0b00000111000 << 4;
static const uint32_t SHOW_ZWOELF     = 0b00000011111 << 4;
static const uint32_t SHOW_LAST_SEVEN = 0b00001111111 << 4;
static const uint32_t SHOW_VOR        = 0b11100000000 << 4;
static const uint32_t SHOW_UHR        = 0b00000000111 << 4;
// static const uint32_t SHOW_FUNK    = 0b00011110000 << 4;

static const uint32_t ONE_MINUTE    = 0b1000 | WORD_CLOCK_ROW_0;
static const uint32_t TWO_MINUTES   = 0b0100 | WORD_CLOCK_ROW_1;
static const uint32_t THREE_MINUTES = 0b0010 | WORD_CLOCK_ROW_8;
static const uint32_t FOUR_MINUTES  = 0b0001 | WORD_CLOCK_ROW_9;

// clang-format on


void translate_time_to_led_positions(const HourAndMinute* hour_minute, uint32_t* t_rows_to_write)
{
    const uint8_t hour = hour_minute->hour;
    const uint8_t minute = hour_minute->minute;
    assert(hour < 24);
    assert(minute < 60);
    for (uint8_t row = 0; row < WORD_CLOCK_MAX_ROWS; ++row)
    {
        t_rows_to_write[row] = 0;
    }

    // minutes
    switch (minute)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_ES_IST;
        t_rows_to_write[9] |= WORD_CLOCK_ROW_9 | SHOW_UHR;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_LAST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        t_rows_to_write[1] |= WORD_CLOCK_ROW_1 | SHOW_FIRST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        t_rows_to_write[2] |= WORD_CLOCK_ROW_2 | SHOW_LAST_SEVEN;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        t_rows_to_write[1] |= WORD_CLOCK_ROW_1 | SHOW_LAST_SEVEN;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_LAST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_VOR;
        t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_FIRST_FOUR;
        break;
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_ES_IST;
        t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_FIRST_FOUR;
        break;
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_LAST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_FIRST_FOUR;
        break;
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
        t_rows_to_write[1] |= WORD_CLOCK_ROW_1 | SHOW_FIRST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_LAST_FOUR;
        t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_FIRST_FOUR;
        break;
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
        t_rows_to_write[2] |= WORD_CLOCK_ROW_2 | SHOW_LAST_SEVEN;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_VOR;
        break;
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
        t_rows_to_write[1] |= WORD_CLOCK_ROW_1 | SHOW_FIRST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_VOR;
        break;
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
        t_rows_to_write[0] |= WORD_CLOCK_ROW_0 | SHOW_LAST_FOUR;
        t_rows_to_write[3] |= WORD_CLOCK_ROW_3 | SHOW_VOR;
        break;
    default:
        break;
    }

    if (minute < 25)
    {
        switch (hour)
        {
        case 0:
        case 12:
            t_rows_to_write[8] |= WORD_CLOCK_ROW_8 | SHOW_ZWOELF;
            break;
        case 1:
        case 13:
            t_rows_to_write[5] |= WORD_CLOCK_ROW_5 | SHOW_FIRST_FOUR;
            break;
        case 2:
        case 14:
            t_rows_to_write[5] |= WORD_CLOCK_ROW_5 | SHOW_LAST_FOUR;
            break;
        case 3:
        case 15:
            t_rows_to_write[6] |= WORD_CLOCK_ROW_6 | SHOW_FIRST_FOUR;
            break;
        case 4:
        case 16:
            t_rows_to_write[6] |= WORD_CLOCK_ROW_6 | SHOW_LAST_FOUR;
            break;
        case 5:
        case 17:
            t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_TEST;
            break;
        case 6:
        case 18:
            t_rows_to_write[7] |= WORD_CLOCK_ROW_7 | SHOW_SECHS;
            break;
        case 7:
        case 19:
            t_rows_to_write[8] |= WORD_CLOCK_ROW_8 | SHOW_SIEBEN;
            break;
        case 8:
        case 20:
            t_rows_to_write[7] |= WORD_CLOCK_ROW_7 | SHOW_LAST_FOUR;
            break;
        case 9:
        case 21:
            t_rows_to_write[9] |= WORD_CLOCK_ROW_9 | SHOW_NEUN;
            break;
        case 10:
        case 22:
            t_rows_to_write[9] |= WORD_CLOCK_ROW_9 | SHOW_FIRST_FOUR;
            break;
        case 11:
        case 23:
            t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_ELF;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (hour)
        {
        case 0:
        case 12:
            t_rows_to_write[5] |= WORD_CLOCK_ROW_5 | SHOW_FIRST_FOUR;
            break;
        case 1:
        case 13:
            t_rows_to_write[5] |= WORD_CLOCK_ROW_5 | SHOW_LAST_FOUR;
            break;
        case 2:
        case 14:
            t_rows_to_write[6] |= WORD_CLOCK_ROW_6 | SHOW_FIRST_FOUR;
            break;
        case 3:
        case 15:
            t_rows_to_write[6] |= WORD_CLOCK_ROW_6 | SHOW_LAST_FOUR;
            break;
        case 4:
        case 16:
            t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_TEST;
            break;
        case 5:
        case 17:
            t_rows_to_write[7] |= WORD_CLOCK_ROW_7 | SHOW_SECHS;
            break;
        case 6:
        case 18:
            t_rows_to_write[8] |= WORD_CLOCK_ROW_8 | SHOW_SIEBEN;
            break;
        case 7:
        case 19:
            t_rows_to_write[7] |= WORD_CLOCK_ROW_7 | SHOW_LAST_FOUR;
            break;
        case 8:
        case 20:
            t_rows_to_write[9] |= WORD_CLOCK_ROW_9 | SHOW_NEUN;
            break;
        case 9:
        case 21:
            t_rows_to_write[9] |= WORD_CLOCK_ROW_9 | SHOW_FIRST_FOUR;
            break;
        case 10:
        case 22:
            t_rows_to_write[4] |= WORD_CLOCK_ROW_4 | SHOW_ELF;
            break;
        case 11:
        case 23:
            t_rows_to_write[8] |= WORD_CLOCK_ROW_8 | SHOW_ZWOELF;
            break;
        default:
            break;
        }
    }
    switch (minute % 5)
    {
    case 1:
        t_rows_to_write[0] |= ONE_MINUTE;
        break;
    case 2:
        t_rows_to_write[0] |= ONE_MINUTE;
        t_rows_to_write[1] |= TWO_MINUTES;
        break;
    case 3:
        t_rows_to_write[0] |= ONE_MINUTE;
        t_rows_to_write[1] |= TWO_MINUTES;
        t_rows_to_write[8] |= THREE_MINUTES;
        break;
    case 4:
        t_rows_to_write[0] |= ONE_MINUTE;
        t_rows_to_write[1] |= TWO_MINUTES;
        t_rows_to_write[8] |= THREE_MINUTES;
        t_rows_to_write[9] |= FOUR_MINUTES;
        break;
    default:
        break;
    }

    for (uint8_t row = 0; row < WORD_CLOCK_MAX_ROWS; ++row)
    {
        uint32_t correct_led_binary = (t_rows_to_write[row] & ROW_MASK) | (~t_rows_to_write[row] & COLUMN_MINUTE_MASK);
        t_rows_to_write[row] = correct_led_binary;
    }
}