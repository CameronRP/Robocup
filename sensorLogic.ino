//This will look at the sensors and do some logic stuff

//=================VARIABLES=========================



//======================FUNCTIONS======================




//=============FACEING A WALL========================

//A very curde way of checkign if the robot is facing a wall
#define WEIGHT_BUFFER_SIZE 5

RunningAverage* leftWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);
RunningAverage* rightWeight = new RunningAverage(WEIGHT_BUFFER_SIZE);

void initSensorLogic(){
  leftWeight->clear();
}

void updateSensorLogic(){
  //Serial.println("===");
  //Serial.println("L: " + String(getIR2()));
  //Serial.println("H: " + String(getIR1()));
  //Serial.println("Diff: " + String(getIR2()-getIR1()));
  //Serial.println("Range: " + String(getIR2()));
  //Serial.println(getIR2());
  
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
      if (percentDiff > 10) {
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
      if (percentDiff > 10) {
        rweight = true;
      }
    }
  }
  if (rweight){
    rightWeight->addValue(WEIGHT_BUFFER_SIZE);
  } else {
    rightWeight->addValue(0);
  }
}

boolean weightAtLeft(void){
  if (leftWeight->getAverage() >= 3){
    return true;
  } else {
    return false;
  }
}

boolean weightAtRight(void){
  if (rightWeight->getAverage() >= 3){
    return true;
  } else {
    return false;
  }
}
