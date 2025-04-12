#include "pretty_printer.h"

#include <stdio.h>

void uint32_to_binary(uint32_t number)
{
    // 25 bits are enough.
    for (int i = 24; i >= 0; --i)
    {
        printf("%ld", (number >> i) & 1);
        if (i % 4 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}