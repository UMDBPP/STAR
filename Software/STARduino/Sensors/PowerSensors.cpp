//RockSat: Power Senors
//Michael Walker
#include "PowerSensors.h"

VoltageSense::VoltageSense(int _sensePin = voltage_sensePin, uint8_t _numSamples=1){
/*
 * constructor implementation
 * 
 * Inputs: 
 * _sensePin - the analog pin the data will be taken from
 * _numSamples - number of samples that are avearged
 * 
 * Output:
 * None
 * 
 * Return:
 * none
 */	
	set_sense_pin(_sensePin)
	set_number_samples(_numsamples)
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
		
	for( uint8_t i=0; i < numSamples; i++){
		voltage_value += analogRead(sensePin);
	}
	voltage_value = voltage_value/numSamples; //averaging
	voltage_value = voltage_value*voltage_constant; //for converting to usable units
	return voltage_value;
}

VoltageSense::~VoltageSense(){ }

CurrentSense::CurrentSense(int _sensePin = A4, uint8_t _numSamples=1){
/*
 * Reads voltage analog sensors from analog pin A4 
 * Supersampling support
 * 
 * Inputs: 
 * _sensePin - the analog pin the data will be taken from
 * _numSamples - number of samples that are avearged
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 */		
 	set_sense_pin(_sensePin)
	set_number_samples(_numSamples)
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
	
	for( uint8_t i=0; i < numSamples; i++){
		current_value += analogRead(sensePin);
	}
	current_value = current_value/numSamples; //making the sum an average
	current_value = current_value*current_constant; //convert to different units
	return current_value;
}	

CurrentSense::~CurrentSense(){ }