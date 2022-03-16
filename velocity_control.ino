#include <PID_v1.h>
// https://community.element14.com/products/arduino/b/blog/posts/simple-arduino-dc-motor-control-with-encoder-part-2
#define Kp 0.2 // Proportional Const
#define Ki 0.2 // Integral Const
#define Kd 0 // Derivative Const

int motor_pin1 = 6;
int motor_pin2 = 7;
int motor_pwr = 8;

int A = 2;
int B = 3;
int I = 18;

volatile int countTick = 0;
volatile char precTick = 0;
volatile char tickA = 0;
volatile char tickB = 0;


double input = 0;
double output = 0;
double setpointMotor = 20;
unsigned long startTime; //for timer
unsigned long currentTime;
unsigned long countInterrupts = 0;
const unsigned long SAMPLE_TIME = 100;
const unsigned long INTRPT_COUNT = 20; 
double rpm_speed = 0;


PID motor(&input, &output, &setpointMotor, Kp, Ki, Kd, DIRECT);
double storeB = 0;



void setup() {
  initMotor();

  Serial.begin(115200);
  while(!Serial){
    // wait for Serial to start  
  }
}

void loop() {
  currentTime = millis();
  tickA = digitalRead(A);
  tickB = digitalRead(B);
  motor.Compute();
  Serial.println(output);
  
  if (precTick != countTick){
    Serial.print("tick :");
    Serial.println(countTick);
  }
  precTick = countTick;


  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(8, 50); //ENA pin

  //Controlling spin direction of motors
  Serial.print("Change direction\n");
  digitalWrite(motor_pin1, HIGH);
  digitalWrite(motor_pin2, LOW);

}

void updateTickA() {
  if (tickB == LOW){
    countTick = countTick + 1;
  }
  else{
    countTick = countTick - 1;
  }
}

void updateTickB() {
  if (tickA == LOW){
    countTick = countTick - 1;
  }
  else{
    countTick = countTick + 1;
  }
}

void initMotor(){
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(I, INPUT);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  attachInterrupt(digitalPinToInterrupt(A), updateTickA, RISING);
  attachInterrupt(digitalPinToInterrupt(B), updateTickB, RISING);

  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pwr, OUTPUT);

  startTime = millis();
  motor.SetOutputLimits(0, 255);
  motor.SetSampleTime(SAMPLE_TIME);
  motor.SetMode(AUTOMATIC);
}

void isr(){
  countInterrupts++;
  if (countInterrupts == INTRPT_COUNT){
    input = (float) INTRPT_COUNT * 60 / (float) (1000 * 2048 * (currentTime - startTime));
    startTime = currentTime;
    countInterrupts = 0;
  }
}
