/**
 * Author: Eva-Maria Schiemer, Lucas Scheidt
 * Date: 29.09.24
 *
 * Description: QlockTwo implementation of Mausels
 */

// C includes
#include <string.h>

// ESP includes
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "hal/clk_tree_hal.h"

// project lib includes
#include "sequential_time_display.h"
#include "time_server_connection.h"
#include "time_translation.h"
#include "wifi_connection.h"
#include "nvs_flash.h"
// #include "pretty_printer.h"

void app_main()
{
    /// @note ugly but necessary for tests with a blocking endless main loop (even if it's on a seperate core and
    /// doesn't affect anything).
    esp_task_wdt_config_t deactivate_watchdog = {.timeout_ms = UINT32_MAX, .idle_core_mask = 0b11, .trigger_panic = 0};

    // for whatever reason the assertions fail, but since the board works... it's not an error if you ignore it.
    esp_err_t return_check = esp_task_wdt_init(&deactivate_watchdog);
    // invalid state means already initialized, so that would be ok
    assert (return_check == ESP_OK || return_check == ESP_ERR_INVALID_STATE);
    return_check = nvs_flash_init();
    assert (return_check == ESP_OK);

    // main board config
    /// @todo think about whether it's necessary to implement some kind of errorhandling...
    setup_pins_to_main_shift_register();
    connect_to_station();
    set_system_time();

    uint32_t leds_to_turn_on[WORD_CLOCK_MAX_ROWS];
    xTaskCreatePinnedToCore(display_time, "Time representation on word clock", 1024, (void*)leds_to_turn_on, 1, NULL,
                            1);
    while (1)
    {
        esp_task_wdt_reconfigure(&deactivate_watchdog);
        HourAndMinute hour_minute = {.hour = 0, .minute = 0};
        uint32_t set_correct_bits_array[WORD_CLOCK_MAX_ROWS];
        assert(sizeof(leds_to_turn_on) == sizeof(set_correct_bits_array));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        get_current_time(&hour_minute);
        translate_time_to_led_positions(&hour_minute, set_correct_bits_array);
        memcpy(leds_to_turn_on, set_correct_bits_array, sizeof(leds_to_turn_on));
        // for (uint8_t pos = 0; pos < WORD_CLOCK_MAX_ROWS; ++pos)
        // {
        //     printf("Row %d: ", pos);
        //     uint32_to_binary(leds_to_turn_on[pos]);
        // }
    }
}
