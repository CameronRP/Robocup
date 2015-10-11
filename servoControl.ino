// THIS SHOULD BE SOMEWHERE ELSE BUT IT ISN'T AT THE MOMENT

int numberOfWeights = 0;

void incrementWeightCount(void) {
  numberOfWeights++;
}

void resetWeightCount(void) {
  numberOfWeights = 0;
}




#include <Herkulex.h>
//#include <SoftwareSerial.h>
int switchPressed = 0;

//=========================== Servo identities =================
const int rail = 0xfc; // motor ID of the back - winder 
const int tray = 0xfb; 
const int box =  0xfa;
const int lift = 0xf9; //motor ID of the front - lifter

const int allServos = 0xfe;
//============================


//========================= Constant servo angles ==============
const int raisedAngleLift = 30;
const int slidingAngleLift = -50;
const int loweredAngleLift = -140;

const int raisedAngleTray = 20; // safe below the plug of the R long range IR is about 65
const int loweredAngleTray = 140;
//===========================


//========================= Declarations for Pickup =======================

enum PickingState {WAITING, DROPPING_FRONT, SWEEPING, RAISING_FRONT, WINDING_IN, RESETTING};
PickingState pickingState = WAITING;
const int lowerLiftServoTime = 500;
const int raiseLiftServoTime = 2854;
const int watchDogUpTime = 10000;
const int windOutTime = 4500; //5000 was a little bit too far
const int sweepTime = 100;
const int windInRailSpeed = 400;


long timeDone = 0;



boolean seenWeight = false;
boolean railGoingUp = false;
boolean isRailGoingDown(void);
boolean railGoingDown = false;
boolean isRailGoingDown(void) { return railGoingDown; }
boolean isRailMoving(void) { return (railGoingDown || railGoingUp); }
boolean waitingToPick = false;
boolean isWeightWaiting(void) { return waitingToPick; }


// ========================= Servo setup ==========================
void setupServo(void) {
  //pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  //digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  Herkulex.beginSerial2(115200); //open serial port 2 to talk to the motors
  Herkulex.reboot(lift);            //reboot motor
  Herkulex.reboot(rail);
  Herkulex.reboot(tray);
  Herkulex.reboot(box);
  //Herkulex.reboot(allServos);
  delay(500);
  Herkulex.initialize();
  delay(200);

}
// =====================================================================

// ================================= RAIL SERVO FUNCTIONS ===============

// Stops the rail servo. Returns 0 if the motor has a speed of 0, and 1 if it was not.
// Even if the servo has a speed of 0, it will still send a command to stop it.
int stopRailServo(void) {
   //if (serial) Serial.println("Nigger we stoppin'");
   if (Herkulex.getSpeed(rail) == 0){
     Herkulex.moveSpeedOne(rail, 0, 0, LED_GREEN);
     return 0;
   } else {
     Herkulex.moveSpeedOne(rail, 0, 0, LED_GREEN);
     return 1;
  }
}


// Moves rail
// 
void moveRailServo(int speedValue) {
  if (serial) {
    Serial.print("Moving rail servo with speed ");
    Serial.println(speedValue);
  }
  Herkulex.moveSpeedOne(rail, -speedValue, 0, LED_GREEN);
}


void sweepRailServo() {
  if (serial) {
    Serial.print("Sweeping rail servo");
  }
  Herkulex.moveSpeedOne(rail, -400, sweepTime, LED_GREEN);
}

void windRailOut(void) {
  
  Herkulex.moveSpeedOne(rail, 500, 0, LED_GREEN);
}

// ========================= END RAIL ==========================




// ========================= LIFT SERVO FUNCTIONS ================

// Stops the rail servo. Returns 0 if the motor has a speed of 0, and 1 if it was not.
// Even if the servo has a speed of 0, it will still send a command to stop it.
int stopLiftServo(void) {
   //if (serial) Serial.println("Nigger we stoppin'");
   if (Herkulex.getSpeed(lift) == 0){
     Herkulex.moveSpeedOne(lift, 0, 0, LED_GREEN);
     return 0;
   } else {
     Herkulex.moveSpeedOne(lift, 0, 0, LED_GREEN);
     return 1;
  }
}


// Moves rail
// 
void moveLiftServo(int speedValue) {
  if (serial) {
    Serial.print("Moving lift servo with speed ");
    Serial.println(speedValue);
  }
  Herkulex.moveSpeedOne(lift, speedValue, 0, LED_BLUE);
}




// Returns current angle of lift servo as an int
float getLiftAngle(void) {
  return Herkulex.getAngle(lift);
}


// Checks where the lift servo is. Returns 2 if the front is raised, and 1 if the front is lowered,
// and 0 if the it is between raised and lowered. Tolerance is +/- 5 deg
int checkFrontServoPos(void) {
  
  int pos = getLiftAngle();
  
  if (pos >= (raisedAngleLift - 5)) return 2;
  else if (pos <= (loweredAngleLift + 5)) return 1;
  else return 0;
} 

// raises the lift servo. Returns 0 if already at raised position, and 1 if it was not.
// Regardless of above will command servo to go to raised position over a time of 2.5s.
//
// NOTE: DELAY FOR SERVO TO MOVE NOT BUILT INTO THIS FUNCTION 
void raiseLiftServo(int thisOne) {
  Herkulex.moveOneAngle(lift, raisedAngleLift, thisOne, LED_GREEN);
}

