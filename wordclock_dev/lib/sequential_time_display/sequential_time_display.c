/**
 * Author: Lucas Scheidt
 * Date: 04.10.24
 *
 * Description: Implementation for row display cycle.
 *     For the frequencies and timings of the patterns the timing limits of the 74HC959 shift register are to be
 *     followed.
 *     Operation Voltage is around 5V, for that configuration the timings are around these values (specified in
 *     the TI datasheet):
 *     - Clock Frequency: 25 MHz (40 ns per Period)
 *     - Pulse high or low for shift or load: 20 ns
 *     - Clear low (active) until execution: 20 ns
 *     - Serial in state before shift: 25 ns
 *     - Shift high before load high: 19 ns
 *     - Clear low (active) before load high: 13 ns
 *     - Clear high (inactive) before shift high: 12 ns
 *
 *     Despite the maximum ratings it's desirable to have the on cycle of the LED's be as long as possible without any
 *     flickering.
 *     According to IEEE flicker effects for LED's are considered harmless above frequencies of 3 kHz.
 *     For the clock there are 10 rows to be displayed, so for a target frequency of 5 kHz or 200 µs per cycle,
 *     (little buffer over the recommendet 3 kHz) a single row has to be on for about 10 µs (200 µs / 2 / 10 rows).
 *     And within that 10 µs 25 loads / shifts need to be done. So a single shift operation should take 400 ns.
 *     That way the desired frequency can be achieved. But note that the "on" time for each row is only a tenth of the
 *     actual frequency of 5 kHz.
 */

#include "sequential_time_display.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

#include <time_translation.h>

// clang-format off
static const gpio_num_t load_register_clk  = GPIO_NUM_32;
static const gpio_num_t shift_register_clk = GPIO_NUM_33;
static const gpio_num_t clear_reagister    = GPIO_NUM_25; // clears at LOW
static const gpio_num_t serial_input_pin   = GPIO_NUM_13;
// clang-format on

void setup_pins_to_main_shift_register()
{
    /// @note gpio_config avoids the quite unobvious behaviour, that some pins serve as JTAG support and won't act
    ///    as you'd expect they would by regularly setting the direction with the respective call.
    gpio_config_t shift_reg_config = {
        .pin_bit_mask =
            1ULL << load_register_clk | 1ULL << shift_register_clk | 1ULL << clear_reagister | 1ULL << serial_input_pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t check_result = gpio_config(&shift_reg_config);
    assert(check_result == ESP_OK);
    // init transmission delays are only for safety (clear register is low active)
    gpio_set_level(serial_input_pin, 0);
    esp_rom_delay_us(1);
    gpio_set_level(clear_reagister, 0);
    esp_rom_delay_us(1);
    gpio_set_level(clear_reagister, 1);
    esp_rom_delay_us(1);
    gpio_set_level(load_register_clk, 0);
}

// static void delay_for_n_ns(uint32_t t_delay_in_ns)
// {
//     /// @note the system states around 160 ticks that are being used per microsecond, so the finest resolution
//     /// is about 6.25 ns per operation.
//     double ticks_to_wait = t_delay_in_ns / 6.25;
//     for(uint32_t tick_count = 0; tick_count < ticks_to_wait; ++tick_count);
// }
//
static void delay_for_200_ns()
{
    /// @note with a tick time of 6.25 ns waiting 32 ticks is equivalent to waiting for 200 ns.
    for(uint8_t tick_count = 0; tick_count < 32; ++tick_count);
}

/// @brief void* is utterly ugly but needed to be understood by xTaskCreate.
void display_time(void* t_rows_to_display)
{
    /// @note this will be the main loop of the MCU and it should be called as the highest prioritized thread, if
    /// posible on it's own core so that NO PREEMPTION causes the timing to be interrupted.
    /// I know this implementation is quite fragile but it's only a hobby project so come on, don't be that mean!
    uint32_t* rows_to_display = t_rows_to_display;
    for (size_t row_index = 0; row_index < WORD_CLOCK_MAX_ROWS;)
    {
        gpio_set_level(load_register_clk, 0);
        // loading stage takes about approx. 25 * 0.4 µs = 10 µs
        for (size_t bit_position = 0; bit_position < WORD_CLOCK_WORD_LENGTH; ++bit_position)
        {
            gpio_set_level(shift_register_clk, 0);
            gpio_set_level(serial_input_pin, rows_to_display[row_index] >> bit_position & 1);
            delay_for_200_ns();
            gpio_set_level(shift_register_clk, 1);
            delay_for_200_ns();
        }
        gpio_set_level(load_register_clk, 1);
        esp_rom_delay_us(10);
        row_index = (row_index != WORD_CLOCK_MAX_ROWS - 1) ? row_index + 1 : 0;
    }
}
