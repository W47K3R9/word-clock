// C includes
#include <string.h>

// ESP includes
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "nvs_flash.h"

#include "sequential_time_display.h"
#include "time_server_connection.h"
#include "time_translation.h"
#include "wifi_connection.h"
// #include "pretty_printer.h"

void app_main()
{
    /// @note ugly but necessary for tests with a blocking endless main loop (even if it's on a seperate core and
    /// doesn't affect anything).
    const esp_task_wdt_config_t deactivate_watchdog = {.timeout_ms = UINT32_MAX, .idle_core_mask = 0b11, .trigger_panic = 0};

    esp_err_t return_check = esp_task_wdt_init(&deactivate_watchdog);
    // invalid state means already initialized, so that would be ok
    assert(return_check == ESP_OK || return_check == ESP_ERR_INVALID_STATE);
    return_check = nvs_flash_init();
    assert(return_check == ESP_OK);

    // main board config
    /// @todo think about whether it's necessary to implement some kind of errorhandling.
    /// That way also the endless loop could be interrupted if some error occurs.
    setup_pins_to_main_shift_register();
    connect_to_station();
    set_system_time();
    uint32_t leds_to_turn_on[WORD_CLOCK_MAX_ROWS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    xTaskCreatePinnedToCore(display_time,
                            "Time representation on word clock",
                            1024,
                            (void*)leds_to_turn_on,
                            1,
                            NULL,
                            1);
    const gpio_num_t user_led = GPIO_NUM_21;
    uint8_t toggle = 0;
    activate_screen();
    // ReSharper disable once CppDFAEndlessLoop
    while (1)
    {
        gpio_set_level(user_led, toggle);
        esp_task_wdt_reconfigure(&deactivate_watchdog);
        HourAndMinute hour_minute = {.hour = 0, .minute = 0};
        uint32_t set_correct_bits_array[WORD_CLOCK_MAX_ROWS];
        assert(sizeof(leds_to_turn_on) == sizeof(set_correct_bits_array));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        get_current_time(&hour_minute);
        translate_time_to_led_positions(&hour_minute, set_correct_bits_array);
        // only to be safe that the bits only get displayed after masking etc.
        memcpy(leds_to_turn_on, set_correct_bits_array, sizeof(leds_to_turn_on));

        /// @note this is for debugging purposes.
        // for (uint8_t pos = 0; pos < WORD_CLOCK_MAX_ROWS; ++pos)
        // {
        //     printf("Row %d: ", pos);
        //     uint32_to_binary(leds_to_turn_on[pos]);
        // }
        toggle ^= 1;
    }
}