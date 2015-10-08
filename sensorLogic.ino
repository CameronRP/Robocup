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
  
  //=========Right side weight detect
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

  //=============Mid weight detect
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
}




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
