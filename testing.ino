//Code to run for testing.


#define commonAnode true

void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  updateLights();
  //=============================================================
  
  int tick = 0;
  while (true){
    updateAll();
    Serial.println(isWallClose());
  }
}

void testUsSensorsDetect(){
  while(true){
    //delay(500);
    boolean weight = false;
    if (getUsLow() == -1 && getUsHigh() > 100){
      weight = true;
    }
    else if (getUsLow() < getUsHigh()){
      weight = true;
    }
    //Serial.println("====");
    //Serial.println("L: " + String(getUsLow()));
    //Serial.println("H: " + String(getUsHigh()));
    //Serial.println(weight);
    digitalWrite(28, weight);
  }
}



void testTurnToWeight(){
  while(true){
    updateSensors();
    updateMotors();
    if (turningToWeight()){
      
    } 
    else if (weightAtLeft()){
      turnToWeightAtLeft();
    } 
    else if (weightAtRight()){
      turnToWeightAtRight();
    }
  }
}
