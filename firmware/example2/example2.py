import sys

import arduinoudev
import serial

port = sys.argv[1]

name = arduinoudev.serial_handshake(port, error=True)
print name

ser = serial.Serial(port=port,
                    timeout=0.1,
                    baudrate=115200)
while 1:
    c = ser.read()
    if c != 'a':
        print "ERROR",repr(c)

