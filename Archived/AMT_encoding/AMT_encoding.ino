//#include <Encoder.h>


int A = 3;
int B = 19;
int I = 18;

volatile int countTick = 0;
volatile int countIndex = 0;
volatile char precTick = 0;
volatile char precIndex = 0;
volatile char tick = 0;
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
  if (digitalRead(A) == LOW)
  {
    countTick = countTick - 1;
  }
  else
  {
    countTick = countTick + 1;
  }
}

void updateIndex() {
  if (countTick > 0)
  {
    countIndex = countIndex + index;
  }
  else
  {
    countIndex = countIndex - index;
  }
  Serial.print("turn :");
  Serial.println(countIndex);
  countTick = 0;
}
