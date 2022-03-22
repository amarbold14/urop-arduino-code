#include <PID_v1.h>
#include <Encoder.h>

// https://community.element14.com/products/arduino/b/blog/posts/simple-arduino-dc-motor-control-with-encoder-part-2
#define Kp 0.05 // Proportional Const
#define Ki 0.1 // Integral Const
#define Kd 0 // Derivative Const

int motor_pin1 = 6;
int motor_pin2 = 7;
int motor_pwr = 8;

Encoder encoder1(2, 3);
int tick_rev_ratio = 4*1000; //1 revolution = 4*1000 ticks on the encoder
long encoder_tick_now = 0;
long encoder_tick_prev = 0;

// Controller Parameters
double measured_input = 0;
double control_effort = 0;
double reference_input = 300; // Set the reference
unsigned long time_now; 
unsigned long time_prev;
const unsigned long SAMPLE_TIME = 10; //10 millis for PID controller
//const unsigned long INTRPT_COUNT = 20; 
PID motor(&measured_input, &control_effort, &reference_input, Kp, Ki, Kd, DIRECT);


void setup() {
  initMotor();
  Serial.begin(115200);
}

void loop() {
  encoder_tick_now = encoder1.read();
  time_now = micros();
  measured_input = compute_velocity(time_now, time_prev, encoder_tick_now, encoder_tick_prev);
  motor.Compute();
//  Serial.print(control_effort);
//  Serial.print(" ");
//  Serial.print(measured_input);
//  Serial.print(" ");
//  Serial.println(reference_input);
  drive_motor_closed_loop(control_effort);
//  drive_motor_open_loop();
  
  encoder_tick_prev = encoder_tick_now;
  time_prev = time_now;
  delay(10);
}

// Drive motor open-loop - start
void drive_motor_open_loop(){
  analogWrite(motor_pwr, 140);
}
// Drive motor open-loop - end

// Drive motor closed-loop - start
void drive_motor_closed_loop(double control_effort){
  if (control_effort > 0){
      digitalWrite(motor_pin1, HIGH);
      digitalWrite(motor_pin2, LOW);
      analogWrite(motor_pwr, control_effort);
  }
  else if (control_effort < 0){
      digitalWrite(motor_pin1, LOW);
      digitalWrite(motor_pin2, HIGH);
      analogWrite(motor_pwr, -control_effort);
  }
}


// Compute current velocity - start [rpm]
float compute_velocity(unsigned long time_now, unsigned long time_prev, long tick_now, long tick_prev){
  float dtime = time_now - time_prev; //in [ns]
  float dtick = tick_now - tick_prev;
//  float tick_vel_ns = dtick/dtime; // in [tick/ns]
//  float tick_vel_min = tick_vel_ns * 1e6 * 60; // in [tick/min]
//  float rpm = tick_vel_min/tick_rev_ratio; // in [r/min]
  float rpm = dtick * 1e6 * 60 / (dtime * tick_rev_ratio); // in [r/min]
  Serial.print(dtick);  Serial.print(" "); Serial.println(rpm);
  return rpm;
}
// Compute current velocity - end

// Initialize motor - start
void initMotor(){
  // Init Motor
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pwr, OUTPUT);
  digitalWrite(motor_pin1, HIGH);
  digitalWrite(motor_pin2, LOW);

  // Init Controller
  motor.SetOutputLimits(-255, 255); //PWM Speed to L298N
  motor.SetSampleTime(SAMPLE_TIME);
  motor.SetMode(AUTOMATIC);
}
// Initialize motor - end
