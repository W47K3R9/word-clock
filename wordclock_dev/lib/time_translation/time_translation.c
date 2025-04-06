/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 *
 * Description:
 *
 */

#include "time_translation.h"

/// @brief variables to determine the qlock row which will sequentially be turned on.
static const uint32_t QLOCK_ROW_0 = 1 << 24;
static const uint32_t QLOCK_ROW_1 = 1 << 23;
static const uint32_t QLOCK_ROW_2 = 1 << 22;
static const uint32_t QLOCK_ROW_3 = 1 << 21;
static const uint32_t QLOCK_ROW_4 = 1 << 20;
static const uint32_t QLOCK_ROW_5 = 1 << 19;
static const uint32_t QLOCK_ROW_6 = 1 << 18;
static const uint32_t QLOCK_ROW_7 = 1 << 17;
static const uint32_t QLOCK_ROW_8 = 1 << 16;
static const uint32_t QLOCK_ROW_9 = 1 << 15;
static const uint32_t MAX_ROWS = 10;

/// @brief variable for the qlock words to be displayed (horizontal positions of corresponding LED's)
// clang-format off
static const uint32_t MASK = 0x7FF0
static const uint32_t ES_IST  = ~(0b00100011111 << 4) & MASK;
static const uint32_t EINS    = ~(0b00001111111 << 4) & MASK;
static const uint32_t ZWEI    = ~(0b11111110000 << 4) & MASK;
static const uint32_t DREI    = ~(0b00001111111 << 4) & MASK;
static const uint32_t VIER    = ~(0b11111110000 << 4) & MASK;
static const uint32_t FUENF   = ~(0b11111110000 << 4) & MASK;
static const uint32_t SECHS   = ~(0b00000111111 << 4) & MASK;
static const uint32_t SIEBEN  = ~(0b00000011111 << 4) & MASK;
static const uint32_t ACHT    = ~(0b11111110000 << 4) & MASK;
static const uint32_t NEUN    = ~(0b11100001111 << 4) & MASK;
static const uint32_t ZEHN    = ~(0b00001111111 << 4) & MASK;
static const uint32_t ELF     = ~(0b11111000111 << 4) & MASK;
static const uint32_t ZWOELF  = ~(0b11111100000 << 4) & MASK;
static const uint32_t ZWANZIG = ~(0b11110000000 << 4) & MASK;
static const uint32_t VIERTEL = ~(0b11110000000 << 4) & MASK;
static const uint32_t HALB    = ~(0b00001111111 << 4) & MASK;
static const uint32_t VOR     = ~(0b00011111111 << 4) & MASK;
static const uint32_t NACH    = ~(0b11111110000 << 4) & MASK;
static const uint32_t UHR     = ~(0b11111111000 << 4) & MASK;
// clang-format on

/// @brief bits for one column / row / minute combination to be displayed
#define QLOCK_WORD_LENGTH 25

