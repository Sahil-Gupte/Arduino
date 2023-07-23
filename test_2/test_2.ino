  //example input 1000(rpm), 1/3(feeding-->on/off) , 1/2/3(lead screw-->reverse,forward,kill)

#define M_DIR1 7 //top-wheel1
#define M_IP1 9 //top-wheel1

#define LIM 21
#define LIM2 20

//#define LASER 

#define SERVO 12 //servo
#define flap_dir
#define flap_ip

//#define M_DIR2 6 //top-wheel2
//#define M_IP2 7 //top-wheel2

#define M_DIR3 10 //feeding wheel dir 1
#define M_IP3 11 //feeding wheel pwm 1

#define M_DIR4 13  // lead screw dir 2
#define M_IP4 7 //lead screw pwm 2

#define IR_IP A5

////solenoid
#define SOL_IP1 21
#define SOL_IP2 20

//encoder-interrupt
#define RS 21
#define CLK 2 //rpm 
#define LS_DIST 3 //encoderA
#define LS_DIR 19 //encoderB

bool dir_flag, ls_pos, ip_flag = false, ip_flag2 = false;
volatile long wheel_angle, counter, old_counter;
unsigned long l_timer = 0, l_timer2 = 0;
int InByte, InByte2, InByte3, InitReading, InitReading2;
unsigned long timer = 0, timer1 = 0, timer2 = 0, timer3 = 0, up_timer = 0, down_timer = 0, act_timer = 0, deact_timer = 0;
float error, kp, pid, last_error, kd, D;
int M1_nPWM, M2_nPWM;
int l_count;


int MIN_RPM, MAX_RPM = 1000, M1_PWM, M2_PWM;
volatile long RPM_counter1 = 0, RPM_counter2 = 0, dist_counter = 0;
long RPM1 = 0 , RPM2 = 0, currentRPM1, currentRPM2, newRPM1, newRPM2;
bool flag  = false, flag2 = false;
unsigned long uu = 0, ww = 0, zz = 0;

volatile unsigned long chan, taimur;

volatile long isr_count, isr_countb = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(30);
  Serial.println("INIT");
  pinMode(M_DIR1 , OUTPUT);
  //pinMode(M_DIR2 , OUTPUT);
  pinMode(M_DIR3 , OUTPUT);
  pinMode(M_DIR4 , OUTPUT);
  pinMode(SERVO , OUTPUT);

  //pinMode(LASER , OUTPUT);

  pinMode(M_IP1 , OUTPUT);
  pinMode(M_IP3 , OUTPUT);
  pinMode(M_IP4 , OUTPUT);
  pinMode(LIM, INPUT_PULLUP);
  pinMode(LIM2, INPUT_PULLUP);


  pinMode(SOL_IP1 , OUTPUT);
  pinMode(SOL_IP2 , OUTPUT);

  pinMode(CLK,  INPUT_PULLUP);
  pinMode(LS_DIST, INPUT_PULLUP);
  pinMode(LS_DIR, INPUT_PULLUP);
  pinMode(RS ,  INPUT_PULLUP);

  //pinMode(LASER, OUTPUT); //IR Laser
  pinMode(IR_IP, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LS_DIST), lead, RISING);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoderA, RISING); //interrupt0

  timer = millis();
  timer1 = millis();

  analogWrite(M_IP1, 0);
  analogWrite(M_IP3, 0);
  analogWrite(M_IP4, 0);

  //  while (digitalRead(LIM) == 1) {
  //    Serial.println("Moving back to initial position");
  //    digitalWrite(M_DIR4, HIGH);
  //    analogWrite(M_IP4, 100);
  //  }

  //  if (digitalRead(LIM) == 0)
  //  { Serial.println("At INIT POS");
  //    digitalWrite(M_DIR4, LOW);
  //    digitalWrite(M_IP4, LOW);
  //    counter = 0;Serial.println(digitalRead(LIM));
  //  }

  attachInterrupt(digitalPinToInterrupt(LIM), leadscrew1, FALLING);
  attachInterrupt(digitalPinToInterrupt(LIM2), leadscrew2, FALLING);
}
void loop() {
  rpm();
//  if ((dir_flag) == 1) {
//    Serial.print("\tDirection: Back ");
//  }
//  else {
//    Serial.print("\tDirection: Forward ");
//  }
//  if (digitalRead(LIM) == 0) {
//    counter = 0;
//  }
//    Serial.print("\tLIM1: ");
//    Serial.print(isr_count);
//    Serial.print("\tLIM2: ");
//    Serial.print(isr_countb);
//  Serial.print("\tCounter: ");
//  Serial.println(counter);

  if (Serial.available() > 3) {
    InByte = Serial.parseInt();
    InByte2 = Serial.parseInt();
    InByte3 = Serial.parseInt();
    digitalWrite(M_DIR1, HIGH);
    //digitalWrite(M_DIR2, HIGH);
    digitalWrite(10, HIGH);
  }
  if (InByte2 == 1) {
    digitalWrite(M_DIR3, HIGH);
    analogWrite(M_IP3 , 120);
  }
  if (InByte2 == 2) {
    digitalWrite(M_DIR3, LOW);
    analogWrite(M_IP3 , 120);
  }
  if (InByte2 == 3) {
    kill();
  }


  if (InByte3 == 1) {
    digitalWrite(M_DIR4, HIGH);
    analogWrite(M_IP4, 50);
    InByte3 = 0;
  } else if (InByte3 == 2) {
    digitalWrite(M_DIR4, LOW);
    analogWrite(M_IP4, 70);
    InByte3 = 0;
  } else if (InByte3 == 3) {
    analogWrite(M_IP4, 0);
  }

  if (millis() - timer1 > 10) {
    set_pid();
    timer1 = millis();

  }
}

void leadscrew1() {
  analogWrite(M_IP4, 0);
  isr_count++;
}

void leadscrew2() {
  isr_countb++;
  analogWrite(M_IP4, 0);    
}

void lead() {
  if (digitalRead(LS_DIR) == 0) {
    dir_flag = true;
    counter--;
  } else {
    dir_flag = false;
    counter++;
  }
}

void servo(int servo, int angle) {
  int del = (11 * angle) + 500;
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(del);
  digitalWrite(SERVO, LOW);
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
      Serial.print("\tRPM:");
      Serial.println(RPM1);
  Serial.flush();
}

void pwm(uint8_t pwm_pin, uint8_t Speed) {
  if (millis() - timer3 > 10)
  {
    analogWrite(pwm_pin, Speed);
            Serial.print("\nPWM:");
            Serial.print(Speed);
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
