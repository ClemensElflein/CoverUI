/**
 * @file LEDcontrol_RM-ECOW-V110.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_LEDCONTROL_RMECOWV110_HPP
#define YARDFORCE_LEDCONTROL_RMECOWV110_HPP

#include <Arduino.h>
#include "../LEDcontrol.hpp"

// 1st row: Lifted, Wire (WLAN), Battery empty, Charge
#define LED_PIN_LIFTED PA0
#define LED_PIN_WIRE PA4
#define LED_PIN_BAT PC4
#define LED_PIN_CHARGE PA6

// 2nd row: 4, 6, 8, 10HR
#define LED_PIN_4H PA7
#define LED_PIN_6H PC5
#define LED_PIN_8H PB1
#define LED_PIN_10H PB10

// 3rd row: S1, Reserved for Setup (WLAN), S2
#define LED_PIN_S1 PA5
#define LED_PIN_S2 PC7

// 4th row: Lock
#define LED_PIN_LOCK PA1

// Backside
#define LED_PIN_REAR PB0

#define LED_ANIMATE_DELAY 20 // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

class LEDcontrolRMECOWV110 : public LEDcontrol
{
public:
    LEDcontrolRMECOWV110(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback){};

    void sequence_animate_handler() override;
    void sequence_countdown_handler() override;

    unsigned int boot_animation() override; // A short boot animation which return the amount of ms it will take
    void set_base10_leds(char digit) override;
    void force_off_num_seq_leds(bool force) override;

private:
    void show_countdown_state(unsigned int sec, LED_state state);
};

extern LEDcontrolRMECOWV110 leds;

#endif // YARDFORCE_LEDCONTROL_RMECOWV110_HPP
