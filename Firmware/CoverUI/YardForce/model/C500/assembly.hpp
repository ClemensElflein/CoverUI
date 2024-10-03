/**
 * @file C500/assembly.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic500 CoverUI Assembly for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_MDL_C500_ASSEMBLY_HPP
#define YARDFORCE_MDL_C500_ASSEMBLY_HPP

#include "../../include/Buttons.hpp"
#include "../../include/main.h"
#include "LEDcontrol.hpp"

#ifdef MOD_EMERGENCY
#include "../../include/Emergency.hpp"
#endif

// ***** LEDs *****
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

// ***** Buttons *****
#define BTN_CLK_PIN PF4
#define BTN_OK_PIN PF5
#define BTN_S1_PIN PB2
#define BTN_S2_PIN PB10
#define BTN_LOCK_PIN PB11
#define BTN_MON_PIN PB12
#define BTN_TUE_PIN PB13
#define BTN_WED_PIN PB14
#define BTN_THU_PIN PB15
#define BTN_FRI_PIN PC6
#define BTN_SAT_PIN PC7
#define BTN_SUN_PIN PC8
#define BTN_PLAY_PIN PA11
#define BTN_HOME_PIN PA12

// ***** Emergency *****
#define PIN_HALL_STOP_WHITE PC5
#define PIN_HALL_STOP_YELLOW PB6
#define PIN_HALL_WHEEL_RED PB7
#define PIN_HALL_WHEEL_BLUE PB8

extern LEDcontrolC500 leds;

class ButtonsC500 : public Buttons {
   public:
    ButtonsC500(const std::map<uint8_t, ButtonDef> &t_kBtnDefByNumMap) : Buttons(t_kBtnDefByNumMap) {};

    /**
     * @brief Check if one of the internal button functionality got pressed and do it's function.
     * OK + CLK = Display FW version
     * OK + SUN = LED animation
     */
    void handle_sys_req() override {
        if (!is_pressed(BTN_OK_NUM))
            return;

        if (is_pressed(BTN_SUN_NUM))
            leds.sequence_start(&LEDcontrol::sequence_animate_handler);
        else if (is_pressed(BTN_CLK_NUM))
            leds.show_num(FIRMWARE_VERSION);
        return;
    };
};
extern ButtonsC500 buttons;

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif  // YARDFORCE_MDL_C500_ASSEMBLY_HPP
