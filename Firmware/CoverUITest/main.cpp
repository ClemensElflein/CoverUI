/**
 *  Version 0.93 v. 10.11.2023  AH: Add subscribe packet, change --lcd option functionality
 *  Version 0.92 v. 15.09.2023  AH: Switch to getopt and add --lcd option
 *  Version 0.91 v. 27.05.2022  CE: schnelles Blinken von ontime 500 auf 250 ms verkleinert
 */

// #define _serial_debug_
// #define _testCOBS_

#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <exception>

#include <chrono>
#include <thread>

// Linux headers
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include <getopt.h>  // GNU getopt to parse command-line options

#include "COBS.h"
#include "BttnCtl.h"
#include "datatypes.h"
#include "CRC.h"
#include <cmath>
using namespace std;

COBS cobs;
#define buffersize 256

/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i) (((i) & 0x80ll) ? '1' : '0'), (((i) & 0x40ll) ? '1' : '0'), (((i) & 0x20ll) ? '1' : '0'), (((i) & 0x10ll) ? '1' : '0'), \
                                      (((i) & 0x08ll) ? '1' : '0'), (((i) & 0x04ll) ? '1' : '0'), (((i) & 0x02ll) ? '1' : '0'), (((i) & 0x01ll) ? '1' : '0')
/* --- end PRINTF_BYTE_TO_BINARY macros --- */

#define BATT_ABS_MAX 28.7f
#define BATT_ABS_Min 21.7f

#define BATT_FULL BATT_ABS_MAX - 0.3f
#define BATT_EMPTY BATT_ABS_Min + 0.3f

static string VERSION = "V 0.92";
char DEFAULT_SERIAL[] = "/dev/ttyUSB0";

static uint number_inbuffer = 0;
uint8_t read_buf[buffersize];
uint8_t serial_buf[buffersize];
uint32_t read_ptr = 0;
uint32_t write_ptr = 0;
uint8_t COBS_buf[buffersize];

uint bar_test_delay = 20;
struct ll_status status_message = {0};                  // LL state
struct ll_high_level_state last_high_level_state = {0}; // HL state
uint8_t ui_topic_bitmask = Topic_set_leds;              // UI subscription, default to Set_LEDs
uint16_t ui_interval = 1000;                            // UI send msg (LED/State) interval (ms)

/****************************************************************************************************
 *
 * PacketReceived() there is a serial stream in the buffer
 * decode it with cobs, calc crc and decode into the messagestruct
 *****************************************************************************************************/

void PacketReceived()
{
#ifdef _serial_debug_
    printf("\nPacketReceived()");
    printf("\nbefore decode count = %d values are ", number_inbuffer);
    for (int i = 0; i < number_inbuffer; i++)
    {
        printf(", 0x%02x", COBS_buf[i]);
    }
#endif

    // deocode COBS
    uint8_t decoded_buffer[number_inbuffer];
    size_t data_size = cobs.decode(COBS_buf, number_inbuffer - 1, (uint8_t *)decoded_buffer);

#ifdef _serial_debug_
    printf("\nafter decode count = %d values are ", (int)data_size);
    for (int i = 0; i < data_size; i++)
    {
        printf(", 0x%02x", decoded_buffer[i]);
    }
#endif

    // calculate the CRC
    if (data_size < 3)
        return;
    uint16_t calc_crc = CRC::Calculate(decoded_buffer, data_size - 2, CRC::CRC_16_CCITTFALSE());

    if (decoded_buffer[0] == Get_Version && data_size == sizeof(struct msg_get_version))
    {
        struct msg_get_version *message = (struct msg_get_version *)decoded_buffer;
        if (message->crc == calc_crc)
        {
            printf("button board has version: %d\n", message->version);
        }
        else
        {
            printf("GetVersion invalid CRC");
        }
    }
    else if (decoded_buffer[0] == Get_Button && data_size == sizeof(struct msg_event_button))
    {
        struct msg_event_button *message = (struct msg_event_button *)decoded_buffer;
        if (message->crc == calc_crc)
        {
            printf("button %d was pressed with duration %d\n", message->button_id, message->press_duration);
        }
        else
        {
            printf("Get_Button invalid CRC");
        }
    }
    else if (decoded_buffer[0] == Get_Rain && data_size == sizeof(struct msg_event_rain))
    {
        struct msg_event_rain *message = (struct msg_event_rain *)decoded_buffer;
        if (message->crc == calc_crc)
        {
            printf("rain value %d (threshold %d)\n", message->value, message->threshold);
        }
        else
        {
            printf("Get_Rain invalid CRC");
        }
    }
    else if (decoded_buffer[0] == Get_Emergency && data_size == sizeof(struct msg_event_emergency))
    {
        struct msg_event_emergency *message = (struct msg_event_emergency *)decoded_buffer;
        if (message->crc == calc_crc)
        {
            printf("Emergency state " PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(message->state));
        }
        else
        {
            printf("Get_Emergency invalid CRC");
        }
    }
    else if (decoded_buffer[0] == Get_Subscribe && data_size == sizeof(struct msg_event_subscribe))
    {
        struct msg_event_subscribe *message = (struct msg_event_subscribe *)decoded_buffer;
        if (message->crc == calc_crc)
        {
            printf("Subscribe to topic_bitmask " PRINTF_BINARY_PATTERN_INT8 ", interval %dms\n", PRINTF_BYTE_TO_BINARY_INT8(message->topic_bitmask), message->interval);
            ui_topic_bitmask = message->topic_bitmask;
            ui_interval = message->interval;
        }
        else
        {
            printf("Get_Subscribe invalid CRC");
        }
    }
}

