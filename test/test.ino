#define M_DIR1 5 //top-wheel1
#define M_IP1 6 //top-wheel1

#define SERVO 13

//#define M_DIR2 6 //top-wheel2
//#define M_IP2 7 //top-wheel2

#define M_DIR3 8 //feeding wheel dir 1
#define M_IP3 9 //feeding wheel pwm 1

#define M_DIR4 6 // lead screw dir 2
#define M_IP4 7 //lead screw pwm 2
#define M_DIR5 10 //Flap dir
#define M_IP5 11 //Flap pwm

#define IR_IP A5

////solenoid
#define SOL_IP1 3
#define SOL_IP2 4

//encoder-interrupt
#define RS 21
#define CLK 2 //rpm 
#define CLK2 3 //leadscrew count
#define CLK3 19 //leadscrew dir

int InByte, InByte2, InByte3, InitReading, InitReading2;
unsigned long timer = 0, timer1 = 0, timer2 = 0, soltimer = 0, timer3 = 0, up_timer = 0, down_timer = 0, act_timer = 0, deact_timer = 0;
float error, kp, pid, last_error, kd, D;
int M1_nPWM, M2_nPWM;

int MIN_RPM, MAX_RPM = 1000, M1_PWM, M2_PWM;
volatile long RPM_counter1 = 0, RPM_counter2 = 0, dist_counter = 0;
long RPM1 = 0 , RPM2 = 0, currentRPM1, currentRPM2, newRPM1, newRPM2;

bool flag  = false, flag2 = false, dir_flag;
unsigned long uu = 0, ww = 0, zz = 0;


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30);
  Serial.println("INIT");
  pinMode(M_DIR1 , OUTPUT);
  //pinMode(M_DIR2 , OUTPUT);
  pinMode(M_DIR3 , OUTPUT);
  pinMode(M_DIR4 , OUTPUT);
  pinMode(SERVO , OUTPUT);
  pinMode(SERVO, OUTPUT);
  pinMode(M_IP1 , OUTPUT);
  //pinMode(M_IP2 , OUTPUT);
  pinMode(M_IP3 , OUTPUT);
  pinMode(M_IP4 , OUTPUT);
  pinMode(M_DIR5 , OUTPUT);
  pinMode(M_IP5 , OUTPUT);

  pinMode(SOL_IP1 , OUTPUT);
  pinMode(SOL_IP2 , OUTPUT);

  pinMode(CLK,  INPUT_PULLUP);
  pinMode(CLK2, INPUT_PULLUP);
  pinMode(CLK3, INPUT_PULLUP);
  pinMode(RS ,  INPUT_PULLUP);

  //pinMode(LASER, OUTPUT); //IR Laser
  pinMode(IR_IP, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoderA, RISING); //interrupt0
  attachInterrupt(digitalPinToInterrupt(CLK2), dist, RISING);
  attachInterrupt(digitalPinToInterrupt(CLK3), leadDir, RISING);

  timer = millis();
  timer1 = millis();
}
void loop() {
  rpm();

  if (Serial.available() > 3) {
    InByte = Serial.parseInt();
    InByte2 = Serial.parseInt();
    InByte3 = Serial.parseInt();
    digitalWrite(M_DIR1, HIGH);
    //digitalWrite(M_DIR2, HIGH);
    digitalWrite(10, HIGH);
    if (InByte2 == 1) {
      digitalWrite(M_DIR3, HIGH);
      analogWrite(M_IP3 , 110);
    }
    if (InByte2 == 2) {
      digitalWrite(M_DIR3, LOW);
      analogWrite(M_IP3 , 110);
    }
    if (InByte2 == 3) {
      kill();
    }
  }
  if (InByte2 == 4) {

    servo(SERVO, 180);
  }

  if (InByte2 == 5) {
    servo(SERVO, 90);
  }

  if (InByte3 == 1) {
    soltimer = millis();
    digitalWrite(SOL_IP1, LOW);
    digitalWrite(SOL_IP2, LOW);
    digitalWrite(SOL_IP2, HIGH);
    Serial.println("Hello");
    if (millis() - soltimer > 50) {
      Serial.println("Hi");
      digitalWrite(SOL_IP2, LOW);
    }
  }

  if (millis() - timer1 > 10) {
    set_pid();
    timer1 = millis();
  }

}



void servo(int servo, int angle) {
  int del = (11 * angle) + 500;
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(del);
  digitalWrite(SERVO, LOW);
}

void dist() {
  dist_counter++;
}

void updateEncoderA() {
  RPM_counter1 ++;
}

void updateEncoderB() {
  RPM_counter2 ++;
}

void rpm() {
  if (millis() - timer > 10) {
    cli();
    RPM1 = RPM_counter1 * 5.8 ; //PPRC = 60 seconds / PPR;
    RPM2 = RPM_counter2 * 5.8;
    timer = millis();
    RPM_counter1 = 0;
    RPM_counter2 = 0;
    sei();

  }
  currentRPM1 = RPM1;
  currentRPM2 = RPM2;
//  Serial.print("\tRPM:");
//  Serial.println(RPM1);
//  Serial.flush();

}

void leadDir() {
  if (digitalRead(CLK3)) {
    dir_flag = true;
  } else {
    dir_flag = false;
  }
}

void pwm(uint8_t pwm_pin, uint8_t Speed) {
  if (millis() - timer3 > 10)
  {
    analogWrite(pwm_pin, Speed);
//    Serial.print("\nPWM:");
//    Serial.print(Speed);
    timer3 = millis();
  }
}

void up() {
  digitalWrite(M_DIR3, HIGH);
  analogWrite(M_IP3 , 200);
  Serial.println("Moving up");
  Serial.println(digitalRead(IR_IP));
  flag  = true;
}

void down() {
  digitalWrite(M_DIR3, LOW);
  analogWrite(M_IP3 , 200);
  Serial.println("Moving down");
  if (!digitalRead(IR_IP)) {
    //analogWrite(M_IP3 , 0);
    Serial.println("Stopping");
  }
}

void kill() {
  digitalWrite(M_IP3, LOW);
  //Serial.println("Stopping");
}

void set_pid() {

  kp = 0.3;
  kd = 0.75;
  newRPM1 = InByte;
  error = newRPM1 - currentRPM1;

  D = (error - last_error) / 10;

  pid = (error * kp) + (D * kd);

  if (pid > 1)
    pid = 1;
  if (pid < -1)
    pid = -1;

  M1_nPWM = M1_nPWM + pid;
  if (M1_nPWM > 255)
    M1_nPWM = 255;
  if (M1_nPWM < 0)
    M1_nPWM = 0;
  pwm(M_IP1, M1_nPWM);

  last_error = error;
}
