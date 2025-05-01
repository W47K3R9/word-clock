#include "pretty_printer.h"

#include <stdio.h>

void uint32_to_binary(const uint32_t number)
{
    // 25 bits are enough.
    for (int i = 25; i >= 0; --i)
    {
        printf("%u", (number >> i) & 1);
        if (i % 4 == 0)
        {
            printf(" ");
        }
        if (i == 4 || i == 15)
        {
            printf("|");
        }
    }
    printf("\n");
}