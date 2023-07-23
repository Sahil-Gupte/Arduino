int i, j;
void setup() {
  // put your setup code here, to run once:
  Serial. begin(115200);
}
void loop()
{
  if (Serial.available() > 0)
  Serial.print("PWM: ");
    Serial.println(i, DEC); {
    i = Serial.parseInt();
  
  for (i = 0; i <= 100; i++) {
    Serial.println(i);
  }
  delay(1000);

  for (j = 100; j >= 0; j--) {
    Serial.println(j);
  }
  }
}
