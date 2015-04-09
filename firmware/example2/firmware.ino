#include <inttypes.h>

#include <UDEV.h>

const int PIN_LED = 13;
UDEV      udev(Serial);

void setup() {                
  udev.begin();
  udev.serial_handshake();   //blocks for 10 seconds by default

  //reinit serial comms for application code
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
}


void loop() {
    static int i = 0;
    if (Serial.available() >= 2) {
        char cmd = Serial.read();
        char value = Serial.read();
        udev.process(cmd, value);
    }
    delay(10);
    Serial.write('a');

    if ((++i % 10) == 0)
        digitalWrite(PIN_LED, 0x01 ^ digitalRead(PIN_LED));
}

