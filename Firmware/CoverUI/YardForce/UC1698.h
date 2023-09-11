/**
 * @file UC1698.h
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI UC1698 display driver class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        UC1698 display driver for monochrome COG LCD with parallel interface, for STM32.
 *        Derived from https://github.com/henningmz/uc1698 AdafruitGFX implementation, stripped down and heavily optimized.
 * @version 0.1
 * @date 2023-09-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UC1698_H
#define __UC1698_H

#include <Arduino.h>

#define PIN_CS PC7   // Chip Select pin
#define PIN_RST PA12 // ReSTart pin
#define PIN_RD PC9   // Read control line
#define PIN_WR PC8   // Write control line
#define PIN_CD PA8   // Command/Data (A0)

#define GPIO_DATA GPIOB_BASE  // GPIO port/base of the data lines. Assumed that D0-D7 are wired to ONE port!
#define GPIO_DATA_MASK 0x00ff // Mask which forms the data. Assumed that D0-D7 are all connected consecutive via LSB 0

// Shorthands to set GPIO Mode data pins to input/output
#define GPIO_DATA_MODE_INPUT ((GPIO_TypeDef *)GPIO_DATA)->MODER = (((GPIO_TypeDef *)GPIO_DATA)->MODER & 0xffff0000) & 0xffff0000;         // Mode 0 = Input
#define GPIO_DATA_MODE_OUTPUT ((GPIO_TypeDef *)GPIO_DATA)->MODER = (((GPIO_TypeDef *)GPIO_DATA)->MODER & 0xffff0000) | 0b0101010101010101 // Mode 01 = Output

#define NOP_CYCLE_NS 21 // For STM32F030x it's 1/48MHz which is 20.9ns (but we use int for not wasting time in slow FP calculations)
#define NOOP asm("nop\n")

#define UC1698_RESET_DELAY_MS 150
#define UC1698_GET_STATUS_SIZE 3

#ifndef UC1698_DISPLAY_WIDTH
#define UC1698_DISPLAY_WIDTH 240
#endif
#ifndef UC1698_DISPLAY_HEIGHT
#define UC1698_DISPLAY_HEIGHT 128
#endif

namespace display
{

    class UC1698
    {
    public:
        UC1698() {}

        bool init();

        void drawPixelTriplet(bool t_pixel1State, bool t_pixel2State, bool t_pixel3State);
        void fillScreen(bool t_color_black);

        uint8_t *getStatus();

        void setColumnAddress(uint8_t t_col);
        void setDisplayEnable(bool t_sleep_mode);
        void setLCDMappingControl(bool mirrorX, bool mirrorY);
        void setRowAddress(uint8_t t_row);
        void setVBiasPotentiometer(uint8_t vBiasPotentiometer);
        void setWindowProgramArea(uint8_t t_x1, uint8_t t_x2, uint8_t t_y1, uint8_t t_y2, bool t_outside_mode = false);

        void writeCommand(uint8_t data);
        void writeCommand(const uint8_t *data_array, uint32_t length);
        void writeData(uint16_t data);
        void writeData(const uint8_t *data_array, uint32_t length);

    private:
        void initConnection_();
        void initDisplay_();

        void nopDelay_(uint_fast16_t t_ns);
        uint8_t read_();
        void writeSeq_(uint8_t data);
    };
} // namespace display

#endif // __UC1698_H