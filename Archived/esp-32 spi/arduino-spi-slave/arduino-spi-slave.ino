#include <SPI.h>

int chipSelectPin = 53;
static const int spiClk = 1000000; // 2 MHz


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
