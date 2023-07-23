#include <Wire.h>
byte x = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Value is:");
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}
void receiveEvent() {
  while (Wire.available()) {
    x = Wire.read();
    Serial.print(x);
  }

}
void loop() {
  if (Serial.available() > 0) {
    Wire.beginTransmission(10);
    Wire.write(Serial.read());
    Wire.endTransmission();
  }

}
