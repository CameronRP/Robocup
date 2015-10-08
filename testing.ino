//Code to run for testing.

#include <avr/io.h>
#include <avr/interrupt.h>

int lowEchoPin = 20;
int highEchoPin = 21;  //21
int trigPin = 25;

int usOutput = 21;

volatile long pulseStartTime = 0;
volatile boolean lowRec = false;
volatile boolean highRec = false;

//int usPulsePin = 21;

#define US_BUFFER_SIZE 5

RunningAverage* usLowRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usLowRAError = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRAError = new RunningAverage(US_BUFFER_SIZE);

void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  //testUsSensors();
  pinMode(usOutput, OUTPUT);
  testUsPulse();
  
  while(true){
    digitalWrite(usOutput, HIGH);
    delayMicroseconds(8);
    digitalWrite(usOutput, LOW);
    delayMicroseconds(8);
  }
}

/*
void setupUltrasonicPulse(){
  pinMode(usPulsePin, OUTPUT);
  digitalWrite(usPulsePin, LOW);
}
*/


void testUsPulse(){
  while(true){
    updateUS();
    //Serial.println("Start pulse");
    Serial.println(getUsRight());
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
    delay(100);
  }
}


void pulse(){
  digitalWrite(usOutput, HIGH);
  delayMicroseconds(7);
  digitalWrite(usOutput, LOW);
  delayMicroseconds(7);
}


void testUsSensors(){
  while(true){
    updateUS();
    Serial.println(getUsRight());
  }
}

void testInterrupt(){
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
  
  while(true){
    updateUsSensors();
    if (lowRec && highRec) {
      Serial.println("======");
      if (validUsHigh()){
        Serial.println("H: " + String(usHighRA->getAverage()));
      } else {
        Serial.println("H: no val.");
      }
      if (validUsLow()){
        Serial.println("L: " + String(usLowRA->getAverage()));
      } else {
        Serial.println("L: no val.");
      }
    }
  }
}

boolean validUsHigh(){
  return (usHighRAError->getAverage() <= 3); 
}

boolean validUsLow(){
  return (usLowRAError->getAverage() <= 3); 
}

void updateUsSensors(){
  /*
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
  */
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

void testTurnToWeight(){
  while(true){
    updateUS();
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
