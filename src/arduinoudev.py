import time
import serial

def _crc8maxim(crc, c):
    crc ^= ord(c)
    for i in range(8):
        if crc & 0x1:
            crc = (crc >> 1) ^ 0x8C
        else:
            crc = (crc >> 1)
    return crc

def crc8maxim(s):
    crc = 0
    for c in s:
        crc = _crc8maxim(crc,c)
    return crc

def is_valid_name(name,maxlen=8):
    return all([n.isalnum() for n in name]) and len(name) <= maxlen

def open_device(port):
    return serial.Serial(port, baudrate=9600, timeout=10.)

def get_device_name(device, maxlen=8):
    device.write('N?')

    name_and_crc = device.read(maxlen+2)
    name = name_and_crc[:maxlen]
    crc = name_and_crc[maxlen:]

    #check the name contains some ascii
    some_ascii = [n.isalnum() for n in name]
    if any(some_ascii):
        #check the crc matches (it is sent as a hex string for readability)
        if '%X'%crc8maxim(name) == crc:
            return name
        else:
            raise ValueError("CRC doesnt match %r (%X != %s)" % (name_and_crc,crc8maxim(name),crc))
    else:
        raise ValueError("Name not set: %r" % name)

def set_device_name(device,name,maxlen=8,verbose=False):
    if not is_valid_name(name):
        raise ValueError("Invalid name")
    
    #right padd with NULL
    padded_name = ''.join([name[i] if i < len(name) else '\0' for i in range(maxlen)])

    s = 'N='
    s += padded_name
    s += '%X'%crc8maxim(padded_name)
    device.write(s)
    if verbose:
        print "RAW: ",
        for c in s:
            print "0x%X" % ord(c),
        print

def reset_device(device):
    device.setRTS(False)
    device.setDTR(False)
    time.sleep(0.25)
    device.setRTS(True)
    device.setDTR(True)
    time.sleep(0.25)

def serial_handshake(port):
    with open_device(port) as ser:
        reset_device(ser)
        time.sleep(5.0)
        ser.flushInput()
        name = get_device_name(ser)

    return name.strip('\0')

