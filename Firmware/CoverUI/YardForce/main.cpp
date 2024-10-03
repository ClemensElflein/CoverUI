/**
 * @file main.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI main for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.4
 * @date 2023-11-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/main.h"

#include <Arduino.h>        // Stock CoverUI is build now via Arduino framework (instead of HAL), which is ATM the only framework with STM32F030R8 and GD32F330R8 support
#include <HardwareTimer.h>  // Required for framework-arduinogd32

// ----- Timer -----
#ifdef MCU_STM32
#ifdef STM32F030x8
#define TIM_SLOW TIM6    // Basic timer
#define TIM_FAST TIM17   // General purpose timer
#define TIM_EVENT TIM14  // General purpose timer
#define TIM_QUICK TIM16  // General purpose timer
#elif defined(STM32F4xx)
#define TIM_SLOW TIM3    // General purpose timer
#define TIM_FAST TIM4    // General purpose timer
#define TIM_EVENT TIM9   // General purpose timer
#define TIM_QUICK TIM10  // General purpose timer
#endif
#define TIM_DEFAULT_PREEMPT_PRIO TIM_IRQ_PRIO
#define TIM_DEFAULT_SUB_PRIO TIM_IRQ_SUBPRIO
HardwareTimer *hwtimer(TIM_TypeDef *instance, uint32_t freq, callback_function_t callback, uint32_t preemptPriority = TIM_DEFAULT_PREEMPT_PRIO, uint32_t subPriority = TIM_DEFAULT_SUB_PRIO) {
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setOverflow(freq, HERTZ_FORMAT);
    Timer->setInterruptPriority(preemptPriority, subPriority);
    Timer->attachInterrupt(callback);
    Timer->resume();
    return Timer;
}
#else  // GD32
#define TIM_SLOW TIMER16
#define TIM_FAST TIMER15
#define TIM_QUICK TIMER14
HardwareTimer *hwtimer(uint32_t instance, uint32_t freq, timerCallback_t callback, uint32_t preemptPriority = 0, uint32_t subPriority = 0) {
    HardwareTimer *Timer = new HardwareTimer(instance);
    Timer->setPeriodTime(freq, FORMAT_HZ);
    // FIXME: GD's HW timer class doesn't has INT preemption- and sub- priorities implemented yet. Will become an issue latest with GD32 & LVGL!
    Timer->attachInterrupt(callback);
    Timer->start();
    return Timer;
}
#endif

#define ALIVE_CYCLE_MILLIS 500  // Non-timer alive blink
uint32_t alive_cycle_next = 0;

#define TIM_QUICK_FREQUENCY 200                                 // Hz
#define TIM_QUICK_PERIOD_MS (1.0 / TIM_QUICK_FREQUENCY * 1000)  // Milliseconds

HardwareTimer *timer_slow;  // Used for blink-slow LEDs and magic buttons
HardwareTimer *timer_fast;  // Used for blink-fast LEDs
#ifdef YARDFORCE_ABC_DISPLAY_HPP
HardwareTimer *timer_event;  // Used for lv_timer_handler() and LED/Value to display logic conversion
#endif
HardwareTimer *timer_quick;  // Button debouncer and LED sequences

// Forward declaration, see ../main.cpp
extern void core1();
extern void getDataFromBuffer();
#ifdef YARDFORCE_EMERGENCY_HPP
extern Emergency emergency;
#endif

void timer_slow_callback_wrapper();
void timer_fast_callback_wrapper();
void timer_event_callback_wrapper();
void timer_quick_callback_wrapper();

// YardForce implementation specific
#ifdef MOD_RAIN
Rain rain;
#endif

#ifdef MCU_STM32
HardwareSerial serial_ll(UART_LL_RX, UART_LL_TX);  // Serial connection to LowLevel MCU, JP2 Pin 1+3
#else                                              // MCU_GD32
HardwareSerial serial_ll((uint8_t)UART_LL_RX, (uint8_t)UART_LL_TX, 1);  // Serial connection to LowLevel MCU, J6/JP2 Pin 1+3
#endif

void setup() {
#ifdef MDL_RMECOWV100
    // RM-ECOW-V1.0.0 might have a populated ESP-WROOM-02 module.
    // Disable it, because one might have made MOD_HALL but flash or test another FW variant
    pinMode(PF7, OUTPUT);    // ESP-WROOM-02 EN pin
    digitalWrite(PF7, LOW);  // Disable ESP-WROOM-02 module so that it's GPIOs doesn't collide with ours
#endif

#ifdef YARDFORCE_ABC_DISPLAY_HPP
    if (!display.init())
        display.set_backlight(LED_blink_fast, 60000);  // TODO: Make some better assert handling than 60 sec. fast blink
    else
        display.set_backlight();
#endif

    // We've hardware timer on mass, let's use them.
    timer_slow = hwtimer(TIM_SLOW, 2, timer_slow_callback_wrapper, 30);   //   2Hz (500ms) timer, used for LED-blink-slow and magic buttons
    timer_fast = hwtimer(TIM_FAST, 10, timer_fast_callback_wrapper, 20);  //  10Hz (100ms) timer, used for LED-blink-fast
#ifdef YARDFORCE_ABC_DISPLAY_HPP
    timer_event = hwtimer(TIM_EVENT, 100, timer_event_callback_wrapper, 10);  // 100Hz   (10ms) timer, used for displays lv_timer_handler() and LED-2-Display logic
#endif
    // Don't increase value of this timers preemptPriority parameter higher than the default,
    // as this timer callback also handles HardwareSerial as well as button-debouncer,
    // which would mess expected processing of those.
    timer_quick = hwtimer(TIM_QUICK, TIM_QUICK_FREQUENCY, timer_quick_callback_wrapper);  // 200Hz (5ms) timer, used for Buttons debouncer and LED- sequences

    serial_ll.begin(115200);

    delay(100);  // Some required stupid delay, dunno why :-/

    // "Hi there" and jammed button mounting detection
    do {
        // LED blink to say it's alive
        // (this processing delay is also required to get the debouncer filled with a consistent state (NUM_BUTTON_STATES * 5ms)
        delay(leds.boot_animation() + 500);

    } while (buttons.is_pressed());
}

/**
 * @brief Stupid timer callback wrapper to work-around callback_function_t and timerCallback_t framework-arduino differences.
 *   Also, framework-arduinogd32 implementation doesn't support callback arguments nor std::bind and thus no ptr to member functionality!
 */
