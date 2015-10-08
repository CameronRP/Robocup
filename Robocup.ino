//This is the arduino code for the Robocup 2015 competition by Lui and Cameron (team 13).

#include <math.h>
#include <Servo.h>


//=================TESTING AND SERIAL CODE ON===================================
//To test code, set testing to true. This will run the testingCode() function in testing.ino. Go to testing.ino for more info.
//You will probably also want serial set to true.
//===================================================================
boolean testing = true;
boolean serial = true;


int weightLocation = 0;



//
enum State {SEARCHING, DETECTED, JUST_LOST_SIGHT_OF_FOOD, PICK_UP_FOOD};
State state = SEARCHING;  //state to start in.


void setup(){
  
  setupSensors();
  setupSimpleOutputs();
  setupDCMotor();
  setupSensorLogic();
  
  if (serial) { Serial.begin(9600); }
  if (testing) { testingCode(); }
  
  bothMotors(0, -1);
}

void loop(){
  updateSensors();
  pulseUsSensors();
  updateMotors();
  updateLights();
  //midWeightDetect();
  updateSensorLogic();
  switch (state){
    case SEARCHING:
      bothMotors(0, -1);
      //wallFollow();
      if (serial) {
        Serial.print("\nWeight Location = ");
        Serial.println(weightLocation);
      }
      
      break;
    case DETECTED:
      detected();
      break;

    case JUST_LOST_SIGHT_OF_FOOD:
      justLostSightOfFood();
      break;

    case PICK_UP_FOOD:
      pickUpFood();
      break;
    default:  //This state should not be entered.
      errorFunction("Went into default state.");
      break;
  }

  if (serial){
    Serial.println("some info to print");
    Serial.println("Some other info to print");
  }
}

void errorFunction(String message){
  Serial.println(message);
  Serial.println("Just some text in here in case the message isn't coming through properly");
  if (!serial) {Serial.begin(9600); }
  while (true) {
    Serial.println(message);
    setLedWarning(true);
    delay(300);
    setLedWarning(false);
    delay(300);
  }
}

//Initial Hindering
//Drives to opponents HQ and places box,
void initialHindering(void){

}




void wallFollow(){
  
 
  updateUS();   
  if(getUsLeft() < 320 && getUsRight() < 320 && getUsLeft() > 20 && getUsRight() > 20) {
    getOutOfHere();
  } else 
  
  if (getUsLeft() < 250 && (getUsLeft() > 20)) {
    turnLeftSharp();
  } else if ((getUsLeft() < 600) && millis() > getLeftMotorStopTime() && getUsLeft() > 20) {
    setLeftMotor((getUsLeft() - 250) / 5);
  } else if ((getUsLeft() > 500 || getUsLeft() < 20) && millis() > getLeftMotorStopTime()){
    leftMotor(50, -1);
  } else
  
  if (getUsRight() < 250 && getUsRight() > 20) {
    turnRightSharp();
  } else if ((getUsRight() < 500) && millis() > getRightMotorStopTime() && getUsRight() > 20) {
    setRightMotor((getUsRight() - 250) / 5);
  }  else if ((getUsRight() > 500 || getUsRight() < 20) && millis() > getRightMotorStopTime()){
    rightMotor(40, -1);
  }
}

/*
//====================================================================================
//Wall following
void wallFollowing(void){
  tick++;
  //updateIRVals();
  //updateAllSwitch();
  //updateLimitSwitch();
  //updateUSValues();

  if (floorSwitch == 1) {
    on(LED2);
  } else {
    off(LED2);
  }

  // put your main code here, to run repeatedly:
  if (conductionSwitch == 1) {
    on(LED1);
    delay(100);
    if (conductionSwitch == 1) {
      huntTheFood();
      delightfullyMoist();
    }
  }

  if (leftSwitch) {
    setBothMotor(0);
    setRightMotor(40);
    delay(500);
    huntTheFood();
  }

  if (rightSwitch) {
    setBothMotor(0);
    setLeftMotor(40);
    delay(500);
    huntTheFood();
  }

  rightUSmm = rightUSVal();
  leftUSmm = leftUSVal();

  Serial.print("right val: ");
  Serial.println(rightUSmm);
  Serial.print("left val: ");
  Serial.println(leftUSmm);


  if ((rightUSmm <= 450) && (leftUSmm <= 450)) {
    updateAllSwitch();

    for(i = 0; (i < 200) &&((leftSwitch == 0) && (rightSwitch == 0)); i++) {
      delay(10);
      updateAllSwitch();
    }

    if (i <= 495) {
      if (leftSwitch) {
        setBothMotor(0);
        setRightMotor(40);
        delay(700);
        huntTheFood();
      } else{

      if (rightSwitch) {
          setBothMotor(0);
          setLeftMotor(40);
          delay(700);
          huntTheFood();
        }
      }
    } else {



    if (rightUSmm < leftUSmm) {
      turnRightSome();
    } else {
      turnLeftSome();
      }
    }
  }else { if (rightUSmm < 650) {
    setRightMotor((rightUSmm / 10 - 20));
  } else {
    setRightMotor(50);
  }

  if (leftUSmm < 650) {
    setLeftMotor((leftUSmm / 10 - 20));
  } else {
    setLeftMotor(50);
  }
  }

  if (foodSeenIR()) {
    seenWeightCount++;
    if (seenWeightCount >= 8){
      huntTheFood();
      seenWeightCount = 0;
    }
  } else seenWeightCount -= 2;

}
*/
//======================================================================================
//Move somewhere less searched
void findNewSearchSpot(void){

}

//=======================================================================================
//Food is detected
void detected(void){
  bothMotors(0, -1);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
  digitalWrite(25 + weightLocation, LOW);
}

//======================================================================================
//Just lost sight of food
void justLostSightOfFood(void){
  //TODO
}

//======================================================================================
//Picking up food
void pickUpFood(void){
  //TODO
}

void getOutOfHere(void) {
  
  int number = (int) millis();
  int dur = (number % 200) + 800;
  digitalWrite(28, HIGH);
  
  if ((number % 2) == 1) {
    digitalWrite(26, HIGH);
    
    setRightMotor(-50);
    setLeftMotor(20);
    delay(dur);
    digitalWrite(26, LOW);
  } else {
    digitalWrite(27, HIGH);
    setRightMotor(20);
    setLeftMotor(-50);
    delay(dur);
    digitalWrite(27, LOW);
  }
  
  setBothMotor(40);
  digitalWrite(28, LOW);
}
