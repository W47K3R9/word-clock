/**
 * Author: Eva-Maria Schiemer, Lucas Scheidt
 * Date: 29.09.24
 *
 * Description: QlockTwo implementation of Mausels
 */

#include "../lib/time_translation/time_translation.h"
#include "esp_log.h"
#include "esp_task.h"
#include "nvs_flash.h"
#include "time_server_connection.h"
#include "time_translation.h"
#include "wifi_connection.h"
#include <string.h>
/// @note created by AI... may contain bullshit
void uint32_to_binary(uint32_t number)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        printf("%ld", (number >> i) & 1);
        if (i % 4 == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void app_main()
{
    esp_err_t return_check = nvs_flash_init();
    if (return_check != ESP_OK)
    {
        /// @note since we're working with C we have ugly elements like error-codes for example...
        ///     If at some time in the future one would want to make a real professional version of this code an
        ///     error list might be helpful. For now we'll stick with that.
        exit(-1);
    }
    connect_to_station();
    set_system_time();
    while (1)
    {
        uint32_t leds_to_turn_on[MAX_ROWS];
        int hour_and_minute[2];
        char date_time_buffer[100];
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /// @todo hour_and_minute should be replaced by some kind of struct.
        get_current_time(date_time_buffer, sizeof(date_time_buffer), hour_and_minute);
        translate_time_to_led_positions(hour_and_minute[0], hour_and_minute[1], leds_to_turn_on);
        for (uint8_t pos = 0; pos < MAX_ROWS; ++pos)
        {
            printf("Row %d: ", pos);
            uint32_to_binary(leds_to_turn_on[pos]);
        }
        printf("current date and time is = %s\n", date_time_buffer);
    }
}
