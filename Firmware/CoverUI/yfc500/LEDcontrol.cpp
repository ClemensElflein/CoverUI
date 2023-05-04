/**
 * @file LEDcontrol.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <map>
#include <cstring>
#include "LEDcontrol.h"
#include "../BttnCtl.h" // LED_state is defined in BttnCtl.h

LEDcontrol::LEDcontrol() {}

/**
 * @brief Setup LED GPIOs
 *
 */
void LEDcontrol::setup()
{
    printf("LedControl Setup\n");

    for (uint32_t led_num : _leds)
    {
        pinMode(led_num, OUTPUT);
    }
}

/**
 * @brief Set any of known LED_state states for the given LED num.
 *
 * @param led_num
 * @param state
 * @param change_state Indicate if the state get written to _led_states_bin buffer
 */
void LEDcontrol::set(uint8_t led_num, LED_state state, bool change_state)
{
    switch (state)
    {
    case LED_state::LED_on:
        digitalWrite(_leds[led_num], HIGH);
        break;
    case LED_state::LED_off:
        digitalWrite(_leds[led_num], LOW);
        break;
    case LED_state::LED_blink_slow:
    case LED_state::LED_blink_fast:
        // Get handled by timers
        break;
    }
    if (change_state)
        _change_led_states(led_num, state);
}

void LEDcontrol::set(uint64_t all_state)
{
    for (uint8_t led = 0; led < NUM_LEDS; led++)
    {
        uint8_t led_state = (all_state >> (3 * led)) & 0b111;
        set(led, static_cast<LED_state>(led_state));
    }
}

/**
 * @brief Set base10 related LEDs for the given (numeric) character
 *
 * @param digit numeric character
 */
void LEDcontrol::_set_base10_leds(char digit)
{
    for (uint8_t bit = 0; bit <= 6; bit++) // We've 6 LEDs for base10 number representation
    {
        bool on = (_base10_leds[digit - '0'] >> bit) & 0b1;
        set(bit + 4, on ? LED_state::LED_on : LED_state::LED_off, false);
    }
}

LED_state LEDcontrol::get(uint8_t led_num)
{
    return (LED_state)((_led_states_bin >> (3 * led_num)) & 0b111);
}

void LEDcontrol::_force(uint8_t led_num, bool force, uint32_t *_force_type)
{
    uint32_t led_bin = 1 << led_num;

    if (force)
        *_force_type |= led_bin;
    else
        *_force_type &= ~led_bin;
}

void LEDcontrol::force_off(uint8_t led_num, bool force)
{
    _force(led_num, force, &_force_led_off);
    if (force)
        set(led_num, LED_state::LED_off, false); // Directly set without changing state
    else
        set(led_num, get(led_num), false); // Restore state
}

void LEDcontrol::force_on(uint8_t led_num, bool force)
{
    _force(led_num, force, &_force_led_on);
    if (force)
        set(led_num, LED_state::LED_on, false); // Directly set without changing state
    else
        set(led_num, get(led_num), false); // Restore state
}

void LEDcontrol::_change_led_states(uint8_t led_num, LED_state state)
{
    _led_states_bin &= ~((uint64_t)(0b111) << (3 * led_num)); // Be safe for future LED_state changes and mask out the whole led
    _led_states_bin |= (uint64_t)(state) << (3 * led_num);    // Set new state
}

/**
 * @brief Check if <led_num> has <state>
 *
 * @param led_num
 * @param state
 * @return true
 * @return false
 */
bool LEDcontrol::has(uint8_t led_num, LED_state state)
{
    return (_led_states_bin >> (3 * led_num) & (uint64_t)(0b111)) == (uint64_t)(state);
}

void LEDcontrol::blink_timer_elapsed(LED_state blink_state)
{
    // Synchronous blink is only for cosmetic nature, probably only interesting for a nice looking CoverUITest
    static std::map<LED_state, LED_state> sync_blink_map = {
        {LED_state::LED_blink_slow, LED_state::LED_on},
        {LED_state::LED_blink_fast, LED_state::LED_on}};

    if (blink_state != LED_state::LED_blink_fast && blink_state != LED_state::LED_blink_slow) // Ensure that this method only get called for blinking LED states
        return;

    for (uint8_t led_num = 0; led_num < NUM_LEDS; led_num++) // FIXME: Find some more efficient instead of looping through all NUM_LEDS
    {
        if (has(led_num, blink_state) && !(_force_led_off & (1 << led_num)))
        {
            set(led_num, sync_blink_map[blink_state], false); // Set LED without state change
        }
    }
    // Synchronous toggle
    if (sync_blink_map[blink_state] == LED_state::LED_on)
        sync_blink_map[blink_state] = LED_state::LED_off;
    else
        sync_blink_map[blink_state] = LED_state::LED_on;
}

/**
 * @brief Identify LED by short blink code
 *
 * @param led_num
 */
