#include <Wire.h>
#define potpin A5
#define SLAVE 9
//char x[] = "Hello\n";
char y = 0;
byte packet = 0;

void setup() {
  Wire.begin(10);
  Serial.begin(115200);
  Serial.println("Value is:");
  Wire.onReceive(fromrx);
  packet |= 0x01;
  //  packet |= 0x69;
  Serial.println("Waiting");
  delay(1000);
  debugTransmit(69, SLAVE);
  Serial.println("Init packet sent");
}
void fromrx() {
  while (Wire.available()) {
    y = Wire.read();
    Serial.print(y);
  }
}
void loop() {
  if (Serial.available() > 0) {
    debugTransmit(Serial.read(), SLAVE);
    //Serial.println(analogRead(A5));
  }
}

bool debugTransmit(uint8_t data, uint8_t slave) {
  if (!(data > 0xFF)) {
    Wire.beginTransmission(slave);
    Wire.write(data);
    Wire.endTransmission();
    return true;
  } else return false;
}

/* for (int i=0; i<sizeof(x); i++ ){
   Wire.write(x[i]);
   Serial.print(x[i]);
   }

  delay (1000);
  }
*/
