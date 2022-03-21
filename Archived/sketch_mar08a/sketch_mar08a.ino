int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);


  pinMode(9, OUTPUT); 
//  pinMode(motor2pin1, OUTPUT);
//  pinMode(motor2pin2, OUTPUT);
//  pinMode(10, OUTPUT);
}

void loop() {
  //Controlling speed (0 = off and 255 = max speed):
  analogWrite(9, 255); //ENA pin
//  analogWrite(10, 200); //ENB pin

  //Controlling spin direction of motors
  Serial.print("Change direction\n");
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

//  digitalWrite(motor2pin1, HIGH);
//  digitalWrite(motor2pin2, LOW);
  delay(100);

//  digitalWrite(motor1pin1, LOW);
//  digitalWrite(motor1pin2, HIGH);

//  digitalWrite(motor2pin1, LOW);
//  digitalWrite(motor2pin2, HIGH);
  delay(100);
}
