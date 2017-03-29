//RockSat: Power Senors
//Michael Walker
#include "PowerSensors.h"

// Analog Pins for sensors
#define voltage_sensePin A0
#define current_sensePin A4

//number of samples 
#define voltage_numSamples
#define current_numSamples 

VoltageSense::VoltageSense(int voltage_sensePin, uint8_t voltage_numSamples=1){
/*
 * constructor implementation
 * 
 * Inputs: 
 * voltage_sensePin - the analog pin the data will be taken from
 * numSamples - number of samples that are avearged
 * 
 * Output:
 * None
 * 
 * Return:
 * none
 */	
	pinMode(sensePin,INPUT);
	
}

uint16_t VoltageSense::read_voltage(){
/*
 * Reads voltage analog sensors from anlong pin A0 
 * Supersampling support
 * 
 * Inputs: 
 * voltage_sensePin - the analog pin the data will be taken from
 * numSamples - number of samples that are avearged
 * 
 * Output:
 * none
 * 
 * Return:
 * voltage_value - Sensor value or supersample average
 */	
	
	int voltage_value = 0;
	
	if(numSamples == 1){
		voltage_value = analogRead(sensePin); //if only one sample don't need to average
	}
	
	else{
		for( uint8_t i=0; i<= numSamples; i++){
			voltage_value = analogRead(sensePin) + voltage_value;
		}
		voltage_value  = voltage_value/numSamples;
	}
	return voltage_value;
}

VoltageSense::~VoltageSense(){ }

CurrentSense::CurrentSense(int current_sensePin, uint8_t current_numSamples=1){
/*
 * Reads voltage analog sensors from analog pin A4 
 * Supersampling support
 * 
 * Inputs: 
 * current_sensePin - the analog pin the data will be taken from
 * numSamples - number of samples that are avearged
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 */		
 	pinMode(sensePin,INPUT);
	
}

uint16_t CurrentSense::read_current(){
/*
 * Reads current analog sensors from anlong pin A4 
 * Supersampling support
 * 
 * Inputs: 
 * voltage_sensePin - the analog pin the data will be taken from
 * numSamples - number of samples that are avearged
 * 
 * Output:
 * none
 * 
 * Return:
 * current_value - Sensor value or supersample average
 */		
	int current_value = 0;
	
	if(numSamples == 1){
		current_value = analogRead(sensePin); //in only one sample, don't need to average
	}
	
	else{
		for( uint8_t i=0; i<= numSamples; i++){
			current_value = analogRead(sensePin);
		}
		current_value = current_value/numSamples;
		
	}
	return current_value;
}	

CurrentSense::~CurrentSense(){ }