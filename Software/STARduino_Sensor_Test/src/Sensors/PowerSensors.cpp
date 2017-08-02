//RockSat: Power Senors
//Michael Walker
#include "PowerSensors.h"

VoltageSense::VoltageSense(int _sensePin, uint8_t _numSamples){
/*
 * constructor implementation of class that reads voltage from analog pin A0
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
	if (_numSamples >= VOLTAGE_MAX_SAMPLES){
		_numSamples = VOLTAGE_MAX_SAMPLES;
	}
	else if (_numSamples <= 1){
		_numSamples = 1;
	}
	
	set_sense_pin(_sensePin);
	set_number_samples(_numSamples);
	pinMode(sensePin,INPUT);
	
}

uint16_t VoltageSense::read_voltage(){
/*
 * Reads voltage analog sensors from anlong pin A0 (if not specified)
 * Supersampling support
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * voltage_value - Sensor value or supersample average
 */	
	
	int voltagevalue_ = 0;
		
	for( uint8_t i=0; i < numSamples; i++){
		voltagevalue_ += analogRead(sensePin);
	}
	voltagevalue_ = voltagevalue_/numSamples; //averaging
	voltagevalue_ = voltagevalue_*VOLTAGE_CONSTANT; //for converting to usable units
	return voltagevalue_;
}

VoltageSense::~VoltageSense(){ }

CurrentSense::CurrentSense(int _sensePin, uint8_t _numSamples){
/*
 * Builds the class for sampling current from analog pin A4
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
 	if (_numSamples >= CURRENT_MAX_SAMPLES){
		_numSamples = CURRENT_MAX_SAMPLES;
	}
	else if (_numSamples <= 1){
		_numSamples = 1;
	}
	
	set_sense_pin(_sensePin);
	set_number_samples(_numSamples);
	pinMode(sensePin,INPUT);
	
}

uint16_t CurrentSense::read_current(){
/*
 * Reads current analog sensors from anlong pin A4 (if not specified) 
 * Supersampling support
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * currentvalue_ - Sensor value or supersample average
 */		
	int currentvalue_ = 0;
	
	for( uint8_t i=0; i < numSamples; i++){
		currentvalue_ += analogRead(sensePin);
	}
	currentvalue_ = currentvalue_/numSamples; //making the sum an average
	currentvalue_ = currentvalue_*CURRENT_CONSTANT; //convert to different units
	return currentvalue_;
}	

CurrentSense::~CurrentSense(){ }