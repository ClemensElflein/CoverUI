/**
 * @file LEDcontrol_C500.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 LED driver header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_C500_HPP
#define YARDFORCE_LEDCONTROL_C500_HPP

#include <Arduino.h>
#include "../LEDcontrol.hpp"

// 1st row: 2,4,6,8HR
#define LED_PIN_2HR PA4
#define LED_PIN_4HR PA5
#define LED_PIN_6HR PA6
#define LED_PIN_8HR PA7

// 2nd row: S1, S2, LOCK
#define LED_PIN_S1 PA0
#define LED_PIN_S2 PA1
#define LED_PIN_LOCK PC4

// 3rd row: Mon-Sun
#define LED_PIN_MON PA15
#define LED_PIN_TUE PC10
#define LED_PIN_WED PC11
#define LED_PIN_THU PC12
#define LED_PIN_FRI PD2
#define LED_PIN_SAT PB3
#define LED_PIN_SUN PB4

// 4th row: Lifted, Wire, Bat, Charge
#define LED_PIN_LIFTED PC0
#define LED_PIN_WIRE PC1
#define LED_PIN_BAT PC2
#define LED_PIN_CHARGE PC3

// Backside
#define LED_PIN_REAR PB0

#define LED_ANIMATE_DELAY 15 // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

class LEDcontrolC500 : public LEDcontrol
{
public:
    LEDcontrolC500(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback) {}

    void sequence_animate_handler() override;

    unsigned int boot_animation(); // A short boot animation which return the amount of ms it will take
    void set_base10_leds(char digit) override;
    void force_off_num_seq_leds(bool force) override;
};

extern LEDcontrolC500 leds; // Main LED controller object

#endif // YARDFORCE_LEDCONTROL_C500_HPP