/****************************************************************************************************
 *
 * reply wait for timeout ms if return with result
 *
 *****************************************************************************************************/

#define timeout 100

void reply(int com_port)
{
    number_inbuffer = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    int num_bytes = 0;
    try
    {
        num_bytes = read(com_port, &read_buf, sizeof(read_buf));
    }
    catch (...)
    {
        std::cout << "Serial error check cable " << '\n';
        return;
    }

    //  printf("*** Error *** no connection to target, check cable");

    if (num_bytes != 0)
    {
        for (int i = 0; i < num_bytes; i++)
        {
            serial_buf[write_ptr] = read_buf[i];
            write_ptr++;
            if (write_ptr >= buffersize)
                write_ptr = 0;
        }

#ifdef _serial_debug_

#endif

        while (write_ptr != read_ptr)
        {
            COBS_buf[number_inbuffer] = serial_buf[read_ptr];
            number_inbuffer++;
            read_ptr++;
            if (read_ptr >= buffersize)
                read_ptr = 0;

#ifdef _serial_debug_
            printf("\ncount %d , value 0x%2x ", number_inbuffer, COBS_buf[number_inbuffer - 1]);
#endif

            if (COBS_buf[number_inbuffer - 1] == 0)
            {
                PacketReceived();
                number_inbuffer = 0;
            }
        }
    }
}

/****************************************************************************************************
 *
 * sendMessage send the command strucutre via COBS and serial Port and wait for reply
 *
 *****************************************************************************************************/

void sendMessage(void *message, size_t size, int serial_port)
{
    // packages need to be at least 1 byte of type, 1 byte of data and 2 bytes of CRC

    if (size < 4)
    {
        return;
    }
    uint8_t *data_pointer = (uint8_t *)message;
    uint16_t *crc_pointer = (uint16_t *)(data_pointer + (size - 2));

    // calculate the CRC
    *crc_pointer = CRC::Calculate(message, size - 2, CRC::CRC_16_CCITTFALSE());
    // structure is filled and CRC calculated, so print out, what should be encoded
    //

#ifdef _serial_debug_
    printf("\nprint struct before encoding %d byte : ", (int)size);
    uint8_t *temp = data_pointer;
    for (int i = 0; i < size; i++)
    {
        printf("0x%02x , ", *temp);
        temp++;
    }
#endif

    // encode message
    uint8_t out_buf[100];
    size_t encoded_size = cobs.encode((uint8_t *)message, size, out_buf);
    out_buf[encoded_size] = 0;
    encoded_size++;

#ifdef _serial_debug_
    printf("\nencoded message               byte : ");
    for (uint i = 0; i < encoded_size; i++)
    {
        printf("0x%02x , ", out_buf[i]);
    }
#endif
    write(serial_port, out_buf, encoded_size);
    // if (!reply(serial_port)) printf("\n*** got no answer ***");
}

/****************************************************************************************************
 *
 * Init Serial Port 115200 baud, 8,n,n
 *
 *****************************************************************************************************/

int Init_serial_board(char *channel)
{
    // Init serial port
    int ret_val = -1;
    struct termios tty;

    int serial_port = open(channel, O_RDWR);
    if (serial_port < 0)
    {
#ifdef _serial_debug_
        printf("Error %i from open: \n", errno);
#endif
        return (ret_val);
    }

    if (tcgetattr(serial_port, &tty) != 0)
    {
#ifdef _serial_debug_
        printf("Error %i from tcgetattr: \n", errno);
#endif
        return (ret_val);
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;         // Clear all bits that set the data size
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
#ifdef _serial_debug_
        printf("Error %i from tcsetattr: \n", errno);
#endif
        return (ret_val);
    }
    return (ret_val = serial_port);
}

