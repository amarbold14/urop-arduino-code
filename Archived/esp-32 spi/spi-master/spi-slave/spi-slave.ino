/*

 SCP1000 Barometric Pressure Sensor Display

 Shows the output of a Barometric Pressure Sensor on a

 Uses the SPI library. For details on the sensor, see:

 http://www.sparkfun.com/commerce/product_info.php?products_id=8161

 http://www.vti.fi/en/support/obsolete_products/pressure_sensors/

 This sketch adapted from Nathan Seidle's SCP1000 example for PIC:

 http://www.sparkfun.com/datasheets/Sensors/SCP1000-Testing.zip

 Circuit:

 SCP1000 sensor attached to pins 6, 7, 10 - 13:

 DRDY: pin 6

 CSB: pin 7

 MOSI: pin 11

 MISO: pin 12

 SCK: pin 13

 created 31 July 2010

 modified 14 August 2010

 by Tom Igoe

 */

// the sensor communicates using SPI, so include the library:
#include <SPI.h>
static const int spiClk = 1000000; // 1 MHz


//Sensor's memory register addresses:
const int PRESSURE = 0x1F;      //3 most significant bits of pressure
const int PRESSURE_LSB = 0x20;  //16 least significant bits of pressure
const int TEMPERATURE = 0x21;   //16 bit temperature reading
const byte READ = 0b11111100;     // SCP1000's read command
const byte WRITE = 0b00000010;   // SCP1000's write command

const int chipSelectPin = 53;

void setup() {

  Serial.begin(9600);

  // start the SPI library:
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 8 (16/8=2Mhz)


  // initialize the data ready and chip select pins:
  pinMode(chipSelectPin, INPUT);
}

void loop() {

  byte byte_received, byte_send;

  
  SPI.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  byte_received = SPI.transfer(byte_send);
  SPI.endTransaction();
  Serial.println(byte_received, BIN);

  
}