void translate_time_to_led_positions(int t_hour, int t_minute, uint32_t* t_rows_to_write)
{
    for (uint8_t row = 0; row < MAX_ROWS; ++row)
    {
        t_rows_to_write[row] = 0;
    }
    switch (t_minute)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        t_rows_to_write[0] |= QLOCK_ROW_0 | ES_IST;
        t_rows_to_write[9] |= QLOCK_ROW_9 | UHR;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        t_rows_to_write[0] |= QLOCK_ROW_0 | FUENF;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        t_rows_to_write[1] |= QLOCK_ROW_1 | ZEHN;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        t_rows_to_write[2] |= QLOCK_ROW_2 | VIERTEL;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        t_rows_to_write[1] |= QLOCK_ROW_1 | ZWANZIG;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        t_rows_to_write[0] |= QLOCK_ROW_0 | FUENF;
        t_rows_to_write[3] |= QLOCK_ROW_3 | VOR;
        t_rows_to_write[4] |= QLOCK_ROW_4 | HALB;
        break;
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        t_rows_to_write[0] |= QLOCK_ROW_0 | ES_IST;
        t_rows_to_write[4] |= QLOCK_ROW_4 | HALB;
        break;
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        t_rows_to_write[0] |= QLOCK_ROW_0 | FUENF;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        t_rows_to_write[4] |= QLOCK_ROW_4 | HALB;
        break;
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
        t_rows_to_write[1] |= QLOCK_ROW_1 | ZEHN;
        t_rows_to_write[3] |= QLOCK_ROW_3 | NACH;
        t_rows_to_write[4] |= QLOCK_ROW_4 | HALB;
        break;
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
        t_rows_to_write[2] |= QLOCK_ROW_2 | VIERTEL;
        t_rows_to_write[3] |= QLOCK_ROW_3 | VOR;
        break;
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
        t_rows_to_write[1] |= QLOCK_ROW_1 | ZEHN;
        t_rows_to_write[3] |= QLOCK_ROW_3 | VOR;
        break;
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
        t_rows_to_write[0] |= QLOCK_ROW_0 | FUENF;
        t_rows_to_write[3] |= QLOCK_ROW_3 | VOR;
        break;
    default:
        break;
    }

    if (t_minute < 25)
    {
        switch (t_hour)
        {
        case 0:
        case 12:
            t_rows_to_write[8] |= QLOCK_ROW_8 | ZWOELF;
            break;
        case 1:
        case 13:
            t_rows_to_write[5] |= QLOCK_ROW_5 | EINS;
            break;
        case 2:
        case 14:
            t_rows_to_write[5] |= QLOCK_ROW_5 | ZWEI;
            break;
        case 3:
        case 15:
            t_rows_to_write[6] |= QLOCK_ROW_6 | DREI;
            break;
        case 4:
        case 16:
            t_rows_to_write[6] |= QLOCK_ROW_6 | VIER;
            break;
        case 5:
        case 17:
            t_rows_to_write[4] |= QLOCK_ROW_4 | FUENF;
            break;
        case 6:
        case 18:
            t_rows_to_write[7] |= QLOCK_ROW_7 | SECHS;
            break;
        case 7:
        case 19:
            t_rows_to_write[8] |= QLOCK_ROW_8 | SIEBEN;
            break;
        case 8:
        case 20:
            t_rows_to_write[7] |= QLOCK_ROW_7 | ACHT;
            break;
        case 9:
        case 21:
            t_rows_to_write[9] |= QLOCK_ROW_9 | NEUN;
            break;
        case 10:
        case 22:
            t_rows_to_write[9] |= QLOCK_ROW_9 | ZEHN;
            break;
        case 11:
        case 23:
            t_rows_to_write[4] |= QLOCK_ROW_4 | ELF;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (t_hour)
        {
        case 0:
        case 12:
            t_rows_to_write[5] |= QLOCK_ROW_5 | EINS;
            break;
        case 1:
        case 13:
            t_rows_to_write[5] |= QLOCK_ROW_5 | ZWEI;
            break;
        case 2:
        case 14:
            t_rows_to_write[6] |= QLOCK_ROW_6 | DREI;
            break;
        case 3:
        case 15:
            t_rows_to_write[6] |= QLOCK_ROW_6 | VIER;
            break;
        case 4:
        case 16:
            t_rows_to_write[4] |= QLOCK_ROW_4 | FUENF;
            break;
        case 5:
        case 17:
            t_rows_to_write[7] |= QLOCK_ROW_7 | SECHS;
            break;
        case 6:
        case 18:
            t_rows_to_write[8] |= QLOCK_ROW_8 | SIEBEN;
            break;
        case 7:
        case 19:
            t_rows_to_write[7] |= QLOCK_ROW_7 | ACHT;
            break;
        case 8:
        case 20:
            t_rows_to_write[9] |= QLOCK_ROW_9 | NEUN;
            break;
        case 9:
        case 21:
            t_rows_to_write[9] |= QLOCK_ROW_9 | ZEHN;
            break;
        case 10:
        case 22:
            t_rows_to_write[4] |= QLOCK_ROW_4 | ELF;
            break;
        case 11:
        case 23:
            t_rows_to_write[8] |= QLOCK_ROW_8 | ZWOELF;
            break;
        default:
            break;
        }
    }

    switch (t_minute % 5)
    {
    case 0:
        break;
    case 1:
        t_rows_to_write[9] |= 0b1;
        break;
    case 2:
        t_rows_to_write[9] |= 0b11;
        break;
    case 3:
        t_rows_to_write[9] |= 0b111;
        break;
    case 4:
        t_rows_to_write[9] |= 0b1111;
        break;
    default:
        break;
    }
}