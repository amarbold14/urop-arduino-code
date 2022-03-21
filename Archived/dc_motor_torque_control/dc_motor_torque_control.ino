/*
 * Current Sensor Sample Code: https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout/arduino-code
 * Current Sensor Library: INA219
 * 
 */


#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219; //Init current sensor, A4 to SDA, A5 to SCL

int motor1pin1 = 2; //Init motor pins
int motor1pin2 = 3;
int motor1ena = 9;
float current_mA = 0;
const float torque_constant = 0.033; //1420S010 torque constant [Nm/A]

void setup() {
  Serial.begin(9600);

  uint32_t currentFrequency;
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor1ena, OUTPUT); 

}

void loop() {
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(motor1ena, 255); //ENA pin

  //Controlling spin direction of motors
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  current_mA = ina219.getCurrent_mA();
  Serial.print("Current:       "); Serial.print(current_mA); Serial.print(" mA");
  Serial.print("Torque:       "); Serial.print(current_mA*torque_constant); Serial.println(" mNm");
  delay(100);

  
//  digitalWrite(motor1pin1, LOW);
//  digitalWrite(motor1pin2, HIGH);
}
