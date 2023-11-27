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
    unsigned int start_ = 0;
    unsigned int current_ = 0;
    unsigned int min_ = 0xffffffff;
    unsigned int max_ = 0;
    unsigned int avg_ = 0;
    unsigned int sum_ = 0;
    unsigned int cnt_ = 0;
    static bool s_dwt_initialized_;
}; // CortexMCycleCounter

#endif // __CORTEXMCYCLECOUNTER_HPP
