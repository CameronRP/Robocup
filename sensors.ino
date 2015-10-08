//This does the sensors stuff

//=====================#Defines=====================
#define US_TIMEOUT 4000  //This works to about 900mm x 3, becuase lui timesed it by 3. fuck you lui

#include <avr/io.h>
#include <avr/interrupt.h>

int lowEchoPin = 20;
int highEchoPin = 21;
int trigPin = 25;



volatile long pulseStartTime = 0;
volatile boolean lowRec = false;
volatile boolean highRec = false;

//int usPulsePin = 21;

#define US_BUFFER_SIZE 5

RunningAverage* usLowRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usLowRAError = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRAError = new RunningAverage(US_BUFFER_SIZE);





//====================Pins====================
//Switches Pins
const int limitSwitchPin = A5;
const int conductionSwitchPin = A2;
const int leftFrontSwitchPin = A3;
const int rightFrontSwitchPin = A4;
const int floorSwitchPin = A6;

//IR sensors Pins
const int ir1Pin = A2;  //Left Upper
const int ir2Pin = A3;  //Left Lower
const int ir3Pin = A6;  //Middle Upper
const int ir4Pin = A11;  //Right Lower
const int ir5Pin = A10;  //Right Upper
const int ir6Pin = A7;  //Middle lower

//US snesors Pins
const int usHighTrigPin = 25;
const int usHighEchoPin = 21;
const int usLowTrigPin = 25;
const int usLowEchoPin = 20;

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
int ir1ReadingSize = 9;
int ir1RawVals[9] = {453, 326, 245, 205, 160, 125, 105, 85, 65};  //Raw ir values at each distance.
int ir1DisVals[9] = {100, 150, 200, 250, 300, 400, 500, 600, 800};  //Distance in mm

int ir2ReadingSize = 8;
int ir2RawVals[9] = {475, 340, 263, 230, 190, 139, 100, 80, 45};  //Raw ir values at each distance.
int ir2DisVals[9] = {100, 150, 200, 250, 300, 400, 500, 600, 800};  //Distance in mm

int ir3ReadingSize = 8;
int ir3RawVals[7] = {539, 500, 395, 306, 255, 190, 150};  //Raw ir values at each distance.
int ir3DisVals[7] = {250, 300, 400, 500, 600, 800, 1000};  //Distance in mm

int ir4ReadingSize = 8;
int ir4RawVals[9] = {467, 338, 279, 220, 195, 151, 110, 85, 35};  //Raw ir values at each distance.
int ir4DisVals[9] = {100, 150, 200, 250, 300, 400, 500, 600, 800};  //Distance in mm

int ir5ReadingSize = 9;
int ir5RawVals[9] = {448, 328, 238, 185, 155, 119, 99, 85, 60};  //Raw ir values at each distance.
int ir5DisVals[9] = {100, 150, 200, 250, 300, 400, 500, 600, 800};  //Distance in mm

int ir6ReadingSize = 8;
int ir6RawVals[8] = {500, 446, 390, 302, 245, 205, 160, 140};  //Raw ir values at each distance.
int ir6DisVals[8] = {200, 250, 300, 400, 500, 600, 800, 1000};  //Distance in mm


#define SHORT_IR_ERROR_FROM_AVG 40

int ir1 = 0;
RunningAverage* ir1RA = new RunningAverage(10);
int getIR1Raw(void) { return ir1; }
int getIR1(void) { 
 // if (abs(ir1 - ir1RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
 // } else {
    return ir1RA->getAverage(); 
  //}
}

int ir2 = 0;
RunningAverage* ir2RA = new RunningAverage(10);
int getIR2Raw(void) { return ir2; }
int getIR2(void) { 
  //if (abs(ir2 - ir2RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
  //} else {
    return ir2RA->getAverage(); 
  //}
}

