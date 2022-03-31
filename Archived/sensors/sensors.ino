
#include "Adafruit_HTU31D.h"

Adafruit_HTU31D htu = Adafruit_HTU31D();
uint32_t timestamp;


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // wait till serial port opens
  }
  Serial.println("Adafruit HTU31D test");

  if (!htu.begin(0x40)) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
  timestamp = millis();
}

void loop() {
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
