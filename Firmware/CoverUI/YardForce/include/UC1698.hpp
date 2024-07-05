/**
 * @file UC1698.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI UC1698 display controller class header for OpenMower https://github.com/ClemensElflein/OpenMower
 *        UC1698 display driver for monochrome COG LCD with parallel interface, for STM32.
 *        Derived from https://github.com/henningmz/uc1698 AdafruitGFX implementation, stripped down and heavily optimized.
 * @version 0.3
 * @date 2023-11-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UC1698_HPP
#define __UC1698_HPP

#include <Arduino.h>

#define PIN_RST PA12 // ReSTart pin
#define PIN_RD PC9   // Read control line

#define PIN_CS PC7                                                          // Chip Select pin
#define SET_CS GPIOC->ODR = GPIOC->ODR | 0b10000000                         // Set CS (PC7) = high
#define CLR_CS GPIOC->ODR = GPIOC->ODR & 0b11111111111111111111111101111111 // Clear CS (PC7) = low

#define PIN_CD PA8                                                          // Command/Data (A0)
#define SET_CD GPIOA->ODR = GPIOA->ODR | 0b100000000                        // Set CD (PA8) = high
#define CLR_CD GPIOA->ODR = GPIOA->ODR & 0b11111111111111111111111011111111 // Clear CD (PA8) = low

#define PIN_WR PC8                                                          // Write control line. Only for Mode set
#define SET_WR GPIOC->ODR = GPIOC->ODR | 0b100000000                        // Set WR (PC8) = high
#define CLR_WR GPIOC->ODR = GPIOC->ODR & 0b11111111111111111111111011111111 // Clear WR (PC8) = low

#define GPIO_DATA GPIOB_BASE  // GPIO port/base of the data lines. Assumed that D0-D7 are wired to ONE port!
#define GPIO_DATA_MASK 0x00ff // Mask which forms the data. Assumed that D0-D7 are all connected consecutive via LSB 0

// Shorthands to set GPIO Mode data pins to input/output
#define GPIO_DATA_MODE_INPUT ((GPIO_TypeDef *)GPIO_DATA)->MODER = (((GPIO_TypeDef *)GPIO_DATA)->MODER & 0xffff0000) & 0xffff0000;         // D0-D7 Mode 0 = Input
#define GPIO_DATA_MODE_OUTPUT ((GPIO_TypeDef *)GPIO_DATA)->MODER = (((GPIO_TypeDef *)GPIO_DATA)->MODER & 0xffff0000) | 0b0101010101010101 // D0-D7 Mode 01 = Output

#define NOOP asm("nop\n") // For STM32F030x it's 1/48MHz which is 20.9ns

#define UC1698_RESET_DELAY_MS 150
#define UC1698_GET_STATUS_SIZE 3

#ifndef UC1698_DISPLAY_WIDTH
#define UC1698_DISPLAY_WIDTH 240
#endif
#ifndef UC1698_DISPLAY_HEIGHT
#define UC1698_DISPLAY_HEIGHT 128
#endif

// Enable for benchmarking specific code
// #define BENCHMARK

#ifdef BENCHMARK
#include "include/CortexMCycleCounter.hpp"
#endif

namespace yardforce
{
    namespace display
    {
        namespace controller
        {
            class UC1698
            {
            public:
                UC1698(){};

                bool init(); // Initialize control and data lines

                void setWindowProgramArea(uint8_t t_x1, uint8_t t_x2, uint8_t t_y1, uint8_t t_y2, bool t_outside_mode = false);
                void drawPixelTriplet(bool pixel1State, bool pixel2State, bool pixel3State);
                void fillScreen(bool t_color_black); // Fill screen (t_color: false = white, true = black)

                void writeData(uint16_t data);
                void writeData(const uint8_t *data_array, unsigned int length);

            private:
                void initConnection_(); // Initialize control and data lines
                void initDisplay_();    // Initialize control and data lines

                uint8_t *getStatus();
                void setColumnAddress(unsigned int t_col);
                void setRowAddress(unsigned int t_row);
                void setDisplayEnable(bool t_sleep_mode);
                void setLCDMappingControl(bool mirrorX, bool mirrorY);
                void setVBiasPotentiometer(unsigned int vBiasPotentiometer);

                void writeCommand(uint8_t data);
                void writeCommand(const uint8_t *data_array, unsigned int length);

                uint8_t read_();
                void writeSeq_(uint8_t data);

#ifdef BENCHMARK
                CortexMCycleCounter cycle_cnt_init_;
                CortexMCycleCounter cycle_cnt_writeSeq_;
#endif
            };
        } // namespace controller
    }     // namespace display
} // namespace yardforce

#endif // __UC1698_HPP