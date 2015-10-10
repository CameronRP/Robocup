//This will look at the sensors and do some logic stuff

#define ON_GREEN_R 128
#define ON_GREEN_B 58
#define ON_GREEN_G 110
#define ON_GREEN_C 301

#define ON_BLACK_R 109
#define ON_BLACK_B 47
#define ON_BLACK_G 82
#define ON_BLACK_C 226

#define ON_BLUE_R 132
#define ON_BLUE_B 106
#define ON_BLUE_G 125
#define ON_BLUE_C 367




//=================VARIABLES=========================


enum Position {CORNER, WALL_LEFT_SIDE, WALL_RIGHT_SIDE, WALL_IN_FRONT, WALL_AT_LEFT, WALL_AT_RIGHT, NO_WALL};
Position position = NO_WALL;

void updatePositionState(){
  if (isWallCloseLeftAngle() && isWallCloseRightAngle()) {
    position = CORNER;  
  } else if (isWallCloseLeft() && isWallCloseRight() || isWallCloseMid()){
    position = WALL_IN_FRONT;
  } else if (isWallCloseLeft()){
    position = WALL_AT_LEFT;
  } else if (isWallCloseRight()){
    position = WALL_AT_RIGHT;
  } else if (isWallCloseLeftAngle()) {
    position = WALL_LEFT_SIDE;
  } else if (isWallCloseRightAngle()){
    position = WALL_RIGHT_SIDE;
  } 
  else {
    position = NO_WALL;
  };
}


#define COLOUR_BUFFER_SIZE 5
RunningAverage* onBlueRA = new RunningAverage(COLOUR_BUFFER_SIZE);
RunningAverage* onBlackRA = new RunningAverage(COLOUR_BUFFER_SIZE);
RunningAverage* onGreenRA = new RunningAverage(COLOUR_BUFFER_SIZE);



//======================FUNCTIONS======================
boolean onGreen(void) { return (onGreenRA->getAverage() == COLOUR_BUFFER_SIZE); }
boolean onBlue(void) { return (onBlueRA->getAverage() == COLOUR_BUFFER_SIZE); }
boolean onBlack(void) { return (onBlackRA->getAverage() == COLOUR_BUFFER_SIZE); }




//=============FACEING A WALL========================

//A very curde way of checkign if the robot is facing a wall
#define WEIGHT_BUFFER_SIZE 15

RunningAverage* leftWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);
RunningAverage* rightWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);
RunningAverage* midWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);

//==========Wall booleans========================
boolean wallAtLeft = false;
boolean wallAtRight = false;
boolean wallAtMid = false;
boolean wallAtLeftAngle = false;
boolean wallAtRightAngle = false;
boolean wallCloseLeft = false;
boolean wallCloseRight = false;
boolean wallCloseMid = false;
boolean wallCloseLeftAngle = false;
boolean wallCloseRightAngle = false;
//===============================================

//=================EDGE DETECT VARIABLES====================
long incEdgeDetectLeftTime = 0;
long incEdgeDetectRightTime = 0;
long decEdgeDetectLeftTime = 0;
long decEdgeDetectRightTime = 0;
boolean doubleEdgeAtLeft = false;
boolean doubleEdgeAtRight = false;
//================================================

//=========================================

void setupSensorLogic(){
  leftWeight->clear();
  rightWeight->clear();
  midWeight->clear();
}

