#include "mbed.h"

#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

using namespace mbed;

class ShiftRegister {

public:

/*
* Constructor
*/
    ShiftRegister(DigitalOut &clk, DigitalOut &dat,
                  DigitalOut &latch, DigitalOut &clr,
                  DigitalOut &oe);

/*
* Destructor
*/
    ~ShiftRegister();

/*
* Write Value
*/
    template<typename T>
    int write(T val, int bits);

    void clear();

private:
    DigitalOut &_clk;
    DigitalOut &_dat;
    DigitalOut &_latch;
    DigitalOut &_clr;
    DigitalOut &_oe;

}; // end class ShiftRegister


template<typename T>
int ShiftRegister::write(T val, int bits) {
    T tmpval = ~0;
    int tmpsz = sizeof(val) * 8;
    val = tmpval & val;
    bits = tmpsz < bits ? tmpsz : bits;
    for (char i = 0; i < bits; i++) {
        _dat.write((val >> i) & 0x01);
        _clk = 0;
        _clk = 1;
    }
    _latch = 0;
    _latch = 1;
    return val;
}

#endif //SHIFTREGISTER_H
