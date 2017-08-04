#include <Wire.h>
#include <Arduino.h>
#include "HoneywellPressure.h"

/*
 * Default constructor; initialize Arduino I2C library.
 * Sets supersampling to max.
 */
HoneywellPressureI2C::HoneywellPressureI2C(){
	//Wire.begin();
    numSamples = PRESSURE_MAX_SAMPLES;
}

/*
 * Constructor with provided supersampling value.
 *
 * Inputs:
 * _samples: number of samples to average in supersampling.
 *
 */
HoneywellPressureI2C::HoneywellPressureI2C(uint8_t _samples){
	Wire.begin();
	if ((_samples <= PRESSURE_MAX_SAMPLES) && (_samples > 0)) {
	    numSamples = _samples;
    } else {
	    numSamples = PRESSURE_MAX_SAMPLES;
    }
}

/*
 * Destructor. Empty but explicitly defined.
 */
HoneywellPressureI2C::~HoneywellPressureI2C(){ }

/*
 * Pull data from the sensor. Really the heart of the class.
 * Incorporates supersampling.
 * If error in data retrieval, breaks and changes status variable.
 *
 * Outputs: Supersampled pressure counts (raw ints, not converted.)
 *
 * Return: 0 if nominal, 1 if not. User should check this!
 */
uint8_t HoneywellPressureI2C::recieve_data(void) {
    uint16_t runningPressure_ = 0;
    uint16_t runningTemp_ = 0;

    for (uint8_t i=0; i<numSamples; i++) {
        uint8_t stat = 0; // Status byte
        uint8_t bytes[4] = {0};  //four bytes to store sensor data

        Wire.requestFrom(PRESSURE_I2C_ADDR, (uint8_t) 4);    //request sensor data
        for (uint8_t i = 0; i < 4; i++) {
            delay(4); // Is this delay necessary? We need 30 Hz performance from ALL sensors...
            bytes[i] = Wire.read();
        }
        stat = (bytes[0] & 0xc0) >> 6; // first 2 bits from first byte are the status
        switch(stat) {
            case 0: 
                status = PRESSURE_SENSOR_STATUS::NORMAL;
                break;
            case 1:
                status = PRESSURE_SENSOR_STATUS::COMMAND_MODE;
                break;
            case 2:
                status = PRESSURE_SENSOR_STATUS::STALE_DATA;
                break;
            case 3:
                status = PRESSURE_SENSOR_STATUS::DIAGNOSTIC_FAULT;
                break;
            default:
                status = PRESSURE_SENSOR_STATUS::MISSING;
                break;
        }
        if(status != PRESSURE_SENSOR_STATUS::NORMAL) {
            return 2; // If there's a failure, the data is going to be invalid. Just return.
        }

        // Extract the pressure and temp data
        uint16_t pressuredata_ = ((bytes[0] & 0x3f) << 8) + bytes[1];
        uint16_t temperaturedata_ = ((bytes[2] << 8) + (bytes[3] & 0xe0)) >> 5;
        if (temperaturedata_ == 65535) {
            status = PRESSURE_SENSOR_STATUS::MISSING; // If data is invalid, set status and exit.
            return 1;
        }
        runningPressure_ += pressuredata_;
        runningTemp_ += temperaturedata_;

    }

    pressureCounts = runningPressure_ / numSamples;
    tempCounts =  runningTemp_ / numSamples;
    return 0;
}

/*
 * Convert temperature ADC counts to actual values.
 * Values in degrees C.
 * NOTE: DO NOT USE UNLESS THE FLOAT CONVERSION HIT IS OKAY.
 *
 * Returns: Temperature in degrees C as a float.
 */
float HoneywellPressureI2C::convert_temp() {
    float convtemp_ = (tempCounts * 0.0977f) - 50.0f;
    return convtemp_;
}

/*
 * Convert pressure ADC counts to actual values.
 * Values in PSI. (Sorry...)
 * NOTE: DO NOT USE UNLESS THE FLOAT CONVERSION HIT IS OKAY.
 *
 * Returns: Pressure in PSI as a float.
 */
float HoneywellPressureI2C::convert_pressure() {
    float convpress_ = ((pressureCounts - PRESSURE_MIN_COUNTS) * (PRESSURE_MAX_VAL - PRESSURE_MIN_VAL)) / (PRESSURE_MAX_COUNTS - PRESSURE_MIN_COUNTS);
    convpress_ += PRESSURE_MIN_VAL;
    return convpress_;
}

/*
 * Set new supersampling value. Includes bounds check.
 *
 * Inputs: New number of samples for supersampling.
 */
void HoneywellPressureI2C::set_number_samples(uint8_t _samples) {
    if ((_samples <= PRESSURE_MAX_SAMPLES) && (_samples > 0)) {
	    numSamples = _samples;
    } else {
	    numSamples = PRESSURE_MAX_SAMPLES;
    }
} 
