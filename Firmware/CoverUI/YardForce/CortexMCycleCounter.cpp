#include <Arduino.h>
#include <stdint.h>
#include "include/CortexMCycleCounter.hpp"

// Cortex-M DWT timer
#define ARM_CM_DEMCR (*(uint32_t *)0xE000EDFC)
#define ARM_CM_DWT_CTRL (*(uint32_t *)0xE0001000)
#define ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

CortexMCycleCounter::CortexMCycleCounter()
{
    if (!s_dwt_initialized_)
    {
        if (ARM_CM_DWT_CTRL != 0) // DWT avail?
        {
            ARM_CM_DEMCR |= 1 << 24;
            ARM_CM_DWT_CYCCNT = 0;
            ARM_CM_DWT_CTRL |= 1 << 0; // Set bit 0
        }
        s_dwt_initialized_ = true;
    }
}

void CortexMCycleCounter::start()
{
#ifdef STM32F030x8 // No DWT MCUs like STM32f0x
    start_ = micros();
#else // DWT capable MCU
    start_ = ARM_CM_DWT_CYCCNT;
#endif
}

void CortexMCycleCounter::stop()
{
#ifdef STM32F030x8 // No DWT MCUs like STM32f0x
    const uint32_t stop = micros();
#else // DWT capable MCU
    const uint32_t stop = ARM_CM_DWT_CYCCNT;
#endif

    current_ = stop - start_;

    // Min/Max
    if (current_ > max_)
        max_ = current_;
    if (current_ < min_)
        min_ = current_;

    // Avg
    cnt_++;
    sum_ += current_;
    avg_ = sum_ / cnt_;
}

bool CortexMCycleCounter::s_dwt_initialized_ = false; // Init private static