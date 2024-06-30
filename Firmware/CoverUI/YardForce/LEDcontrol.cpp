/**
 * @file LEDcontrol.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI LED driver for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-10-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <map>
#include <cstring>
#include "include/LEDcontrol.hpp"
#include "../BttnCtl.h" // LED_state is defined in BttnCtl.h

LEDcontrol::LEDcontrol(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds) : kPinByLedNumPtr(t_kPinByLedNumPtr), kNumLeds(t_kNumLeds){};
LEDcontrol::LEDcontrol(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_cb)(char digit)) : kPinByLedNumPtr(t_kPinByLedNumPtr), kNumLeds(t_kNumLeds), set_base10_leds_cb(t_set_base10_leds_cb){};

/**
 * @brief Setup LED GPIOs
 *
 */
void LEDcontrol::setup()
{
    for (size_t p = 0; p < kNumLeds; p++)
    {
        auto pin = *(kPinByLedNumPtr + p);
        if (pin != LED_PIN_NC)
            pinMode(pin, OUTPUT);
    }
}

/**
 * @brief Set any of known LED_state states for the given LED num.
 *
 * @param led_num
 * @param state
 * @param change_state Indicate if the state get written to led_states_bin_ buffer
 */
void LEDcontrol::set(uint8_t led_num, LED_state state, bool change_state)
{
    auto pin = *(kPinByLedNumPtr + led_num);
    if (pin != LED_PIN_NC)
    {
        switch (state)
        {
        case LED_state::LED_on:
            digitalWrite(pin, HIGH);
            break;
        case LED_state::LED_off:
            digitalWrite(pin, LOW);
            break;
        case LED_state::LED_blink_slow:
        case LED_state::LED_blink_fast:
            // Get handled by timers
            break;
        }
    }
    if (change_state)
        change_led_states_(led_num, state);
}

/**
 * @brief Toggle (on/off) the given LED num
 *
 * @param led_num
 * @param change_state indicate if the state get written to led_states_bin_ buffer
 */
void LEDcontrol::toggle(uint8_t led_num, bool change_state) {
    if (has(led_num, LED_state::LED_off))
        set(led_num, LED_state::LED_on, change_state);
    else
        set(led_num, LED_state::LED_off, change_state);
}

/**
 * @brief Set LED based on binary state representation.
 * This method set only the OM controlled LEDs
 *
 * @param all_state
 */
void LEDcontrol::set(uint64_t all_state)
{
    for (unsigned int led = 0; led <= LED_NUM_OM_MAX && led < kNumLeds; led++)
    {
        uint8_t led_state = (all_state >> (3 * led)) & 0b111;
        set(led, static_cast<LED_state>(led_state));
    }
}

LED_state LEDcontrol::get(uint8_t led_num)
{
    return (LED_state)((led_states_bin_ >> (3 * led_num)) & 0b111);
}

void LEDcontrol::force_(uint8_t led_num, bool force, uint32_t *force_type_)
{
    uint32_t led_bin = 1 << led_num;

    if (force)
        *force_type_ |= led_bin;
    else
        *force_type_ &= ~led_bin;
}

void LEDcontrol::force_off(uint8_t led_num, bool force)
{
    force_(led_num, force, &force_led_off_);
    if (force)
        set(led_num, LED_state::LED_off, false); // Directly set without changing state
    else
        set(led_num, get(led_num), false); // Restore state
}

void LEDcontrol::force_on(uint8_t led_num, bool force)
{
    force_(led_num, force, &force_led_on_);
    if (force)
        set(led_num, LED_state::LED_on, false); // Directly set without changing state
    else
        set(led_num, get(led_num), false); // Restore state
}

void LEDcontrol::change_led_states_(uint8_t led_num, LED_state state)
{
    led_states_bin_ &= ~((uint64_t)(0b111) << (3 * led_num)); // Be safe for future LED_state changes and mask out the whole led
    led_states_bin_ |= (uint64_t)(state) << (3 * led_num);    // Set new state
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
    return (led_states_bin_ >> (3 * led_num) & (uint64_t)(0b111)) == (uint64_t)(state);
}