void LEDcontrol::identify(uint8_t led_num)
{
    force_off(led_num, false);
    force_on(led_num, true);
    delay(100); // FIXME: Might fail in ISR

    force_off(led_num, true);
    force_on(led_num, false);
    delay(100); // FIXME: Might fail in ISR

    // stop with forced off
    force_off(led_num, true);
    force_on(led_num, false);
}

void LEDcontrol::show_num(uint16_t num)
{
    _seq_num_value = num;
    sequence_start(&LEDcontrol::_seq_num_handler);
}

/******************************************************************************************
 *                           LED "Sequence" stuff                                         *
 ******************************************************************************************
 * Timer based implementation for LED sequences like animation, FW version, ...           *
 * to overcome the tricky use of HAL_Delay() which is heavily ISR fragile.                *
 * Looks a little bit over-complicated, but ...                                           *
 ******************************************************************************************/

/**
 * @brief Start the given sequence handler
 *
 * @param handler
 */
void LEDcontrol::sequence_start(void (LEDcontrol::*handler)())
{
    if (_seq_start_tick > 0)
        return; // There's already/still a running sequence

    _seq_step = 0;
    _seq_start_tick = millis();
    _seq_handler = handler;
}

/**
 * @brief Process LED sequence
 * Get called by 5ms timer (which is pretty fast enough for our LED sequences)
 */
void LEDcontrol::process_sequence()
{
    if (_seq_start_tick == 0)
        return; // No sequence

    (this->*_seq_handler)(); // Call sequence handler
}

/**
 * @brief Get next sequence step for the given step-rate
 *
 * @param step_rate in ms
 * @return uint16_t step (n>1) or 0 if the next step isn't reached now
 */
uint16_t LEDcontrol::_seq_get_next_step(uint16_t step_rate)
{
    static uint16_t last_step_tick = 0;

    uint16_t step_tick = ((((millis() - _seq_start_tick) + (step_rate - 1))) / step_rate) * step_rate; // Round to the next nearest multiple of <step_rate>
    if (step_tick == last_step_tick)
        return 0; // Not a new <step_rate> step

    last_step_tick = step_tick;
    return ++_seq_step;
}

/**
 * @brief Animate sequence handler. Has to be started by sequence_start()
 */
void LEDcontrol::sequence_animate_handler()
{
    uint16_t step = _seq_get_next_step(15); // Animation sequence runs in 15ms steps

    switch (step)
    {
    case 0: // Next sequence step not reached now
        return;
    case 1 ... NUM_LEDS: // LED on
        set(NUM_LEDS - step, LED_state::LED_on, false);
        return;
    case NUM_LEDS + 1 ... 2 * NUM_LEDS: // LED off
        set((2 * NUM_LEDS) - step, LED_state::LED_off, false);
        return;
    default:
        _seq_start_tick = 0;  // Sequence end
        set(_led_states_bin); // Restore states
        return;
    }
}

void LEDcontrol::_force_off_num_seq_leds(bool force)
{
    force_off(LED_NUM_LIFTED, force);                    // Num change signalling LED
    for (uint8_t i = LED_NUM_MON; i >= LED_NUM_SUN; i--) // Base10 related LEDs
        force_off(i, force);
}

/**
 * @brief Sequence handler for displaying an uint16_t <_seq_num_value> as base10 values
 * number by number. Use i.e. for FW version, or error display
 */
void LEDcontrol::_seq_num_handler()
{
    const uint8_t steps_per_char = 10;

    static char s_buf[6]; // FIXME: uint16_t max = 65535 but should be made dynamic!!
    static uint8_t s_num_chars;
    static uint8_t s_cur_idx; // Current displaying digit index

    uint16_t step = _seq_get_next_step(100); // Animation sequence runs in 100ms steps

    if (step == 0) // Next sequence step not reached now
        return;

    if (step == 1) // Sequence start, init vars
    {
        s_cur_idx = 0;
        itoa(_seq_num_value, s_buf, 10);
        s_num_chars = std::strlen(s_buf);
        _force_off_num_seq_leds(true); // Force related signalling LEDs off
        return;
    }

    if (step >= (s_num_chars + 1) * steps_per_char) // End (last char sent)
    {
        _force_off_num_seq_leds(false); // Un-Force related signalling LEDs
        _seq_start_tick = 0;            // Sequence end
        return;
    }

    uint16_t sub_step = (s_cur_idx + 1) * steps_per_char; // Per digit steps

    if (step == sub_step) // Num Display start
    {
        set(LED_NUM_LIFTED, LED_state::LED_on, false); // New number indicator on
        _set_base10_leds(s_buf[s_cur_idx]);
        return;
    }

    if (step == sub_step + 1) // +100ms
    {
        set(LED_NUM_LIFTED, LED_state::LED_off, false); // New number indicator off
        s_cur_idx++;
        return;
    }
}
