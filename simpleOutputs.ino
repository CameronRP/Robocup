//Simple outputs, not motors. So basically just LEDs at the moment. Maybe US speakers at some point.

//====================PINS====================
//LEDS
const int ledWarningPin = 0;  //TODO find pin for this

//====================FUNCTIONS=====================
void setupSimpleOutputs(){
  pinMode(ledWarningPin, OUTPUT);
}


void setLedWarning(boolean b){
  digitalWrite(ledWarningPin, b);
}
