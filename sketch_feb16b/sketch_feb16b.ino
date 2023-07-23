#define dir 5
#define pwm 6
int i;
void setup() {
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(dir , OUTPUT);
  pinMode(pwm , OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char a = Serial.read();
    if (a == 'a') {
      for (i = 0 ; i <= 255 ; i++) {
        digitalWrite(dir , HIGH);
        analogWrite(pwm , i);
        Serial.println("High");
        Serial.println(i);
        delay(10);
      }
      delay(1000);
      for (i = 255 ; i >= 0 ; i--) {
        digitalWrite(dir , HIGH);
        analogWrite(pwm , i);
        Serial.println("LOW");
        Serial.println(i);
        delay(10);
      }
      delay(10);
      for (i = 0 ; i <= 255 ; i++) {
        digitalWrite(dir , LOW);
        analogWrite(pwm , i);
        Serial.println("High");
        Serial.println(i);
        delay(10);
      }
      delay(1000);
      for (i = 255 ; i >= 0 ; i--) {
        digitalWrite(dir , LOW);
        analogWrite(pwm , i);
        Serial.println("LOW");
        Serial.println(i);
        delay(10);
      }
      delay(10);
    }
  }
}
