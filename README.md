# arduino-udev

Get and set information by querying serial devices

## testing udev rules

On linux:

    udevadm test $(udevadm info -q path -n /dev/ttyUSB0)
    udevadm control --reload-rules
