/**
 * @file ST75256.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI ST75256 LCD controller/driver class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        ST75256 for monochrome/gray COG LCD with 4-wire-SPI interface, for STM32.
 * @version 0.1
 * @date 2023-11-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __ST75256_HPP
#define __ST75256_HPP

#include <Arduino.h>

#define PIN_RST PA12 // Restart pin

// A0: 1 = Data, 0 = Control
#define PIN_DC PA11
#define GPIO_PORT_DC GPIOA
#define GPIO_PIN_DC 11
#define SET_DC_CTRL GPIO_PORT_DC->ODR &= ~(1 << GPIO_PIN_DC)
#define SET_DC_DATA GPIO_PORT_DC->ODR |= (1 << GPIO_PIN_DC)

#define ST75256_RESET_WAIT_MS 1

#ifndef ST75256_DISPLAY_WIDTH
#define ST75256_DISPLAY_WIDTH 256
#endif
#ifndef ST75256_DISPLAY_HEIGHT
#define ST75256_DISPLAY_HEIGHT 64
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
            class ST75256
            {
            public:
                ST75256(){};

                bool init(); // Initialize control and data lines, as well as display

                void send_ctrl(uint8_t ctrl);                    // Send control command
                void send_data(uint8_t data);                    // Send one data byte
                void send_data(uint8_t *data, size_t data_size); // Send multiple data bytes
                void set_column_address(uint8_t xs, uint8_t xe); // Set Column Address Area for next DDRAM writes
                void set_page_address(uint8_t ys, uint8_t ye);   // Set Page Address Area for next DDRAM writes

            private:
                void initConnection_();                               // Initialize control and data lines
                void initDisplay_();                                  // Initialize control and data lines
                void write_SPI_(uint8_t *data, size_t data_size = 1); // Write data via SPI

#ifdef BENCHMARK
                CortexMCycleCounter cycle_cnt_init_;
                CortexMCycleCounter cycle_cnt_spi_write_;
#endif
            };
        } // namespace controller
    }     // namespace display
} // namespace yardforce

#endif // __ST75256_HPP