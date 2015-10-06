
const int left_motor = 11;
const int right_motor = 10;

Servo leftMotor1;
Servo rightMotor1;

void setupDCMotor(void) {
  
  leftMotor1.attach(left_motor);
  rightMotor1.attach(right_motor);
  setBothMotor(0);
     
}




// Sets the speed of the left motor. +ve forward, -ve backward. Range is from -60 to 50.
// If value outside this range given, then it will default to nearest limit.
void setLeftMotor(int value) {
  Serial.print("Setting L motor to value of: ");
  Serial.println(value);
  if (value > 50) {
    value = 50;
  }
  else if (value < -60) {
    value = -60;
  }
  leftMotor1.write(value + 90);
}

// Sets the speed of the right motor. +ve forward, -ve backward. Range is from -60 to 50.
// If value outside this range given, then it will default to nearest limit.
void setRightMotor(int value) {
  Serial.print("Setting R motor to value of: ");
  Serial.println(value);
  if (value > 50) {
    value = 50;
  }
  else if (value < -60) {
    value = -60;
  }
  rightMotor1.write(value + 90);
}

void setBothMotor(int value) {

  setRightMotor(value);
  setLeftMotor(value);
}

void turnLeftSharp(void) {
  
  setLeftMotor(-40);
  setRightMotor(40);
  delay(300);
}

void turnRightSharp(void) {
  setRightMotor(-40);
  setLeftMotor(40);
  delay(300);
}

void getOutOfHere(void) {
  
  int number = (int) millis();
  
  digitalWrite(led3, HIGH);
  
  if ((number % 2) == 1) {
    digitalWrite(led1, HIGH);
    setRightMotor(-60);
    setLeftMotor(20);
    delay((number % 200) + 800);
    digitalWrite(led1, LOW);
  } else {
    digitalWrite(led2, HIGH);
    setRightMotor(20);
    setLeftMotor(-60);
    delay((number % 200) + 800);
    digitalWrite(led2, LOW);
  }
  
  digitalWrite(led3, LOW);
}
    
  
