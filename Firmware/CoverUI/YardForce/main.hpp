/**
 * @file main.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI main header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.3
 * @date 2023-09-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __YARDFORCE_MAIN_H
#define __YARDFORCE_MAIN_H

#include <Arduino.h> // Stock CoverUI is build now via Arduino framework (instead of HAL), which is ATM the only framework with STM32F030R8 and GD32F330R8 support
#include "hwtimer.hpp"
#include <list>

#include "LEDcontrol.h"
LEDcontrol leds; // Main LED controller object

#include "Buttons.hpp"
Buttons buttons; // Main Buttons object

#ifdef MDL_SAXPRO // Model SAxPRO
void add_sim_button(uint8_t button_id, uint32_t press_timeout);
#include "WYM240128K1.hpp"
#endif
#ifdef MOD_RAIN
#include "Rain.hpp"
#endif
#ifdef MOD_HALL
#include "Emergency.hpp"
#endif

#define FIRMWARE_VERSION 200 // FIXME: Should go into a common header, probably preferable as PROTOCOL_VERSION
#define FIRMWARE_VERSION_THIS 100

// STM32/GD32 are single cores, also without threads.
// Send mutex calls of main.cpp to nirvana. Dangerous?
#define auto_init_mutex(name)
#define mutex_enter_blocking(ptr)
#define mutex_exit(ptr)

// Timer
#define TIM_QUICK_FREQUENCY 200                                // Hz
#define TIM_QUICK_PERIOD_MS (1.0 / TIM_QUICK_FREQUENCY * 1000) // Milliseconds

extern void core1();
extern void getDataFromBuffer();

uint8_t bit_getbutton(uint32_t press_timeout, bool &still_pressed);
void timer_slow_callback_wrapper();
void timer_fast_callback_wrapper();
void timer_event_callback_wrapper();
void timer_quick_callback_wrapper();

// OM names. Could also use those, but I prefer logic names instead of physical ones
#define uart1 &serial_ll

// Some dummy Pico-SDK definitions. Not used but by this we'll NOT pollution original code to much with #ifdefs
#define pio0 NULL
#define pio1 NULL
typedef bool *PIO;
#define buzzer_SM_CYCLE 10800

// YardForce implementation specific
#ifdef MOD_RAIN
Rain rain;
#endif
#ifdef MOD_HALL
Emergency emergency;
#endif
HardwareTimer *timer_slow;  // Used for blink-slow LEDs and magic buttons
HardwareTimer *timer_fast;  // Used for blink-fast LEDs
#ifdef MDL_SAXPRO           // Model SAxPRO
HardwareTimer *timer_event; // Used for lv_timer_handler() and LED-2-display logic
#endif
HardwareTimer *timer_quick; // Button debouncer and LED sequences

#ifdef MCU_STM32
#ifdef MDL_C500
HardwareSerial serial_ll(PA3, PA2); // Serial connection to LowLevel MCU, JP2 Pin 1+3
#elif defined(MDL_SAXPRO)
HardwareSerial serial_ll(PA10, PA9); // Serial connection to LowLevel MCU, JP2 Pin 1+3
#endif
#else // MCU_GD32
HardwareSerial serial_ll((uint8_t)PA3, (uint8_t)PA2, 1); // Serial connection to LowLevel MCU, J6/JP2 Pin 1+3
#endif

struct SimButton // Simulate button
{
    uint8_t button_id;
    uint32_t press_timeout;
};

std::list<SimButton> sim_button_queue;

void setup()
{
    leds.setup();
    buttons.setup();
#ifdef MDL_SAXPRO // Model SAxPRO
    if (!display::init())
        display::set_backlight(LED_blink_fast, 60000); // TODO: Make some better assert handling than 60 sec. fast blink
    else
        display::set_backlight();
#endif
#ifdef MOD_HALL
    emergency.setup();
#endif

    // We've hardware timer on mass, let's use them.
    timer_slow = hwtimer(TIM_SLOW, 2, timer_slow_callback_wrapper, 30);  //   2Hz (500ms) timer, used for LED-blink-slow and magic buttons
    timer_fast = hwtimer(TIM_FAST, 10, timer_fast_callback_wrapper, 20); //  10Hz (100ms) timer, used for LED-blink-fast
#ifdef MDL_SAXPRO
    timer_event = hwtimer(TIM_EVENT, 100, timer_event_callback_wrapper, 10); // 100Hz   (10ms) timer, used for displays lv_timer_handler() and LED-2-Display logic
#endif
    // Don't increase value of this timers preemptPriority parameter higher than the default,
    // as this timer callback also handles HardwareSerial as well as button-debouncer,
    // which would mess expected processing of those.
    timer_quick = hwtimer(TIM_QUICK, TIM_QUICK_FREQUENCY, timer_quick_callback_wrapper); // 200Hz   (5ms) timer, used for Buttons debouncer and LED- sequences

    serial_ll.begin(115200);

#ifdef MDL_C500
    leds.set(LED_NUM_REAR, LED_state::LED_blink_slow); // We're alive -> blink // FIXME: Should become a simple delay in main loop or similar, because a timer might walk on, even if main crashes

    delay(100); // Some required stupid delay, dunno why :-/

    // "Hi there" and jammed button mounting detection
    bool tmp;
    do
    {
        // LED blink to say it's alive
        // (this processing delay is also required to get the debouncer filled with a consistent state (NUM_BUTTON_STATES * 5ms)
        leds.sequence_start(&LEDcontrol::sequence_animate_handler);

    } while (bit_getbutton(500, tmp));
    delay((NUM_LEDS * 15 * 2) + 500); // Anim get played async + 1/2 sec. extra delay
#endif
}

void loop() // This loop() doesn't loop!
{
    // Drop off into infinite core1() at main.cpp, for button processing (waste (one more?) stack entry! but let remain original code untouched)
    core1();
}

#ifdef MDL_C500 // Model Classic 500
/**
 * @brief Check if one of the "magic buttons" got pressed and do his function.
 * At the moment the following magic buttons exists:
 * OK + Clock = Display base FW version
 * OK + Home  = Display this FW version
 * OK + Sun   = LED animation
 */
