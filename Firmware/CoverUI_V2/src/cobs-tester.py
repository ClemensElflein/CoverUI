import logging
import time

import serial
import pytest
from cobs import cobs


def crc16_ccitt_false(data: bytearray, offset=None, length=None):
    if not offset:
        offset = 0
    if not length:
        length = len(data)

    if data is None or offset < 0 or offset > len(data) - 1 and offset + length > len(data):
        return 0
    crc = 0xFFFF
    for i in range(0, length):
        crc ^= data[offset + i] << 8
        for j in range(0, 8):
            if (crc & 0x8000) > 0:
                crc = (crc << 1) ^ 0x1021
            else:
                crc = crc << 1
    return crc & 0xFFFF


def crc_check(received_data: bytearray):
    return received_data[-2:] == crc_checksum(received_data[:-2])


def crc_checksum(data: bytearray):
    return bytearray(crc16_ccitt_false(data).to_bytes(2, 'little'))


class TestCoverUI:
    ZERO_BYTE = b'\x00'  # COBS 1-byte delimiter is hex zero as a (binary) bytes character

    PACKET_VERSION = 0xB0
    PACKET_BUZZ = 0xB1
    PACKET_LEDS = 0xB2
    PACKET_BTN = 0xB3

    @pytest.fixture()
    def serial_comm(self):
        serial_port = 'COM47'
        serial_comm = serial.Serial(serial_port, baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=3)
        logging.info('opened serial port: {0}'.format(serial_port))
        return serial_comm

    def crc_cobs_send(self, serial_comm, data):
        data_crc = data + bytearray(crc16_ccitt_false(data).to_bytes(2, 'little'))
        data_encoded = cobs.encode(data_crc) + self.ZERO_BYTE
        serial_comm.write(data_encoded)  # write data with COBS-terminating packet

    def read_cobs_crc(self, serial_comm):
        time.sleep(0.1)
        data = serial_comm.read_until(self.ZERO_BYTE)
        if len(data) > 0:
            decode_str = data[0:(len(data) - 1)]  # take everything except the trailing zero byte
            data_decoded = cobs.decode(decode_str)
            assert crc_check(data_decoded), "CRC check failed"
            return data_decoded

        raise RuntimeError("Serial didn't return nothing")

    def test_get_version(self, serial_comm):
        self.crc_cobs_send(serial_comm, bytearray([self.PACKET_VERSION, 0xCC, 0xDD, 0xDD]))
        version_reply = self.read_cobs_crc(serial_comm)

        assert version_reply[0] == self.PACKET_VERSION
        assert version_reply[2] == 200

    def test_one_long_buzz(self, serial_comm):
        self.crc_cobs_send(serial_comm, bytearray([self.PACKET_BUZZ, 1, 255, 255]))

    def test_five_short_buzz(self, serial_comm):
        self.crc_cobs_send(serial_comm, bytearray([self.PACKET_BUZZ, 5, 100, 100]))

    def test_all_leds_on(self, serial_comm):
        self.crc_cobs_send(serial_comm, bytearray([self.PACKET_LEDS, 0xCC]) +
                           0xFFFFFFFFFFFFFF.to_bytes(8, "little"))

    def test_last_led_fast_second_to_last_slow(self, serial_comm):
        self.crc_cobs_send(serial_comm, bytearray([self.PACKET_LEDS, 0xCC])
                           + 0b101110.to_bytes(8, "little"))

    def test_wait_for_button01_press(self, serial_comm):
        got_button_press = False
        for i in range(1000):
            try:
                button_reply = self.read_cobs_crc(serial_comm)
                assert button_reply[0] == self.PACKET_BTN
                assert button_reply[1] == 1
                got_button_press = True
                break
            except RuntimeError:
                pass

        assert got_button_press
