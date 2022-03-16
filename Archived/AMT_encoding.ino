
int motor1pin1 = 6;
int motor1pin2 = 7;

int motor2pin1 = 4;
int motor2pin2 = 5;

int A = 3;
int B = 19;
int I = 18;

volatile int countTick = 0;
volatile int countIndex = 0;
volatile char precTick = 0;
volatile char precIndex = 0;
volatile char tickA = 0;
volatile char tickB = 0;
volatile char index = 0;


void setup() {
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(I, INPUT);
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);

  attachInterrupt(digitalPinToInterrupt(A), updateTickA, RISING);
  attachInterrupt(digitalPinToInterrupt(B), updateTickB, RISING);
  Serial.begin(9600);

}

void loop() {
  tick = digitalRead(A);
  tickB = digitalRead(B);
  index = digitalRead(I);
  if (precIndex != countIndex){
    Serial.print("turn :");
    Serial.println(countIndex);
  }
  if (precTick != countTick){
    Serial.print("tick :");
    Serial.println(countTick);
  }
  precIndex = countIndex;
  precTick = countTick;

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

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

//  digitalWrite(motor2pin1, LOW);
//  digitalWrite(motor2pin2, HIGH);
  delay(100);
}

void updateTickA() {
  if (tickB == LOW)
  {
    countTick = countTick + 1;
  }
  else
  {
    countTick = countTick - 1;
  }
}

void updateTickB() {
  if (tickA == LOW)
  {
    countTick = countTick - 1;
  }
  else
  {
    countTick = countTick + 1;
  }
}

// Not used yet

//void updateIndex() {
//  if (countTick > 0)
//  {
//    countIndex = countIndex + index;
//  }
//  else
//  {
//    countIndex = countIndex - index;
//  }
//  Serial.print("turn :");
//  Serial.println(countIndex);
//  countTick = 0;
//}
