/**
 * @file main.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce Classic 500 CoverUI main header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-04-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __YFC500_MAIN_H
#define __YFC500_MAIN_H

#define FIRMWARE_VERSION 200 // FIXME: Should go into a common header

/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i) (((i)&0x80ll) ? '1' : '0'), (((i)&0x40ll) ? '1' : '0'), (((i)&0x20ll) ? '1' : '0'), (((i)&0x10ll) ? '1' : '0'), \
                                      (((i)&0x08ll) ? '1' : '0'), (((i)&0x04ll) ? '1' : '0'), (((i)&0x02ll) ? '1' : '0'), (((i)&0x01ll) ? '1' : '0')
#define PRINTF_BINARY_PATTERN_INT16 PRINTF_BINARY_PATTERN_INT8 PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) PRINTF_BYTE_TO_BINARY_INT8((i) >> 8), PRINTF_BYTE_TO_BINARY_INT8(i)
/* --- end PRINTF_BYTE_TO_BINARY macros --- */

#include <Arduino.h> // Stock CoverUI is build now via Arduino framework (instead of HAL), which is ATM the only framework with STM32F030R8 and GD32F330R8 support
#include "settings.h"
#include "LEDcontrol.h"
#include "Buttons.h"

// FIXME: Doesn't work anymore! Due to arduino framework?
#ifdef DEBUG_SEMIHOSTING
extern "C" void initialise_monitor_handles(void);
#endif

// Current STM32F030 implementation is single core without threads. // FIXME: Check num of Cortex M4 (GD32) cores
// Send mutex calls of main.cpp to nirvana. Dangerous? // FIXME: Does Arduino has/need mutexes so that we can honor mutex calls (even if only one core)
#define auto_init_mutex(name)
#define mutex_enter_blocking(ptr)
#define mutex_exit(ptr)

// UART-LL specific settings
#define UART_LL_DMA_BUFFSIZE 50
uint8_t uart_ll_dma_buffer[UART_LL_DMA_BUFFSIZE]; // DMA buffer size has to be at least one COBS cmd length
// extern DMA_HandleTypeDef HDMA_UART_LL_RX;

// Defined in main.cpp
extern void core1();
extern void getDataFromBuffer(const uint8_t *data, uint16_t size);

// Own forward declarations
uint8_t bit_getbutton(uint32_t press_timeout, bool &still_pressed);

// Some dummy Pico-SDK definitions. Not used but by this we'll NOT pollution original code to much with #ifdefs
#define pio0 NULL
#define pio1 NULL
    typedef bool *PIO;
#define buzzer_SM_CYCLE 10800

// YFC500 specific
LEDcontrol LedControl; // Main LED controller object
Buttons Btns;          // Main Buttons object

void setup()
{
#ifdef DEBUG_SEMIHOSTING
    initialise_monitor_handles(); // Semihosting
#endif
    printf("Main Setup\n");
    LedControl.setup();

    // We've hardware timer on mass, let's use them
    // (choose those which are available on STM32 as well as GD32 model, preferable the simpler basic/generic ones)

    // 500ms (2Hz) timer, used for LED-blink-slow
    HardwareTimer *Timer_500ms = new HardwareTimer(TIM16);
    Timer_500ms->setOverflow(2, HERTZ_FORMAT);             
    Timer_500ms->attachInterrupt(std::bind(&LEDcontrol::blink_timer_elapsed, &LedControl, LED_state::LED_blink_slow));
    Timer_500ms->resume();

    // 100ms (10Hz) timer, used for LED-blink-fast
    HardwareTimer *Timer_100ms = new HardwareTimer(TIM15);
    Timer_100ms->setOverflow(10, HERTZ_FORMAT);
    Timer_100ms->attachInterrupt(std::bind(&LEDcontrol::blink_timer_elapsed, &LedControl, LED_state::LED_blink_fast));
    Timer_100ms->resume();

    // 5ms (200Hz) timer, used for button debouncing and LED sequences
    HardwareTimer *Timer_5ms = new HardwareTimer(TIM14);
    Timer_5ms->setOverflow(200, HERTZ_FORMAT);
    Timer_5ms->attachInterrupt(std::bind(&LEDcontrol::process_sequence, &LedControl));
    Timer_5ms->resume();
}

