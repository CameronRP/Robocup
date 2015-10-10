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

//============boolean countdown timers============
boolean turningToWeightAtRight = false;
boolean isTurningToWeightAtRight(void) { return turningToWeightAtRight; };
long turningToWeightAtRightStopTime = 0;

boolean turningToWeightAtLeft = false;
boolean isTurningToWeightAtLeft(void) { return turningToWeightAtLeft; };
long turningToWeightAtLeftStopTime = 0;

boolean turningToWeight(void) {
  return (turningToWeightAtLeft || turningToWeightAtRight);
}

int movingP = 110;
//===================MOVING STATE=================
enum MovingState { TURN_TO_WEIGHT_AT_LEFT, TURN_TO_WEIGHT_AT_RIGHT, MOVING_TO_WEIGHT_IN_FRONT, NOT_MOVING, MOVING_TO_WEIGHT_AT_RIGHT_EDGE, MOVING_TO_WEIGHT_AT_LEFT_EDGE, INTERRUPT_TRUE, MOVE_BACK_LONG, MOVE_BACK_SHORT, MOVE_TO_LEFT, MOVE_TO_RIGHT, MOVING_BIT_TO_LEFT, MOVING_BIT_TO_RIGHT};
MovingState movingState = NOT_MOVING;
MovingState previousMovingState = NOT_MOVING;


Servo leftMotorServo;
Servo rightMotorServo;

void setupDCMotor(void) {
  
  leftMotorServo.attach(left_motor);
  rightMotorServo.attach(right_motor);
}

void updateMovingState(int s){
  previousMovingState = movingState;
  movingState = (MovingState) s;  
}


void updateMotors(void){
  
  long time = millis();
  
  //Update turnign booleans
  if (turningToWeightAtRight && turningToWeightAtRightStopTime < time){
    turningToWeightAtRight = false;
  }
  if (turningToWeightAtLeft && turningToWeightAtLeftStopTime < time){
    turningToWeightAtLeft = false;
  }
  
  
  //Update left motor
  if (leftMotorGoalPower != leftMotorActualPower || (time >= leftMotorStopTime && leftMotorMovingFixTime)){
    //Left motor value needs to be updated
    
    if (serial) {
      //Serial.print("Updating L motor to value of: ");
      //Serial.println(leftMotorGoalPower);
    }
    if (time >= leftMotorStopTime && leftMotorMovingFixTime){
      leftMotorGoalPower = 0;
      leftMotorMovingFixTime = false;
    } 
    leftMotorActualPower = setLeftMotor(leftMotorGoalPower);
//    leftMotorActualPower = leftMotorGoalPower;
  }
  
  //Update right motor
  if (rightMotorGoalPower != rightMotorActualPower || (time >= rightMotorStopTime && rightMotorMovingFixTime)){
    //right motor value needs to be updated
    if (serial) {
      //Serial.print("Updating R motor to value of: ");
    }
    if (time >= rightMotorStopTime && rightMotorMovingFixTime){
      rightMotorGoalPower = 0;
      rightMotorMovingFixTime = false;
    }
    rightMotorActualPower = setRightMotor(rightMotorGoalPower);
    if (rightMotorActualPower == 0 && leftMotorActualPower == 0){
      stopMoving();
    }
  }
}

//=============================SET MOVEMENT FUNCTION===================
void turnToWeightAtLeft(void){
  if (serial) Serial.println("Turning to weight at left");
  updateMovingState((int) TURN_TO_WEIGHT_AT_LEFT);
  movingP = 2;
  int duration = 100;
  leftMotor(-30, duration);
  rightMotor(50, duration);
  turningToWeightAtRightStopTime = millis() + duration;
}

void turnToWeightAtRight(void){
  if (serial) Serial.println("Turning to weight at right");
  movingP = 2;
  updateMovingState((int) TURN_TO_WEIGHT_AT_RIGHT);
  int duration = 100;
  leftMotor(50, duration);
  rightMotor(-30, duration);
  turningToWeightAtLeftStopTime = millis() + duration;
}

