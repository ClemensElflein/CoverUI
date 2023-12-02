/**
 * @file LEDcontrol_RM-EC3-V11.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce LED driver header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_LEDCONTROL_RMEC3V11_HPP
#define YARDFORCE_LEDCONTROL_RMEC3V11_HPP

#include <Arduino.h>
#include "../LEDcontrol.hpp"

// 1st row: Lifted, Wire (WLAN), Battery empty, Charge
#define LED_PIN_S1 PC4
#define LED_PIN_SETUP PA1
#define LED_PIN_S2 PC5
#define LED_PIN_BACKLIGHT PA8

#define LED_NUM_BACKLIGHT 20

#define LED_ANIMATE_DELAY 50 // Delay (ms) between LEDs of animate sequence (boot/power-on anim)

class LEDcontrolRMEC3V11 : public LEDcontrol
{
public:
    LEDcontrolRMEC3V11(const uint32_t *t_kPinByLedNumPtr, const size_t t_kNumLeds, void (LEDcontrol::*t_set_base10_leds_callback)(char digit)) : LEDcontrol(t_kPinByLedNumPtr, t_kNumLeds, t_set_base10_leds_callback){};

    void sequence_animate_handler() override;
    void sequence_countdown_handler() override;

    unsigned int boot_animation() override; // A short boot animation which return the amount of ms it will take
    //void set_base10_leds(char digit) override;
    //void force_off_num_seq_leds(bool force) override;

private:
    void show_countdown_state(unsigned int sec, LED_state state);
};

extern LEDcontrolRMEC3V11 leds;

#endif // YARDFORCE_LEDCONTROL_RMEC3V11_HPP
