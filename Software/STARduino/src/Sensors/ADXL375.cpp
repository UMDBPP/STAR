#include <Wire.h>
#include "ADXL375.h"

ADXL375::ADXL375() {
    //Wire.begin();
    this->numSamples = ADXL_MAX_SAMPLES;
}

ADXL375::ADXL375(uint8_t samples) {
    Wire.begin();
	if ((samples <= ADXL_MAX_SAMPLES) && (samples > 0)) {
	    numSamples = samples;
    } else {
	    numSamples = ADXL_MAX_SAMPLES;
    }
}

/*
 * Destructor. Empty but explicit.
 */
 ADXL375::~ADXL375() { }
 
void ADXL375::begin() {
    Wire.begin();
    uint8_t enable_byte = 1 << 3;
    write_register(ADXL_POWER_CTL_REG, enable_byte);
}

uint8_t ADXL375::read_register(uint8_t _addr, uint8_t& _data) {
    Wire.beginTransmission(ADXL_ADDRESS);
    uint8_t bytesSent = Wire.write(_addr);
    if(bytesSent != 1) { // Verify command byte was sent successfully
        return 1;
    } 
    
    if(Wire.endTransmission(false)) { // Return code should be zero
        return 1;
    }
    
    Wire.requestFrom(ADXL_ADDRESS, 1);
    _data = Wire.read();
    return 0;
}

uint8_t ADXL375::write_register(uint8_t _addr, uint8_t& _value) {
    Wire.beginTransmission(ADXL_ADDRESS);
    uint8_t bytesSent = Wire.write(_addr);
    if(bytesSent != 1) { // Verify command byte was sent successfully
        return 1;
    }   
    bytesSent = Wire.write(_value);
    return (!bytesSent || Wire.endTransmission()); // BytesSent should be 1, return code from endTransmission should be zero
}

uint8_t ADXL375::recieve_data() {
    // 32-bit to prevent overflow
    int32_t runningX = 0;
    int32_t runningY = 0;
    int32_t runningZ = 0;
    
    for(uint8_t i = 0; i < numSamples; i++) {
        
        Wire.beginTransmission(ADXL_ADDRESS);
    
        while(Wire.available()) {
            Wire.read();
        }

        uint8_t bytesSent = Wire.write(ADXL_DATA_FORMAT); // ADXL format is first byte of the sequence
        if(bytesSent != 1) { // Verify command byte was sent successfully
            return 1;
        } 
        
        if(Wire.endTransmission(false)) { // Return code should be zero
            return 1;
        }
        
        Wire.requestFrom(ADXL_ADDRESS, 7); // Read registers 0x31-0x37 (DATA_FORMAT - DATAZ1)
        uint8_t av = Wire.available();
        if(Wire.available() != 7) {
            return 1;
        }
        
        uint8_t format = Wire.read();
        uint8_t x0 = Wire.read();
        uint8_t x1 = Wire.read();
        uint8_t y0 = Wire.read();
        uint8_t y1 = Wire.read();
        uint8_t z0 = Wire.read();
        uint8_t z1 = Wire.read();
        
        int16_t x = ((x1 << 8) | x0);
        int16_t y = ((y1 << 8) | y0);
        int16_t z = ((z1 << 8) | z0);
        runningX += x;
        runningY += y;
        runningZ += z;
    }
    
    int16_t x = runningX / numSamples;
    int16_t y = runningY / numSamples;
    int16_t z = runningZ / numSamples;
    
    rawAccel = Vector3<int16_t>(x, y, z);
    
    return 0;
}

Vector3<float> ADXL375::convert_data() {
    
    float x_float  = ADXL_CONVERSION_FACTOR*this->rawAccel.x;
    float y_float  = ADXL_CONVERSION_FACTOR*this->rawAccel.y;
    float z_float  = ADXL_CONVERSION_FACTOR*this->rawAccel.z;
    
    return Vector3<float>(x_float, y_float, z_float);
}

uint8_t ADXL375::set_sample_rate(uint8_t rate) {
    rate = rate & 0b1111; // Rate is only four bits
    
    uint8_t register_val;
    
    uint8_t return_code = read_register(ADXL_BW_RATE_REG, register_val); // Read existing register value
    
    if(return_code != 0) {
        return return_code;
    }
    
    register_val &= ~(0b1111); // Unset bottom four bits to set new rate
    register_val |= rate; // Set new rate
    
    return_code = write_register(ADXL_BW_RATE_REG, register_val); // Write new rate to device
    
    return return_code;
}

/*
 * Set new supersampling value. Includes bounds check.
 *
 * Inputs: New number of samples for supersampling.
 */
void ADXL375::set_number_samples(uint8_t samples) {
    if ((samples <= ADXL_MAX_SAMPLES) && (samples > 0)) {
	    numSamples = samples;
    } else {
	    numSamples = numSamples;
    }
} 