void moveToWeightInFront(void){
  if (serial) Serial.println("Move to weight in front");
  movingP = 3;
  updateMovingState((int) MOVING_TO_WEIGHT_IN_FRONT);
  leftMotor(40, -1);
  rightMotor(40, -1);
}

void moveToWeightAtLeftEdge(void){
  if (serial) Serial.println("Turning to weight at left edge");
  movingP = 7;
  updateMovingState((int) MOVING_TO_WEIGHT_AT_LEFT_EDGE);
  int duration = 1000;
  leftMotor(-30, duration);
  rightMotor(50, duration);  
}

void moveToWeightAtRightEdge(void){
  if (serial) Serial.println("Turning to weight at right edge");
  movingP = 7;
  updateMovingState((int) MOVING_TO_WEIGHT_AT_RIGHT_EDGE);
  int duration = 1000;
  leftMotor(50, duration);
  rightMotor(-30, duration);  
}

void stopMoving(void){
  if (serial) Serial.println("stop moving");
  movingP = 10;
  updateMovingState((int) NOT_MOVING);
  leftMotor(0, -1);
  rightMotor(0, -1);   
}


void moveBitToRight(void){
  if (serial) Serial.println("Turning bit to right");
  movingP = 5;
  updateMovingState((int) MOVING_BIT_TO_RIGHT);
  int duration = 300;
  leftMotor(50, duration);
  rightMotor(-30, duration);  
}

void moveBitToLeft(void){
  if (serial) Serial.println("Turning bit to left");
  movingP = 5;
  updateMovingState((int) MOVING_BIT_TO_LEFT);
  int duration = 300;
  leftMotor(-30, duration);
  rightMotor(50, duration);  
}

void moveToRight(void){
  if (serial) Serial.println("Turning to right");
  movingP = 4;
  updateMovingState((int) MOVE_TO_RIGHT);
  int duration = 600;
  leftMotor(50, duration);
  rightMotor(-30, duration);  
}

void moveToLeft(void){
  if (serial) Serial.println("Turning to left");
  movingP = 4;
  updateMovingState((int) MOVE_TO_LEFT);
  int duration = 600;
  leftMotor(-30, duration);
  rightMotor(50, duration);  
}

void moveBackShort(void){
  if (serial) Serial.println("move back short");
  movingP = 1;
  updateMovingState((int) MOVE_BACK_SHORT);
  int duration = 800;
  leftMotor(-40, duration);
  rightMotor(-40, duration);
}

void moveBackLong(void){
  if (serial) Serial.println("moving back long");
  movingP = 4;
  updateMovingState((int) MOVE_BACK_LONG);
  int duration = 1200;
  leftMotor(-40, duration);
  rightMotor(-40, duration);
}

/*
void moveForward(void){
  movingP = 1;
  updateMovingState((int) MOVE_BACK_LONG);
  int duration = 1200;
  leftMotor(-40, duration);
  rightMotor(-40, duration);
}
*/

int randomTurn = 0;
void moveRandom(void){
  if (serial) Serial.println("Random turn");
  movingP = 8;
  int duration = (int) random(500) + 500;
  if (randomTurn){
    randomTurn = 0;
    if (random(1)){
      leftMotor(-40, duration);
      rightMotor(60, duration);
    } else {
      leftMotor(60, duration);
      rightMotor(-40, duration);
    }
  } else {
    randomTurn = 1;
    leftMotor(60, duration);
    rightMotor(60, duration);
  }
}
//======================================================================================================================

void leftMotor(int power, long duration){
  if (duration == -1){
    leftMotorMovingFixTime = false;
  } else {
    leftMotorMovingFixTime = true;
    leftMotorStopTime = millis() + duration;
  }
  leftMotorGoalPower = power;
}

void rightMotor(int power, long duration){
  if (duration == -1){
    rightMotorMovingFixTime = false;
  } else {
    rightMotorMovingFixTime = true;
    rightMotorStopTime = millis() + duration; 
  }
  rightMotorGoalPower = power;
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
int setLeftMotor(int value) {
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
  return value;
}

// Sets the speed of the right motor. +ve forward, -ve backward. Range is from -60 to 50.
// If value outside this range given, then it will default to nearest limit.
int setRightMotor(int value) {
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
  return value;
}

