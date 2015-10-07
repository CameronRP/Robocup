//This uhh, Class?, File? controls the motors....
//General syntax, <left, right, both>Motor(power, duration)
//Power from -60 to 50
//duration in milliseconds, set to -1 if until told otherwise

int leftMotorGoalPower = 0;                //The Power that the left motor will be set to in the next update
int rightMotorGoalPower = 0;               //The Power that the right motor will be set to in the next update
int leftMotorActualPower = 0;              //The Power of the left motor
int rightMotorActualPower = 0;             //The Power of the right motor
long leftMotorStopTime = 0;                //Time that the left motor should stop moving
long rightMotorStopTime = 0;               //Time that the right motor should stop moving
//boolean leftMotorSetDuration = false;      //True if the left motor has been set to move a finite value of time
//boolean rightMotorSetDuration = false;     //True if the right motor has been set to move a finite value of time
boolean leftMotorMovingFixTime = false;     //True if the left motor has been set to move a finite value and is still doing so
boolean rightMotorMovingFixTime = false;    //True if the right motor has been set to move a finite value and is still doing so

const int left_motor = 11;
const int right_motor = 10;

Servo leftMotorServo;
Servo rightMotorServo;

void setupDCMotor(void) {
  
  leftMotorServo.attach(left_motor);
  rightMotorServo.attach(right_motor);
  setBothMotor(0);
     
}


void updateMotors(){
  
  long time = millis();
  //Update left motor
  if (leftMotorGoalPower != leftMotorActualPower || (time >= leftMotorStopTime && leftMotorMovingFixTime)){
    //Left motor value needs to be updated
    if (serial) {
      Serial.print("Updating L motor to value of: ");
      Serial.println(leftMotorGoalPower);
    }
    if (time >= leftMotorStopTime && leftMotorMovingFixTime){
      stopLeftMotor();
      leftMotorActualPower = 0;
      leftMotorMovingFixTime = false;
    } else {
      setLeftMotor(leftMotorGoalPower);
      leftMotorActualPower = leftMotorGoalPower;
    }
  }
  
  //Update right motor
  if (leftMotorGoalPower != rightMotorActualPower || (time >= rightMotorStopTime && rightMotorMovingFixTime)){
    //right motor value needs to be updated
    if (serial) {
      Serial.print("Updating R motor to value of: ");
    }
    if (time >= rightMotorStopTime && rightMotorMovingFixTime){
      if (serial) {Serial.println("0");}  
      stopRightMotor();
      rightMotorActualPower = 0;
      rightMotorMovingFixTime = false;
    } else {
      if (serial) {Serial.println(rightMotorGoalPower);}
      setRightMotor(rightMotorGoalPower);
      rightMotorActualPower = rightMotorGoalPower;
    }
  }
}

void leftMotor(int power, long duration){
  if (duration == -1){
    leftMotorMovingFixTime = false;
  } else {
    leftMotorMovingFixTime = true;
    leftMotorStopTime = millis() + duration; // Do we want to do this here, or when the speed is actually changed....?
  }
  leftMotorGoalPower = power;
}

void rightMotor(int power, long duration){
  if (duration == -1){
    rightMotorMovingFixTime = false;
  } else {
    rightMotorMovingFixTime = true;
    rightMotorStopTime = millis() + duration; // Do we want to do this here, or when the speed is actually changed....?
  }
  rightMotorGoalPower = power;
}

void stopLeftMotor(void) {
  setLeftMotor(0);
}

void stopRightMotor(void) {
  setRightMotor(0);
}

void stopBothMotor(void) {
  stopLeftMotor();
  stopRightMotor();
}

void bothMotors(int power, long duration){
  leftMotor(power, duration);
  rightMotor(power, duration);
}

int getLeftMotorGoalPower(void){
  return leftMotorGoalPower;
}

int getRightMotorGoalPower(void){
  return rightMotorGoalPower;
}

int getRightMotorActualPower(void){
  if (rightMotorServo.read() == rightMotorActualPower) {
    return rightMotorActualPower;
  } else {
    errorFunction("Right motor actual power is wrong value, reading: " + String(rightMotorServo.read()) + ", and variable says: " + String(rightMotorActualPower));
    return -1;
  }
}
int getLeftMotorActualPower(void){
  if (leftMotorServo.read() == leftMotorActualPower) {
    return leftMotorActualPower;
  } else {
    errorFunction("Left motor actual power is wrong value, reading: " + String(leftMotorServo.read()) + ", and variable says: " + String(leftMotorActualPower));
    return -1;
  }
}

long getLeftMotorStopTime(void){
  return leftMotorStopTime;
}

long getRightMotorStopTime(void){
  return rightMotorStopTime;
}

boolean isLeftMotorMovingFixTime(void){
  return leftMotorMovingFixTime;
}

boolean isRightMotorMovingFixTime(void){
  return rightMotorMovingFixTime;
}

// Sets the speed of the left motor. +ve forward, -ve backward. Range is from -60 to 50.
// If value outside this range given, then it will default to nearest limit.
void setLeftMotor(int value) {
  if (serial) {
    Serial.print("Setting L motor to value of: ");
    Serial.println(value);
  }
  if (value > 50) {
    value = 50;
  }
  else if (value < -60) {
    value = -60;
  }
  leftMotorServo.write(value + 90);
}

// Sets the speed of the right motor. +ve forward, -ve backward. Range is from -60 to 50.
// If value outside this range given, then it will default to nearest limit.
void setRightMotor(int value) {
  if (serial) {
    Serial.print("Setting R motor to value of: ");
    Serial.println(value);
  }
  if (value > 50) {
    value = 50;
  }
  else if (value < -60) {
    value = -60;
  }
  rightMotorServo.write(value + 90);
}

void setBothMotor(int value) {

  setRightMotor(value);
  setLeftMotor(value);
}

void turnLeftSharp(void) {
  
  leftMotor(-40, 150);
  rightMotor(40, 150);
}

void turnRightSharp(void) {
  rightMotor(-40, 150);
  leftMotor(40, 150);
}


