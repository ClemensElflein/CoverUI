/**
 * @file SAxPRO/assembly.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Assembly for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_MDL_SAXPRO_ASSEMBLY_HPP
#define YARDFORCE_MDL_SAXPRO_ASSEMBLY_HPP

#include "../../include/Buttons.hpp"
#include "../../include/LEDcontrol.hpp"
#include "../../include/main.h"

// FIXME: AbstractDisplay depends on leds
extern LEDcontrol leds;
#include "Display.hpp"

#ifdef MOD_EMERGENCY
#include "../../include/Emergency.hpp"
#endif

// ***** LEDs *****

#define LED_PIN_BACKLIGHT PA11  // SAxPRO only has one LED, the backlight LED
#define LED_NUM_BACKLIGHT 19

// ***** Buttons *****

#define BTN_PLAY_PIN PC0  // or Start
#define BTN_HOME_PIN PC1
#define BTN_UP_PIN PB14
#define BTN_DOWN_PIN PB13
#define BTN_OK_PIN PB12  // or Enter
#define BTN_BACK_PIN PB15

extern Buttons buttons;

// ----- Hatch -----

#include "../../include/ABC_Hatch.hpp"

class HatchSAXPRO : public AbstractHatch {
   public:
    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt);
};

extern HatchSAXPRO hatch;

// ----- LowLevel Serial -----

#define UART_LL_RX PA3
#define UART_LL_TX PA2

#endif  // YARDFORCE_MDL_SAXPRO_ASSEMBLY_HPP
