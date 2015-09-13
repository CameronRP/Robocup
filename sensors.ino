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
const int ir1Pin = 0;
const int ir2Pin = 0;
const int ir3Pin = 0;
const int ir4Pin = 0;
const int ir5Pin = 0;
const int irTopPin = 0;

//US snesors Pins
const int usLeftTrigPin = 31;
const int usLeftEchoPin = 43;
const int usRightTrigPin = 30;
const int usRightEchoPin = 42;

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
int ir1 = 0;
int getIR1(void) { return ir1; }
int ir2 = 0;
int getIR2(void) { return ir2; }
int ir3 = 0;
int getIR3(void) { return ir3; }
int ir4 = 0;
int getIR4(void) { return ir4; }
int ir5 = 0;
int getIR5(void) { return ir5; }
int irTop = 0;
int getIRTop(void) { return irTop;}

//US Sensors
int usLeft = 0;
int getUSLeft(void) { return usLeft; }
int usRight = 0;
int getUSRight(void) { return usRight; }


//=====================CALCULATION VARIABLES=====================
long usLeftDuration = 0;
long usRightDuration = 0;

//========================SETUP SENSORS INPUTS===================================
void setupSensors(void) {
  Serial.println(usRight);
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

  //US sensors
  pinMode(usLeftTrigPin, OUTPUT);
  pinMode(usLeftEchoPin, INPUT);
  pinMode(usRightTrigPin, OUTPUT);
  pinMode(usRightEchoPin, INPUT);
  digitalWrite(usLeftTrigPin, LOW);
  digitalWrite(usRightTrigPin, LOW);
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
  ir1 = digitalRead(ir1Pin);
  ir2 = digitalRead(ir2Pin);
  ir3 = digitalRead(ir3Pin);
  ir4 = digitalRead(ir4Pin);
  ir5 = digitalRead(ir5Pin);
  irTop = digitalRead(irTopPin);
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
