#define potpin A5 
void setup() {
  Serial.begin(115200);
  Serial.print("Data is:");
}
void loop() {
  Serial.println(analogRead(A5));
}
