#define FEED_PIN 23
#define FEED_CH 5
#define FEED_DIR 22
#define encoder0PinA  2  
#define encoder0PinB  4 
#define Switch 5 

volatile unsigned int encoder0Pos = 0;

void setup() {

  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  attachInterrupt(0, doEncoder, RISING); // encoder pin on interrupt 0 - pin2
  Serial.begin (115200);
  Serial.println("start");                // a personal quirk
  PWM_SETUP(FEED_CH, PWM_FREQ, PWM_RES);
  PWM_ATTACH(FEED_PIN, FEED_CH);

}

void loop() {
  // do some stuff here - the joy of interrupts is that they take care of themselves
  Serial.print("Position:");
  Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
    if(Ps3.data.button.square){
      digitalWrite(FEED_DIR, HIGH);
      SET_PWM(FEED_CH, 200);
      feed_bool = true;
    } else if(Ps3.data.button.circle){
       digitalWrite(FEED_DIR, LOW);
  //    SET_PWM(FEED_CH, 200);
  //    feed_bool = true;
  //  } else{
  //    SET_PWM(FEED_CH, 0);
  //    feed_bool = false;
  //  }

   if (!digitalRead(FEED_BTN)) {
      feed_bool = true;
      SET_PWM(FEED_CH, 200);
    } else {
      feed_bool = false;
      SET_PWM(FEED_CH, 0);
    }
}

void doEncoder() {
  if (digitalRead(encoder0PinB) == HIGH) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
}
