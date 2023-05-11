#include "mbed.h"

#ifndef COVERUI_TEST_DUMMYOUT_H
#define COVERUI_TEST_DUMMYOUT_H

class DummyOut: public mbed::DigitalOut {
public:
    explicit DummyOut(PinName pin) : DigitalOut(pin) { }
    void write(__attribute__((unused)) int value)
    {
        //noop
    }
};

#endif //COVERUI_TEST_DUMMYOUT_H