void LEDcontrol::blink_timer_elapsed(LED_state blink_state)
{
    // Synchronous blink is only for cosmetic nature, probably only interesting for a nice looking CoverUITest
    static std::map<LED_state, LED_state> sync_blink_map = {
        {LED_state::LED_blink_slow, LED_state::LED_on},
        {LED_state::LED_blink_fast, LED_state::LED_on}};

    if (blink_state != LED_state::LED_blink_fast && blink_state != LED_state::LED_blink_slow) // Ensure that this method only get called for blinking LED states
        return;

    for (size_t led_num = 0; led_num < kNumLeds; led_num++)
    {
        if (has(led_num, blink_state) && !(force_led_off_ & (1 << led_num)))
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
    delay(100);

    force_off(led_num, true);
    force_on(led_num, false);
    delay(100);

    // stop with forced off
    force_off(led_num, true);
    force_on(led_num, false);
}

/*******************************************************************************************
 *                           LED "Sequence" stuff                                          *
 *******************************************************************************************
 * Timer based implementation for LED sequences like animation, FW version, ...            *
 * to overcome the tricky use of HAL_Delay() which is heavily ISR fragile.                 *
 * Looks a little bit over-complicated, but ...                                            *
 * AH20230511: Not required anymore because arduino framework doesn't has this delay()/ISR *
 *   issue anymore. But because it's already written and works, ...                        *
 *******************************************************************************************/

/**
 * @brief Start the given sequence handler
 *
 * @param handler
 * @param boolean abort_running aborts a currently running sequence
 */
void LEDcontrol::sequence_start(void (LEDcontrol::*handler)(), bool abort_running)
{
    if (abort_running)
    {
        set(led_states_bin_); // Restore states
    }

    if (seq_start_tick_ > 0 && !abort_running)
        return; // There's already/still a running sequence

    seq_step_ = 0;
    seq_start_tick_ = millis();
    seq_handler_ = handler;
}

/**
 * @brief Process LED sequence
 * Get called by 5ms timer (which is pretty fast enough for our LED sequences)
 */
void LEDcontrol::process_sequence()
{
    if (seq_start_tick_ == 0)
        return; // No sequence

    (this->*seq_handler_)(); // Call sequence handler
}

/**
 * @brief Get next sequence step for the given step-rate
 *
 * @param step_rate in ms
 * @return uint16_t step (n>1) or 0 if the next step isn't reached now
 */
uint16_t LEDcontrol::seq_get_next_step_(uint16_t step_rate)
{
    static uint16_t last_step_tick = 0;

    uint16_t step_tick = ((((millis() - seq_start_tick_) + (step_rate - 1))) / step_rate) * step_rate; // Round to the next nearest multiple of <step_rate>
    if (step_tick == last_step_tick)
        return 0; // Not a new <step_rate> step

    last_step_tick = step_tick;
    return ++seq_step_;
}

void LEDcontrol::show_num(uint16_t num)
{
    seq_num_value_ = num;
    sequence_start(&LEDcontrol::seq_num_handler_);
}

/**
 * @brief Sequence handler for displaying an uint16_t <seq_num_value_> as base10 values
 * number by number. Use i.e. for FW version, or error display
 */
void LEDcontrol::seq_num_handler_()
{
    const uint8_t steps_per_char = 10;

    static char s_buf[6]; // Current largest number is a uint16_t, whose max. = 65535 but probably need to be made dynamic!!
    static uint8_t s_num_chars;
    static uint8_t s_cur_idx; // Current displaying digit index

    uint16_t step = seq_get_next_step_(100); // Animation sequence runs in 100ms steps

    if (step == 0) // Next sequence step not reached now
        return;

    if (step == 1) // Sequence start, init vars
    {
        s_cur_idx = 0;
        itoa(seq_num_value_, s_buf, 10);
        s_num_chars = std::strlen(s_buf);
        force_off_num_seq_leds(true); // Force related signalling LEDs off
        return;
    }

    if (step >= (s_num_chars + 1) * steps_per_char) // End (last char sent)
    {
        force_off_num_seq_leds(false); // Un-Force related signalling LEDs
        seq_start_tick_ = 0;           // Sequence end
        return;
    }

    uint16_t sub_step = (s_cur_idx + 1) * steps_per_char; // Per digit steps

    if (step == sub_step) // Num Display start
    {
        set(LED_NUM_LIFTED, LED_state::LED_on, false); // New number indicator on
        (this->*set_base10_leds_cb)(s_buf[s_cur_idx]); // Set base10 LEDs callback
        return;
    }

    if (step == sub_step + 1) // +100ms
    {
        set(LED_NUM_LIFTED, LED_state::LED_off, false); // New number indicator off
        s_cur_idx++;
        return;
    }
}