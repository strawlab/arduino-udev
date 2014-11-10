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

def is_valid_name(name):
    return all([n.isalnum() for n in name]) and len(name) <= 8

def get_device_name(device):
    device.write('N?')

    name_and_crc = device.read(10)
    name = name_and_crc[:8]
    crc = name_and_crc[8:]

    #check the name contains some ascii
    some_ascii = [n.isalnum() for n in name]
    if any(some_ascii):
        #check the crc matches (it is sent as a hex string for readability)
        if '%X'%crc8maxim(name) == crc:
            return name
        else:
            print repr(name)
            raise ValueError("CRC doesnt match")
    else:
        raise ValueError("Name not set: %r" % name)


def set_device_name(device,name):
    if not is_valid_name(name):
        raise ValueError("Invalid name")
    
    #right padd with NULL
    padded_name = ''.join([name[i] if i < len(name) else '\0' for i in range(8)])

    device.write('N=')
    device.write(padded_name)
    device.write('%X'%crc8maxim(padded_name))

