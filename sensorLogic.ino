//This will look at the sensors and do some logic stuff

//=================VARIABLES=========================



//======================FUNCTIONS======================




//=============FACEING A WALL========================

//A very curde way of checkign if the robot is facing a wall
#define WEIGHT_BUFFER_SIZE 15

RunningAverage* leftWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);
RunningAverage* rightWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);
RunningAverage* midWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);

void setupSensorLogic(){
  leftWeight->clear();
  rightWeight->clear();
  midWeight->clear();
}

void updateSensorLogic(){
  
  boolean lweight = false;
  if (getIR2() != -1 && getIR2() > 50){
    if (getIR1() == -1){
      //Serial.println("Diff: " + String(getIR2() - getIR1()));
      if (getIR2() > (getIR1Raw()) + 15) {
        lweight = true;
      }
    } else {
      //Serial.println("Diff: " + String(getIR2() - getIR1Raw()));
      int percentDiff = (getIR2()-getIR1())*100/getIR2();
      if (percentDiff > 15) {
        lweight = true;
      }
    }
  }
  if (lweight){
    leftWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    leftWeight->addValue(0);
  }
  
  //=========Right side weight detect
  boolean rweight = false;
  if (getIR4() != -1 && getIR4() > 50){
    if (getIR5() == -1){
      //Serial.println("Diff: " + String(getIR2() - getIR1()));
      if (getIR4() > (getIR5Raw()) + 15) {
        rweight = true;
      }
    } else {
      //Serial.println("Diff: " + String(getIR2() - getIR1Raw()));
      int percentDiff = (getIR4()-getIR5())*100/getIR4();
      if (percentDiff > 15) {
        rweight = true;
      }
    }
  }
  if (rweight){
    rightWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    rightWeight->addValue(0);
  }
  
  boolean mweight = false;
  if (getIR6() != -1 && getIR6() > 100){
    if (getIR3() == -1){
      //Serial.println("Diff: " + String(getIR2() - getIR1()));
      if (getIR6() > (getIR6Raw()) + 15) {
        mweight = true;
      }
    } else {
      //Serial.println("Diff: " + String(getIR2() - getIR1Raw()));
      int percentDiff = (getIR6()-getIR4())*100/getIR6();
      if (percentDiff > 25) {
        mweight = true;
      }
    }
  }
  if (mweight){
    midWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    midWeight->addValue(0);
  }
}




boolean weightAtLeft(void){
  if (leftWeight->getAverage() >= 10){
    return true;
  } else {
    return false;
  }
}

boolean weightAtRight(void){
  if (rightWeight->getAverage() >= 10){
    return true;
  } else {
    return false;
  }
}

boolean weightAtMid(void){
  if (midWeight->getAverage() >= 10){
    return true;
  } else {
    return false;
  }
}
