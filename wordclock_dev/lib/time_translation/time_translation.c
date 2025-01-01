/**
 * Author: Lucas Scheidt
 * Date: 29.09.24
 *
 * Description:
 *
 */

#include "time_translation.h"

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