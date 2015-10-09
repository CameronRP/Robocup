//Simple outputs, not motors. So basically just LEDs at the moment. Maybe US speakers at some point.

//====================PINS====================
//LEDS
const int ledWarningPin = 29;
const int led1 = 26;
const int led2 = 27;
const int led3 = 28;

//====================FUNCTIONS=====================
void setupSimpleOutputs(void){
  pinMode(ledWarningPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}


void setLedWarning(boolean b){
  digitalWrite(ledWarningPin, b);
}

void updateLights(void) {
  /*
  digitalWrite(led1, weightAtLeft());
  digitalWrite(led2, weightAtMid());
  digitalWrite(led3, weightAtRight());
  */
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(ledWarningPin, HIGH);
}
