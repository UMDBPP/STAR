//
//  Photo_Capture_Loop.c
//  
//
//  Created by Clark Esty on 2/2/17.
//
//

#include <wiringPi.h>
#include <time.h>
#include <stdio.h>

#include "Pulse_Functions.h"    // Calls pulse functions

#define PulsePin     0      // Pin that pulse is going too - BCM GPIO 17

int main(void) {
    // Write a loop that turns on the photo after every 30 shots and then turns it off after the first of the next one
    Setup_Pulse();  // initial raspberry pi for sync pulse
    
    counter = 0;    // photo counter
    
    for (;;) {
        // Take photo
        counter += coutner;
        if counter == 1 {
            End_Pulse();
        }
        if counter == 30 {
            Start_Pulse();
            counter = 0;
        }
        // Add in command to break loop when desired
    }
    Terminate_Pulse();  // Set pulse to off position incase loop breaks with it on
}
