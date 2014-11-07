#include <inttypes.h>

#include <UDEV.h>

const int PIN_LED = 2;
UDEV      udev(Serial);

void setup() {                
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  udev.begin();
  udev.setup(PIN_LED);
}


void loop() {
  digitalWrite(PIN_LED, 0x01 ^ digitalRead(PIN_LED));
  delay(20);
}

