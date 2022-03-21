/* Steven Yeung
 * 
 * Vanilla stepper motor drive
 * 
 * Connect ENA+ DIR- PUL- to GND
 * PUL+ to 8
 * DIR+ to 9
 * 
 * GND of arduino and power supply don't need to be connected
 * 
 * A+ to B- : Black, green, red, blue
 */

const int PinDir = 9; //direction pin
const int PinPul = 8; //pulse pin
const int step_interval_ = 500;   //delay in milliseconds between step
const int step_per_rev_ = 400;   //check this on the stepper controller board, the whole rail is about 2725/400 turns

boolean direction_now_ = HIGH;
long step_count_ = 0;
long desired_count_ = 0;
 
void setup()
{
Serial.begin(115200);
pinMode(PinDir, OUTPUT);
pinMode(PinPul, OUTPUT);
digitalWrite(PinDir, direction_now_); // low CW / high CCW 
digitalWrite(PinPul, HIGH); //borda de descida
}
 
void loop()
{
  desired_count_ = read_serial(desired_count_);
  check_status_and_distribute();
}


//Check whether position is reached - start
void check_status_and_distribute(){ 
  if (desired_count_ != step_count_){
    spin_til_count(desired_count_);
  }
  else {
    Serial.print("Desired position reached: ");
    Serial.println(step_count_);
  }
}
//Check whether position is reached - end

// Get serial command start
long read_serial(long old_desired_value){
  long serial_input_desired_value = old_desired_value;
  while(Serial.available() > 0 ){
    String serial_input_command = Serial.readString();
    String serial_input_command_0 = getValue(serial_input_command,'_',0);
    serial_input_desired_value = serial_input_command_0.toInt();
  }
  return serial_input_desired_value;
}

String getValue(String data, char separator, int index) // Delimit string based on the deliminator
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
// Get serial command end

// Motor spin - Start
void spin_til_count(long desired_count){ // Spin until globally reach a desired pulse count
  while (desired_count != step_count_){
    if (desired_count > step_count_){ //rotate CCW
      change_direction(HIGH);
      step_once();
    }
    else{
      change_direction(LOW);
      step_once();
    }
  }
  Serial.println(step_count_);
}

void change_direction(boolean desired_dir){
  digitalWrite(PinDir,desired_dir);
  direction_now_ = desired_dir;
}

void step_once(){
  digitalWrite(PinPul, HIGH); // step once
  delayMicroseconds(step_interval_); 
  digitalWrite(PinPul, LOW); 
  delayMicroseconds(step_interval_); 
  if (direction_now_ == HIGH){
    step_count_++;
  }
  else if (direction_now_ == LOW){
    step_count_--;
  }
}
// Motor spin - end