void timer_slow_callback_wrapper() {
    leds.blink_timer_elapsed(LED_state::LED_blink_slow);
#ifdef MOD_RAIN
    rain.process();
#endif
}

void timer_event_callback_wrapper() {
#ifdef YARDFORCE_ABC_DISPLAY_HPP
    display.loop_low_prio();
#endif
}

void timer_fast_callback_wrapper() {
#ifdef YARDFORCE_EMERGENCY_HPP
    emergency.periodic_send();
#endif
    buttons.handle_sys_req();
    leds.blink_timer_elapsed(LED_state::LED_blink_fast);
}

void timer_quick_callback_wrapper() {
    getDataFromBuffer();
#ifdef YARDFORCE_ABC_DISPLAY_HPP
    display.tick_inc(TIM_QUICK_PERIOD_MS);
#endif
#ifdef YARDFORCE_EMERGENCY_HPP
    emergency.read_and_send_if_emergency();
#endif
    buttons.process_states();
    leds.process_sequence();
}

/****************************************************************
 * Some dump OM wrapper for not polluting original code to much *
 ****************************************************************/

bool uart_is_readable(HardwareSerial *Serial) {
    return Serial->available();
}

void uart_putc(HardwareSerial *Serial, uint8_t c) {
    Serial->write(c);
}

void Force_LED_off(uint8_t led_num, bool force) {
    leds.force_off(led_num, force);  // This only effect blink states
}

void buzzer_program_put_words(PIO pio, unsigned int sm, uint32_t repeat, uint32_t duration, uint32_t gap) {
    // YFC500 doesn't has (not yet?) a buzzer on CoverUI
}

// ----- YardForce specific main loop, mainly does the same as ../main.cpp::core1()
unsigned int last_button_id = 0;
unsigned int last_button_cnt;
void loop() {
    // Scan the buttons in the same order as original OM FW does
    for (auto const &it : buttons.kBtnDefByNumMap)  // Loop over Button-Num -> button pin map
    {
        uint32_t start = millis();  // start press_timeout measurement
        if (buttons.is_pressed(it.first)) {
            if (it.first != last_button_id)
                last_button_cnt = 0;
            last_button_id = it.first;
            int8_t led_num = buttons.get_led(last_button_id);
            if (led_num > BTN_HAS_NO_LED)
                Force_LED_off(led_num, true);
            while (buttons.is_pressed(it.first) && (millis() - start) < 1000)  // wait for button released but max. 1000ms
                ;
            if (buttons.is_pressed(it.first))  // Still pressed
            {
                if (last_button_cnt < 2) {
                    last_button_cnt++;
                    if (led_num >= 0)
                        for (int i = 0; i < last_button_cnt; i++)
                            leds.identify(led_num);
                }
            } else  // Button released
            {
#ifdef YARDFORCE_ABC_HATCH_HPP
                last_button_id = hatch.handle_button(last_button_id, last_button_cnt);
#endif
                if (last_button_id)
                    buttons.send(last_button_id, last_button_cnt);
                last_button_id = 0;
                last_button_cnt = 0;
            }
            if (led_num >= 0)
                Force_LED_off(led_num, false);
            break;
        }
    }

#ifdef YARDFORCE_ABC_HATCH_HPP
    hatch.process_queued();
#endif

    // Backside LED alive blink
    if (millis() > alive_cycle_next) {
        alive_cycle_next = millis() + ALIVE_CYCLE_MILLIS;
        leds.toggle(LED_NUM_REAR);
    }
}
