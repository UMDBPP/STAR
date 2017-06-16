#define BASE_ADDR *(unsigned int volatile *)0x41004400
#define DIRSET_A *(unsigned int volatile *)(BASE_ADDR + 0x08)
#define OUTSET_A *(unsigned int volatile *)(BASE_ADDR + 0x18)

#define RESET_REGISTER *(unsigned int volatile *)0xE000ED0C

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
  // Pull "Kill" ports high:
  pinMode(11, OUTPUT); // Pi Kill
  digitalWrite(11, HIGH);
  //pinMode(38, OUTPUT); // External Kill
  //digitalWrite(38, HIGH);
  DIRSET_A |= (1 << 13);
  OUTSET_A |= (1 << 13);
  delay(5000);
  digitalWrite(11,LOW);
  delay(5000);

}

void loop() {
  //Serial.println("TEST.");
  delay(1000);
}





