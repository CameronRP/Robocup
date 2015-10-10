//This does the sensors stuff

//=====================#Defines=====================
#define US_TIMEOUT 4000  //This works to about 900mm x 3, becuase lui timesed it by 3. fuck you lui

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C
 
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18
 
#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define    GYRO_BUFFER_SIZE  5

RunningAverage* gyroXRA = new RunningAverage(GYRO_BUFFER_SIZE);
RunningAverage* gyroYRA = new RunningAverage(GYRO_BUFFER_SIZE);
RunningAverage* gyroZRA = new RunningAverage(GYRO_BUFFER_SIZE);

const int lowEchoPin = 18;  //20
const int highEchoPin = 19;  //21
const int lowTrigPin = 17;  //25
const int highTrigPin = 16;



volatile long pulseStartTime = 0;
volatile boolean lowRec = false;
volatile boolean highRec = false;

//int usPulsePin = 21;

#define US_BUFFER_SIZE 5

RunningAverage* usLowRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usLowRAError = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRA = new RunningAverage(US_BUFFER_SIZE);
RunningAverage* usHighRAError = new RunningAverage(US_BUFFER_SIZE);


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
uint16_t clear, red, green, blue;
int getClearVal() { return (int) clear; }
int getRedVal() { return (int) red; }
int getGreenVal() { return (int) green; }
int getBlueVal() { return (int) blue; }



//====================Pins====================
//Switches Pins
const int limitSwitchPin = A5;
const int touchLPin = 36;
const int touchRPin = 35;
const int conductionSensorPin = 37;
// const int digitalIRWeightDetectPin = NEED A PIN FOR THIS BAD BOY!

//IR sensors Pins
const int ir1Pin = A2;  //Left Upper
const int ir2Pin = A3;  //Left Lower
const int ir3Pin = A6;  //Middle Upper
const int ir4Pin = A11;  //Right Lower
const int ir5Pin = A10;  //Right Upper
const int ir6Pin = A7;  //Middle lower

//US snesors Pins
const int usHighTrigPin = 25;
const int usHighEchoPin = 21;
const int usLowTrigPin = 25;
const int usLowEchoPin = 20;

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
boolean digitalIRWeightDetect = false;
boolean isWeightAttached(void) { return digitalIRWeightDetect; }

int ir1 = 0;
RunningAverage* ir1RA = new RunningAverage(5);
int getIR1Raw(void) { return ir1; }
int getIR1(void) { 
    return ir1RA->getAverage(); 
}

int ir2 = 0;
RunningAverage* ir2RA = new RunningAverage(5);
int getIR2Raw(void) { return ir2; }
int getIR2(void) { 
    return ir2RA->getAverage(); 
}

int ir3 = 0;
RunningAverage* ir3RA = new RunningAverage(5);
int getIR3Raw(void) { return ir3; }
int getIR3(void) { return ir3RA->getAverage(); }

int ir4 = 0;
RunningAverage* ir4RA = new RunningAverage(5);
int getIR4Raw(void) { return ir4; }
int getIR4(void) { return ir4RA->getAverage(); }

int ir5 = 0;
RunningAverage* ir5RA = new RunningAverage(5);
int getIR5Raw(void) { return ir5; }
int getIR5(void) { return ir5RA->getAverage(); }

int ir6 = 0;
RunningAverage* ir6RA = new RunningAverage(5);
int getIR6Raw(void) { return ir6; }
int getIR6(void) { return ir6RA->getAverage(); }

//US Sensors
int usHigh = 0;
int getUsHigh(void) { 
  if (validUsHigh()){
    return usHighRA->getAverage();  
  } else {
    return -1;
  }
}
int usLow = 0;
int getUsLow(void) { 
  if (validUsLow()){
    return usLowRA->getAverage();  
  } else {
    return -1;
  }
}
//=====================CALCULATION VARIABLES=====================
long usLeftDuration = 0;
long usRightDuration = 0;
//=======================================

//==========================COLOUR INTERRUPT=========================
volatile long r = 0;
void timerIsr()
{
  sei();
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  r +=((int) (Buf[12]<<8 | Buf[13]) -12);
}
//=============================================

//========================GET ANGLE==================
int getAngle(){
  return r*90/-148000;
}
//========================================

