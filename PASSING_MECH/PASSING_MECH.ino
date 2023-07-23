#define M_DIR 4
#define M_IP 5 

void setup() {
  Serial.begin(115200);
  pinMode(M_DIR , OUTPUT);
  pinMode(M_IP , OUTPUT);
}

void loop() {
      digitalWrite(M_DIR, HIGH);
      analogWrite(M_IP , 100);
      Serial.println("Moving");
    }
