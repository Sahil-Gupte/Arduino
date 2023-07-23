#include <Ps3Controller.h>

#define CONTROLLER_PS3
#define CONTROLLER_ESP_NOW

#define PWM_RES 8
#define PWM_FREQ 1000
#define PWM_SETUP ledcSetup
#define PWM_ATTACH ledcAttachPin
#define SET_PWM ledcWrite

#define MA_CH 0
#define MB_CH 1
#define MC_CH 2
#define TMA_CH 3
#define TMB_CH 4

#define MA_PIN 12
#define MB_PIN 18
#define MC_PIN 16
#define TMA_PIN 25 // throwing motor A pin
#define TMB_PIN 24
#define BP_PIN 33 // Ball pickking motor

#define MOTOR_A_DIR 14
#define MOTOR_B_DIR 19
#define MOTOR_C_DIR 17

#define WDT_TIMEOUT 3

TaskHandle_t Task1;
TaskHandle_t Task2;

// better code
float max_pwm = 80.0;
uint16_t bruh_A = 0;
uint16_t bruh_B = 0;
uint16_t bruh_C = 0;
float val_x = 0;
float val_y = 0;
float val_w = 0;

float mA_Speed, mB_Speed, mC_Speed, motorSpeedA, motorSpeedB, motorSpeedC;
boolean mA_dir, mB_dir, mC_dir;

boolean pick_state = LOW;

unsigned long new_time = 0, elapsed_time = 0, prev_time = 0;

void setup() {
  Serial.begin(115200);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisconnect);
  Ps3.begin("00:71:cc:e7:ea:6e");

  pinMode(MA_PIN, OUTPUT);
  pinMode(MB_PIN, OUTPUT);
  pinMode(MC_PIN, OUTPUT);
  pinMode(BP_PIN, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(MOTOR_C_DIR, OUTPUT);

  PWM_SETUP(MA_CH, PWM_FREQ, PWM_RES);
  PWM_SETUP(MB_CH, PWM_FREQ, PWM_RES);
  PWM_SETUP(MC_CH, PWM_FREQ, PWM_RES);
  PWM_SETUP(TMA_CH, PWM_FREQ, PWM_RES);
  PWM_SETUP(TMB_CH, PWM_FREQ, PWM_RES);

  PWM_ATTACH(MA_PIN, MA_CH);
  PWM_ATTACH(MB_PIN, MB_CH);
  PWM_ATTACH(MC_PIN, MC_CH);
  PWM_ATTACH(TMA_PIN, TMA_CH);
  PWM_ATTACH(TMB_PIN, TMB_CH);

  Serial.println("Ready.");


  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    7,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    Task2code,   /* Task function. */
    "Task2",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    5,           /* priority of the task */
    &Task2,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);
}

//Task 1
void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    prev_time = new_time;
    new_time = micros();

    if (Ps3.isConnected()) {
      getLeftJoyVectors();
      getRightJoyVectors();
      get_speed(val_x, val_y, val_w);
      set_Motion();
    }
    vTaskDelay(1);
  }
}

// Task 2
void Task2code( void * pvParameters ) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());


  for (;;) {
    Serial.print("MAX : "); Serial.print(max_pwm);
    Serial.print(" X : "); Serial.print(Ps3.data.analog.stick.lx);
    Serial.print(" Y : "); Serial.print(Ps3.data.analog.stick.ly);
    Serial.print(" MA : "); Serial.print(bruh_A);
    Serial.print(" MB : "); Serial.print(bruh_B);
    Serial.print(" MC : "); Serial.println(bruh_C);
    if (Ps3.data.button.cross) {
      pick_state = !pick_state;
      digitalWrite(BP_PIN, pick_state);
      Serial.print("Picking: ");Serial.println(pick_state);
    }
//    vTaskDelay(10);
  }
}

void loop() {
  // Keep empty
}


void getLeftJoyVectors() {
  int x = Ps3.data.analog.stick.lx;
  int y = Ps3.data.analog.stick.ly;

  val_x = (float)x / 128.0f;
  val_y = (float)(-1 * y) / 128.0f;

  if ((-10 < x && x < 10) && (-10 < y && y < 10)) {
    val_x = 0; val_y = 0;
  }
}

void getRightJoyVectors() {
  int w = Ps3.data.analog.stick.rx;
  val_w = (float)w / 128.0f;
  if (-10 < w && w < 10) {
    val_w = 0;
  }
}

void get_speed(float x, float y , float w) {
  motorSpeedA = ( (x * (0.67)) + (y * 0) + (w * 0.33) );
  motorSpeedB = ( (x * (-0.33f)) + (y * (-0.58f)) + (w * 0.33) );
  motorSpeedC = ( (x * (-0.33f)) + (y * (0.58f))  + (w * 0.33) );

  mA_dir = motorSpeedA < 0 ? LOW : HIGH;
  mB_dir = motorSpeedB < 0 ? LOW : HIGH;
  mC_dir = motorSpeedC < 0 ? LOW : HIGH;

  motorSpeedA = abs(motorSpeedA);
  motorSpeedB = abs(motorSpeedB);
  motorSpeedC = abs(motorSpeedC);

  mA_Speed = motorSpeedA * max_pwm;
  mB_Speed = motorSpeedB * max_pwm;
  mC_Speed = motorSpeedC * max_pwm;

  bruh_A = round(mA_Speed);
  bruh_B = round(mB_Speed);
  bruh_C = round(mC_Speed);
}

void set_Motion() {
  digitalWrite(MOTOR_A_DIR, mA_dir);
  digitalWrite(MOTOR_B_DIR, mB_dir);
  digitalWrite(MOTOR_C_DIR, mC_dir);

  SET_PWM(MA_CH, bruh_A);
  SET_PWM(MB_CH, bruh_B);
  SET_PWM(MC_CH, bruh_C);
#ifdef DEBUG_LOCOMOTION
  Serial.print("MAX : "); Serial.print(max_pwm); Serial.print(" X : "); Serial.print(joy_data.joy_X); Serial.print(" Y : "); Serial.print(joy_data.joy_Y);// Serial.print(" W : "); Serial.print(joy_W);
  Serial.print(" MA : "); Serial.print(bruh_A); Serial.print(" MB : "); Serial.print(bruh_B); Serial.print(" MC : "); Serial.println(bruh_C);
#endif
}

float map_1(float x, float in_min, float in_max, float out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void onConnect() {
  Serial.println("Connected.");
}

void onDisconnect() {
  Serial.println("Disconnected.");
  SET_PWM(MA_CH, 0);
  SET_PWM(MB_CH, 0);
  SET_PWM(MC_CH, 0);
  val_x = 0; val_y = 0; val_w = 0;
}

void notify()
{
  if ( Ps3.event.button_down.l1 ) {
    max_pwm += 5;
    if (max_pwm > 255) {
      max_pwm = 255;
    }
  } else if (Ps3.event.button_down.r1 ) {
    max_pwm -= 5;
    if (max_pwm < 0) {
      max_pwm = 0;
    }
  }
}
