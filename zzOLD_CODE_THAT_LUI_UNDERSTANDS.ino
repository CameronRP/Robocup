//
//const int left_motor = 11;
//const int right_motor = 10;
//
//Servo leftMotorServo;
//Servo rightMotorServo;
//
//void setupDCMotor(void) {
//  
//  leftMotorServo.attach(left_motor);
//  rightMotorServo.attach(right_motor);
//  setBothMotor(0);
//     
//}
//
//// Sets the speed of the left motor. +ve forward, -ve backward. Range is from -60 to 50.
//// If value outside this range given, then it will default to nearest limit.
//void setLeftMotor(int value) {
//  if (serial) {
//    Serial.print("Setting L motor to value of: ");
//    Serial.println(value);
//  }
//  if (value > 50) {
//    value = 50;
//  }
//  else if (value < -60) {
//    value = -60;
//  }
//  leftMotorServo.write(value + 90);
//}
//
//// Sets the speed of the right motor. +ve forward, -ve backward. Range is from -60 to 50.
//// If value outside this range given, then it will default to nearest limit.
//void setRightMotor(int value) {
//  if (serial) {
//    Serial.print("Setting R motor to value of: ");
//    Serial.println(value);
//  }
//  if (value > 50) {
//    value = 50;
//  }
//  else if (value < -60) {
//    value = -60;
//  }
//  rightMotorServo.write(value + 90);
//}
//
//void setBothMotor(int value) {
//
//  setRightMotor(value);
//  setLeftMotor(value);
//}
//
//void turnLeftSharp(void) {
//  
//  leftMotor(-40, 300);
//  rightMotor(40, 300);
//}
//
//void turnRightSharp(void) {
//  setRightMotor(-40);
//  setLeftMotor(40);
//}
//
//void getOutOfHere(void) {
//  
//  int number = (int) millis();
//  int dur = (number % 200) + 800;
//  digitalWrite(led3, HIGH);
//  
//  if ((number % 2) == 1) {
//    digitalWrite(led1, HIGH);
//    
//    rightMotor(-60, dur);
//    leftMotor(20, dur);
//    delay((number % 200) + 800);
//    digitalWrite(led1, LOW);
//  } else {
//    digitalWrite(led2, HIGH);
//    setRightMotor(20);
//    setLeftMotor(-60);
//    delay((number % 200) + 800);
//    digitalWrite(led2, LOW);
//  }
//  
//  setBothMotor(40);
//  digitalWrite(led3, LOW);
//}
//    
//  
