// Include libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Strain Parameters
#include "HX711.h"
const byte hx711_data_pin = 13; 
const byte hx711_clock_pin = 12;
HX711 scale;
float calibration_factor = -360000; // for force sensor
float strain_HX711;


/* BNO055 Macro */
const int BurstLength=50;  // Modify this
uint16_t BNO055_SAMPLERATE_DELAY_MS = 50;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28); // Check I2C device address and correct line below (by default address is 0x29 or 0x28)
sensors_event_t event , angVelocityData , linearAccelData, magneticAccelData;
float accelX;
float accelY;
float accelZ;
float orientX;
float orientY;
float orientZ;
float magX;
float magY;
float magZ;
float angvX;
float angvY;
float angvZ;
float quatX;
float quatY;
float quatZ;
float quatW;
float temp;


/* Piezos */
const int PIEZO_PIN_A = A0; // Piezo output
const int PIEZO_PIN_B = A1;
float piezoV_A, piezoV_B;


/* mic */
const int mic_PIN = A2;
int mic;

//COLOR: little visual debugger
//const int red=4; //A5, conflict with piezo, change em
//const int green=25; //A1
//const int blue=26; //A0

//Packet
char packet[128];

//Count timer
long lastMillis = 0;
int loopMillis = 0;




void setup()
{
  Serial.begin(115200);
  
  Serial.println("Initializing....");
  Serial.println("Init HX711...");
  /* HX711 */
  scale.begin(hx711_data_pin, hx711_clock_pin);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.println("HX711 done.");

  Serial.println("Init BNO055...");
  /* BNO055 */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    while(1);
  }
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 done.");

  Serial.println("Done init");
}
 
void loop()
{
    compute_loop_time();
    get_piezo();
    get_IMU();
    get_mic();
    get_strain();
    form_packet();
    Serial.println(packet);
}

void compute_loop_time(){
    long currentMillis = millis();
    loopMillis = currentMillis - lastMillis;
    lastMillis = currentMillis;
}

void get_piezo(){
    int piezo_A = analogRead(PIEZO_PIN_A);
    int piezo_B = analogRead(PIEZO_PIN_B);
    piezoV_A = piezo_A*5/1023.0;
    piezoV_B = piezo_B*5/1023.0;
}

void get_IMU(){
      // Accelerometer reading
      bno.getEvent(&event);
      orientX = event.orientation.x;
      orientY = event.orientation.y;
      orientZ = event.orientation.z;

      bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
      angvX = angVelocityData.gyro.x;
      angvY = angVelocityData.gyro.y;
      angvZ = angVelocityData.gyro.z;

      bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
      accelX = linearAccelData.acceleration.x;
      accelY = linearAccelData.acceleration.y;
      accelZ = linearAccelData.acceleration.z;

      bno.getEvent(&magneticAccelData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
      magX = magneticAccelData.magnetic.x;
      magY = magneticAccelData.magnetic.y;
      magZ = magneticAccelData.magnetic.z;

      // Quaternion data
      imu::Quaternion quat = bno.getQuat();
      quatW = quat.w();
      quatY = quat.y();
      quatX = quat.x();
      quatZ = quat.z();
}

void get_strain(){
  strain_HX711 = scale.get_units();
}

void get_mic(){
    mic = analogRead(mic_PIN);
}

void form_packet(){
    sprintf(packet, "%d_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%.2f_%d_%.2f_#", loopMillis, orientX, orientY, orientZ, angvX, angvY, angvZ, 
                                                                      accelX, accelY, accelZ, magX, magY, magZ, quatX, quatY, quatZ, quatW, 
                                                                      piezoV_A, piezoV_B, mic, strain_HX711);

}
