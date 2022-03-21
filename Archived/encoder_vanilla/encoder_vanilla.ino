/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// One rotation is 4*2048
Encoder encoder1(2, 3);
//Encoder encoder2(3, 8);

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}

long oldPosition_1  = 0;
//long oldPosition_2  = -999;

void loop() {
  long newPosition_1 = encoder1.read();
//  long newPosition_2 = encoder2.read();
  if ((newPosition_1 != oldPosition_1) ) {
//if (newPosition_2 != oldPosition_2) {
    oldPosition_1 = encoder1.read();
//    oldPosition_2 = encoder2.read();
    Serial.print("Encoder 1:");
    Serial.println(newPosition_1);
//    Serial.print(";  Encoder 2:");
//    Serial.println(newPosition_2);
  }
}
