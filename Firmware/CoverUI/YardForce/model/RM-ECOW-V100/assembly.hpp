/**
 * @file RM-ECOW-V100/assembly.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Assembly for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_MDL_RMECOWV100_ASSEMBLY_HPP
#define YARDFORCE_MDL_RMECOWV100_ASSEMBLY_HPP

#include "../../include/Buttons.hpp"
#include "../../include/main.h"
#include "../RM-ECOW-V1x/LEDcontrol.hpp"

#ifdef MOD_EMERGENCY
#include "../../include/Emergency.hpp"
#endif

// ***** LEDs *****
// 1st row: Lifted, Wire (WLAN), Battery empty, Charge
#define LED_PIN_LIFTED PB11
#define LED_PIN_WIRE PB12
#define LED_PIN_BAT PB14
#define LED_PIN_CHARGE PB15
// 2nd row: 4, 6, 8, 10HR
#define LED_PIN_4H PA7
#define LED_PIN_6H PC5
#define LED_PIN_8H PB1
#define LED_PIN_10H PB10
// 3rd row: S1, Setup (WLAN), S2
#define LED_PIN_S1 PA5
#define LED_PIN_SETUP PF5
#define LED_PIN_S2 PC7
// 4th row: Lock
#define LED_PIN_LOCK PA1
// Backside
#define LED_PIN_REAR PB13

// ***** Buttons *****
#define BTN_S1_PIN PA4
#define BTN_S2_PIN PC6
#define BTN_LOCK_PIN PA0
#define BTN_4H_PIN PA6
#define BTN_6H_PIN PC4
#define BTN_8H_PIN PB0
#define BTN_10H_PIN PB2
#define BTN_PLAY_PIN PC11
#define BTN_HOME_PIN PC12
#define BTN_SETUP_PIN PF4

// ***** Emergency *****
#define PIN_STOP_JP5 PC10
#define PIN_STOP_JP6 PA15

#define PIN_HALL1 PA8   // LIFT
#define PIN_HALL2 PF6   // LIFTX
#define PIN_HALL3 PA12  // LBUMP
#define PIN_HALL4 PA11  // RBUMP

extern LEDcontrolRMECOWV1x leds;

class ButtonsRMECOWV100 : public Buttons {
   public:
    ButtonsRMECOWV100(const std::map<uint8_t, ButtonDef> &t_kBtnDefByNumMap) : Buttons(t_kBtnDefByNumMap) {};

    /**
     * @brief Check if one of the internal button functionality got pressed and do it's function.
     * SETUP + 4H = Display FW version
     * SETUP + 10H = LED animation
     */
    void handle_sys_req() override {
        if (!is_pressed(BTN_SETUP_NUM))
            return;

        if (is_pressed(BTN_10H_NUM))
            leds.sequence_start(&LEDcontrol::sequence_animate_handler);
        else if (is_pressed(BTN_4H_NUM))
            leds.show_num(FIRMWARE_VERSION);
        return;
    };
};
extern ButtonsRMECOWV100 buttons;

#include "../RM-ECOW-V1x/Hatch.hpp"

// LowLevel Serial Pins
#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif  // YARDFORCE_MDL_RMECOWV100_ASSEMBLY_HPP