void slidingLiftServo(int thisOne) {
  Herkulex.moveOneAngle(lift, slidingAngleLift, thisOne, LED_GREEN);
}

// Lowers the lift servo. Returns 0 if already at lowered position, and 1 if it was not.
// Regardless of above will command servo to go to lowered position over a time of 0.5s
//
// NOTE: DELAY FOR SERVO TO MOVE NOT BUILT INTO THIS FUNCTION 
void lowerLiftServo(int lowerTime) {
  if (serial) Serial.println("Lowering lift servo");
  Herkulex.moveOneAngle(lift, loweredAngleLift, lowerTime, LED_GREEN);
}

// ============================ END LIFT SERVO ==========================



// ============================== TRAY SERVO FUNCTIONS ===========

// Returns current angle of lift servo as an int
int getTrayAngle(void) {
  return Herkulex.getAngle(tray);
}

// Returns 1 if up, 2 if down, 0 if between
// Tolerance of 5 deg out
int checkTrayPos(void) {
  int pos = Herkulex.getAngle(tray);
  
  if (pos >= (raisedAngleTray - 5)) return 2;
  else if (pos <= (loweredAngleTray + 5)) return 1;
  else return 0;
} 


void raiseTray(void) {
  int pos = Herkulex.getAngle(tray);
  Herkulex.moveOneAngle(tray, raisedAngleTray, 2600, LED_GREEN);
}

void lowerTray(void) {
  Herkulex.moveOneAngle(tray, loweredAngleTray, 1000, LED_BLUE);
}


int stopTrayServo(void) {
   //if (serial) Serial.println("Nigger we stoppin'");
   if (Herkulex.getSpeed(tray) == 0){
     Herkulex.moveSpeedOne(tray, 0, 0, LED_GREEN);
     return 0;
   } else {
     Herkulex.moveSpeedOne(tray, 0, 0, LED_GREEN);
     return 1;
  }
}


// Moves rail
// 
void moveTrayServo(int speedValue) {
  if (serial) {
    Serial.print("Moving tray servo with speed ");
    Serial.println(speedValue);
  }
  Herkulex.moveSpeedOne(tray, speedValue, 0, LED_BLUE);
}




boolean weightReadyToPick(void) {
  return (isConduction() && (numberOfWeights < 3));
}




void updatePickUp(void) {
  long time = millis();
  
  if (weightReadyToPick() && !(pickingState == WAITING)) { waitingToPick = true; }
  
  switch (pickingState) {
    case WAITING:
    //stopLiftServo();
    stopRailServo();
    
    if (weightReadyToPick()) {
      if (serial) Serial.println("There's food here");
      if (isRailMoving()) {
        if (serial) Serial.println("But we can't pick it up right now");
        waitingToPick = true;
      } else {
        if (serial) Serial.println("And we're going to pick it up!");
        if (waitingToPick) { waitingToPick = false; }
        pickingState = DROPPING_FRONT;
        lowerLiftServo(lowerLiftServoTime);
        timeDone = time + lowerLiftServoTime;
      }
    }
    if (serial) Serial.println("No food ready to pick");
    break;
    
    case DROPPING_FRONT:
      if (time > timeDone) {
        pickingState = SWEEPING;
        sweepRailServo();
        timeDone = time + sweepTime;
        
      }
      break;
    
    case SWEEPING:
      if (time > timeDone) {
        stopRailServo();
        pickingState = RAISING_FRONT;
        raiseLiftServo(raiseLiftServoTime);
        timeDone = time + raiseLiftServoTime;
        railGoingUp = true;
      }
      break;
    
    case RAISING_FRONT:
      if (time > timeDone) {
        pickingState = WINDING_IN;
        timeDone = time + watchDogUpTime;
        if (serial) Serial.println("Setting timeout to: " + String(timeDone));
      }
      break;
    
    case WINDING_IN:  
      moveRailServo(windInRailSpeed);
      if (serial) Serial.println("Winding in, time is " + String(time));
      if (isDigIR()) { seenWeight = true; }
      
      
      if (isLimit()) {
        stopRailServo();
        railGoingUp = false;
        railGoingDown = true;
        delay(5);
        pickingState = RESETTING;
        lowerLiftServo(500);
        windRailOut();
        timeDone = time + windOutTime;
      }
      else if (time > timeDone) {
        stopRailServo();
        stopLiftServo();
        stopTrayServo();
        setLeftMotor(0);
        setRightMotor(0);
        errorFunction("Timed out going up to the limit switch");
      }
      break;
      
      case RESETTING:
        
        if (seenWeight) { incrementWeightCount(); }
        
        if (time > timeDone) {
          stopRailServo();
          shake(200);
          raiseLiftServo(200);
          railGoingDown = false;
          pickingState = WAITING;
        }
        break;
        
      default:
        errorFunction("The pickingState is invalid");
        break;
  }
}





void dumpWeights(void) {
  raiseTray();
  shake(3000);
  lowerTray();
  shake(1500);
  resetWeightCount();
}
