#include "EZGPIO.h"

int LEDPIN = 9;
int BIASPIN = 28;
int PIKILL = 16;
int EXTKILL = 13;

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
  DIRSET_A = (1 << PIKILL);
  OUTSET_A = (1 << PIKILL);
  DIRSET_A = (1 << EXTKILL);
  OUTSET_A = (1 << EXTKILL);
  
  DIRSET_A = (1 << BIASPIN);
  DIRSET_A = (1 << LEDPIN);
  
}

void loop() {
  //Serial.println("TEST.");
  OUTTGL_A = (1 << BIASPIN);
  OUTTGL_A = (1 << LEDPIN);
  delay(5000);
  OUTTGL_A = (1 << BIASPIN);
  OUTTGL_A = (1 << LEDPIN);
  delay(5000);
}





