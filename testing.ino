//Code to run for testing.


boolean commenting = 0;


void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  //testingOutputUsSensorsInMM();
//  testWallFollow();
  
  testWeightDetect();
}


void testingOutputUsSensorsInMM(){
  String output = "";
  int tick = 0;
  unsigned long echoTime = millis();
  while (true){
    delay(500);
    updateUS();
    tick++;
    output = "===";
    output += "\nR: " + String(getUsRight());
    output += "\nL: " + String(getUsLeft());
    Serial.println(output);
    echoTime = millis();
    
  }
}

void testWallFollow(){
  String output = "";
  int tick = 0;
  unsigned long echoTime = millis();
  while (true){
    //delay(500);
    updateUS();
    setupSimpleOutputs();  
    
    if(getUsLeft() < 320 && getUsRight() < 320 && getUsLeft() > 20 && getUsRight() > 20) {
      getOutOfHere();
      updateUS();
    } else 
    
    if (getUsLeft() < 250 && (getUsLeft() > 50)) {
      digitalWrite(led1, HIGH);
      turnLeftSharp();
      digitalWrite(led1, LOW);
    } else if (getUsLeft() < 600) {
      setLeftMotor((getUsLeft() - 250) / 5);
    } else if (getUsLeft() > 600 || getUsRight() < 50){
      setLeftMotor(50);
    } else
    
    if (getUsRight() < 300 && getUsRight() > 50) {
      digitalWrite(led2, HIGH);
      turnRightSharp();
      updateUS();
      digitalWrite(led2, LOW);
    } else if (getUsRight() < 600) {
      setRightMotor((getUsRight() - 250) / 5);
    }  else if (getUsRight() > 600 || getUsRight() < 50){
      setRightMotor(50);
    }
    
  }
}

void testWeightDetect(void){
  
  String output = "";
  int eL = 100;
  int eR = 120;
  int eM = 150;
  int leftCount = 0;
  int rightCount = 0;
  int midCount = 0;
  const int REQUIRED_WEIGHT_DUTY_CYCLE = 10; // 1 - 10
  
  while(true){
    //delay(500);
    updateSensors();
    if (commenting) {
      delay(500);
      output = "===";
      output += "\n1: " + String(getIR1Raw());
      output += "\n2: " + String(getIR2Raw());
      output += "\n\n4: " + String(getIR4Raw());
      output += "\n5: " + String(getIR5Raw());
      output += "\n\n3: " + String(getIR3Raw());
      output += "\n6: " + String(getIR6Raw());
    }
    
    
    //int errorL = (abs(getIR1Raw()-getIR2Raw())*100)/max(getIR1Raw(), getIR2Raw());
    int errorLMM = (abs(getIR1()-getIR2()));
    //boolean weightL = errorL > eL;
    boolean weightLMM = (errorLMM > eL) && (getIR2() != -1);
    //output += "\n%DifL: " + String(errorL);
    if (commenting) {
      output += "\n%DifLmm: " + String(errorLMM);
      output += "\nWeightL: " + String((boolean) weightLMM);
    }
    
    //int errorR = (abs(getIR4Raw()-getIR5Raw())*100)/max(getIR4Raw(), getIR5Raw());
    int errorRMM = (abs(getIR4()-getIR5()));
    //boolean weightR = errorR > eR;
    boolean weightRMM = errorRMM > eR && (getIR4() != -1);
    //output += "\n%DifR: " + String(errorR);
    if (commenting) {
      output += "\n%DifRmm: " + String(errorRMM);
      output += "\nWeightR: " + String((boolean) weightRMM);
    }
    
    //int errorM = (abs(getIR3Raw()-getIR6Raw())*100)/max(getIR3Raw(), getIR6Raw());
    int errorMMM = (abs(getIR3()-getIR6()));
    //boolean weightM = errorM > eM;
    boolean weightMMM = errorMMM > eM && (getIR6() != -1);
    //output += "\n%DifM: " + String(errorM);
    if (commenting) {
      output += "\n%DifMmm: " + String(errorMMM);
      output += "\nWeightM: " + String((boolean) weightMMM);
      Serial.println(output);
    }
    
    if (weightLMM) {
      leftCount++;
      
    } else {
      leftCount--;
    }
  
  if (weightRMM) {
      rightCount++;
    } else {
      rightCount--;
    }
    
    
    if (weightMMM) {
      midCount++;
    } else {
      midCount--;
    }
    
    if (leftCount >= REQUIRED_WEIGHT_DUTY_CYCLE * 2) {
      digitalWrite(26, HIGH);
      
    } else {
      digitalWrite(26, LOW);
    }
  
  if (rightCount >= REQUIRED_WEIGHT_DUTY_CYCLE * 2) {
      digitalWrite(27, HIGH);
    } else {
      digitalWrite(27, LOW);
    }
    
    
    if (midCount >= REQUIRED_WEIGHT_DUTY_CYCLE * 2) {
      digitalWrite(28, HIGH);
    } else {
      digitalWrite(28, LOW);
    }
    
    if (leftCount > 20) {
      leftCount = 20;
    } else  if (leftCount < 0) {
      leftCount = 0;
    }
  
  if (rightCount > 20) {
      rightCount = 20;
    } else if (rightCount < 0) {
      rightCount = 0;
    }
    
    
    if (midCount > 20) {
      midCount = 20;
    } else if (midCount < 0) {
      midCount  =0;
    }
    
  }
}

void testLeftSideWeightDetect(void){
  String output = "";
  int error = 80;
  while(true){
    delay(500);
    updateSensors();
    output = "===";
    int diff = getIR1Raw() - getIR2Raw();
    output += "\nD: " + String(diff);
    output += "\nW: " + String((abs(diff) > error)); 
    Serial.println(output);
  }
}

