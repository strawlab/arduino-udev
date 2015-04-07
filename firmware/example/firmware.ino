#include <inttypes.h>

#include <UDEV.h>

const int PIN_LED = 13;
UDEV      udev(Serial);

void setup() {        
  udev.begin();
  udev.serial_handshake();   //blocks for 10 seconds by default
  pinMode(PIN_LED, OUTPUT);
}


void loop() {
  digitalWrite(PIN_LED, 0x01 ^ digitalRead(PIN_LED));
  delay(20);
}

