
int goal = 140;

void testServo(void) {
  if (serial) Serial.println("Soon we will be...");
  
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  setupServo();
  if (serial) Serial.println("Testing Servos!");
  
  
  //Herkulex.torqueOFF(rail);
  
  Herkulex.setLed(rail, LED_GREEN);
  Herkulex.setLed(tray, LED_PINK);
  Herkulex.setLed(box, LED_BLUE);
  Herkulex.setLed(lift, LED_WHITE);
  Herkulex.moveOneAngle(tray, goal, 2000, LED_GREEN);
  //Herkulex.moveOneAngle(tray, goal, 2000, LED_GREEN);
  delay(2100);
  while (true) {
    
    updateAll();
    //updatePickUp();

    //moveServoOnTouch();
    delayedPrinting();
    iterateAngleOnTouch();
    
    //if (isConduction()) { shake(400); }
    //digitalWrite(led1, isConduction());
    digitalWrite(led2, isLimit());
    //digitalWrite(led3, isLeftFront() || isRightFront());
    
  }
}

//raiseLiftServo
//moveRailServo(
//moveTrayServo
//move
void moveServoOnTouch(void) {
  
  if (isLimit()) {
      stopRailServo();
      digitalWrite(led1, HIGH);
      railGoingUp = false;
      railGoingDown = true;
      delay(50);
  }
  
  digitalWrite(led1, LOW);
  if (isLeftFront()) { 
    raiseLiftServo(2854);
  } else
  if (isRightFront()) {
    Herkulex.moveOneAngle(lift, loweredAngleLift, 1500, LED_GREEN);
  } else if (isConduction()) {
    moveRailServo(200);
        
    //if (isDigIR()) { seenWeight = true; }
    
  }

}

void delayedPrinting(void) {
  Serial.print("\nTray is at: ");
  Serial.println(goal);
  Serial.println(getUsHigh());
  Serial.println(getUsLow());
  //Serial.print("Lift is at: ");
  //Serial.println(goal);
  delay(200);
}

void iterateAngleOnTouch(void) {
  if (isLeftFront()) { goal -= 10; } else
  if (isRightFront()) { goal += 10; }
  Herkulex.moveOneAngle(tray, goal, 200, LED_GREEN);
  //Herkulex.moveOneAngle(lift, 30, 2000, LED_GREEN);
  
}
  
