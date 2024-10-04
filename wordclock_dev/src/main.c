/**
 * Author: Eva-Maria Schiemer, Lucas Scheidt
 * Date: 29.09.24
 *
 * Description: QlockTwo implementation of Mausels
 */

#include "../lib/time_translation/time_translation.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "hal/clk_tree_hal.h"

#include "../lib/sequential_time_display/sequential_time_display.h"
#include "nvs_flash.h"
#include "sequential_time_display.h"
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
    /// @note ugly but necessary for tests with a blocking endless main loop (even if it's on a seperate core and
    /// doesn't affect anything).
    esp_task_wdt_config_t deactivate_watchdog = {.timeout_ms = UINT32_MAX, .idle_core_mask = 0b11, .trigger_panic = 0};
    esp_err_t return_check = esp_task_wdt_init(&deactivate_watchdog);
    assert(return_check == ESP_OK);
    return_check = nvs_flash_init();
    assert(return_check == ESP_OK);

    // connect_to_station();
    // set_system_time();
    setup_pins_to_main_shift_register();

    /*
     * Test pattern should then be:
     *
     *           - O O -
     *           O - - O
     *
     * with "O" for on and "-" for off.
     */
    // clang-format off
    uint32_t test_pattern[MAX_ROWS] = {
        0b00000001000010100000000000000000,
        0, 0, 0, 0,
        0b00000000100101010000000000000000,
        0, 0, 0, 0};
    // clang-format on
    xTaskCreatePinnedToCore(display_time, "Time representation on word clock", 1024, (void*)test_pattern, 1, NULL, 1);
    while (1)
    {
        uint32_t ticks_per_us = esp_rom_get_cpu_ticks_per_us();
        uint32_t clock_freq = clk_hal_cpu_get_freq_hz();
        printf("Ticks per µs: %ld, Frequency of system: %ld\n", ticks_per_us, clock_freq);
        esp_task_wdt_reconfigure(&deactivate_watchdog);
        // uint32_t leds_to_turn_on[MAX_ROWS];
        // int hour_and_minute[2];
        // char date_time_buffer[100];
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // /// @todo hour_and_minute should be replaced by some kind of struct.
        // get_current_time(date_time_buffer, sizeof(date_time_buffer), hour_and_minute);
        // translate_time_to_led_positions(hour_and_minute[0], hour_and_minute[1], leds_to_turn_on);
        // for (uint8_t pos = 0; pos < MAX_ROWS; ++pos)
        // {
        //     printf("Row %d: ", pos);
        //     uint32_to_binary(leds_to_turn_on[pos]);
        // }
        // printf("current date and time is = %s\n", date_time_buffer);
    }
}