void magic_buttons()
{
    if (!buttons.is_pressed(BTN_OK_NUM))
        return;

    if (buttons.is_pressed(BTN_SUN_NUM))
        leds.sequence_start(&LEDcontrol::sequence_animate_handler);
    else if (buttons.is_pressed(BTN_CLK_NUM))
        leds.show_num(FIRMWARE_VERSION);
    else if (buttons.is_pressed(BTN_HOME_NUM))
        leds.show_num(FIRMWARE_VERSION_THIS);
    return;
}
#endif // MDL_C500

/**
 * @brief Stupid timer callback wrapper to work-around callback_function_t and timerCallback_t framework-arduino differences.
 *   Also, framework-arduinogd32 implementation doesn't support callback arguments nor std::bind and thus no ptr to member functionality!
 */
void timer_slow_callback_wrapper()
{
    leds.blink_timer_elapsed(LED_state::LED_blink_slow);
#ifdef MDL_C500 // Model Classic 500
    magic_buttons();
#endif
#ifdef MOD_RAIN
    rain.process();
#endif
}

void timer_event_callback_wrapper()
{
#ifdef MDL_SAXPRO
    display::loop_low_prio();
#endif
}

void timer_fast_callback_wrapper()
{
#ifdef MOD_HALL
    emergency.periodic_send();
#endif
    leds.blink_timer_elapsed(LED_state::LED_blink_fast);
}

void timer_quick_callback_wrapper()
{
    getDataFromBuffer();
#ifdef MDL_SAXPRO // Model SAxPRO
    display::tick_inc(TIM_QUICK_PERIOD_MS);
#endif
#ifdef MOD_HALL
    emergency.read_and_send_if_emergency();
#endif
    buttons.process_states();
    leds.process_sequence();
}

/****************************************************************
 * Some dump OM wrapper for not polluting original code to much *
 ****************************************************************/

bool uart_is_readable(HardwareSerial *Serial)
{
    return Serial->available();
}

void uart_putc(HardwareSerial *Serial, uint8_t c)
{
    Serial->write(c);
}

void Force_LED_off(uint8_t led_num, bool force)
{
    leds.force_off(led_num, force); // This only effect blink states
}

void Blink_LED(PIO dummy, int dummy2, int led_num)
{
    leds.identify(led_num);
}

void buzzer_program_put_words(PIO pio, uint sm, uint32_t repeat, uint32_t duration, uint32_t gap)
{
    // YFC500 doesn't has (not yet?) a buzzer on CoverUI
}

void add_sim_button(uint8_t button_id, uint32_t press_timeout)
{
    sim_button_queue.push_back({button_id, press_timeout});
}

/**
 * @brief Get a simulated button. This function is only required for intelligent CoverUI's which act different than the simple Button/LED-Board,
 * as well as long we don't have a higher level abstraction in LL code. TODO
 *
 * @param press_timeout timeout to watch if a button is (long) pressed. We do not need to timeout this value in real, only honor it's value.
 * @param still_pressed pointer to boolean if it's still pressed after the timeout happen.
 * @return uint8_t button id of a simulated button, or 0 in the case when there's no button to simulate
 */
uint8_t get_sim_button(uint32_t press_timeout, bool &still_pressed)
{
    if (sim_button_queue.empty())
        return 0;

    still_pressed = false;

    SimButton sim_btn = sim_button_queue.front();
    if (press_timeout <= sim_btn.press_timeout)
    {
        still_pressed = true;
        sim_btn.press_timeout -= press_timeout;
    }
    else
    {
        still_pressed = false;
        sim_btn.press_timeout = 0;
    }

    // This doesn't look efficient
    sim_button_queue.pop_front();
    if (sim_btn.press_timeout)
        sim_button_queue.push_front(sim_btn);

    return sim_btn.button_id;
}

uint8_t bit_getbutton(uint32_t press_timeout, bool &still_pressed)
{
    // Check if we've a button to simulate in queue
    uint8_t sim_button = get_sim_button(press_timeout, still_pressed);
    if (sim_button)
        return sim_button;

    still_pressed = false;

    // Scan the buttons in the same order as original OM FW does
    for (uint i : buttons.kOMButtonNrs)
    {
        uint32_t start = millis(); // start press_timeout measurement
        if (buttons.is_pressed(i))
        {
            // wait for button released
            while (buttons.is_pressed(i) && (press_timeout == 0 || (millis() - start) < press_timeout))
                ;
            if (buttons.is_pressed(i))
                still_pressed = true;
            else
                still_pressed = false;
            return (i);
        }
    }
    return 0;
}

#endif /* __YARDFORCE_MAIN_H */