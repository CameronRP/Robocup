//Code to run for testing.


#define commonAnode true

void testingCode(){
  setupAll();
  Serial.println("Testing starting.");
  delay(1000);
  //turnToWeightAtRight();
  while(true){
    updateAll();
    randomMovement();
  }
}

void randomMovement(){
  Serial.println(movingState);
  if (position == CORNER){
    moveBackShort();
  }
  if (movingP > 2){
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

void testRandomMovement(){
  while(true){
    updateAll();
    if (isConduction()) { }
    if(isWallInFront()){
      stopMoving();  
    } 
    else if (turningToWeight()){
      // do nothing new?
      Serial.println("Turning to weight");
    } 
    else if (weightAtLeft()){
      Serial.println("Weight at left");
      turnToWeightAtLeft();
    } 
    else if (weightAtRight()){
      Serial.println("Weight at right");
      turnToWeightAtRight();
      //turn to right for a set time depending on how far away the weight is  
    } 
    else if (weightAtMid()){
      //Serial.println("Weight at mid");
      moveToWeightInFront();
      // move forward  
    } else if (isDoubleEdgeAtRight()){
      moveToWeightAtRightEdge();
      
    } else if (isDoubleEdgeAtLeft()){
      moveToWeightAtLeftEdge();  
    } 
    else {
      switch(position){
        case(CORNER):
          Serial.println("CORNER");
          //reverse for 500ms then rotate a random amount
          stopMoving();
          break;
          
        case(WALL_IN_FRONT):
          Serial.println("Wall in front");
          //turn to left or right
          stopMoving();
          break;
    
        case(WALL_AT_LEFT):
          Serial.println("WALL_AT_LEFT");
          //turn to right for 400ms
          stopMoving();
          break;
    
        case(WALL_AT_RIGHT):
          Serial.println("WALL_AT_RIGHT");
          //turn to left 600ms
          stopMoving();
          break;
    
        case(WALL_RIGHT_SIDE):
          Serial.println("Wall right side");
          //turn to left a little
          stopMoving();
          break;
    
        case(WALL_LEFT_SIDE):
          Serial.println("Wall left side");
          //turn to right a little
          stopMoving();
          break;
    
        case(NO_WALL):
          //Serial.println("No Wall");
          //move forward for a random amoutn then turn, this movement should be interrupted easily
          leftMotor(40, -1);
          rightMotor(40, -1);
          break;
        }  
    }
  }  
}


  
  
  /*
  if(movingState == NOT_MOVING || movingP ){
  switch(position){
    case(CORNER):
      Serial.println("CORNER");
      //reverse for 500ms then rotate a random amount
      stopMoving();
      break;
      
    case(WALL_IN_FRONT):
      Serial.println("Wall in front");
      //turn to left or right
      stopMoving();
      break;

    case(WALL_AT_LEFT):
      Serial.println("WALL_AT_LEFT");
      //turn to right for 400ms
      stopMoving();
      break;

    case(WALL_AT_RIGHT):
      Serial.println("WALL_AT_RIGHT");
      //turn to left 600ms
      stopMoving();
      break;

    case(WALL_RIGHT_SIDE):
      Serial.println("Wall right side");
      //turn to left a little
      stopMoving();
      break;

    case(WALL_LEFT_SIDE):
      Serial.println("Wall left side");
      //turn to right a little
      stopMoving();
      break;

    case(NO_WALL):
      //Serial.println("No Wall");
      //move forward for a random amoutn then turn, this movement should be interrupted easily
      leftMotor(40, -1);
      rightMotor(40, -1);
      break;
    } 
  }
  */


