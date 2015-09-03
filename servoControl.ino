/*
#include <Herkulex.h>
//#include <SoftwareSerial.h>

#define raisedAngle 70
#define loweredAngle -50


int lift = 0xfd; //motor ID of the front - lifter
int rail = 0xfc; // motor ID of the back - winder POSITIVE SPEED MOVES THE MAGNET UPWARDS TOWARDS THE BACK OF THE ROBOT
int allServos = 0xfe;

int switchPressed = 0;


void setupServo(void) {
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  Herkulex.beginSerial2(115200); //open serial port 2 to talk to the motors
  Herkulex.reboot(lift);            //reboot motor
  Herkulex.reboot(rail);
  delay(500);
  Herkulex.initialize();
  delay(200);

}

// Checks where the front servo is. Returns 2 if the front is raise, and 1 if the front is lowered,
// and 0 if the it is between raised and lowered. Tolerance is +/- 5 deg
int checkFrontServo(void) {
  
  int pos = Herkulex.getAngle(lift);
  
  if (pos >= (raisedAngle - 5)) return 2;
  else if (pos <= (loweredAngle + 5)) return 1;
  else return 0;
  
} 

// Stops the rail servo. Returns 0 if the motor has a speed of 0, and 1 if it was not.
// Even if the servo has a speed of 0, it will still send a command to stop it.
int stopRailServo(void) {
  Serial.println("Nigger we stoppin'");
 if (Herkulex.getSpeed(rail) == 0){
   Herkulex.moveSpeedOne(rail, 0, 0, LED_GREEN);
   return 0;
 } else {
   Herkulex.moveSpeedOne(rail, 0, 0, LED_GREEN);
   return 1;
 }
}


void moveRailServo(int speedValue) {
  Serial.print("Moving rail servo with speed ");
  Serial.println(speedValue);
  Herkulex.moveSpeedOne(rail, speedValue, 0, LED_BLUE);
}


// Instructs the rail to head upwards at mid speed (500) until the upper limit switch
// is pressed. At this point the servo will stop for half a second, and then move
// forward for 1.5 seconds at speed 600. The time limit is set in multiples of 100ms,
// for example timeLimit100ms = 50 will limit it to 5 seconds.
// After the time limit is reached the servo will stop where it is, unless it has
// already hit the switch, in which case it will finish this manauever, for which the
// maximum execution time is (theoretically) 2s.
//
// Returns 1 if completes without hitting time limit, else 0.
int returnRailServo (int timeLimit100ms) {
  int tick = 0;
  int i = 0;
  switchPressed = digitalRead(limitSwitch);
  Serial.print("Return rail limit fn start, limit switch is currently ");
  Serial.println(switchPressed);
  
  for (int i = 0; (i <= timeLimit100ms) && (switchPressed == 0); i++) {
    
    switchPressed = digitalRead(limitSwitch);
    
    if (switchPressed == 1) {
      Serial.println("Got the limit switch!");
      stopRailServo();
      delay(100);
      //Move down a bit.
      moveRailServo(-600);
      delay(600);
      stopRailServo();  
    } else {
      
      // Move upwards
      moveRailServo(500);
      Serial.println("Moving upwards ");
      delay(10);
    }
  }
  stopRailServo();
  
  if (i == timeLimit100ms) { 
    Serial.println("SHIT WE FUCKED SHIT UP AND TOOK TOO LONG TO GET HOME");
    Serial.println("SHIT WE FUCKED SHIT UP AND TOOK TOO LONG TO GET HOME");
    Serial.println("SHIT WE FUCKED SHIT UP AND TOOK TOO LONG TO GET HOME");
    Serial.println("SHIT WE FUCKED SHIT UP AND TOOK TOO LONG TO GET HOME");
    return 0;
  }
  else {
    return 1;
  }  
}


// raises the lift servo. Returns 0 if already at raised position, and 1 if it was not.
// Regardless of above will command servo to go to raised position over a time of 2.5s.
//
// NOTE: DELAY FOR SERVO TO MOVE NOT BUILT INTO THIS FUNCTION 
int raiseLiftServo(void) {
  int val = Herkulex.getAngle(lift);
  Serial.print("raise lift servo fn start, with gurrent position ");
  Serial.println(val);
 if (val == raisedAngle){
   Herkulex.moveOneAngle(lift, raisedAngle, 2500, LED_GREEN);
   return 0;
 } else {
   Herkulex.moveOneAngle(lift, raisedAngle, 2500, LED_GREEN);
   return 1;
 }
}


// raises the lift servo. Returns 0 if already at raised position, and 1 if it was not.
// Regardless of above will command servo to go to raised position over a time of 0.5s.
//
// NOTE: DELAY FOR SERVO TO MOVE NOT BUILT INTO THIS FUNCTION 
int raiseLiftServoFast(void) {
  int val = Herkulex.getAngle(lift);
  Serial.print("raise lift servo FAST FAST FAST fn start, with gurrent position ");
  Serial.println(val);
 if (val == raisedAngle){
   Herkulex.moveOneAngle(lift, raisedAngle, 500, LED_GREEN);
   return 0;
 } else {
   Herkulex.moveOneAngle(lift, raisedAngle, 500, LED_GREEN);
   return 1;
 }
}


// Lowers the lift servo. Returns 0 if already at lowered position, and 1 if it was not.
// Regardless of above will command servo to go to lowered position over a time of 0.5s
//
// NOTE: DELAY FOR SERVO TO MOVE NOT BUILT INTO THIS FUNCTION 
int lowerLiftServo(void) {
  Serial.print("lower lift servo fn start with angle currently: ");
  int val = Herkulex.getAngle(lift);
  Serial.println(val);
 if (Herkulex.getAngle(lift) == loweredAngle){
   Herkulex.moveOneAngle(lift, loweredAngle, 500, LED_GREEN);
   return 0;
 } else {
   Herkulex.moveOneAngle(lift, loweredAngle, 500, LED_GREEN);
   return 1;
 }
}




// Servo routine: Instructs the servo motors to move so initially the magnet rails drop, and then
// will put the magnet from the back all the way to the front-most position, at which point it is assumed
// that a weight has been picked up, and then will pull the magnet car back to the rear-most limit.
//
//TO DO: Make it take an input which will make it either go all the way or just part way, ie for lying or standing
// 
//
// IMPORTANT: THIS ASSUMES THAT THE MAGNET CARRIAGE HAS ALREADY BEEN RESET; IE THAT THE returnRailCarriage(10000) FUNCTION HAS BEEN RUN
void pickUpWeight(void) {
  
  stopRailServo();
  raiseLiftServoFast();
  Serial.println("PICKING UP A MOTHER TRUCKING WEIGHT YEAH BOI");
  delay(500);
  Serial.println("Magnet out");
  moveRailServo(-1023);
  delay(3700);
  Serial.println("Lowering");
  stopRailServo();
  lowerLiftServo();
  delay(500);
  Serial.println("Slow sweep upwards");
  moveRailServo(400);
  delay(1000);
  setBothMotor(0);
  Serial.println("Raising the frame");
  raiseLiftServo();
  delay(2500);
  Serial.println("Bringing it up fast");
  moveRailServo(1023);
  delay(800);
  Serial.println("Hunting for the switch");
  returnRailServo(1000);
  Serial.println("End of pickup method reached!");
  
  stopRailServo();
  raiseLiftServoFast();
}
*/


