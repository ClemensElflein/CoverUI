/**
 * @file Rain.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI Rain-Sensor class for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-05-09
 *
 * @copyright Copyright (c) 2023
 *
 */
/* 
 * Some measurements:
 *
 *                     | Simulate  |  Rain   | Still   |  Dry  |
 * MCU/Board | 23°C EU | APEC 35°C |  Water  |  Wet    | again |
 * ----------|---------|-----------|---------|---------|-------|
 * STM32F030 | 812-820 |  ~ 730    |  ~ 300  |         |       |
 * GD32F330  | 930-950 | 928-932   | 350-387 | 445-466 | >800  |
 */
#ifndef YFC500_RAIN_H
#define YFC500_RAIN_H

#include <Arduino.h>
#include "../BttnCtl.h"

#define PIN_RAIN PB1

extern void sendMessage(void *message, size_t size);

class Rain
{
private:
    uint32_t m_val;

public:
    void setup()
    {
    }

    void read()
    {
        pinMode(PIN_RAIN, INPUT_PULLUP); // Charge C
        delay(1);                        // Need a consistent delay for our different MCU clocks
        m_val = analogRead(PIN_RAIN);
    }

    void send()
    {
        msg_event_rain msg = {
            .type = Get_Rain,
            .value = m_val,
            .threshold = 300};
        sendMessage(&msg, sizeof(msg));
    }
};

#endif /* YFC500_RAIN_H */
