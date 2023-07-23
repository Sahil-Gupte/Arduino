#include <Ps3Controller.h>

#define PWM_RES 8
#define PWM_FREQ 1000
#define PWM_SETUP ledcSetup
#define PWM_ATTACH ledcAttachPin
#define SET_PWM ledcWrite

#define THROWING_CH 0
#define THROWING_CH1 1
#define LEAD_CH 2
#define BARRIER_CHA 3
#define BARRIER_CHB 4

#define THROWING_PIN_A 12
#define THROWING_DIR_A 14
#define THROWING_PIN_B 18
#define LEAD_DIR_INT 19

#define LEAD_PIN 25
#define LEAD_DIR 26

#define LEAD_BTN_UP 13
#define LEAD_BTN_DOWN 15
#define PWM_BTN_UP  16
#define PWM_BTN_DOWN 17

//#define FEED_PIN 23
//#define FEED_CH 5
//#define FEED_DIR 22

#define LIMIT_UP 33
#define LIMIT_DOWN 32

#define stepPin 23
#define stepDir 22

#define encoder0PinA  2  
#define encoder0PinB  4 
#define Switch 5 


struct Button {
  const uint8_t PIN;
  volatile uint32_t count;
  bool pressed;
};

Button btnA = {25, 0, false};
Button btnB = {26, 0, false};
Button lead_enc = {21, 0, false};


struct rpm_info {
  unsigned long last_time;
  float prev_error;
  volatile uint32_t target;
  volatile uint32_t current;
  int pwm;
};

rpm_info motorA = {0, 0, 0, 0, 0};
rpm_info motorB = {0, 0, 0, 0, 0};

void IRAM_ATTR isr(void* arg) {
  Button* s = static_cast<Button*>(arg);
  s->count += 1;
  //  if (digitalRead(LEAD_DIR_INT)) {
  //    s->count += 1;
  //  } else s->count -= 1;
  s->pressed = true;
}

volatile bool is_pressed = false;

void IRAM_ATTR isr_limit() {
  SET_PWM(LEAD_CH, 0);
  is_pressed = true;
}


void IRAM_ATTR isr() {
  btnB.count += 1;
  btnB.pressed = true;
}

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;
volatile long rpm, rpm_b;

int max_pwm = 0;

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  motorA.current = btnA.count * 5.8; //0.058;
  motorB.current = btnB.count * 5.8;// 0.058;
  btnA.count = 0;
  btnB.count = 0;
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

bool mA_dir = LOW, mB_dir = LOW;
bool up_bool = false;
bool down_bool = false;
bool feed_bool = false;

// RPM Contorl Variables
//long current_rpmA = 0, long current_rpmB = 0;
uint32_t target_rpm = 0;
//PID RPM
float kp_rpm = 0.3f; float kd_rpm = 0.75f; float ki_rpm = 0.0f;
float P_rpm = 0, I_rpm = 0, D_rpm = 0, PID_rpm;
double error_rpm, prev_err_rpm;

unsigned long last_timer = 0;
unsigned long barr_timerA = 0, barr_timerB = 0, down_timer = 0, up_timer = 0;

