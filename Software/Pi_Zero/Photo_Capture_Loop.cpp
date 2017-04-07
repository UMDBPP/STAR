//
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
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <sstream>
// #include <chrono>
using namespace std;
using namespace cv;

#include "Pulse_Functions.h"    // Calls pulse functions

// #define PulsePin     0      // Pin that pulse is going too - BCM GPIO 17

int main(int argc,char **argv) {
    // Write a loop that turns on the photo after every 30 shots and then turns it off after the first of the next one
    Setup_Pulse();  // initial raspberry pi for sync pulse
    Mat image;
    int counter = 0;    // counter
    int photocount = 1; // photo counter
    
    raspicam::RaspiCam_Cv Camera; //Cmaera object
    
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    
    // Compression parameters for the files (0-9, where 9 is the highest compression)
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(3);
    
    // Check if the camera is able to open and if not, increase counter (for testing phases)
    if (!Camera.open()) {
        cerr<<"Error opening camera"<<endl;
    }
    
    /*
    // Setting camera properties
    Camera.set(CV_CAP_PROP_GAIN, 50); // values range from 0 to 100
    Camera.set(CV_CAP_PROP_EXPOSURE, 50); //-1 is auto, values range from 0 to 100
    Camera.set(CV_CAP_PROP_WHITE_BALANCE_RED_V, 50); //values range from 0 to 100, -1 auto whitebalance
    Camera.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U, 50); //values range from 0 to 100,  -1 auto whitebalance
    */

    //start_time = chrono::high_resolution_clock::now(); 
    clock_t start_time = clock();
    char filename[40];	

    for (;;) {
        // Capture
    	Camera.grab();
    	//printf("Step 1");
	// Get image
        Camera.retrieve(image);
	//printf("Step 2");
	//Camera.release();
	//printf("Step 3");
        // Write the image to a file
        sprintf(filename,"image%d.png",photocount);
	//string filename = "~/pictures/image" + to_string(++counter) + ".png";
        imwrite(filename,image,compression_params);
        photocount ++;
	counter ++;
	//printf("Step 4");
	
	clock_t final_time = clock();
        if (counter == 1) {
            End_Pulse();
        }
        else if (counter == 30) {
            Start_Pulse();
	    clock_t t = final_time - start_time;
	    printf("%.6f",((float)t/CLOCKS_PER_SEC));
            counter = 0;
        }
	
        // Set the current time after each loop run through
        // final_time = gettime_now.tv_nsec;		//Get nS value
	// final_time = chrono::high_resolution_clock::now(); 
	/*
	clock_t final_time = clock();
        if (counter == 30) {
	    clock_t t = final_time - start_time;
	    printf("%.6f",((float)t)/CLOCKS_PER_SEC);
	}
	*/
	// Add in command to break loop when desired 
   }
    Camera.release();
    Terminate_Pulse();  // Set pulse to off position incase loop breaks with it on
}
