//This does the sensors stuff

//=====================#Defines=====================
#define US_TIMEOUT 100000


//====================Pins====================
//Switches Pins
const int limitSwitchPin = A5;
const int conductionSwitchPin = A2;
const int leftFrontSwitchPin = A3;
const int rightFrontSwitchPin = A4;
const int floorSwitchPin = A6;

//IR sensors Pins
const int ir1Pin = A2;
const int ir2Pin = A3;
const int ir3Pin = A1;
const int ir4Pin = A10;
const int ir5Pin = A11;
const int irTopPin = A0;

//US snesors Pins
const int usLeftTrigPin = 31;
const int usLeftEchoPin = 43;
const int usRightTrigPin = 30;
const int usRightEchoPin = 42;
//const int usTopPin = A1;

//====================Output Variables...... AND FUNCTIONS=====================
//The variables that other modues will use
//Switches
boolean limitSwitch = false;
boolean isLimit(void) { return limitSwitch; }
boolean conductionSwitch = false;
boolean isConduction(void) { return conductionSwitch; }
boolean leftFrontSwitch = false;
boolean isLeftFront(void) { return leftFrontSwitch; }
boolean rightFrontSwitch = false;
boolean isRightFront(void) { return rightFrontSwitch; }
boolean floorSwitch = false;
boolean isFloor(void) { return floorSwitch; }

//IR Sensors
int ir1ReadingSize = 8;
int ir1RawVals[8] = {494, 348, 261, 206, 170, 120, 95, 40};  //Raw ir values at each distance.
int ir1DisVals[8] = {100, 150, 200, 250, 300, 400, 500, 800};  //Distance in mm

int ir2ReadingSize = 8;
int ir2RawVals[8] = {491, 352, 273, 226, 189, 130, 105, 40};  //Raw ir values at each distance.
int ir2DisVals[8] = {100, 150, 200, 250, 300, 400, 500, 800};  //Distance in mm

int ir3ReadingSize = 8;
int ir3RawVals[8] = {502, 351, 265, 220, 184, 131, 109, 45};  //Raw ir values at each distance.
int ir3DisVals[8] = {100, 150, 200, 250, 300, 400, 500, 800};  //Distance in mm

int ir4ReadingSize = 8;
int ir4RawVals[8] = {518, 363, 281, 235, 203, 190, 130, 50};  //Raw ir values at each distance.
int ir4DisVals[8] = {100, 150, 200, 250, 300, 400, 500, 800};  //Distance in mm

int ir5ReadingSize = 8;
int ir5RawVals[8] = {499, 356, 268, 214, 175, 128, 105, 70};  //Raw ir values at each distance.
int ir5DisVals[8] = {100, 150, 200, 250, 300, 400, 500, 800};  //Distance in mm


int ir1 = 0;
int getIR1Raw(void) { return ir1; }
int getIR1(void) { return calculateIrInMM(ir1, ir1ReadingSize, ir1RawVals, ir1DisVals); }
int ir2 = 0;
int getIR2Raw(void) { return ir2; }
int getIR2(void) { return calculateIrInMM(ir2, ir2ReadingSize, ir2RawVals, ir2DisVals); }
int ir3 = 0;
int getIR3Raw(void) { return ir3; }
int getIR3(void) { return calculateIrInMM(ir3, ir3ReadingSize, ir3RawVals, ir3DisVals); }
int ir4 = 0;
int getIR4Raw(void) { return ir4; }
int getIR4(void) { return calculateIrInMM(ir4, ir4ReadingSize, ir4RawVals, ir4DisVals); }
int ir5 = 0;
int getIR5Raw(void) { return ir5; }
int getIR5(void) { return calculateIrInMM(ir5, ir5ReadingSize, ir5RawVals, ir5DisVals); }

int irTop = 0;
int getIRTop(void) { return irTop; }

//US Sensors
int usLeft = 0;
int getUsLeft(void) { return usLeft; }
int usRight = 0;
int getUsRight(void) { return usRight; }

//=====================CALCULATION VARIABLES=====================
long usLeftDuration = 0;
long usRightDuration = 0;