/**
 * @brief Init all HW relevant stuff like GPIO, DMA, U(S)ARTs, Timer and Semihosting
 *
 */
void init_mcu()
{
    /*
    MX_DMA_Init();
    MX_USART2_UART_Init(); // to LL-Pico via J6 */
}

void start_peripherals()
{
    /*
    // Start UART-LL DMA receive
    if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(&HUART_LL, uart_ll_dma_buffer, UART_LL_DMA_BUFFSIZE))
    {
        Error_Handler();
    }
    __HAL_DMA_DISABLE_IT(&HDMA_UART_LL_RX, DMA_IT_HT); // Disable "Half Transfer" interrupt*/
}

void loop() // This loop() doesn't loop! See drop off into core1() at the end of this func
{
#ifdef DEBUG_SEMIHOSTING
    initialise_monitor_handles(); // Semihosting
#endif
    float ver = (float)FIRMWARE_VERSION / 100.0;
    printf("\n\n\n\rMower Button-LED-Control Version %2.2f\n", ver);

    LedControl.set(LED_NUM_REAR, LED_state::LED_blink_slow); // We're alive blink. Get switched to manual- fast-blink in the case of an error // FIXME: Not valid anymore since arduino framework. Useless at all?

    // "Hi there" and jammed button mounting detection
    bool tmp;
    do
    {
        // LED blink to say it's alive
        // (this processing delay is also required to get the debouncer filled with a consistent state (NUM_BUTTON_STATES * 5ms)
        LedControl.sequence_start(&LEDcontrol::sequence_animate_handler);

    } while (bit_getbutton(500, tmp));

    // Dev tests
    LedControl.set(LED_NUM_LIFTED, LED_state::LED_on);
    LedControl.set(LED_NUM_WIRE, LED_state::LED_blink_slow);
    LedControl.set(LED_NUM_BAT, LED_state::LED_blink_fast);

    printf("\n\n waiting for commands or button press");

    // Drop off into endless core1() for button processing (waste (one more?) stack entry!)
    core1(); 
}

/****************************************************************
 * Some dump OM wrapper for not polluting original code to much *
 ****************************************************************/

/**
 * @brief OM wrapper
 *
 * @param led_num
 * @param force
 */
void Force_LED_off(uint8_t led_num, bool force)
{
    LedControl.force_off(led_num, force); // This only effect blink states
}

/**
 * @brief OM wrapper
 *
 * @param pioBlock
 * @param statemachine
 * @param led
 */
void Blink_LED(PIO dummy, int dummy2, int led_num)
{
    LedControl.identify(led_num);
}

/**
 * @brief OM wrapper
 *
 * @param anz
 * @param timeON
 * @param timeOFF
 */
static inline void buzzer_program_put_words(PIO pio, uint sm, uint32_t repeat, uint32_t duration, uint32_t gap)
{
    // YFC500 doesn't has (not yet?) a buzzer on CoverUI
}

/**
 * @brief OM wrapper for original function call, to STM's Buttons class implementation
 *
 * @param press_timeout
 * @param still_pressed
 * @return unsigned int
 */
uint8_t bit_getbutton(uint32_t press_timeout, bool &still_pressed)
{
    still_pressed = false;

    // As it's not clear ATM how the implementation will become,
    // let's scan the buttons in the same order as original OM FW does
    const uint8_t scan_order[] = {0, 1, 2, 3, 4, 5, 6, 13, 7, 8, 9, 10, 11, 12}; // Attention: Ours = OM's -1. See Buttons.h: FYC500_Button_Def for index number
    for (uint8_t i : scan_order)
    {
        uint32_t start = millis(); // start press_timeout measurement
        if (Btns.is_pressed(i))
        {
            // wait for button released
            while (Btns.is_pressed(i) && (press_timeout == 0 || (millis() - start) < press_timeout))
                ;
            if (Btns.is_pressed(i))
                still_pressed = true;
            else
                still_pressed = false;
            return (i + 1); // OM's button numbering is n > 0
        }
    }
    return 0;
}

