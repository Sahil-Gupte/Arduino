#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial (RX, TX);
void setup() {
  Serial.begin(9600);
  Serial.println ("Data is:");
  mySerial.begin(9600);
  //mySerial.println ("Data is:");
}

void loop() {
  if (Serial.available() > 0) {
    mySerial.write(Serial.read());
    //    Serial.println(Serial.read());
  }

  if (mySerial.available() > 0) {
    Serial.println(mySerial.read(), HEX);
  }
}