int input_data = 0;
float dist = 0;
int pulse = 0;
bool is_data = false;
bool stepper_dir = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //  pinMode(lead_enc.PIN, INPUT_PULLUP);
  //  pinMode(LEAD_DIR_INT, INPUT);
  //  pinMode(LIMIT_UP, INPUT_PULLUP);
  //  pinMode(LIMIT_DOWN, INPUT_PULLUP);
  //  attachInterrupt(btnB.PIN, isr_limit, FALLING);

  pinMode(LEAD_BTN_UP, INPUT_PULLUP);
  pinMode(LEAD_BTN_DOWN, INPUT_PULLUP);
  pinMode(PWM_BTN_UP, INPUT_PULLUP);
  pinMode(PWM_BTN_DOWN, INPUT_PULLUP);

  //    attachInterruptArg(btnA.PIN, isr, &btnA, FALLING);
  //  attachInterruptArg(btnA.PIN, isr, &btnB, FALLING);
  //  attachInterrupt(btnB.PIN, isr, FALLING);
  //  attachInterruptArg(lead_enc.PIN, isr, &lead_enc, FALLING);
  //  attachInterrupt(LIMIT_UP, isr_limit, FALLING);
  //  attachInterrupt(LIMIT_DOWN, isr_limit, FALLING);

  pinMode(stepPin, OUTPUT);
  pinMode(stepDir ,  OUTPUT);

  pinMode(THROWING_DIR_A, OUTPUT);
  //  pinMode(THROWING_DIR_B, OUTPUT);
  digitalWrite(THROWING_DIR_A, LOW);
  //  digitalWrite(THROWING_DIR_B, LOW);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisconnect);
  Ps3.begin("00:71:cc:e7:ea:6e");

  pinMode(THROWING_PIN_A, OUTPUT);
  pinMode(THROWING_PIN_B, OUTPUT);
  pinMode(LEAD_PIN, OUTPUT);
  pinMode(LEAD_DIR, OUTPUT);

  PWM_SETUP(THROWING_CH, PWM_FREQ, PWM_RES);
  PWM_SETUP(THROWING_CH1, PWM_FREQ, PWM_RES);
  PWM_SETUP(LEAD_CH, PWM_FREQ, PWM_RES);
  //  PWM_SETUP(FEED_CH, PWM_FREQ, PWM_RES);

  PWM_ATTACH(THROWING_PIN_A, THROWING_CH);
  PWM_ATTACH(THROWING_PIN_B, THROWING_CH1);
  PWM_ATTACH(LEAD_PIN, LEAD_CH);
  //  PWM_ATTACH(FEED_PIN, FEED_CH);

  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true);
  //  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  motorA.pwm = 0;
  motorB.pwm = 0;

  Serial.println("Init");
}

void loop() {
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    uint32_t isrCount = 0, isrTime = 0;
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
  }

  if (millis() - last_timer > 10) {
    motorA = get_rpm_info(motorA);
    motorB = get_rpm_info(motorB);
    last_timer = millis();
  }

  if (Ps3.data.button.triangle) {
    digitalWrite(LEAD_DIR, LOW);
    SET_PWM(LEAD_CH, 50);
    up_bool = true;
    down_bool = false;
  } else if (Ps3.data.button.cross) {
    digitalWrite(LEAD_DIR, HIGH);
    SET_PWM(LEAD_CH, 50);
    up_bool = false;
    down_bool = true;
  } else {
    SET_PWM(LEAD_CH, 0);
    down_bool = false;
    up_bool = false;
  }

  if (Ps3.data.button.square)
  {
    digitalWrite(stepDir , HIGH);
    digitalWrite(stepPin , HIGH);
    delayMicroseconds(8);
    digitalWrite(stepPin , LOW);
    delayMicroseconds(8);
    Serial.println("here");

  }else if(Ps3.data.button.circle)
  {
    digitalWrite(stepDir , LOW);
    digitalWrite(stepPin , HIGH);
    delayMicroseconds(8);
    digitalWrite(stepPin , LOW);
    delayMicroseconds(8);
    //Serial.println("aaaaaaaaaaa");
  }
  else
  {
    digitalWrite(stepDir , LOW);
  }
  //  if (!digitalRead(LEAD_BTN_UP)) {
  //    digitalWrite(LEAD_DIR, LOW);
  //    SET_PWM(LEAD_CH, 50);
  //    up_bool = true;
  //    down_bool = false;
  //  } else if (!digitalRead(LEAD_BTN_DOWN)) {
  //    digitalWrite(LEAD_DIR, HIGH);
  //    SET_PWM(LEAD_CH, 50);
  //    up_bool = false;
  //    down_bool = true;
  //  } else {
  //    up_bool = false;
  //    down_bool = false;
  //    SET_PWM(LEAD_CH, 0);
  //  }

  //  if(Ps3.data.button.square){
  //    digitalWrite(FEED_DIR, HIGH);
  //    SET_PWM(FEED_CH, 200);
  //    feed_bool = true;
  //  } else if(Ps3.data.button.circle){
  //    digitalWrite(FEED_DIR, LOW);
  //    SET_PWM(FEED_CH, 200);
  //    feed_bool = true;
  //  } else{
  //    SET_PWM(FEED_CH, 0);
  //    feed_bool = false;
  //  }

  //  if (!digitalRead(FEED_BTN)) {
  //    feed_bool = true;
  //    SET_PWM(FEED_CH, 200);
  //  } else {
  //    feed_bool = false;
  //    SET_PWM(FEED_CH, 0);
  //  }

  //  if (!digitalRead(PWM_BTN_UP) && (millis() - up_timer > 300)) {
  //    max_pwm += 5;
  //    if (max_pwm > 255) {
  //      max_pwm = 255;
  //    }
  //    up_timer = millis();
  //  } else if (!digitalRead(PWM_BTN_DOWN) && (millis() - down_timer > 300)) {
  //    max_pwm -= 5;
  //    if (max_pwm < 0) {
  //      max_pwm = 0;
  //    }
  //    down_timer = millis();
  //  }

  SET_PWM(THROWING_CH, max_pwm);
  SET_PWM(THROWING_CH1, max_pwm);
  Serial.print("PWM: "); Serial.print(max_pwm);
  Serial.print(" Feed:"); Serial.print(feed_bool);
  Serial.print(" Lead Up:"); Serial.print(up_bool); Serial.print(" Lead Down:"); Serial.println(down_bool);
  yield();
}

