//Code to run for testing.


/*
int lowEchoPin = 20;
int highEchoPin = 21;
int trigPin = 25;

int usOutput = 21;
/*
volatile long pulseStartTime = 0;
volatile boolean lowRec = false;
volatile boolean highRec = false;

//int usPulsePin = 21;

#define US_BUFFER_SIZE 5

RunningAverage* usLowRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usLowRAError = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRAError = new RunningAverage(US_BUFFER_SIZE);
*/
void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  
  
  
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
  while (true){
    updateMotors();
    updateSensors();
    updateSensorLogic();
    updateLights();
    Serial.println("====");
    Serial.println(getUsHigh());
    Serial.println(getUsLow());
    /*
    if (!turningToWeight()){
      Serial.println(".");
      if (weightAtLeft()){
        turnToWeightAtLeft();
      } else if (weightAtRight()){
        turnToWeightAtRight();
      }else if (weightAtMid()){
      Serial.println("===");
      leftMotor(40, -1);
      rightMotor(50, -1);
      }
    }
   */ 
  }
}



void testUsSensorsDetect(){
  while(true){
    //delay(500);
    updateUS();
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
