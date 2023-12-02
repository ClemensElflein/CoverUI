/**
 * @file ST75256.cpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce CoverUI ST75256 LCD controller/driver class for OpenMower https://github.com/ClemensElflein/OpenMower
 *        ST75256 for monochrome/gray COG LCD with 4-wire-SPI interface, for STM32.
 * @version 0.1
 * @date 2023-11-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/main.h"

#define TX_BUFFERSIZE 200

namespace yardforce
{
    namespace display
    {
        namespace controller
        {
            SPI_HandleTypeDef hspi2;

            // Initialize control and data lines, as well as display
            bool ST75256::init()
            {
                initConnection_();

                // Reset via RST control line
                digitalWrite(PIN_RST, 0);
                delay(ST75256_RESET_WAIT_MS);
                digitalWrite(PIN_RST, 1);
                delay(ST75256_RESET_WAIT_MS);

#ifdef BENCHMARK
                cycle_cnt_init_.start();
#endif
                initDisplay_();
#ifdef BENCHMARK
                cycle_cnt_init_.stop();
#endif
                return true;
            }

            void ST75256::initConnection_()
            {
                pinMode(PIN_RST, OUTPUT);
                digitalWrite(PIN_RST, 1);

                pinMode(PIN_DC, OUTPUT);
                digitalWrite(PIN_DC, 1);

                // ----- HAL specific inits -----

                // SPI clock & SPI GPIOs
                __HAL_RCC_SPI2_CLK_ENABLE();

                // HAL specific alternate PC3 = SPI2_MOSI
                GPIO_InitTypeDef GPIO_InitStruct = {0};
                GPIO_InitStruct.Pin = GPIO_PIN_3;
                GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull = GPIO_PULLUP;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
                HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

                // HAL specific alternative:
                // PB10 = SPI2_SCK
                // PB12 = SPI2_NSS
                GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
                GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
                GPIO_InitStruct.Pull = GPIO_PULLUP;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
                GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

                // Init SPI2 for HAL usage
                hspi2.Instance = SPI2;
                hspi2.Init.Mode = SPI_MODE_MASTER;
                hspi2.Init.Direction = SPI_DIRECTION_2LINES;
                hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
                hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
                hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
                hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
                hspi2.Init.BaudRatePrescaler = ST75256_SPI_BAUDRATEPRESCALER;
                hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
                hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
                hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
                hspi2.Init.CRCPolynomial = 10;
                HAL_SPI_Init(&hspi2);
            }

            void ST75256::send_data(uint8_t *data, size_t data_size)
            {
                SET_DC_DATA;
                write_SPI_(data, data_size);
            }

            void ST75256::send_data(uint8_t data)
            {
                SET_DC_DATA;
                write_SPI_(&data, 1);
            }

            void ST75256::send_ctrl(uint8_t ctrl)
            {
                SET_DC_CTRL;
                write_SPI_(&ctrl);
            }

            /**
             * @brief Write data via SPI
             *
             * @param data
             */
            void ST75256::write_SPI_(uint8_t *data, size_t data_size)
            {
#ifdef BENCHMARK
                cycle_cnt_spi_write_.start();
#endif
                HAL_SPI_Transmit(&hspi2, data, data_size, 100);
#ifdef BENCHMARK
                cycle_cnt_spi_write_.stop();
#endif
            }

            // Initialize control and data lines
            void ST75256::initDisplay_()
            {
                send_ctrl(0x30); // EXT=0
                send_ctrl(0x94); // Sleep out
                send_ctrl(0x31); // EXT=1
                send_ctrl(0xd7); // Autoread disable
                send_data(0x9f);

                send_ctrl(0x32); // Analog SET
                send_data(0x00); // OSC Freq. adj.
                send_data(0x01); // Freq. on booster cap. -> 6kHz
                send_data(0x05); // Bias=1/9

                /*send_ctrl(0x20); // Gray Level
                send_data(0x01);
                send_data(0x03);
                send_data(0x05);
                send_data(0x07);
                send_data(0x09);
                send_data(0x0b);
                send_data(0x0d);
                send_data(0x10);
                send_data(0x11);
                send_data(0x13);
                send_data(0x15);
                send_data(0x17);
                send_data(0x19);
                send_data(0x1b);
                send_data(0x1d);
                send_data(0x1f);*/

                send_ctrl(0x30); // EXT1=0, EXT0=0
                send_ctrl(0x75); // Page Address setting
                send_data(0X00); // YS=0X00
                send_data(0X1f); // YE=0x1F

                send_ctrl(0x15); // Column Address setting
                send_data(0X00); // XS=0
                send_data(0Xff); // XE=255

                send_ctrl(0xBC);       // [9] Data scan direction
                send_data(0x00);       // MV, MX, MY = Normal = From top-left to bottom-right
                send_ctrl(0b10100110); // [3] Inverse Display
                send_ctrl(0x0c);       // Data Format 0x0C: LSB(DB0) on top, 0x08: LSB(DB0) on bottom (default)

                send_ctrl(0xCA); // [5] Display Control
                send_data(0X00); // CL:CLD=0
                send_data(0X3F); // Duty=64
                send_data(0X20); // Nline=off

                send_ctrl(0xF0); // Display Mode
                send_data(0X10); // 10=Monochrome Mode,11=4Gray

                send_ctrl(0x81); // [21] Set Vop (EV control)
                send_data(0x01); // 0x01 = best, 0x11 only for pixel counting
                send_data(0x02);

                send_ctrl(0x20); // Power control
                send_data(0x0B); // D0=regulator ; D1=follower ; D3=booste, on:1 off:0
                delay(1);
                send_ctrl(0xAF); // Display on
            }

            /**
             * @brief Set Column Address Area for next DDRAM writes
             *
             * @param xs X start address
             * @param xe X end address
             */
            void ST75256::set_column_address(uint8_t xs, uint8_t xe)
            {
                uint8_t data[] = {xs, xe};
                send_ctrl(0b00010101);
                send_data(data, sizeof(data) / sizeof(uint8_t));
            }

            /**
             * @brief Set Page Address Area for next DDRAM writes
             *
             * @param ys Y start address
             * @param ye Y end address
             */
            void ST75256::set_page_address(uint8_t ys, uint8_t ye)
            {
                uint8_t data[] = {ys, ye};
                send_ctrl(0b01110101);
                send_data(data, sizeof(data) / sizeof(uint8_t));
            }

        } // namespace controller

    } // namespace display
} // yardforce