//========================SETUP SENSORS INPUTS===================================
int colourSensorCounter = 0;
void setupSensors(void) {
  setupIMU();
  Timer1.initialize(10000);
  Timer1.attachInterrupt( timerIsr );
  Serial.println("Setting up sensors");
  if (tcs.begin()){
    Serial.println("Colour sensor found.");  
  } else {
    Serial.println("Colour sensor not found");
    while(1);  
  }
  
  pinMode(lowEchoPin, INPUT);
  pinMode(highEchoPin, INPUT);
  pinMode(lowTrigPin, OUTPUT);
  pinMode(highTrigPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(lowEchoPin), lowEcho, FALLING);
  attachInterrupt(digitalPinToInterrupt(highEchoPin), highEcho, FALLING);
  
  digitalWrite(lowTrigPin, LOW);
  digitalWrite(highTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(lowTrigPin, HIGH);
  digitalWrite(highTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(lowTrigPin, LOW);
  digitalWrite(highTrigPin, LOW);
  pulseStartTime = micros();
  
  
  //Switches
  pinMode(limitSwitchPin, INPUT); // NOT DONE YET!!
  pinMode(touchLPin, INPUT);
  pinMode(touchRPin, INPUT);
  pinMode(conductionSensorPin, INPUT);
//  pinMode(digitaIRWeightDetectPin, INPUT); // NEITHER IS THIS ONE!!!


  //IR sensors
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  pinMode(ir5Pin, INPUT);
  pinMode(ir6Pin, INPUT);
  ir1RA->clear();
  ir2RA->clear();
  ir4RA->clear();
  ir5RA->clear();
  ir3RA->clear();
  ir6RA->clear();
}

//====================UPDATE SENSORS====================
int imuUpdateCounter = 20;

void updateSensors(void) {

  //============================COLOUR SENSOR=========================
  colourSensorCounter++;
  if (colourSensorCounter >= 50){
    colourSensorCounter = 0;
    tcs.setInterrupt(false);      // turn on LED
    delay(60);  // takes 50ms to read 
    tcs.getRawData(&red, &green, &blue, &clear);
    tcs.setInterrupt(true);  // turn off LED
  }
  //==========================================================
  
  //======================================Updating Switches============================
  limitSwitch = digitalRead(limitSwitchPin);
  conductionSwitch = digitalRead(conductionSensorPin);
  leftFrontSwitch = digitalRead(touchLPin);
  rightFrontSwitch = digitalRead(touchRPin);
  //digitalIRWeightDetect = digitalRead(digitalIRWeightDetectPin);
  //=======================================================

  //=====================Updating IR Sensors==================
  ir1 = analogRead(ir1Pin);
  ir1RA->addValue(ir1);
  ir2 = analogRead(ir2Pin);
  ir2RA->addValue(ir2);
  ir3 = analogRead(ir3Pin);
  ir3RA->addValue(ir3);
  ir4 = analogRead(ir4Pin);
  ir4RA->addValue(ir4);
  ir5 = analogRead(ir5Pin);
  ir5RA->addValue(ir5);
  ir6 = analogRead(ir6Pin);
  ir6RA->addValue(ir6);
  //=======================================
  
  //==========================PULSE US SENSORS=============================
  if (lowRec && highRec || micros() > pulseStartTime + 18000) {
    digitalWrite(lowTrigPin, LOW);
    digitalWrite(highTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(lowTrigPin, HIGH);
    digitalWrite(highTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(lowTrigPin, LOW);
    digitalWrite(highTrigPin, LOW);
    pulseStartTime = micros();
    lowRec = false;
    highRec = false;
  } 
  //=================================================
}

boolean validUsHigh(){
  return (usHighRAError->getAverage() <= 3); 
}

boolean validUsLow(){
  return (usLowRAError->getAverage() <= 3); 
}

void lowEcho(){
  if (!lowRec){
    long value = (micros() - pulseStartTime)/6 -80;
    //Serial.println(value);
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


//==============================IMU=====================================




// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
 
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}
 
 
// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}
 
 
// Initializations
void setupIMU()
{
  //pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  //digitalWrite(49, 1);                 //Enable IO power on main CPU board
 
  // Arduino initializations
  //Wire.begin();                        //Set up I2C buss
  //Serial.begin(9600);                //Set up serial communications
 
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_16_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);
 
  // Request first magnetometer single measurement
  I2CwriteByte(MAG_ADDRESS,0x0A,0x01);
}

// Main loop, read and display data
void updateIMU()
{
  //delay(200);
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);

  // Gyroscope
  gyroXRA->addValue((int) Buf[8]<<8 | Buf[9]);
  gyroYRA->addValue((int) Buf[10]<<8 | Buf[11]);
  gyroZRA->addValue((int) Buf[12]<<8 | Buf[13]);
 
 
 /*
  Serial.print (gx,DEC); 
  Serial.print ("\t");
  Serial.print (gy,DEC);
  Serial.print ("\t");
  Serial.print (gz,DEC);  
  Serial.println ("\t");  
 */ 
}


