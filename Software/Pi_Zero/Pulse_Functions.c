//
//  Pulse_Functions.c
//  
//
//  Created by Clark Esty on 2/2/17.
//
//

#include "Pulse_Functions.h"

//#define PulsePin     0      // Pin that pulse is going too - BCM GPIO 17

void Setup_Pulse(void) {
    wiringPiSetup () ;
    pinMode (PulsePin, OUTPUT) ;
}

void Start_Pulse(void) {
    digitalWrite (PulsePin, 1);
}

void End_Pulse(void) {
    digitalWrite (PulsePin, 0);
}

void Terminate_Pulse(void) {
    digitalWrite (PulsePin, 0);
}
