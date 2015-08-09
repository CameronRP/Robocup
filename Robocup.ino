//This is the arduino code for the Robocup 2015 competition by Lui and Cameron (team 13).
//Set testing to false to run main code, otherwise set it to true and set testCode to one of the options in the enum.T

enum State {SEARCHING, DETECTED, JUST_LOST_SIGHT_OF_FOOD, PICK_UP_FOOD};
State state = SEARCHING;

boolean testing = true;
int testingType = 0;
boolean serialOut = false;


void setup(){
  if (testing){
    testingCode(testingType);
  }
  if (serialOut){
    //Serial.begin(something here);
  }
}

void loop(){
  //updateSensors();
  updateMotors();
  switch (state){
    case SEARCHING:
      searching();
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
      while(true){
        if (serialOut){
          Serial.print("In default state. state = : ");
          Serial.println(state);
        }
      }
      break;
  }  

  if (serialOut){
    Serial.println("some info to print");
    Serial.println("Some other info to print");
  }
}

void searching(void){
  //TODO
}

void detected(void){
  //TODO
}

void justLostSightOfFood(){
  //TODO
}

void pickUpFood(){
  //TODO
}
