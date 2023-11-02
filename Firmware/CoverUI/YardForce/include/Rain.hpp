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
 * Some measurements (AH20230509):
 *
 *                     | Simulate  |  Rain   | Still   |  Dry  |
 * MCU/Board | 23°C EU | APEC 35°C |  Water  |  Wet    | again |
 * ----------|---------|-----------|---------|---------|-------|
 * STM32F030 | 812-883 |  ~ 730    |  ~ 300  |         |       |
 * GD32F330  | 930-950 | 928-932   | 350-387 | 445-466 | >800  |
 */
#ifndef YFC500_RAIN_H
#define YFC500_RAIN_H

#include <Arduino.h>
#include "../../BttnCtl.h"

#define PIN_RAIN PB1             // Need to be soldered with a cable bridge from FB2/JP2 to R79/PB1
#define RAIN_ADC_THRESHOLD 700   // Why a threshold? Cause it could be made configurable on (Stock-)CoverUI (if i.e. required due to inaccuracy)
#define RAIN_PROCESS_PERIOD 5000 // c.ez proposed "once a second or every 10 seconds"

extern void sendMessage(void *message, size_t size);

class Rain
{
private:
    uint32_t next_period_ = 0;
    uint32_t val_;

public:
    /**
     * @brief Read ADC of rain-sensor (with previous C8 charge impulse)
     *
     */
    void read()
    {
        pinMode(PIN_RAIN, INPUT_PULLUP); // Charge C8 (FB1+FB2)
        delay(1);                        // Need a consistent delay for our different MCU clocks
        val_ = analogRead(PIN_RAIN);
    }

    /**
     * @brief Send 'rain' message via COBS with last read rain-sensor- value (together with (currently static) threshold)
     *
     */
    void send()
    {
        msg_event_rain msg = {
            .type = Get_Rain,
            .value = val_,
            .threshold = RAIN_ADC_THRESHOLD};
        sendMessage(&msg, sizeof(msg));
    }

    /**
     * @brief Process (read & send) rain-sensor- value together with (currently static) threshold
     *
     */
    void process()
    {
        if (millis() < next_period_)
            return;

        next_period_ += RAIN_PROCESS_PERIOD;
        read();
        send();
    }
};

#endif /* YFC500_RAIN_H */
