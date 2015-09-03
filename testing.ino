
void testingCode(){
  Serial.println("First loop");
  int tick = 0;
  
  while (true){
    tick++;
    //Some code
    Serial.print("Right sensor: ");
    Serial.println(usRight);
    Serial.println(tick);
    updateUS();
  }
  
}