// RPM Control Function
rpm_info get_rpm_info(rpm_info data) {
  error_rpm = (float)data.target - (float)data.current;
  P_rpm = error_rpm * kp_rpm;
  // Dont Use I_rpm
  D_rpm = kd_rpm * ((error_rpm - data.prev_error) / (millis() - data.last_time));
  PID_rpm = P_rpm + D_rpm;

  if (PID_rpm > 1.0f) {
    PID_rpm = 1.0f;
  }

  if (PID_rpm < -1.0f) {
    PID_rpm = -1.0f;
  }

  data.pwm += PID_rpm;
  if (data.pwm > 100) data.pwm = 100;
  else if (data.pwm < 0) data.pwm = 0;

  data.prev_error = error_rpm;
  data.last_time = millis();
  return data;
}


void onConnect() {
  Serial.println("Connected.");
}

void onDisconnect() {
  Serial.println("Disconnected.");
}

void notify() {
  if ( Ps3.event.button_down.r1 ) {
    max_pwm += 5;
    if (max_pwm > 255) {
      max_pwm = 255;
    }
  } else if (Ps3.event.button_down.l1 ) {
    max_pwm -= 5;
    if (max_pwm < 0) {
      max_pwm = 0;
    }
  }

//  if(Ps3.event.button_down.square){
//    digitalWrite(stepDir , HIGH);
//    for(int i = 0; i < 60; i++){
//      digitalWrite(stepPin , HIGH);
//      delayMicroseconds(10);
//      digitalWrite(stepPin , LOW);
//      delayMicroseconds(10);
//      Serial.println("Phir");
//    }
//    digitalWrite(stepDir , LOW);
//  } else if(Ps3.event.button_down.circle){
//    digitalWrite(stepDir , LOW);
//    for(int i = 0; i < 60; i++){
//      digitalWrite(stepPin , HIGH);
//      delayMicroseconds(10);
//      digitalWrite(stepPin , LOW);
//      delayMicroseconds(10);
//      Serial.println("Phir");
//    }
//    digitalWrite(stepDir , LOW);    
//  }
  
}
