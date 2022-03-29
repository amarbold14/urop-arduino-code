int motor_pin1 = 6;
int motor_pin2 = 7;
int motor_pwr = 9; //connects to motor enable




void setup() {
  initMotor();
  Serial.begin(115200);
}

void loop() {
  drive_motor_open_loop();
  delay(10);
}

// Drive motor open-loop - start
void drive_motor_open_loop(){
  analogWrite(motor_pwr, 240);
}
// Drive motor open-loop - end

//// Drive motor closed-loop - start
//void drive_motor_closed_loop(double control_effort){
//  if (control_effort > 0){
//      digitalWrite(motor_pin1, HIGH);
//      digitalWrite(motor_pin2, LOW);
//      analogWrite(motor_pwr, control_effort);
//  }
//  else if (control_effort < 0){
//      digitalWrite(motor_pin1, LOW);
//      digitalWrite(motor_pin2, HIGH);
//      analogWrite(motor_pwr, -control_effort);
//  }
//}
//// Drive motor closed-loop - end


// Initialize motor - start
void initMotor(){
  // Init Motor
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pwr, OUTPUT);
  digitalWrite(motor_pin1, LOW); //LOW, HIGH, go out (propulse)
  digitalWrite(motor_pin2, HIGH); // HIGH, LOW, go in (attract)
  
}
// Initialize motor - end