void updateSensorLogic(){
  
  int greenError = abs(getRedVal() - ON_GREEN_R) + abs(getGreenVal() - ON_GREEN_G) + abs(getBlueVal() - ON_GREEN_B) + abs(getClearVal() - ON_GREEN_C);
  int blackError = abs(getRedVal() - ON_BLACK_R) + abs(getGreenVal() - ON_BLACK_G) + abs(getBlueVal() - ON_BLACK_B) + abs(getClearVal() - ON_BLACK_C);
  int blueError = abs(getRedVal() - ON_GREEN_R) + abs(getGreenVal() - ON_BLUE_G) + abs(getBlueVal() - ON_BLUE_B) + abs(getClearVal() - ON_BLUE_C);
  if (greenError < blackError){
    if (greenError < blueError) {
      onGreenRA->addValue(COLOUR_BUFFER_SIZE);
      onBlackRA->addValue(0);
      onBlueRA->addValue(0);
    }
    else {
      onGreenRA->addValue(0);
      onBlackRA->addValue(0);
      onBlueRA->addValue(COLOUR_BUFFER_SIZE);
    }
  } else {
    if (blackError < blueError) {
      onGreenRA->addValue(0);
      onBlackRA->addValue(COLOUR_BUFFER_SIZE);
      onBlueRA->addValue(0);
    }
    else {
      onGreenRA->addValue(0);
      onBlackRA->addValue(0);
      onBlueRA->addValue(COLOUR_BUFFER_SIZE);
    }
  }

  //======================LEFT SIDE WEIGHT DETECT====================
  boolean lweight = false;
  
  if (getIR2() > 250){
    digitalWrite(29, LOW);
    int percentDiff = (getIR2()-getIR1())*100/getIR2();
    //Serial.println(percentDiff);
    if (percentDiff > 25) {
      lweight = true;
    }
  }
  else if (getIR2() > 150){
    digitalWrite(29, LOW);
    int percentDiff = (getIR2()-getIR1())*100/getIR2();
    //Serial.println(percentDiff);
    if (percentDiff > 15) {
      lweight = true;
    }
  }
  else if (getIR2() > 100){
    digitalWrite(29, HIGH);
    
    int percentDiff = (getIR2()-getIR1())*100/getIR2();
    //Serial.println(percentDiff);
    if (percentDiff > 40) {
      lweight = true;
    }
  } else {
    digitalWrite(29, LOW);
  }
  if (lweight){
    leftWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    leftWeight->addValue(0);
  }
  //=================================================
  
  //=========RIGHT SIDE WEIGHT DETECT=================================
  boolean rweight = false;
  if (getIR4() > 100){
    int percentDiff = (getIR4()-getIR5())*100/getIR4();
    if (percentDiff > 20) {
      rweight = true;
    }
  }
  if (rweight){
    rightWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    rightWeight->addValue(0);
  }
  //================END RIGHT SIDE WEIGHT DETECT=========

  //=============MID WEIGHT DETECT========================================
  boolean mweight = false;
  if (getUsLow() == -1 && getUsHigh() > 100){
    mweight = true;
  }
  else if (getUsLow() < getUsHigh()){
    mweight = true;
  }
  if (mweight){
    midWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    midWeight->addValue(0);
  }
  //===================END MID WEIGHT DETECT=============
  
  //============WALL DETECT================================================
  wallAtLeft = (getIR1() > 320);
  wallAtRight = (getIR5() > 330);
  wallAtMid = (getUsHigh() < 60);
  wallAtLeftAngle = (getIR3() > 410);
  wallAtRightAngle = (getIR6() > 420);
  wallCloseLeft = (getIR1() > 240);
  wallCloseRight = (getIR5() > 250);
  wallCloseMid = (getUsHigh() < 200);
  wallCloseLeftAngle = (getIR3() > 360);
  wallCloseRightAngle = (getIR6() > 380);
  //============END WALL DETECT==============================
  
  //=============EDGE DETECT=================================================
  doubleEdgeAtRight = false;
  if (getIR6Raw() > 150 && getIR6Raw() > getIR6() + 50){
    incEdgeDetectRightTime = millis();
    //Serial.println("edge detect right inc");
  }
  if (getIR6Raw() > 150 && getIR6Raw() < getIR6() - 50){
    doubleEdgeAtRight = (millis() - incEdgeDetectRightTime < 500);
  }
  doubleEdgeAtLeft = false;
  if (getIR3Raw() > 150 && getIR3Raw() > getIR3() + 50){
    incEdgeDetectLeftTime = millis();
  }
  if (getIR3Raw() > 150 && getIR3Raw() < getIR3() - 50){
    doubleEdgeAtLeft = (millis() - incEdgeDetectLeftTime < 500);
  }
  //============EDGE DETECT END============================================== 
  updatePositionState();
}

//=====================================================Wall close function=========================================
boolean isWallAtRight(){ return wallAtRight; }

boolean isWallAtLeft(){ return wallAtLeft; }

boolean isWallAtMid(){ return wallAtMid; }

boolean isWallAtLeftAngle(){ return wallAtLeftAngle; }

boolean isWallAtRightAngle(){ return wallAtRightAngle; }

boolean isWallCloseRight(){ return wallCloseRight; }

boolean isWallCloseLeft(){ return wallCloseLeft; }

boolean isWallCloseMid(){ return wallCloseMid; }

boolean isWallCloseLeftAngle(){ return wallCloseLeftAngle; }

boolean isWallCloseRightAngle(){ return wallCloseRightAngle; }

boolean isWallInFront(){
  return (wallAtRight || wallAtLeft || wallAtMid || wallAtLeftAngle || wallAtRightAngle);
}

boolean isWallClose(){
  return (wallCloseRight || wallCloseLeft || wallCloseMid || wallCloseLeftAngle || wallCloseRightAngle);
}
//==============================================================================================================

//==================EDGE DETECT FUNCTIONS============================================
boolean isDoubleEdgeAtRight() { return doubleEdgeAtRight; }

boolean isDoubleEdgeAtLeft() { return doubleEdgeAtLeft; }
//==================EDGE DETECT FUNCTION END=======================================


//=============================WEIGHT DETECT FUNCTIONS==============================
boolean weightAtLeft(void){
  if (leftWeight->getAverage() >= 12){
    return true;
  } else {
    return false;
  }
}

boolean weightAtRight(void){
  if (rightWeight->getAverage() >= 12){
    return true;
  } else {
    return false;
  }
}

boolean weightAtMid(void){
  if (midWeight->getAverage() >= 12){
    return true;
  } else {
    return false;
  }
}
//============================WEIGHT DETECT FUNCTIONS END===============================
