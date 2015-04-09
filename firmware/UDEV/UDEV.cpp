#include "UDEV.h"
#include <stdlib.h>

UDEV::UDEV(HardwareSerial &serial, int pinLed, int eepromBase, int maxLen) :
    _serial(serial),
    _pinLed(pinLed),
    _eepromBase(eepromBase),
    _maxLen(maxLen)
 {
}

udev_state_t UDEV::read_and_process(float block, int pin_led, int del) {

    udev_state_t ok = ID_NOP;

    //convert block to a millisecond counter
    int ms = block*1000;

    int c = -1;

    while (true) {
        if (pin_led >= 0)
            digitalWrite(pin_led, 0x01 ^ digitalRead(pin_led));

        if (!_serial)
            goto pause;

        if (!_serial.available())
            goto pause;

        c = _serial.read();

        if ((c != -1) && ((char)c == 'N'))
            break;

    pause:

        if (ms <= 0)
            break;

        delay(del);
        ms -= del;
    }

    char cmd = c;
    if ((c != -1) && (cmd == 'N')) {
        while (!_serial.available());
        char val = _serial.read();
        ok = process(cmd, val);
    }

    return ok;

}

static byte CRC8(byte *data, byte len) {
    byte crc = 0x00;
    while (len--) {
        byte extract = *data++;
        for (byte i = 8; i; i--) {
          byte sum = (crc ^ extract) & 0x01;
          crc >>= 1;
          if (sum) {
            crc ^= 0x8C;
          }
          extract >>= 1;
        }
    }
    return crc;
}

udev_state_t UDEV::process(char cmd, char value) {

    udev_state_t ok = ID_NOP;

    char name[_maxLen];
    memset(&name,0,_maxLen);

    if (cmd=='N') {
        ok = ID_FAIL_READ;
        if (value=='=') {
            //read the string
            for (int i=0; i<_maxLen; i++) {
                while( !_serial.available() );
                char c = _serial.read();
                name[i] = c;
            }
            //read the crc
            char crc[2] = {0,0};
            while( !_serial.available() );
            crc[0] = _serial.read();
            while( !_serial.available() );
            crc[1] = _serial.read();

            byte crca = strtoul(crc,NULL,16);
            byte crcb = CRC8((byte *)name,_maxLen);

            //if the crc is ok write to the eeprom
            if (crca == crcb) {
                for (int j=0; j<_maxLen; j++) {
                    EEPROM.write( _eepromBase + j, name[j]);
                }
                ok = ID_WRITTEN;
            } else {
                ok = ID_FAIL_CRC;
            }
        } else if (value=='?') {
            for (int j=0; j<_maxLen; j++) {
                char c = EEPROM.read( _eepromBase + j);
                name[j] = c;
                _serial.write(c);
            }

            unsigned char crc = CRC8((byte *)name,_maxLen);
            _serial.print(crc,HEX);

            ok = ID_READ;
        }
    }

    return ok;
}

void UDEV::serial_handshake(float block) {
    udev_state_t ok = read_and_process(block, _pinLed, 20);

    digitalWrite(_pinLed, ok != ID_READ);

    delay(500);
    _serial.flush();
    _serial.end();
}

void UDEV::begin(void) {
    pinMode(_pinLed, OUTPUT);
    _serial.begin(9600);
    delay(50);
}

