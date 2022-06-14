// Include libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


// WIFI Parameters
#include <WiFi.h>
const char* ssid = "MIT"; //yourNetworkName";
const char* password =  "";//"yourNetworkPass";
//const char* ssid = "ritata"; //yourNetworkName";
//const char* password =  "88888888";//"yourNetworkPass";
const uint16_t port = 8090;
//const char * host = "18.58.5.155"; //RasPi on MIT network
//const char * host = "192.168.0.100"; //RasPi on data_transfer
const char * host = "18.21.156.5"; //PC
//const char * host = "192.168.1.195"; //Home



// Acceleration Parameters
const int BurstLength=50;  // Modify this
/* Set the delay between fresh samples */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 50;
// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);


// Strain Parameters
#include "HX711.h"
const byte hx711_data_pin = 14; //A5
const byte hx711_clock_pin = 32;
HX711 scale(hx711_data_pin, hx711_clock_pin);
const int strainInterval=9;

// Audio Parameters
#include <driver/I2S.h>
const i2s_port_t I2S_PORT = I2S_NUM_0;
int32_t audioData;

// Storage
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
String transdata[BurstLength];

unsigned long times;
const int strainLength=(int)(BurstLength/strainInterval);
int strain[strainLength];
int packetCounter=1;

//COLOR!
const int red=4; //A5
const int green=25; //A1
const int blue=26; //A0


void setup()
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
  bno.setExtCrystalUse(true);
}
 
void loop()
{
    // Begin WiFi
    WiFiClient client;

    /* Check connection of Wifi - could be removed */
    if (!client.connect(host, port)) {
        Serial.println("Fail to connect to host.");
        delay(10);
        return;}
    else{    
      digitalWrite(red,HIGH); //turn green from yellow
      digitalWrite(green,LOW); 
      digitalWrite(blue,HIGH);
    }
 
    // Burst Sample Accelerometer and Strain and Audio
    int i=0;
    int32_t sample = 0;   
    
    
    while(i<BurstLength) {
      times=micros();

      // Accelerometer reading
      sensors_event_t  angVelocityData , linearAccelData;
      bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
      bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
      angvZ = angVelocityData.gyro.z;
      accelZ = linearAccelData.acceleration.z;


      // Accelerometer reading
//      sensors_event_t orientationData , angVelocityData , linearAccelData, magneticAccelData;
//      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
//      bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
//      bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
//      bno.getEvent(&magneticAccelData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
      
//      orientX = orientationData.orientation.x;
//      orientY = orientationData.orientation.y;
//      orientZ = orientationData.orientation.z;

//      bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
//      angvX = angVelocityData.gyro.x;
//      angvY = angVelocityData.gyro.y;
//      angvZ = angVelocityData.gyro.z;

//      bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
//      accelX = linearAccelData.acceleration.x;
//      accelY = linearAccelData.acceleration.y;
//      accelZ = linearAccelData.acceleration.z;
      
//      bno.getEvent(&magneticAccelData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
//      magX = magneticAccelData.magnetic.x;
//      magY = magneticAccelData.magnetic.y;
//      magZ = magneticAccelData.magnetic.z;

      // Quaternion data
      imu::Quaternion quat = bno.getQuat();
      quatW = quat.w();
      quatY = quat.y();
      quatX = quat.x();
      quatZ = quat.z();
      
      // Temperature
      int8_t boardTemp = bno.getTemp();
      temp = boardTemp;
      
      // Read audio data
      int bytes_read = i2s_pop_sample(I2S_PORT, (char *)&sample, portMAX_DELAY);
      audioData=sample;
      
//      if (i%strainInterval==0) {
//        strain[i/strainInterval]=scale.read();
//        transdata[i] = String(times) + "," + String(accelX) + "," + String(accelY) + "," + String(accelZ) + "," + strain[i/strainInterval] + "," + String(audioData) + "," + String(orientX) + "," + String(orientY) + "," + String(orientZ) + "," + String(angvX) + "," + String(angvY) + "," + String(angvZ) + "," + String(magX) + "," + String(magY) + "," + String(magZ) + "," + String(quatX) + "," + String(quatY) + "," + String(quatZ) + "," + String(quatW) + "," + String(temp) +  '\n'; 
//        }
//      else {
//        transdata[i] = String(times) + "," + String(accelX) + "," + String(accelY) + "," + String(accelZ) + ",Nah," + String(audioData)+ "," + String(orientX) + "," + String(orientY) + "," + String(orientZ) + "," + String(angvX) + "," + String(angvY) + "," + String(angvZ) + "," + String(magX) + "," + String(magY) + "," + String(magZ) + "," + String(quatX) + "," + String(quatY) + "," + String(quatZ) + "," + String(quatW) + "," + String(temp) +  '\n';}
//        i+=1;}

      if (i%strainInterval==0) {
        strain[i/strainInterval]=scale.read();
        transdata[i] = String(times) + "," + String(accelZ) + "," + strain[i/strainInterval] + "," + String(audioData) + "," +  String(angvZ) + "," + String(quatX) + "," + String(quatY) + "," + String(quatZ) + "," + String(quatW) + "," + String(temp) +  '\n'; 
        }
      else {
        transdata[i] = String(times) + "," + String(accelZ) + "," + "NaN" + "," + String(audioData) + "," +  String(angvZ) + "," + String(quatX) + "," + String(quatY) + "," + String(quatZ) + "," + String(quatW) + "," + String(temp) +  '\n';}
        i+=1;}

      
    digitalWrite(green,HIGH);
    digitalWrite(blue,LOW); //turn from green to blue 
    
    // Transmit Data
    client.println("D");
    i=0;
    while(i<BurstLength) {
      client.println(transdata[i]);
      i+=1;}

    // End WiFi
    client.stop();
//    Serial.println("Data sent at " + String(round(millis()/1000)) + " Packet " +String(packetCounter));
    packetCounter++;
}