/********************************
 *  TODO
 ********************************/

/**
 * @brief Check if one of the "magic buttons" got pressed and do his function.
 * At the moment the following magic buttons exists:
 * OK + Clock = Display FW version
 * OK + Sun   = LED animation
 */
void magic_buttons()
{
    if (!Btns.is_pressed(6)) // OK
        return;

    if (Btns.is_pressed(13)) // SUN
        LedControl.sequence_start(&LEDcontrol::sequence_animate_handler);
    else if (Btns.is_pressed(0)) // Clock
        LedControl.show_num(FIRMWARE_VERSION);
    return;
}

/**
 * @brief Timer callback(s)
 *  TIM_BLINK_SLOW (LED blink- slow) is configure at 500ms
 *  TIM_BLINK_FAST (LED blink- fast) is configured at 100ms
 *  TIM_5MS 5ms timer i.e. used for button debouncing and LED-Sequences
 *
 * @param htim
 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM_BLINK_SLOW)
    {
        LedControl.blink_timer_elapsed(LED_state::LED_blink_slow);*/
/*#ifdef _serial_debug_
        printf("Button status " PRINTF_BINARY_PATTERN_INT16 " " PRINTF_BINARY_PATTERN_INT16 " " PRINTF_BINARY_PATTERN_INT16 " " PRINTF_BINARY_PATTERN_INT16 "\n",
               PRINTF_BYTE_TO_BINARY_INT16(Btns.get_status(0)), PRINTF_BYTE_TO_BINARY_INT16(Btns.get_status(1)), PRINTF_BYTE_TO_BINARY_INT16(Btns.get_status(2)), PRINTF_BYTE_TO_BINARY_INT16(Btns.get_status(3)));
#endif*/
/*magic_buttons();
}
else if (htim->Instance == TIM_BLINK_FAST)
LedControl.blink_timer_elapsed(LED_state::LED_blink_fast);
else if (htim->Instance == TIM_5MS)
{
Btns.process_states();         // Read all defined gpio-port debouncer
LedControl.process_sequence(); // Process LED sequence
}
}*/

/**
 * @brief RX idle callback which has to process the data within the circular DMA buffer.
 *        Take attention when enabling the printf() debugging. Semihosting will burn to much CPU for reliable (>1-2 circled) buffer processing!!!
 *
 * @param huart UART handle
 * @param pos Position of last rx data within buffer
 */
/*void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t pos)
{
    static uint16_t old_ll_pos;
    uint16_t size;

    if (huart->Instance == UART_LL)
    {
        if (pos == old_ll_pos) // no new data
            return;

        if (pos > old_ll_pos) // buffer simply increased without going over end of buffer
        {
            size = pos - old_ll_pos;
            // printf("\nDMA(incr) old_ll_pos %u, pos %u = size %u", old_ll_pos, pos, size);
            getDataFromBuffer(&uart_ll_dma_buffer[old_ll_pos], size);
        }
        else // pos < old_ll_pos => buffer pos circled
        {
            // Remaining end of circular buffer
            size = UART_LL_DMA_BUFFSIZE - old_ll_pos;
            // printf("\nDMA(rest) old_ll_pos %u, pos %u = size %u", old_ll_pos, pos, size);
            if (size > 0) // Not really necessary, could also call getDataFromBuffer() with a size of 0
            {
                getDataFromBuffer(&uart_ll_dma_buffer[old_ll_pos], size);
            }
            // Start of circular buffer
            // printf("\nDMA(incr) old_ll_pos %u, pos %u = size %u", old_ll_pos, pos, pos);
            if (pos > 0) // Not really necessary, could also call getDataFromBuffer() with a size of 0
            {
                getDataFromBuffer(&uart_ll_dma_buffer[0], pos);
            }
        }
        old_ll_pos = pos;
    }
}*/

#endif /* __YFC500_MAIN_H */