/****************************************************************************************************
 *
 * fill command structure LED command and send via serial port
 *
 *****************************************************************************************************/

void set_LED_state(int com_port, uint cmd1, uint cmd2, uid_t cmd3)
{
}

/****************************************************************************************************
 *
 * fill command structure getversion and send via serial port
 *
 *****************************************************************************************************/

/****************************************************************************************************
 *
 * fill command structure buzzercommand and send via serial port
 *
 *****************************************************************************************************/

void set_buzzer(int com_port, uint ontime, uint offtime, uint count)
{
}

void getversion(int com_port)
{
    printf("Get Version\n");
    // Fill structure
    struct msg_get_version msg;
    msg.type = Get_Version;

    sendMessage(&msg, sizeof(msg), com_port);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    reply(com_port);
}

void setLed(struct msg_set_leds &msg, int led, uint8_t state)
{
    // mask the current led state
    uint64_t mask = ~(((uint64_t)0b111) << (led * 3));
    msg.leds &= mask;
    msg.leds |= ((uint64_t)(state & 0b111)) << (led * 3);
}

void setBars7(struct msg_set_leds &msg, double value)
{
    int on_leds = round(value * 7.0);
    for (int i = 0; i < 7; i++)
    {
        setLed(msg, LED11 - i, i < on_leds ? LED_on : LED_off);
    }
}

void setBars4(struct msg_set_leds &msg, double value)
{
    int on_leds = round(value * 4.0);
    for (int i = 0; i < 4; i++)
    {
        setLed(msg, LED18 - i, i < on_leds ? LED_on : LED_off);
    }
}

void LEDstatic(int com_port)
{

    struct msg_set_leds msg;
    msg.type = Set_LEDs;
    msg.leds = 0;

    for (int led = 0; led < 18; led++)
    {
        setLed(msg, LED18 - led, LED_on);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        sendMessage(&msg, sizeof(msg), com_port);
        setLed(msg, LED18 - led, LED_off);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        sendMessage(&msg, sizeof(msg), com_port);
    }
}

