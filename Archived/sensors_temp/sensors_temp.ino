#include <Adafruit_Sensor.h>
#include <Adafruit_HTU31D.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_HTU31D htu = Adafruit_HTU31D();
uint32_t timestamp;

// Connect the MEMS AUD output to the Arduino A0 pin
int mic = A0;
int micOut;



void setup() {
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10); // wait till serial port opens
  }
  Serial.println("Adafruit HTU31D test");

  if (!htu.begin(0x40)) 
  {
    Serial.println("Couldn't find temperature sensor!");
    while (1);
  }
  timestamp = millis();
 
  /* Initialise the imu sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Oops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
  bno.setExtCrystalUse(true);
}

void loop() {
  micOut = analogRead(mic);
  Serial.print("Audio: "); 
  Serial.println(micOut);
  getTemp();
  getBnoRawData();
}

void getTemp() {
  sensors_event_t humidity, temp;
  
  htu.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  Serial.print("Temp: "); 
  Serial.print(temp.temperature);
  Serial.println(" C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" \% RH");
  delay(500);
}




void getBnoRawData() {
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> magnetic = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> linear_accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  imu::Quaternion quat = bno.getQuat();
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  Serial.println(magnetic[0]);
  Serial.println(magnetic[1]);
  Serial.println(magnetic[2]);
  delay(100);
  
}
