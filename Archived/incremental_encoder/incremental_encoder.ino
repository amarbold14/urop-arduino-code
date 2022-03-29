#include <PID_v1.h>
#include <Encoder.h>

Encoder encoder1(2, 3);
int tick_rev_ratio = 4*1000; //1 revolution = 4*1000 ticks on the encoder
long encoder_tick_now = 0;
long encoder_tick_prev = 0;
unsigned long time_now; 
unsigned long time_prev;
double measured_input = 0;


void setup() {
  Serial.begin(115200);
}

void loop() {
  encoder_tick_now = encoder1.read();
  time_now = micros();
  measured_input = compute_velocity(time_now, time_prev, encoder_tick_now, encoder_tick_prev);
  
  encoder_tick_prev = encoder_tick_now;
  time_prev = time_now;
  Serial.println(encoder_tick_now);
  delay(10);
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
