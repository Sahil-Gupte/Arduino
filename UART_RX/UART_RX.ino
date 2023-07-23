#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial (RX, TX);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Data received is:");
  mySerial.println("Data received is:");
}
void loop() {
  if (Serial.available() > 0) {
    mySerial.write(Serial.read());
  }

  if (mySerial.available() > 0) {
    char g = mySerial.read();
    Serial.print(g);
  }
}