//========================SETUP SENSORS INPUTS===================================
void setupSensors(void) {
  Serial.println("Setting up sensors");
  //Switches
  pinMode(limitSwitchPin, INPUT);
  pinMode(conductionSwitchPin, INPUT);
  pinMode(leftFrontSwitchPin, INPUT);
  pinMode(rightFrontSwitchPin, INPUT);
  pinMode(floorSwitchPin, INPUT);

  //IR sensors
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  pinMode(ir5Pin, INPUT);
  pinMode(irTopPin, INPUT);
  //pinMode(irTopPin, INPUT);

  //US sensors
  pinMode(usLeftTrigPin, OUTPUT);
  pinMode(usLeftEchoPin, INPUT);
  pinMode(usRightTrigPin, OUTPUT);
  pinMode(usRightEchoPin, INPUT);
  digitalWrite(usLeftTrigPin, LOW);
  digitalWrite(usRightTrigPin, LOW);
//  pinMode(usTopPin, INPUT);
}

//====================UPDATE SENSORS====================
//Updates the IR and Switches, not the US sensors
//No buffer used at the moment
void updateSensors(void) {
  //Updating Switches
  limitSwitch = digitalRead(limitSwitchPin);
  conductionSwitch = digitalRead(conductionSwitchPin);
  leftFrontSwitch = digitalRead(leftFrontSwitchPin);
  rightFrontSwitch = digitalRead(rightFrontSwitchPin);
  floorSwitch = digitalRead(floorSwitchPin);

  //Updating IR Sensors
  ir1 = analogRead(ir1Pin);
  ir2 = analogRead(ir2Pin);
  ir3 = analogRead(ir3Pin);
  ir4 = analogRead(ir4Pin);
  ir5 = analogRead(ir5Pin);
  irTop = analogRead(irTopPin);
  //irTop = digitalRead(irTopPin);
  
  //usTop = analogRead(usTopPin);
}

//====================UPDATE US SENSORS====================
//Updates the US sensors as they are not updates in the main update
void updateUS(void) {
  usLeftDuration = getUSDuration(usLeftEchoPin, usLeftTrigPin);
  delay(20);
  usRightDuration = getUSDuration(usRightEchoPin, usRightTrigPin);

  usLeft = msToMM(usLeftDuration);
  usRight = msToMM(usRightDuration);
}

//====================Functions to read US values====================

//Converts the value from a ultrasonic sensor sensor to mm, -1 is returnd if the value is too far away to read.
int msToMM(long microseconds)
{
  return microseconds / 3 / 2;
}

long getUSDuration(int echoPin, int trigPin) {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
//  ulong
  return pulseIn(echoPin, HIGH, US_TIMEOUT);
}

//Used the recorded distances to estimate the distance from the raw IR reading.
//For each IR sensor there is a RawArray, DisArray and readingsSize.
//When the IR sensor is calibrated each reading has a raw (analog value from ir sensor) value and distance value (distance from front of robot in mm).
//These points are stored in RawArray and DisArray with the same index.
//To calculate the distance the inputed IR value is used and the two IR readings that it is between are used to make a y = mx + c function.
//The inputed IR value is then used in this function to aproximate the distance.
//If the ir value is not found to be between and two data points the function returns -1;
int calculateIrInMM(int rawVal, int readingsSize, int* rawArray, int* disArray){
  //y = m(x - x1) + y1
  for (int i = 0; i < readingsSize-1; i++){
    
    if(rawArray[i] >= rawVal && rawVal >= rawArray[i+1]){
      int runn = rawArray[i+1] - rawArray[i];
      int rise = disArray[i+1] - disArray[i];
      double m = ((double) rise)/((double) runn);
      return (int) (m*((double) (rawVal - rawArray[i])) + disArray[i]);
    }
  }
  return -1;
}

// Returns the US reading
// Error -1 is 3 values, 1st greater than 700,
/*
long getUSDistance(int echoPin, int trigPin) {
  int a, b, c = 0;
  a = microsecondsToCentimeters(getUSDuration(echoPin, trigPin));

  if (a < 700) {
    return a;
  }

  b = microsecondsToCentimeters(getUSDuration(echoPin, trigPin));
  c = microsecondsToCentimeters(getUSDuration(echoPin, trigPin));

  if ((b + c) > 2000) {
    if (b <= 10) {
      return b;
    }
    else if (c <= 10) {
      return c;
    }
    return -1;
  }
  return b;
}
*/
