
const int eL = 100;
const int eR = 120;
const int eM = 150;

const int weightDetectLoopIterations = 20;
const int REQUIRED_WEIGHT_DUTY_CYCLE = 19; // needs to be less than the above variable

int weightDetect(void){
  
  int leftCount = 0;
  int rightCount = 0;
  int midCount = 0;
  int i = 0;
  
  boolean weightLeft = false;
  boolean weightRight = false;
  boolean weightMid = false;
  
  while(i++ < weightDetectLoopIterations) {
    //delay(500);
    updateSensors();
    
    /*false
    if (serial) {
      delay(500);
      output = "===";
      output += "\n1: " + String(getIR1Raw());
      output += "\n2: " + String(getIR2Raw());
      output += "\n\n4: " + String(getIR4Raw());
      output += "\n5: " + String(getIR5Raw());
      output += "\n\n3: " + String(getIR3Raw());
      output += "\n6: " + String(getIR6Raw());
    }
    */
    
    //int errorL = (abs(getIR1Raw()-getIR2Raw())*100)/max(getIR1Raw(), getIR2Raw());
    int errorLMM = (abs(getIR1()-getIR2()));
    //boolean weightL = errorL > eL;
    boolean weightLMM = (errorLMM > eL) && (getIR2() != -1);
    
    //int errorR = (abs(getIR4Raw()-getIR5Raw())*100)/max(getIR4Raw(), getIR5Raw());
    int errorRMM = (abs(getIR4()-getIR5()));
    //boolean weightR = errorR > eR;
    boolean weightRMM = errorRMM > eR && (getIR4() != -1);
    
    //int errorM = (abs(getIR3Raw()-getIR6Raw())*100)/max(getIR3Raw(), getIR6Raw());
    int errorMMM = (abs(getIR3()-getIR6()));
    //boolean weightM = errorM > eM;
    boolean weightMMM = errorMMM > eM && (getIR6() != -1);
    //output += "\n%DifM: " + String(errorM);
    
    if (weightLMM) {
      leftCount++; 
    }
  if (weightRMM) {
      rightCount++;
    }
    if (weightMMM) {
      midCount++;
    } 
    
    Serial.print("\ni = ");
    Serial.println(i);
    Serial.print("leftcount = ");
    Serial.println(leftCount);
    Serial.print("rightcount = ");
    Serial.println(rightCount);
    Serial.print("midcount = ");
    Serial.println(midCount);
        
  }
  if (leftCount >= REQUIRED_WEIGHT_DUTY_CYCLE) {
    weightLeft = true;
  }
  if (rightCount >= REQUIRED_WEIGHT_DUTY_CYCLE) {
    weightRight = true;
  }

  if (midCount >= REQUIRED_WEIGHT_DUTY_CYCLE) {
    weightMid = true;
  }
  
  Serial.println(weightLeft);
  Serial.println(weightRight);
  Serial.println(weightMid);
  if (!weightLeft && !weightRight && !weightMid) { Serial.println("Yup I'm going to kick your cunt"); return 0; } 
  
  if (weightLeft ^ weightRight ^ weightMid) {
    if (weightLeft) { return 1; }
    if (weightRight) { return 2; }
    if (weightMid) { return 3; }
  } else
  
  if (~weightLeft) {
    if (rightCount > midCount) { return 2; }
    else { return 3; }
  } else
  
  if (~weightRight) {
    if (leftCount > midCount) { return 1; }
    else { return 3; }
  } else 
  
  if (~weightMid) {
    if (rightCount > leftCount) { return 2; }
    else { return 1; }
  } else {
    
    errorFunction("We thought we found a weight, but it's not in any of them. So Lui's code is fucked at the bottom of weightDetect");
    return -1;
  }
}
