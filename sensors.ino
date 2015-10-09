//This does the sensors stuff

//=====================#Defines=====================
#define US_TIMEOUT 4000  //This works to about 900mm x 3, becuase lui timesed it by 3. fuck you lui

#include <avr/io.h>
#include <avr/interrupt.h>

int lowEchoPin = 0;  //20
int highEchoPin = 0;  //21
int trigPin = 0;  //25



volatile long pulseStartTime = 0;
volatile boolean lowRec = false;
volatile boolean highRec = false;

//int usPulsePin = 21;

#define US_BUFFER_SIZE 5

RunningAverage* usLowRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usLowRAError = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRAError = new RunningAverage(US_BUFFER_SIZE);


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
uint16_t clear, red, green, blue;
int getClearVal() { return (int) clear; }
int getRedVal() { return (int) red; }
int getGreenVal() { return (int) green; }
int getBlueVal() { return (int) blue; }



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

int ir1 = 0;
RunningAverage* ir1RA = new RunningAverage(5);
int getIR1Raw(void) { return ir1; }
int getIR1(void) { 
 // if (abs(ir1 - ir1RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
 // } else {
    return ir1RA->getAverage(); 
  //}
}

int ir2 = 0;
RunningAverage* ir2RA = new RunningAverage(5);
int getIR2Raw(void) { return ir2; }
int getIR2(void) { 
  //if (abs(ir2 - ir2RA->getAverage()) > SHORT_IR_ERROR_FROM_AVG) {
  //  return -1;
  //} else {
    return ir2RA->getAverage(); 
  //}
}

int ir3 = 0;
RunningAverage* ir3RA = new RunningAverage(5);
int getIR3Raw(void) { return ir3; }
int getIR3(void) { return ir3RA->getAverage(); }

int ir4 = 0;
RunningAverage* ir4RA = new RunningAverage(5);
int getIR4Raw(void) { return ir4; }
int getIR4(void) { return ir4RA->getAverage(); }

int ir5 = 0;
RunningAverage* ir5RA = new RunningAverage(5);
int getIR5Raw(void) { return ir5; }
int getIR5(void) { return ir5RA->getAverage(); }

int ir6 = 0;
RunningAverage* ir6RA = new RunningAverage(5);
int getIR6Raw(void) { return ir6; }
int getIR6(void) { return ir6RA->getAverage(); }

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
int colourSensorCounter = 0;
void setupSensors(void) {
  Serial.println("Setting up sensors");

  if (tcs.begin()){
    Serial.println("Colour sensor found.");  
  } else {
    Serial.println("Colour sensor not found");
    while(1);  
  }
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
  
  colourSensorCounter++;
  if (colourSensorCounter >= 1){
    colourSensorCounter = 20;
    tcs.setInterrupt(false);      // turn on LED

    delay(60);  // takes 50ms to read 
    
    tcs.getRawData(&red, &green, &blue, &clear);
  
    tcs.setInterrupt(true);  // turn off LED
  }
  
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
  
  
  if (lowRec && highRec || micros() > pulseStartTime + 18000) {
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

//====================Functions to read US values====================

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
