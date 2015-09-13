//Code to run for testing.


void testingCode(){
  Serial.println("Testing starting.");
  testingOutputUsSensorsInMM();
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
    output += "\nRight sensor: " + String(getUSRight());
    output += "\nLeft  sensor: " + String(getUSLeft());
    output += "==========";
    Serial.println(output);
    echoTime = millis();
  }
}

void testingOutputIrSensors(){
  String output = "";
  int tick = 0;
  while (true){
    delay(2);
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

void testingOutputIrSensorsRaw(){
  String output = "";
  int tick = 0;
  while (true){
    delay(2);
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

void testingSwitches(){
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
