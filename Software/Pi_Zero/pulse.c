#include <wiringPi.h>
#include <time.h>
#include <stdio.h>

// Pin - wiringPi pin 0 is BCM_GPIO 17.
#define PulsePin     0

#define CLOCKS_PER_SEC 1000000000 //Clock speed for the Pi
unsigned long previousTime = 0;        // will store last time status was updated
#define ON_TIME (10/1000)*CLOCKS_PER_SEC          //Clock cylces of on-time
#define OFF_TIME (990/1000)*CLOCKS_PER_SEC          //Clock cycles of off-time

//#define DEBUG 1

int main (void) {
  #ifdef DEBUG
    printf ("Raspberry Pi Pulse\n") ;
  #endif
	
  wiringPiSetup () ;
  pinMode (PulsePin, OUTPUT) ;

  for (;;) {
	unsigned long currentTime = clock();
 	if((State == HIGH) && (currentTime - previousTime >= ON_TIME)) {
	State = LOW ; 
	previousTime = currentTime;
    digitalWrite (PulsePin, State) ; 
   }
  
  else if ((State == LOW) && (currentTime - previousTime >= OFF_TIME)) {
    State = HIGH ; 
	previousTime = currentTime;
    digitalWrite (PulsePin, State) ; 
   }
  }
  return 0 ;
}