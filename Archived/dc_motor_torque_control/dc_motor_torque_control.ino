/*
 * Current Sensor Sample Code: https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout/arduino-code
 * Current Sensor Library: INA219
 * 
 */


#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219; //Init current sensor, A4 to SDA, A5 to SCL

int motor1pin1 = 6; //Init motor pins
int motor1pin2 = 7;
int motor1ena = 8;
float current_mA = 0;
const float torque_constant = 0.033; //1420S010 torque constant [Nm/A]
float torque_measured = 0;
long pwm = 0;

void setup() {
  Serial.begin(9600);

  // Setup Current Sensor
  uint32_t currentFrequency;
//  if (! ina219.begin()) {
//    Serial.println("Failed to find INA219 chip");
//    while (1) { delay(10); }
//  }

  // Setup Motor
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor1ena, OUTPUT); 

}

void loop() {
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(motor1ena, pwm); //ENA pin

  //Controlling spin direction of motors
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

//  torque_measured = get_torque(current_mA);
  pwm = read_serial(pwm);

//  Serial.print("PWM:"); Serial.print(pwm); Serial.print(" ;");
//  Serial.print("Current:"); Serial.print(current_mA); Serial.print(" mA;");
//  Serial.print("Torque:"); Serial.print(torque_measured); Serial.println(" mNm");
//  Serial.print(pwm); 
//  Serial.print(" ");
//  Serial.print(current_mA); 
  Serial.print(" ");
  Serial.println(torque_measured);
//  Serial.print(" ");
  
  delay(100);
}



// Get torque signal - start
float get_torque(float & current){ //Pass by reference, directly updates current value
  current = ina219.getCurrent_mA();
  return current*torque_constant;
}

// Get serial command start
long read_serial(long old_desired_value){
  long serial_input_desired_value = old_desired_value;
  while(Serial.available() > 0 ){
    String serial_input_command = Serial.readString();
    String serial_input_command_0 = getValue(serial_input_command,'_',0); //get the first value before '_' from serial input
    serial_input_desired_value = serial_input_command_0.toInt();
  }
  return serial_input_desired_value;
}

String getValue(String data, char separator, int index) // Delimit string based on the deliminator
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// Get serial command end
