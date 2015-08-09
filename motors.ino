//This uhh, Class?, File? controls the motors....
//General syntax, <left, right, both>Motor(power, duration)
//Power from -100 to 100
//duration in milliseconds, set to -1 if until told otherwise

int leftMotorGoalPower = 0;                //The Power that the left motor will be set to in the next update
int rightMotorGoalPower = 0;               //The Power that the right motor will be set to in the next update
int leftMotorActualPower = 0;              //The Power of the left motor
int rightMotorActualPower = 0;             //The Power of the right motor
long leftMotorStopTime = 0;                //Time that the left motor should stop moving
long rightMotorStopTime = 0;               //Time that the right motor should stop moving
//boolean leftMotorSetDuration = false;      //True if the left motor has been set to move a finite value of time
//boolean rightMotorSetDuration = false;     //True if the right motor has been set to move a finite value of time
boolean leftMotorMovingFixVal = false;     //True if the left motor has been set to move a finite value and is still doing so
boolean rightMotorMovingFixVal = false;    //True if the right motor has been set to move a finite value and is still doing so

void updateMotors(){
  long time = millis();
  //Update left motor
  if (leftMotorGoalPower != leftMotorActualPower || (time >= leftMotorStopTime && leftMotorMovingFixVal)){
    //Left motor value needs to be updated
    if (time >= leftMotorStopTime && leftMotorMovingFixVal){
      //code to stop left motor
    } else {
      //Code to set left motor value to (leftfMotorGoalPower)
      leftMotorActualPower = leftMotorGoalPower;
    }
  }
  
  //Update right motor
  if (leftMotorGoalPower != rightMotorActualPower || (time >= rightMotorStopTime && rightMotorMovingFixVal)){
    //right motor value needs to be updated
    if (time >= rightMotorStopTime && rightMotorMovingFixVal){
      //code to stop right motor
    } else {
      //Code to set right motor value to (rightfMotorGoalPower)
      rightMotorActualPower = rightMotorGoalPower;
    }
  }
}

void leftMotor(int power, long duration){
  if (duration == -1){
    leftMotorMovingFixVal = false;
  } else {
    leftMotorMovingFixVal = true;
    leftMotorStopTime = millis() + duration;
  }
  leftMotorGoalPower = power;
}

void rightMotor(int power, long duration){
  if (duration == -1){
    rightMotorMovingFixVal = false;
  } else {
    rightMotorMovingFixVal = true;
    rightMotorStopTime = millis() + duration;
  }
  rightMotorGoalPower = power;
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

int getLeftMotorActualPower(void){
  return leftMotorActualPower;
}

int getRightMotorActualPower(void){
  return rightMotorActualPower;
}

long getLeftMotorStopTime(void){
  return leftMotorStopTime;
}

long getRightMotorStopTime(void){
  return rightMotorStopTime;
}

boolean isLeftMotorMovingFixVal(void){
  return leftMotorMovingFixVal;
}

boolean isRightMotorMovingFixVal(void){
  return rightMotorMovingFixVal;
}
