//This will look at the sensors and do some logic stuff

//=================VARIABLES=========================



//======================FUNCTIONS======================




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
//=========================================================

void setupSensorLogic(){
  leftWeight->clear();
  rightWeight->clear();
  midWeight->clear();
}

void updateSensorLogic(){
  
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
