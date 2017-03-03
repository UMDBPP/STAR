//
//  Photo_Capture_Loop.c
//  
//
//  Created by Clark Esty on 2/2/17.
//
//

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam.h>
using namespacestd;

#include "Pulse_Functions.h"    // Calls pulse functions

// #define PulsePin     0      // Pin that pulse is going too - BCM GPIO 17

int main(int argc,char **argv) {
    // Write a loop that turns on the photo after every 30 shots and then turns it off after the first of the next one
    Setup_Pulse();  // initial raspberry pi for sync pulse
    
    int counter = 0;    // photo counter
    raspicam::RaspiCam Camera; //Cmaera object
	
    for (;;) {
	//capture
    	Camera.grab();
    	//allocate memory
    	unsigned char *data=new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
        counter ++;
        if (counter == 1) {
            End_Pulse();
        }
        else if (counter == 30) {
            Start_Pulse();
            counter = 0;
        }
        // Add in command to break loop when desired
    }
    Terminate_Pulse();  // Set pulse to off position incase loop breaks with it on
}
