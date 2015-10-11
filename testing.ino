//Code to run for testing.


#define commonAnode true

enum TestState {INIT, RANDOM_MOVEMENT, WEIGHT_ON_WALL, GO_HOME, PICK_UP_WEIGHT, WEIGHT_AT_LEFT, WEIGHT_AT_RIGHT};
TestState testState = INIT;
int north = 0;
int homeColour = 0;
long pickUpStartTime = 0;

void testingCode(){
  Serial.println("Testing starting.");
  /*
  while(true){
    updateAll();
    Serial.println(getUsLow());
    //randomMovement();  
  }
  /*
  */




  setupAll();
  testState = RANDOM_MOVEMENT;
  
  while(true){
    //Serial.print("State: ");
    //Serial.println(testState);
    updateAll();
    updatePickUp();
    switch(testState){
      case RANDOM_MOVEMENT:
        randomMovement();
        break;
      case PICK_UP_WEIGHT:
        pickUpWeight();
        break;
      case WEIGHT_ON_WALL:
        //weightOnWall();
        break;
      case GO_HOME:
        //goHome();
        break;
      case WEIGHT_AT_LEFT:
        weightAtLeftFunction();
        break;
      case WEIGHT_AT_RIGHT:
        weightAtRightFunction();
        break;
    }
  }
  
}

void initTest(){
  setupAll();  
}

long weightAtLeftTime = 0;
long weightAtRightTime = 0;

void randomMovement(){
  if (isLeftTouchWeight()) {
    weightAtLeftTime = millis();
    testState = WEIGHT_AT_LEFT;
  }
  if (isRightTouchWeight()) {
    weightAtLeftTime = millis();
    testState = WEIGHT_AT_RIGHT;
  }
  //Serial.println(movingState);
  if (isConduction()){
      pickUpStartTime = millis();
      testState = PICK_UP_WEIGHT;
  }
  if (position == CORNER){
    moveBackShort();
  }
  if (movingP > 2){
    if (isLeftTouchWeight()) {turnToWeightAtLeftTouch(); }
    if (isRightTouchWeight()) {turnToWeightAtRightTouch(); }
    if (weightAtRight()) { turnToWeightAtRight(); }
    if (weightAtLeft()) { turnToWeightAtLeft(); }
  }
  if (movingP > 3){
    if (weightAtMid()) { moveToWeightInFront(); }
  }
  if (movingP > 4){
    if (isWallInFront()) { moveBackLong(); }
  }
  if (movingP > 5){
    if (isWallAtRight()) { moveToLeft(); }
    if (isWallAtLeft()) { moveToRight(); }
  }
  if (movingP > 6){
    if (isWallCloseRight()) { moveBitToLeft(); }
    if (isWallCloseLeft()) { moveBitToRight(); }
  }
  if (movingP > 7){
    if (isDoubleEdgeAtRight()) { moveToWeightAtRightEdge(); }
    if (isDoubleEdgeAtLeft()) { moveToWeightAtLeftEdge(); }
  }
  if (movingP > 8){
    moveRandom();
  }
  if (isConduction()){
    stopMoving();
  }
}



void pickUpWeight(){
  if (pickUpStartTime +2000 < millis()){
    testState = RANDOM_MOVEMENT;
  }
  updatePickUp();
  stopMoving();
}

void weightAtRightFunction(){
  leftMotor(30, -1);
  rightMotor(0, -1);
  if (weightAtLeftTime + 1000 < millis()){
    testState = RANDOM_MOVEMENT;  
  }  
  
}

void weightAtLeftFunction(){
  leftMotor(0, -1);
  rightMotor(30, -1);
  if (weightAtRightTime + 1000 < millis()){
    testState = RANDOM_MOVEMENT;  
  }
}
