#include <PID_v1.h>
#include <Encoder.h>

// https://community.element14.com/products/arduino/b/blog/posts/simple-arduino-dc-motor-control-with-encoder-part-2
#define Kp 0.2 // Proportional Const
#define Ki 0.2 // Integral Const
#define Kd 0 // Derivative Const

int motor_pin1 = 6;
int motor_pin2 = 7;
int motor_pwr = 8;

Encoder encoder1(2, 3);
int tick_rev_ratio = 4*1000; //1 revolution = 4*1000 ticks on the encoder
long encoder_tick = 0;

// Controller Parameters
double input = 0;
double output = 0;
double reference_input = 20;
unsigned long startTime; //for timer
unsigned long currentTime;
const unsigned long SAMPLE_TIME = 10; //10 millis
const unsigned long INTRPT_COUNT = 20; 
PID motor(&input, &output, &reference_input, Kp, Ki, Kd, DIRECT);


void setup() {
  initMotor();
  Serial.begin(115200);

}

void loop() {
//  currentTime = millis();
  encoder_tick = encoder1.read();
  Serial.println(encoder_tick);
  motor.Compute();
//  Serial.println(output);

  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(8, 255); //ENA pin
  //Controlling spin direction of motors
  digitalWrite(motor_pin1, HIGH);
  digitalWrite(motor_pin2, LOW);

}


void initMotor(){
  // Init Motor
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pwr, OUTPUT);

  // Init Controller
  startTime = millis();
  motor.SetOutputLimits(-255, 255); //PWM Speed to L298N
  motor.SetSampleTime(SAMPLE_TIME);
  motor.SetMode(AUTOMATIC);
}
