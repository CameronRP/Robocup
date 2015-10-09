//Code to run for testing.


byte gammatable[256];


//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#define commonAnode true



void testingCode(){
  Serial.println("Testing starting.");
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  updateLights();
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  
  
  while (true){
    updateAll();
    if (onBlue()) Serial.println("Blue");
    if (onBlack()) Serial.println("Black");
    if (onGreen()) Serial.println("Green");
    
  }
}

void testUsSensorsDetect(){
  while(true){
    //delay(500);
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
