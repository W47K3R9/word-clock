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
 *     And within that 10 µs 31 loads / shifts need to be done. So a single shift operation should take 323 ns.
 *     With the clock resolution an operation speed of 325 ns can approx. be achieved.
 *     That way the desired frequency can be achieved. But note that the "on" time for each row is only a tenth of the
 *     actual frequency of 5 kHz.
 */

#include "sequential_time_display.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"

#include <time_translation.h>

// @formatter:off
static const gpio_num_t serial_input_pin   = GPIO_NUM_1;
static const gpio_num_t output_enable      = GPIO_NUM_2; // LOW active
static const gpio_num_t load_register_clk  = GPIO_NUM_4;
static const gpio_num_t shift_register_clk = GPIO_NUM_5;
static const gpio_num_t clear_register     = GPIO_NUM_6; // clears at LOW
static const gpio_num_t user_led           = GPIO_NUM_21;

// after writing the value into the shift register, another six shifts need to happen to put the value to the end.
static const uint8_t shift_to_end_diff = 31 - WORD_CLOCK_WORD_LENGTH;
// @formatter:on

void setup_pins_to_main_shift_register()
{
    /// @note gpio_config avoids the quite unobvious behaviour, that some pins serve as JTAG support and won't act
    ///    as you'd expect they would by regularly setting the direction with the respective call.
    const gpio_config_t shift_reg_config = {
        .pin_bit_mask =
        1ULL << load_register_clk | 1ULL << shift_register_clk | 1ULL << clear_register | 1ULL << serial_input_pin |
        1ULL << output_enable | 1ULL << user_led,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    const esp_err_t check_result = gpio_config(&shift_reg_config);
    assert(check_result == ESP_OK);
    // init transmission delays are only for safety (clear register is low active)
    gpio_set_level(output_enable, 0);
    gpio_set_level(user_led, 0);
    gpio_set_level(serial_input_pin, 0);
    esp_rom_delay_us(1);
    gpio_set_level(clear_register, 0);
    esp_rom_delay_us(1);
    gpio_set_level(load_register_clk, 1);
    esp_rom_delay_us(1);
    gpio_set_level(clear_register, 1);
    gpio_set_level(load_register_clk, 0);
    gpio_set_level(output_enable, 1);
}

void activate_screen()
{
    gpio_set_level(output_enable, 0);
}


/// @note this is a candidate that would for sure get optimized away if not stated otherwise...
__attribute__((used)) static void delay_for_screen_rate()
// static void delay_for_screen_rate()
{
    /// @note with a tick time of 6.25 ns waiting 26 ticks is equivalent to waiting for 162.5 ns.
    /// Waiting two times (one time for off, one time for on) that duration leads to the desired shift operation
    /// duration of 325 ns.
    for (uint8_t tick_count = 0; tick_count < 26; ++tick_count);
}

static void load_value()
{
    delay_for_screen_rate();
    gpio_set_level(shift_register_clk, 1);
    delay_for_screen_rate();
    gpio_set_level(shift_register_clk, 0);
}

static void shift_to_end()
{
    // needed in order to get the LSB to the correct position in the shift register.
    for (size_t bit_position = 0; bit_position < shift_to_end_diff; ++bit_position)
    {
        gpio_set_level(serial_input_pin, 1);
        load_value();
    }
}

/// @brief void* is utterly ugly but needed to be understood by xTaskCreate.
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void display_time(void* t_rows_to_display)
{
    /// @note this will be the main loop of the MCU and it should be called as the highest prioritized thread, if
    /// posible on it's own core so that NO PREEMPTION causes the timing to be interrupted.
    /// I know this implementation is quite fragile but it's only a hobby project so come on, don't be that mean!
    const uint32_t* rows_to_display = t_rows_to_display;
    for (size_t row_index = 0; row_index < WORD_CLOCK_MAX_ROWS;)
    {
        for (size_t bit_position = 0; bit_position < WORD_CLOCK_WORD_LENGTH; ++bit_position)
        {
            // setup-time SER before SRCLK (HI): 25 ns @ 4.5 V
            gpio_set_level(serial_input_pin, rows_to_display[row_index] >> bit_position & 1);
            load_value();
        }
        shift_to_end();
        // shift to end stage takes about approx. 6 * 0.325 µs = 1.950 µs
        // whole shift operation delay: 8.125 µs + 1.950 µs = 10.075 µs
        gpio_set_level(load_register_clk, 1);
        esp_rom_delay_us(5);
        gpio_set_level(load_register_clk, 0);
        esp_rom_delay_us(5);
        row_index = (row_index != WORD_CLOCK_MAX_ROWS - 1) ? row_index + 1 : 0;
    }
}