int ir3 = 0;
RunningAverage* ir3RA = new RunningAverage(10);
int getIR3Raw(void) { return ir3; }
int getIR3(void) { 
  if (abs(ir3 - ir3RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
    return -1;
  } else {
    return ir3RA->getAverage(); 
  }
}

int ir4 = 0;
RunningAverage* ir4RA = new RunningAverage(10);
int getIR4Raw(void) { return ir4; }
int getIR4(void) { 
  //if (abs(ir4 - ir4RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
  //} else {
    return ir4RA->getAverage(); 
  //}
}

int ir5 = 0;
RunningAverage* ir5RA = new RunningAverage(10);
int getIR5Raw(void) { return ir5; }
int getIR5(void) { 
  //if (abs(ir5 - ir5RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
  //} else {
    return ir5RA->getAverage(); 
  //}
}

int ir6 = 0;
RunningAverage* ir6RA = new RunningAverage(10);
int getIR6Raw(void) { return ir6; }
int getIR6(void) { 
  if (abs(ir6 - ir6RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
    return -1;
  } else {
    return ir6RA->getAverage(); 
  }
}

//US Sensors
int usHigh = 0;
int getUsHigh(void) { 
  if (validUsHigh()){
    return usHighRA->getAverage();  
  } else {
    return -1;
  }
}
int usLow = 0;
int getUsLow(void) { 
  if (validUsLow()){
    return usLowRA->getAverage();  
  } else {
    return -1;
  }
}
//=====================CALCULATION VARIABLES=====================
long usLeftDuration = 0;
long usRightDuration = 0;



//========================SETUP SENSORS INPUTS===================================
void setupSensors(void) {
  Serial.println("Setting up sensors");
  
  pinMode(lowEchoPin, INPUT);
  pinMode(highEchoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(lowEchoPin), lowEcho, FALLING);
  attachInterrupt(digitalPinToInterrupt(highEchoPin), highEcho, FALLING);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  pulseStartTime = micros();
  
  
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
  pinMode(ir6Pin, INPUT);
  ir1RA->clear();
  ir2RA->clear();
  ir4RA->clear();
  ir5RA->clear();
  ir3RA->clear();
  ir6RA->clear();
  //pinMode(ir6Pin, INPUT);

  //US sensors
  /*
  pinMode(usHighTrigPin, OUTPUT);
  pinMode(usHighEchoPin, INPUT);
  pinMode(usLowTrigPin, OUTPUT);
  pinMode(usLowEchoPin, INPUT);
  digitalWrite(usHighTrigPin, LOW);
  digitalWrite(usLowTrigPin, LOW);
  */
//  pinMode(us6Pin, INPUT);
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
  ir1RA->addValue(ir1);
  

  ir2 = analogRead(ir2Pin);
  ir2RA->addValue(ir2);

  ir3 = analogRead(ir3Pin);
  ir3RA->addValue(ir3);

  ir4 = analogRead(ir4Pin);
  ir4RA->addValue(ir4);

  ir5 = analogRead(ir5Pin);
  ir5RA->addValue(ir5);
  
  ir6 = analogRead(ir6Pin);
  ir6RA->addValue(ir6);
  
  //ir6 = digitalRead(ir6Pin);
  
  //us6 = analogRead(us6Pin);
  
  
  if (lowRec && highRec) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    pulseStartTime = micros();
    lowRec = false;
    highRec = false;
  }
}

boolean validUsHigh(){
  return (usHighRAError->getAverage() <= 3); 
}

boolean validUsLow(){
  return (usLowRAError->getAverage() <= 3); 
}

void lowEcho(){
  if (!lowRec){
    long value = (micros() - pulseStartTime)/6 -80;
    if (value < 2000){
      usLowRA->addValue(value);
      usLowRAError->addValue(0);
    } else {
      usLowRAError->addValue(US_BUFFER_SIZE);
    }
    lowRec = true;
  } else {
    Serial.println("sencodn pulse lower");
  }
}

void highEcho(){
  if (!highRec){
    long value = (micros() - pulseStartTime)/6 -80;
    if (value < 2000){
      usHighRA->addValue(value);
      usHighRAError->addValue(0);
    } else {
      usHighRAError->addValue(US_BUFFER_SIZE);
    }
    highRec = true;
  } else {
    Serial.println("sencodn pulse higher");
  }
}

//====================UPDATE US SENSORS====================
//Updates the US sensors as they are not updates in the main update
void updateUS(void) {
  
  
  usHigh = msToMM(getUSDuration(usHighEchoPin, usHighTrigPin));
  delay(2);
  usLow = msToMM(getUSDuration(usLowEchoPin, usLowTrigPin));
}

//====================Functions to read US values====================

//Converts the value from a ultrasonic sensor sensor to mm, -1 is returnd if the value is too far away to read.
int msToMM(long microseconds)
{
  return microseconds / 6;
}

long getUSDuration(int echoPin, int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  /*
  pulse();
  pulse();
  pulse();
  pulse();
  pulse();
  pulse();
  pulse();
  pulse();
  */
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

/*
void pulseUsSensors(void) {
  digitalWrite(usLeftTrigPin, LOW);
  digitalWrite(usRightTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(usLeftTrigPin, HIGH);
  digitalWrite(usRightTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(usLeftTrigPin, LOW);
  digitalWrite(usRightTrigPin, LOW);
}
*/

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
