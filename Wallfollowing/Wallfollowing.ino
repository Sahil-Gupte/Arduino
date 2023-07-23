#define IN1 3      
#define IN2 4     
#define IN3 5
#define IN4 6
#define EN1 10
#define EN2 12  
void setup() {
  // put your setup code here, to run once:
  pinMode(EN1, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);
}
void forward()
{
    analogWrite(EN1, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(EN2, 255);
   // delay(100);
}
void left()
{
    analogWrite(EN1, 100);
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    analogWrite(EN1, 100);
   // delay (100);
}
void right()
{
   analogWrite(EN1, 100);
   digitalWrite (IN1, LOW);
   digitalWrite (IN2, HIGH);
   digitalWrite (IN3, HIGH);
   digitalWrite (IN4, LOW);
   analogWrite(EN1, 100);
   //delay (100);
}
void loop() {
 forward();
 delay(2000);
 left();
 delay(2000);
 right();
 delay(2000);
}
