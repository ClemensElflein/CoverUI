#include "mbed.h"
#include "ShiftRegister.h"

using namespace mbed;

ShiftRegister::ShiftRegister(DigitalOut &clk, DigitalOut &dat, DigitalOut &latch, DigitalOut &clr, DigitalOut &oe)
        : _clk(clk), _dat(dat), _latch(latch), _clr(clr), _oe(oe) {
    _clk = 1;
    _dat = 1;
    _clr = 0;
    _oe = 1;
    _clr = 1;
    _oe = 0;
}

ShiftRegister::~ShiftRegister() {
    // Nothing to do!
    clear();
}


void ShiftRegister::clear() {
    _clk = 1;
    _dat = 1;
    _clr = 0;
    _oe = 1;
    _clr = 1;
    _oe = 0;
}
