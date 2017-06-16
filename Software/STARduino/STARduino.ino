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
  DIRSET_A |= (1 << 13);
  OUTSET_A |= (1 << 13);

}

void loop() {
  //Serial.println("TEST.");

  delay(1000);
}





