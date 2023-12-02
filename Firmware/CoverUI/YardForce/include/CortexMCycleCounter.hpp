/**
 * @file ExecutionTimer.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief Simple execution timer class, for simple execution stopwatch measurement.
 * Mainly copied from here https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
 * @version 0.1
 * @date 2023-11-21
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __CORTEXMCYCLECOUNTER_HPP
#define __CORTEXMCYCLECOUNTER_HPP

#include <Arduino.h>

class CortexMCycleCounter
{
public:
    CortexMCycleCounter();
    ~CortexMCycleCounter(){};

    void start(); // Start measurement
    void stop();  // Stop  measurement

private:
    unsigned long start_ = 0;
    unsigned long current_ = 0;
    unsigned long min_ = 0xffffffff;
    unsigned long max_ = 0;
    unsigned long avg_ = 0;
    unsigned long sum_ = 0;
    unsigned long cnt_ = 0;
    static bool s_dwt_initialized_;
}; // CortexMCycleCounter

#endif // __CORTEXMCYCLECOUNTER_HPP
