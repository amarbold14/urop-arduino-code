#include <Servo.h>
#include <Encoder.h>

Servo myservo;  // create servo object to control a servo

Encoder encoder1(2, 3);
int tick_rev_ratio = 4*1000; //1 revolution = 4*1000 ticks on the encoder
long encoder_tick_now = 0;
long encoder_tick_prev = 0;
unsigned long time_now; 
unsigned long time_prev;
double measured_input = 0;

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  myservo.attach(13);  // attaches the servo on pin 13 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    Serial.println(analogRead(A5)); // potentiometer reading
    encoder_tick_now = encoder1.read();
    Serial.println(encoder_tick_now);
    delay(30);                       // waits 30ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    Serial.println(analogRead(A5));
    myservo.write(pos);              // tell servo to go to position in variable 'pos'

    encoder_tick_now = encoder1.read();
    Serial.println(encoder_tick_now);
    
    delay(30);                       // waits 30ms for the servo to reach the position
  }
}

// Compute current velocity - start [rpm]
float compute_velocity(unsigned long time_now, unsigned long time_prev, long tick_now, long tick_prev){
  float dtime = time_now - time_prev; //in [ns]
  float dtick = tick_now - tick_prev;
//  float tick_vel_ns = dtick/dtime; // in [tick/ns]
//  float tick_vel_min = tick_vel_ns * 1e6 * 60; // in [tick/min]
//  float rpm = tick_vel_min/tick_rev_ratio; // in [r/min]
  float rpm = dtick * 1e6 * 60 / (dtime * tick_rev_ratio); // in [r/min]
//  Serial.print(dtick);  Serial.print(" "); Serial.println(rpm);
  return rpm;
}
// Compute current velocity - end
