//This will look at the sensors and do some logic stuff

//=================VARIABLES=========================



//======================FUNCTIONS======================




//=============FACEING A WALL========================

//A very curde way of checkign if the robot is facing a wall

boolean isFacingFlatWall(void) {
  int error = 10;
  
  int p2p = getIR1() + (getIR5()-getIR1())/4;
  int p3p = getIR1() + (getIR5()-getIR1())/2;
  int p4p = getIR1() + (getIR5()-getIR1())*3/4;
  
  if (!((p2p-error) < getIR2() && getIR2() < (p2p+error))) {
    return false;
  }
  if (!((p3p-error) < getIR3() && getIR3() < (p3p+error))) {
    return false;
  }
  if (!((p4p-error) < getIR4() && getIR4() < (p4p+error))) {
    return false;
  }
  return true;
}

