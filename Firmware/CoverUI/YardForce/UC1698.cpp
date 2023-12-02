/**
 * @file UC1698.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI UC1698 display controller class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        UC1698 display driver for monochrome COG LCD with parallel interface, for STM32.
 *        Derived from https://github.com/henningmz/uc1698 AdafruitGFX implementation, stripped down and heavily optimized.
 * @version 0.3
 * @date 2023-11-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/UC1698.hpp"

namespace yardforce
{
    namespace display
    {
        namespace controller
        {
            // Initialize control and data lines
            bool UC1698::init()
            {
                initConnection_();

                CLR_CS; // Chip select
                digitalWrite(PIN_RST, 0);
                delay(1);
                digitalWrite(PIN_RST, 1);
                SET_CS;                       // Chip deselect
                delay(UC1698_RESET_DELAY_MS); // Wait for MTP read ready (after RST)

                // Check if it's an UC1698
                uint8_t *status;
                status = getStatus();
                if ((status[2] & 0b11110000) >> 4 != 0b1000)
                {
                    return false;
                }

#ifdef BENCHMARK
                cycle_cnt_init_.start();
#endif
                initDisplay_();
#ifdef BENCHMARK
                cycle_cnt_init_.stop();
#endif

                return true;
            }

            // Fill screen (t_color: false = white, true = black)
            void UC1698::fillScreen(bool t_color_black)
            {
                setWindowProgramArea(0, UC1698_DISPLAY_WIDTH - 1, 0, UC1698_DISPLAY_HEIGHT - 1);

                for (uint8_t i = 0; i < UC1698_DISPLAY_HEIGHT; i++)
                {
                    for (uint8_t j = 0; j < UC1698_DISPLAY_WIDTH / 3; j++)
                    {
                        drawPixelTriplet(t_color_black, t_color_black, t_color_black);
                    }
                }
            }

            // Draw pixel triplet draws 3 pixels with one command.
            // Each UC1698 R,G,B line is connected to 3 x-pixels of his monochrome display.
            void UC1698::drawPixelTriplet(bool pixel1State, bool pixel2State, bool pixel3State)
            {
                writeData((0b1111100000000000 * pixel1State) | (0b0000011111100000 * pixel2State) | (0b0000000000011111 * pixel3State));
            }

            // [3] Get Status & PM
            // and write it to public member array status_a
            uint8_t *UC1698::getStatus()
            {
                static uint8_t status[UC1698_GET_STATUS_SIZE];

                CLR_CS; // Chip select
                CLR_CD; // Command
                for (unsigned int i = 0; i < UC1698_GET_STATUS_SIZE; i++)
                {
                    status[i] = read_();
                }
                SET_CS; // Chip deselect

                return status;
            }

            // [4] Set Column Address
            void UC1698::setColumnAddress(unsigned int t_col)
            {
                writeCommand(0b00000000 | (t_col & 0b00001111));
                writeCommand(0b00010000 | (t_col & 0b01110000) >> 4);
            }

            // [9] Set Row Address
            void UC1698::setRowAddress(unsigned int t_row)
            {
                writeCommand(0b01100000 | (t_row & 0b00001111));
                writeCommand(0b01110000 | (t_row & 0b11110000) >> 4);
            }

            // [17] Set Display Enable
            // Sets also Green Enhance Mode to disabled, Gray Shade to On/Off
            void UC1698::setDisplayEnable(bool t_sleep_mode)
            {
                writeCommand(0b10101100 | t_sleep_mode);
            }

            // [18] Set LCD Mapping Control
            void UC1698::setLCDMappingControl(bool mirrorX, bool mirrorY)
            {
                writeCommand(0b11000000 | (0b100 * mirrorY) | (0b10 * mirrorX));
            }

            // [10] Set VBias Potentiometer
            void UC1698::setVBiasPotentiometer(unsigned int vBiasPotentiometer)
            {
                UC1698::writeCommand(0b10000001);
                UC1698::writeCommand(vBiasPotentiometer);
            }

            /**
             * @brief Set Window Programm Area for next display RAM write data to x1-x2, y1-y2
             * Attention: By design of my monochrome LCD, which has 3 monochrome pixel (triplet on RGB) per pixel,
             * x1 & x2+1 have to be a multiple of 3!!
             * See LVGL's rounder_cb() callback to ensure this.
             *
             * @param t_x1
             * @param t_x2
             * @param t_y1
             * @param t_y2
             * @param t_outside_mode
             */
            void UC1698::setWindowProgramArea(uint8_t t_x1, uint8_t t_x2, uint8_t t_y1, uint8_t t_y2, bool t_outside_mode)
            {
                const uint8_t data[] = {
                    // Set SRAM col/row start address
                    (uint8_t)(0x10 | (((t_x1 / 3) >> 4) & 0b00000111)), // Set Column MSB Address
                    (uint8_t)(0x00 | ((t_x1 / 3) & 0b00001111)),        // Set Column LSB Address
                    (uint8_t)(0x70 | (t_y1 >> 4)),                      // Set Row MSB Address
                    (uint8_t)(0x60 | ((uint8_t)t_y1 & 0b00001111)),     // Set Row LSB Address

                    // Set Windows Program
                    (0xf5), (t_y1),                                  // Starting Row Address
                    (0xf7), (t_y2),                                  // Ending Row Address
                    (0xf4), (uint8_t)(t_x1 / 3),                     // Starting Column Address
                    (0xf6), (uint8_t)((int8_t)((t_x2 + 1) / 3) - 1), // Ending Column Address
                    (uint8_t)(0xf8 | t_outside_mode)                 // Windows Program Mode (0 = inside, 1 = outside)
                };
                writeCommand(data, sizeof(data) / sizeof(uint8_t));
            }

            // Write command
            void UC1698::writeCommand(uint8_t data)
            {
                CLR_CS; // Chip select
                CLR_CD; // Command
                writeSeq_(data);
                SET_CS; // Chip deselect
            }

            void UC1698::writeCommand(const uint8_t *data_array, unsigned int length)
            {
                CLR_CS; // Chip select
                CLR_CD; // Command
                for (; length > 0; length--, data_array++)
                {
                    writeSeq_(*data_array);
                }
                SET_CS; // Chip deselect
            }

            void UC1698::writeData(uint16_t data)
            {
                CLR_CS; // Chip select
                SET_CD; // Data
                uint8_t part = (uint8_t)(data >> 8);
                writeSeq_(part);

                part = (uint8_t)data;
                writeSeq_(part);

                SET_CS; // Chip deselect
            }

            void UC1698::writeData(const uint8_t *data_array, unsigned int length)
            {
                CLR_CS; // Chip select
                SET_CD; // Data
                for (; length > 0; length--, data_array++)
                {
                    writeSeq_(*data_array);
                    NOOP;
                    NOOP; // >=45ns. This NOOPs = 2*21ns + the next cycle. See Thpw80 of UC1698 datasheet page 60
                }
                SET_CS; // Chip deselect
            }

            // Initialize control and data lines
            void UC1698::initConnection_()
            {
                pinMode(PIN_CS, OUTPUT);
                digitalWrite(PIN_CS, 1);
                pinMode(PIN_RST, OUTPUT);
                digitalWrite(PIN_RST, 1);
                pinMode(PIN_RD, OUTPUT);
                digitalWrite(PIN_RD, 1);
                pinMode(PIN_WR, OUTPUT);
                digitalWrite(PIN_WR, 1);
                pinMode(PIN_CD, OUTPUT);
                digitalWrite(PIN_CD, 1);
                GPIO_DATA_MODE_OUTPUT;
            }

            // Initialize control and data lines
            void UC1698::initDisplay_()
            {
                writeCommand(0x28 | 3); // 0x28 = power control command (13nF<LCD<=22nF; Internal Vlcd(*10))
                writeCommand(0x24 | 1); // 0x24 = temperature control (-0.05%/C)
                setLCDMappingControl(0, 1);
                // this->setVBiasPotentiometer(127);
                writeCommand(0b10100110 | 1); // [16] Set Inverse Display. Will save inversion logic in LVGL's flush_cb()
                writeCommand(0b11010000 | 1); // [20] Set Color Pattern to R-G-B
                writeCommand(0b11010110);     // [21] Set Color Mode to RRRRR-GGGGGG-BBBBB, 64k-color

                // fillScreen(true); // Clear Screen
                setDisplayEnable(true);
            }

            // [2*] Read data from port
            uint8_t UC1698::read_()
            {
                GPIO_DATA_MODE_INPUT;
                digitalWrite(PIN_RD, 0); // Trigger read
                NOOP;                    // 3 * 21ns >=60ns. See Tacc80 of UC1698 datasheet page 60
                NOOP;
                NOOP;
                uint8_t data = ((GPIO_TypeDef *)GPIO_DATA)->IDR & GPIO_DATA_MASK;
                digitalWrite(PIN_RD, 1); // Read end
                GPIO_DATA_MODE_OUTPUT;

                return data;
            }

            // Write data sequence
            void UC1698::writeSeq_(uint8_t data)
            {
#ifdef BENCHMARK
                cycle_cnt_writeSeq_.start();
#endif

                CLR_WR; // Trigger write
                ((GPIO_TypeDef *)GPIO_DATA)->ODR = (((GPIO_TypeDef *)GPIO_DATA)->ODR & ~GPIO_DATA_MASK) | data;
                NOOP;   // >=30ns. This NOOP = 21ns + the next cycle for the rising edge. See Tds80 of UC1698 datasheet page 60
                SET_WR; // Write end
#ifdef BENCHMARK
                cycle_cnt_writeSeq_.stop();
#endif
            }
        } // namespace controller
    }     // namespace display
} // yardforce