void testRightSideWeightDetect(void){
  String output = "";
  int error = 80;
  while(true){
    delay(200);
    updateSensors();
    output = "===";
    int diff = getIR4Raw() - getIR5Raw();
    output += "\nD: " + String(diff);
    output += "\nW: " + String((abs(diff) > error)); 
    Serial.println(output);
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
    output += "\nIR2: " + String(getIR2());
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
    output += "\n6: " + String(getIR6());
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
    output += "\n6: " + String(getIR6Raw());
    output += "\n==========";
    Serial.println(output);
  }
}

void testingSwitches(void){
  String output = "";
  int tick = 0;
  while (true){
    delay(500);
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
  int error = 60;
  int p2p = 0;
  int p3p = 0;
  int p4p = 0;
  while (true){
    delay(500);
    updateSensors();
    tick++;
    p2p = getIR1() + (getIR5()-getIR1())/4;
    p3p = getIR1() + (getIR5()-getIR1())/2;
    p4p = getIR1() + (getIR5()-getIR1())*3/4;
    
    output = "==========";
    output += "\nTick: " + String(tick);
    output += "\nP2 e: " + String(p2p-getIR2());
    output += "\nP3 e: " + String(p3p-getIR3());
    output += "\nP4 e: " + String(p4p-getIR4());
    
    boolean p2 = ((p2p-error) < getIR2() && getIR2() < (p2p+error));
    boolean p3 = ((p3p-error) < getIR3() && getIR3() < (p3p+error));
    boolean p4 = ((p4p-error) < getIR4() && getIR4() < (p4p+error));
    
    output += "\nP2 b: " + String(p2);
    output += "\nP3 b: " + String(p3);
    output += "\nP4 b: " + String(p4);
    output += "\nResult:" + String(p2 && p3 && p4);
    
    Serial.println(output);
  }
}


boolean testingFlatWallFacingBoolean(void){
  int error = 25;
  int p2p = getIR1() + (getIR5()-getIR1())/4;
  int  p3p = getIR1() + (getIR5()-getIR1())/2;
  int  p4p = getIR1() + (getIR5()-getIR1())*3/4;
  if (!((p2p-error) < getIR2() && getIR2() < (p2p+error))){
    return false;
  }
  if (!((p3p-error) < getIR3() && getIR3() < (p3p+error))) {
    return false;
  }
  if (!((p4p-error) < getIR4() && getIR4() < (p4p+error))) {
    return false;
  }
  return true;
}

void testingDistanceFromWall(void){
  String output = "";
  int tick = 0;
  int total = 0;
  while(true){
    delay(200);
    updateSensors();
    tick++;
    total = getIR1()+getIR2()+getIR3()+getIR4()+getIR5();
    output = "===";
    output += "\nDis: " + String(total/5);
    Serial.println(output);
  }
}

void testingAngleToWall(void){
  String output = "";
  int tick = 0;
  int diff = 0;
  while(true){
    delay(200);
    updateSensors();
    tick++;
    if (getIR2() < 300 && getIR4() < 300){
      //Wall is close so use short range ir sensors
      diff = getIR4()-getIR2();
      output = "===";
      output += "\nD: " + String(diff);
      output += "\nS: " + String(atan2(diff, 110));
    } else {
      diff = getIR5()-getIR1();
      output = "===";
      output += "\nD: " + String(diff);
      output += "\nL: " + String(atan2(diff, 230));
    }
    Serial.println(output);
  }
}

void testingAngleToWallUsingLongIr(void) {
  String output = "";
  int tick = 0;
  int diff = 0;
  while(true){
    delay(200);
    updateSensors();
    diff = getIR5()-getIR1();
    output = "===";
    output += "\nD: " + String(diff);
    output += "\nA: " + String(atan2(diff, 230));
    Serial.println(output);
  }
}

void testingAngleToWallUsingShortIr(void) {
  String output = "";
  int tick = 0;
  int diff = 0;
  while(true){
    delay(200);
    updateSensors();
    diff = getIR4()-getIR2();
    output = "===";
    output += "\nD: " + String(diff);
    output += "\nA: " + String(atan2(diff, 110));
    Serial.println(output);
  }
}

void test6UsSensor(void) {
  String output = "";
  int tick = 0;
  while(true){
    delay(200);
    updateSensors();
    tick++;
    output = "==========";
    output += "\n6 us raw value: " + String(getIR6());
    output += "\nMid ir: " + String(getIR3Raw());
    Serial.println(output);
    
  }
}

void testingWeightdetectionOnAllIr(){
  String output = "";
  int tick = 0;
  while(true){
    delay(200);
    updateSensors();
    tick++;
    output = "===";
    if (testingFlatWallFacingBoolean()){
      output += "\nfacing flat wall.";
    } else {
      
    }
    
    
    
    
    
    
    
    
    output = "===";
    output += "\n";
  }
}


void testingWeightdetect(void){
  String output = "";
  int tick = 0;
  int error = 150;
  int p2p = 0;
  int p3p = 0;
  int p4p = 0;
  while (true){
    delay(200);
    updateSensors();
    tick++;
    p2p = getIR1() + (getIR5()-getIR1())/4;
    p3p = getIR1() + (getIR5()-getIR1())/2;
    p4p = getIR1() + (getIR5()-getIR1())*3/4;
    
    int p2e = p2p-getIR2();
    int p3e = p3p-getIR3();
    int p4e = p4p-getIR4();
    
    output = "===";
    output += "\n2: " + String(p2e > error);
    output += "\n3: " + String(p3e > error);
    output += "\n4: " + String(p4e > error);
    
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