void LEDfastblink(int com_port)
{
    struct msg_set_leds msg;
    msg.type = Set_LEDs;
    msg.leds = 0;

    for (int led = 0; led < 18; led++)
    {
        setLed(msg, LED18 - led, LED_blink_fast);
        sendMessage(&msg, sizeof(msg), com_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    for (int led = 0; led < 18; led++)
    {
        setLed(msg, LED18 - led, LED_off);
        sendMessage(&msg, sizeof(msg), com_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void LEDslowblink(int com_port)
{
    struct msg_set_leds msg;
    msg.type = Set_LEDs;
    msg.leds = 0;

    for (int led = 0; led < 18; led++)
    {
        setLed(msg, LED18 - led, LED_blink_slow);
        sendMessage(&msg, sizeof(msg), com_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    for (int led = 0; led < 18; led++)
    {
        setLed(msg, LED18 - led, LED_off);
        sendMessage(&msg, sizeof(msg), com_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Buzzertest(int com_port)
{
    printf("Test Buzzer\n");
    set_buzzer(com_port, 100, 40, 3);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    reply(com_port);
    set_buzzer(com_port, 100, 40, 5);
    reply(com_port);
}

void LEDBar7test(int com_port)
{
    struct msg_set_leds msg;
    msg.type = Set_LEDs;
    msg.leds = 0;

    for (int i = 0; i < 10; i++)
    {
        for (double d = 0; d < 1.0; d += 0.05)
        {
            setBars7(msg, d);
            sendMessage(&msg, sizeof(msg), com_port);
            std::this_thread::sleep_for(std::chrono::milliseconds(bar_test_delay));
        }
        for (double d = 0; d < 1.0; d += 0.05)
        {
            setBars7(msg, 1.0 - d);
            sendMessage(&msg, sizeof(msg), com_port);
            std::this_thread::sleep_for(std::chrono::milliseconds(bar_test_delay));
        }
    }
}

void LEDBar4test(int com_port)
{
    struct msg_set_leds msg;
    msg.type = Set_LEDs;
    msg.leds = 0;

    for (int i = 0; i < 10; i++)
    {
        for (double d = 0; d < 1.0; d += 0.05)
        {
            setBars4(msg, d);
            sendMessage(&msg, sizeof(msg), com_port);
            std::this_thread::sleep_for(std::chrono::milliseconds(bar_test_delay));
        }

        for (double d = 0; d < 1.0; d += 0.05)
        {
            setBars4(msg, 1.0 - d);
            sendMessage(&msg, sizeof(msg), com_port);
            std::this_thread::sleep_for(std::chrono::milliseconds(bar_test_delay));
        }
    }
}

/**
 * @brief Thread which handles LowLevel test cases
 *
 */
void *ll_test(void *arg_ptr)
{
    int *com_port = (int *)arg_ptr;

    // Emergency
    const uint8_t kEmergencyBitmaskTests[] = {
        0,
        (LL_EMERGENCY_BITS_STOP | LL_EMERGENCY_BIT_LATCH),
        (LL_EMERGENCY_BITS_STOP | LL_EMERGENCY_BITS_LIFT | LL_EMERGENCY_BIT_LATCH),
        (LL_EMERGENCY_BITS_LIFT | LL_EMERGENCY_BIT_LATCH),
        LL_EMERGENCY_BIT_LATCH};
    const size_t kNumEmergencyBitmaskTests = sizeof(kEmergencyBitmaskTests) / sizeof(uint8_t);
    float emergency_bitmask_test_idx = 0;

    float v_charge_add;
    float a_charge_add;
    float v_battery_add;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ui_interval));
        if (!(ui_topic_bitmask & Topic_set_ll_status)) // Subscribed?
            break;

        // Status Bitmask (emergency) tests
        if (emergency_bitmask_test_idx >= kNumEmergencyBitmaskTests)
            emergency_bitmask_test_idx = 0;
        status_message.emergency_bitmask = kEmergencyBitmaskTests[(unsigned int)emergency_bitmask_test_idx];
        emergency_bitmask_test_idx += 0.25f;

        // Charge V
        if (status_message.v_charge <= 0.0f)
            v_charge_add = +0.95f;
        else if (status_message.v_charge >= 30.0f)
            v_charge_add = -0.923f;
        status_message.v_charge += v_charge_add;

        // Charge A
        if (status_message.charging_current <= 0.0f)
            a_charge_add = +0.135f;
        else if (status_message.charging_current >= 1.2f)
            a_charge_add = -0.123f;
        status_message.charging_current += a_charge_add;

        // Battery V
        if (status_message.v_battery <= BATT_ABS_Min)
            v_battery_add = +0.439f;
        else if (status_message.v_battery >= BATT_ABS_MAX)
            v_battery_add = -0.446f;
        status_message.v_battery += v_battery_add;

        // Prepare and send message
        struct ll_status msg = {
            .type = PACKET_ID_LL_STATUS,
            .emergency_bitmask = status_message.emergency_bitmask,
            .v_charge = status_message.v_charge,
            .v_battery = status_message.v_battery,
            .charging_current = status_message.charging_current};
        sendMessage(&msg, sizeof(msg), *com_port);

        printf("Sent LL-Status, emergency_bitmask " PRINTF_BINARY_PATTERN_INT8 ", Charge %.1fV %.2fA, Battery %.1fV\n", PRINTF_BYTE_TO_BINARY_INT8(msg.emergency_bitmask), msg.v_charge, msg.charging_current, msg.v_battery);
    }
    return 0;
}

void *hl_test(void *arg_ptr)
{
    int *com_port = (int *)arg_ptr;

    // Emergency
    const uint8_t kModeTests[] = {
        0, // ROS not running
        MODE_IDLE,
        MODE_AUTONOMOUS,
        MODE_AUTONOMOUS | (0b01 << 6), // Docking
        MODE_AUTONOMOUS | (0b10 << 6), // Undocking
        MODE_RECORDING,
        MODE_RECORDING | (0b01 << 6),  // Record Outline
        MODE_RECORDING | (0b10 << 6)}; // Record Obstacle
    const size_t kNumModeTests = sizeof(kModeTests) / sizeof(uint8_t);
    float mode_test_idx = 0;

    int gps_add;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ui_interval));
        if (!(ui_topic_bitmask & Topic_set_hl_state)) // Subscribed?
            break;

        // Mode tests
        if (mode_test_idx >= kNumModeTests)
            mode_test_idx = 0;
        last_high_level_state.current_mode = kModeTests[(unsigned int)mode_test_idx];
        mode_test_idx += 0.5f;

        // GPS quality (%)
        if (last_high_level_state.gps_quality <= 0)
            gps_add = +6;
        else if (last_high_level_state.gps_quality >= 100)
            gps_add = -5;
        if (abs(gps_add) != gps_add && abs(gps_add) > last_high_level_state.gps_quality)
            last_high_level_state.gps_quality = 0;
        else
            last_high_level_state.gps_quality += gps_add;

        // Prepare and send message
        struct ll_high_level_state msg = {
            .type = PACKET_ID_LL_HIGH_LEVEL_STATE,
            .current_mode = last_high_level_state.current_mode,
            .gps_quality = last_high_level_state.gps_quality};
        sendMessage(&msg, sizeof(msg), *com_port);

        printf("Sent HL-Status, Mode " PRINTF_BINARY_PATTERN_INT8 ", GPS %d%%\n", PRINTF_BYTE_TO_BINARY_INT8(msg.current_mode), msg.gps_quality);
    }
    return 0;
}

/**
 * @brief Brief usage of this command with infos about the possible options
 *
 * @param fp
 * @param path
 * @return * print
 */
void usage(FILE *fp, const char *path)
{
    // Get command name (last portion of the path)
    const char *basename = strrchr(path, '/');
    basename = basename ? basename + 1 : path;

    fprintf(fp, "usage: %s [OPTION] [serial channel {%s}]\n", basename, DEFAULT_SERIAL);
    fprintf(fp, "  -h, --help\t"
                "Print this help and exit.\n");
    fprintf(fp, "  -l, --lcd\t"
                "Test LCD display (limited set of tests and slower rates to get recognized on slow liquid).\n");
    fprintf(fp, "  -q, --quiet\t"
                "Be quiet and don't send any test data.\n");
}

/****************************************************************************************************
 *
 * main
 *
 *****************************************************************************************************/

int main(int argc, char *argv[])
{
    printf("Testprogramm OpenMower UI-Board Version %s\n", VERSION.c_str());

    char serialchannel[80];
    int help_flag = 0;
    int lcd_flag = 0;
    int quiet_flag = 0;
    int opt;
    pthread_t thread_ll_test, thread_hl_test;
    status_message.v_battery = BATT_ABS_Min;

    // getopt long options
    struct option longopts[] = {
        {"help", no_argument, &help_flag, 1},
        {"lcd", no_argument, &lcd_flag, 'l'},
        {"quiet", no_argument, &lcd_flag, 'q'},
        {0}};

    // infinite loop, to be broken when we are done parsing options
    while (1)
    {
        // One colon after an option indicates that it has an argument, two indicates that the argument is optional. order is unimportant.
        opt = getopt_long(argc, argv, "hlq", longopts, 0);

        if (opt == -1) // indicates that there are no more options
            break;

        switch (opt)
        {
        case 'h':
            help_flag = 1;
            break;
        case 'l':
            lcd_flag = 1;
            break;
        case 'q':
            quiet_flag = 1;
            break;
        case '?': // malformed option
            usage(stderr, argv[0]);
            return 1;
        default:
            break;
        }
    }

    if (help_flag)
    {
        usage(stdout, argv[0]);
        return 0;
    }

    if (lcd_flag)
        bar_test_delay = 100;

    // Non-option args
    if (optind < argc)
    {
        if (argc - optind > 2)
        {
            printf("To many agruments, try cobstest <serialport>\n");
            return 1;
        }
        strcpy(serialchannel, argv[optind++]);
        printf("using serial channel: %s \n", serialchannel);
    }
    else
    {
        printf("No command line parameter for serial channel, using default value : %s\n", DEFAULT_SERIAL);
        strcpy(serialchannel, DEFAULT_SERIAL);
    }

    int com_port = Init_serial_board(serialchannel);
    if (com_port < 0)
    {
        printf("\n\n\r*** ERROR *** can't open serial port. Serial cable connected ?  Program aborted");
        return 1;
    }

    // set_LED_state(com_port , 0 ,LED_on , 0);

    // reply(com_port);

    getversion(com_port);

    if (!lcd_flag && !quiet_flag)
    {
        LEDstatic(com_port);
        LEDfastblink(com_port);
        LEDslowblink(com_port);
        Buzzertest(com_port);
        LEDBar4test(com_port);
        LEDBar7test(com_port);
    }

    if (!quiet_flag)
    {
        pthread_create(&thread_ll_test, NULL, &ll_test, &com_port);
        pthread_create(&thread_hl_test, NULL, &hl_test, &com_port);
    }

    printf("Test getters like Buttons or (Stock-CoverUI) Rain, Emergency, Subscribe\n");
    while (true)
    {
        reply(com_port);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}