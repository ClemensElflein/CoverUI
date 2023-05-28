/**
 * @file Buttons.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Buttons class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.2
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YFC500_BUTTONS_H
#define YFC500_BUTTONS_H

#include <Arduino.h>
#include <stdint.h>
#include "ButtonDebouncer.h"

#ifdef MCU_GD32
#define GPIOA_BASE GPIOA
#define GPIOB_BASE GPIOB
#define GPIOC_BASE GPIOC
#define GPIOF_BASE GPIOF
#endif

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

#define NUM_BUTTONS 14
#define NUM_GPIO_PORTS 4

// Some handy defines
#define BTN_NUM_CLK 0
#define BTN_NUM_HOME 1
#define BTN_NUM_OK 6
#define BTN_NUM_SUN 13

class Buttons
{
private:
    // Somehow static initialization, but's not expected that the PCB will change anymore ;-)
    // All ports with a button get debounced per port, via timer callback
    // const GPIO_TypeDef *kGpioPorts[NUM_GPIO_PORTS] = {GPIOA, GPIOB, GPIOC, GPIOF};
    const uint32_t kGpioPorts[NUM_GPIO_PORTS] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOF_BASE};
    ButtonDebouncer *debouncers_[NUM_GPIO_PORTS] = { // Debouncer obj for each port in the same order as kGpioPorts
        new ButtonDebouncer(), new ButtonDebouncer(),
        new ButtonDebouncer(), new ButtonDebouncer()};

    struct ButtonDef
    {
        uint8_t debouncer_index; // Debouncer index as defined in debouncers_ array
        uint8_t digital_pin;
    };

    // Map OM button number to YFC500 Button-definiton (but we index n=0 and not n>0, which get handled in OM wrapper main.hpp::bit_getbutton())
    // Again: Some-how static with debouncer_index
    const ButtonDef ButtonNrs[NUM_BUTTONS] = {
        {3, BTN_CLK_PIN},  //  0
        {0, BTN_HOME_PIN}, //  1
        {0, BTN_PLAY_PIN}, //  2
        {1, BTN_S1_PIN},   //  3
        {1, BTN_S2_PIN},   //  4
        {1, BTN_LOCK_PIN}, //  5
        {3, BTN_OK_PIN},   //  6
        {1, BTN_MON_PIN},  //  7
        {1, BTN_TUE_PIN},  //  8
        {1, BTN_WED_PIN},  //  9
        {1, BTN_THU_PIN},  // 10
        {2, BTN_FRI_PIN},  // 11
        {2, BTN_SAT_PIN},  // 12
        {2, BTN_SUN_PIN}   // 13
    };

public:
    void setup();
    void process_states();                        // Has to get called regulary i.e. by timer (5ms)
    uint16_t get_status(uint8_t debouncer_index); // Get status of all pins on the given debouncer index (as declared in *kGpioPorts)
    bool is_pressed(uint8_t button_nr);           // Return boolean if the given button number is pressed
};

#endif /* YFC500_BUTTONS_H */
