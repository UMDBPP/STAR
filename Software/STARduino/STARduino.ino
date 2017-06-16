#include "EZGPIO.h"

/* Define Functions */
void setup() {
  /*
     Initalizes the STAR payload. Opens the serial connections, opens log files,
     loads queued commands, and initializes sensors.

     Inputs:
     none

     Output:
     none

     Return:
     none

  */
  Serial.begin(9600);
  
  // Pull "Kill" ports high:
  pinMode(11, OUTPUT); // Pi Kill
  digitalWrite(11, HIGH);
  //pinMode(38, OUTPUT); // External Kill
  //digitalWrite(38, HIGH);
  //set_DIR(PA, 13, false);
  //set_OUTPUT(PA, 13, true);
  //set_DIR(PA, 28, false);
  DIRSET_A = (1 << 13);
  OUTSET_A = (1 << 13);
  DIRSET_A = (1 << 28);
  
}

void loop() {
  //Serial.println("TEST.");
  //set_OUTPUT(PA, 28, true);
  OUTTGL_A = (1 << 28);
  delay(5000);
  //set_OUTPUT(PA, 28, false);
  OUTTGL_A = (1 << 28);
  delay(5000);
}





