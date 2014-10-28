arduino-udev
============

testing udev rules
------------------

    udevadm test $(udevadm info -q path -n /dev/ttyUSB0)
    udevadm control --reload-rules
