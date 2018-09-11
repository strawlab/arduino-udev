cmake_minimum_required(VERSION 2.8.3)
project(arduino_udev)


find_package(catkin REQUIRED COMPONENTS)

catkin_package(CATKIN_DEPENDS)

catkin_python_setup()

catkin_install_python(
    PROGRAMS
    bin/arduino-udev-name
    DESTINATION
    ${CATKIN_PACKAGE_BIN_DESTINATION}
)

