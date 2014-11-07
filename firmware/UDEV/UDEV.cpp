#include "UDEV.h"
#include <stdlib.h>

UDEV::UDEV(HardwareSerial &serial, int eepromBase, int maxLen) :
    _serial(serial),
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
        c = _serial.read();

        if ((c != -1) && ((char)c == 'N'))
            break;

        if (ms <= 0)
            break;

        delay(del);
        ms -= del;

        if (pin_led >= 0)
            digitalWrite(pin_led, 0x01 ^ digitalRead(pin_led));
    }

    char cmd = c;
    if ((c != -1) && (cmd == 'N')) {
        while (!_serial.available());
        char val = _serial.read();
        ok = process(cmd, val);
        _serial.flush();
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
        int eeprom_idx = 0;
        if (value=='=') {
            //read the string
            for (int i=0; i<_maxLen; i++) {
                while( !_serial.available() );
                char c = _serial.read();
                name[i] = c;
            }
            //read the crc
            char crc[2] = {0,0};
            while( _serial.available() <= 2 );
            crc[0] = _serial.read();
            crc[1] = _serial.read();

            byte crca = strtoul(crc,NULL,16);
            byte crcb = CRC8((byte *)name,_maxLen);

            //if the crc is ok write to the eeprom
            if (crca == crcb) {
                for (int j=0; j<_maxLen; j++)
                    EEPROM.write( _eepromBase + j, name[j] );
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
            _serial.print(CRC8((byte *)name,_maxLen), HEX);
            ok = ID_READ;
        }
    }

    return ok;
}

void UDEV::setup(int pin_led) {
    udev_state_t ok = read_and_process(2, pin_led, 100);
    if (ok == ID_WRITTEN)
        read_and_process(1, pin_led, 50);
}
        

