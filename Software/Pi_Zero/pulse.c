#include <wiringPi.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

// Pin - wiringPi pin 0 is BCM_GPIO 17.
#define PulsePin     0

unsigned long previousTime = 0;        // will store last time status was updated
#define ON_TIME .01*CLOCKS_PER_SEC          //Clock cylces of on-time
#define OFF_TIME .99*CLOCKS_PER_SEC          //Clock cycles of off-time

//#define DEBUG 1

int main (void) {
  #ifdef DEBUG
    printf ("Raspberry Pi Pulse\n") ;
  #endif
	
  wiringPiSetup () ;
  pinMode (PulsePin, OUTPUT) ;
  bool State = 0;
  for (;;) {
	unsigned long currentTime = clock();
 	if((State == 1) && (currentTime - previousTime >= ON_TIME)) {
	State = 0;
	previousTime = currentTime;
    digitalWrite (PulsePin, State) ; 
   }
  
  else if ((State == 0) && (currentTime - previousTime >= OFF_TIME)) {
    State = 1;
	previousTime = currentTime;
    digitalWrite (PulsePin, State) ; 
   }
  }
  return 0 ;
}
