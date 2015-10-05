//Code to run for testing.


void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  testTopUsSensor();
}

void testingOutputUsSensorsInMM(){
  String output = "";
  int tick = 0;
  unsigned long echoTime = millis();
  while (true){
    updateUS();
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "Update time: " + String(millis() - echoTime);
    output += "\nRight sensor: " + String(getUsRight());
    output += "\nLeft  sensor: " + String(getUsLeft());
    output += "==========";
    Serial.println(output);
    echoTime = millis();
  }
}

void testOneIr(void){
  String output = "";
  int tick = 0;
  while (true){
    delay(500);
    updateSensors();
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nIR1: " + String(getIR1());
    output += "\n==========";
    Serial.println(output);
  }
}

void testingOutputIrSensors(void){
  String output = "";
  int tick = 0;
  while (true){
    delay(500);
    updateSensors();
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nIR1: " + String(getIR1());
    output += "\nIR2: " + String(getIR2());
    output += "\nIR3: " + String(getIR3());
    output += "\nIR4: " + String(getIR4());
    output += "\nIR5: " + String(getIR5());
    output += "\n==========";
    Serial.println(output);
  }
}

void testingOutputIrSensorsRaw(void){
  String output = "";
  int tick = 0;
  while (true){
    delay(500);
    updateSensors();
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nIR1: " + String(getIR1Raw());
    output += "\nIR2: " + String(getIR2Raw());
    output += "\nIR3: " + String(getIR3Raw());
    output += "\nIR4: " + String(getIR4Raw());
    output += "\nIR5: " + String(getIR5Raw());
    output += "\n==========";
    Serial.println(output);
  }
}

void testingSwitches(void){
  String output = "";
  int tick = 0;
  while (true){
    delay(2);
    updateSensors();
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nLimit: " + String(isLimit());
    output += "\nConduction: " + String(isConduction());
    output += "\nLeft Front: " + String(isLeftFront());
    output += "\nRight Front: " + String(isRightFront());
    output += "\nFloor: " + String(isFloor());
    output += "\n==========";
    Serial.println(output);
  }
}

//Tests the warning function in Robocup.ino
void testWarningLED(){
  errorFunction("Testing error function.");
}

void testingWallFacing(void){
  String output = "";
  int tick = 0;
  int error = 10;
  int p2p = 0;
  int p3p = 0;
  int p4p = 0;
  while (true){
    delay(2);
    updateSensors();
    tick++;
    p2p = getIR1() + (getIR5()-getIR1())/4;
    p3p = getIR1() + (getIR5()-getIR1())/2;
    p4p = getIR1() + (getIR5()-getIR1())*3/4;
    
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nPoint2 error: " + String(p2p-getIR2());
    output += "\nPoint3 error: " + String(p2p-getIR3());
    output += "\nPoint4 error: " + String(p2p-getIR4());
    
    output += "\nPoint2 boolean: " + String(!((p2p-error) < getIR2() && getIR2() < (p2p+error)));
    output += "\nPoint3 boolean: " + String(!((p3p-error) < getIR3() && getIR3() < (p3p+error)));
    output += "\nPoint4 boolean: " + String(!((p4p-error) < getIR4() && getIR4() < (p4p+error)));
    
    output += "\n==========";
    Serial.println(output);
  }
}

void testingAngleToWall(void){
  String output = "";
  int tick = 0;
  int diff = 0;
  while(true){
    delay(2);
    updateSensors();
    tick++;
    output = "==========";
    output = "\nIR1: " + String(getIR1());
    output = "\nIR5: " + String(getIR5());
    diff = getIR5()-getIR1();
    output = "\nDiff: " + String(diff);
    output = "\nAngle: " + String(atan2(diff, 160));
    Serial.println(output);
  }
}

void testTopUsSensor(void) {
  String output = "";
  int tick = 0;
  while(true){
    delay(200);
    updateSensors();
    tick++;
    output = "==========";
    output += "\nTop us raw value: " + String(getIRTop());
    output += "\nMid ir: " + String(getIR3Raw());
    Serial.println(output);
    
  }
}

//Function default layout
/*
void function(){
  String output = "";
  int tick = 0;
  while (true){
    delay(2);
    tick++;
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\n==========";
    Serial.println(output);
  }
}
*/
