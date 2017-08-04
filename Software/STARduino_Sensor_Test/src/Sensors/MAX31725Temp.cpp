#include <Wire.h>
#include <Arduino.h>
#include "MAX31725Temp.h"

#define TEMP_DATA_REGISTER 0x00
#define TEMP_CONFIG_REGISTER 0x01
#define TEMP_THYST_REGISTER 0x02
#define TEMP_TOS_REGISTER 0x03

/*
 * Default constructor, uses max supersampling value and default I2C address.
 * Also init I2C.
 */
MAX31725::MAX31725(uint8_t _samples) {
    //Wire.begin();
    i2cAddress = TEMPERATURE_DEFAULT_ADDR;

    if((_samples <= TEMPERATURE_MAX_SAMPLES) && (_samples > 0)) {
        numSamples = _samples;
    } else {
        numSamples = TEMPERATURE_MAX_SAMPLES;
    }
}

/*
 * Constructor with additional I2C address argument.
 * Allows use of alternate addresses.
 */
MAX31725::MAX31725(uint8_t _i2cAddress, uint8_t _samples) {
    Wire.begin();
    i2cAddress = _i2cAddress;

    if((_samples <= TEMPERATURE_MAX_SAMPLES) && (_samples > 0)) {
        numSamples = _samples;
    } else {
        numSamples = TEMPERATURE_MAX_SAMPLES;
    }
}

/*
 * Destructor. Empty but explicit.
 */
 MAX31725::~MAX31725() { }

/*
 * Write to the configuration register of the sensor.
 * One byte write.
 *
 * Inputs:
 * _newConfig: New configuration byte to send to the sensor.
 * 
 * Outputs:
 * Sets current status value in class.
 *
 * Returns:
 * Arduino Wire library status. Should be check by user.
 */
int MAX31725::write_config_register(uint8_t _newConfig) {
    Wire.beginTransmission(i2cAddress);
    Wire.write(TEMP_CONFIG_REGISTER);
    Wire.write(_newConfig);
    int status_ = Wire.endTransmission();

    if(status_ == 0) {
        currentConfig = _newConfig;
        return 0;
    } else {
        return status_;
    }
}

/*
 * Write new value to temp hysteresis register
 * Used for thermal shutdown config.
 *
 * Inputs:
 * _newThyst: New value for the THYST register
 *
 * Returns:
 * Arduino Wire status code. Should be checked by user.
 */
int MAX31725::write_thyst_register(uint16_t _newThyst) {
    return write_two_bytes(TEMP_THYST_REGISTER, _newThyst);
}

/*
 * Write new value to overtemp shutdown register.
 * Used for thermal shutdown config.
 *
 * Inputs:
 * _newTos: New value for the TOS register
 *
 * Returns:
 * Arduino Wire status code. Should be checked by user.
 */
int MAX31725::write_tos_register(uint16_t _newTos) {
    return write_two_bytes(TEMP_TOS_REGISTER, _newTos);
}

/*
 * Read on-chip config register.
 * Set internal class value and return to user.
 *
 * Outputs: Sets currentConfig value in class.
 *
 * Returns: current configuration as read from sensor.
 */
uint8_t MAX31725::read_config_register() {
    // Set register to read
    Wire.beginTransmission(i2cAddress);
    Wire.write(TEMP_CONFIG_REGISTER);
    int status_ = Wire.endTransmission(false); // Send restart as per datasheet

    // If sensor didn't respond, return current config.
    if(status_ != 0) {
        return currentConfig;
    }

    // Read the single byte of the config register
    Wire.requestFrom(i2cAddress, 1);
    uint8_t config_ = Wire.read();

    // Set the internal value for the class and return said value to the user.
    currentConfig = config_;
    return config_;
}

/*
 * Read hysteresis register from chip
 *
 * Outputs:
 * _regValue: passed by reference 16-bit value to store register value.
 *
 * Returns:
 * Arduino Wire status code. User should use to validity check.
 */
uint8_t MAX31725::read_thyst_register(uint16_t& _regValue) {
    return read_two_bytes(TEMP_THYST_REGISTER, _regValue);
}

/*
 * Read overtemp shutdown register from chip
 *
 * Outputs:
 * _regValue: passed by reference 16-bit value to store register value.
 *
 * Returns:
 * Arduino Wire status code. User should use to validity check.
 */
uint8_t MAX31725::read_tos_register(uint16_t& _regValue) {
    return read_two_bytes(TEMP_TOS_REGISTER, _regValue);
}

/*
 * Read temperature data from chip, store internally.
 * Supports supersampling.
 * NOTE: supersampling temporarily disabled due to way chip stores decimal values
 *
 * Outputs: changes class internal raw temperature value
 *
 * Returns:
 * Arduino Wire status code; user should use to validity check.
 */
uint8_t MAX31725::recieve_data() {
    int status_ = 0;
    uint16_t runningvalue_ = 0; // Reset raw temp value

    // Grab data number of times supersampling value is set to
    for(int i=0; i<numSamples; i++) {
        uint16_t data_ = 0;
        status_ =  read_two_bytes(TEMP_DATA_REGISTER, data_); // Read the data
        
        // If errored, return with failure code.
        if(status_ != 0) {
            return status_;
        }

        runningvalue_ += data_; // Add to running count
    }

    rawTemperature = runningvalue_ / numSamples; // Take the average and set class value.
    return 0; // Return success
}

/*
 * Covert raw temperature to floating point value.
 * May be optimized later to use lookup table.
 * NOTE: SHOULD ONLY BE USED IF FLOATING POINT HIT IS OKAY.
 *
 * Returns: floating point converted temperature in degrees C.
 */
float MAX31725::convert_temp() {
    int8_t intpart_ = (rawTemperature & 0xff00) >> 8; // Convert int part to signed and extract from total data
    float decimalpart_ = 0.0f;

    // The decimal part encoding is funky, so here's a terrible bit of code to extract that:
    uint8_t lsb_ = (uint8_t)((uint16_t)0x00ff & intpart_);
    if((lsb_ & 0b10000000) >> 7) {
        decimalpart_ += 0.5f;
    }
    if((lsb_ & 0b01000000) >> 6) {
        decimalpart_ += 0.25f;
    }
    if((lsb_ & 0b00100000) >> 5) {
        decimalpart_ += 0.125f;
    }
    if((lsb_ & 0b00010000) >> 4) {
        decimalpart_ += 0.0625f;
    }
    if((lsb_ & 0b00001000) >> 3) {
        decimalpart_ += 0.03125f;
    }
    if((lsb_ & 0b00000100) >> 2) {
        decimalpart_ += 0.015625f;
    }
    if((lsb_ & 0b00000010) >> 1) {
        decimalpart_ += 0.0078125f;
    }
    if(lsb_ & 0b00000001) {
        decimalpart_ += 0.00390625f;
    }

    return (float)intpart_ + decimalpart_;
}

/*
 * Setter to modify supersampling value.
 * Bounds checks for valid range.
 *
 * Outputs: Modifys numSamples to new value.
 */
 void MAX31725::set_num_samples(uint8_t _newSamples) {
    if((_newSamples <= TEMPERATURE_MAX_SAMPLES) && (_newSamples > 0)) {
        numSamples = _newSamples;
    } else {
        numSamples = TEMPERATURE_MAX_SAMPLES;
    }
 }

/*
 * Internal two byte transmit function.
 *
 * Inputs:
 * _reg: register to write to
 * _value: 16 bit value to write
 *
 * Returns:
 * Arduino Wire response code; should be passed to user.
 */
int MAX31725::write_two_bytes(uint8_t _reg, uint16_t _value) {
    uint8_t msb_ = ((uint16_t)0xff00 & _value) >> 8;
    uint8_t lsb_ = (uint8_t)((uint16_t)0x00ff & _value);

    Wire.beginTransmission(i2cAddress);
    Wire.write(_reg);
    Wire.write(msb_);
    Wire.write(lsb_);
    return Wire.endTransmission();
}

/*
 * Internal two byte read function.
 *
 * Inputs:
 * _reg: register to read from
 * 
 * Outputs:
 * _data: 16-bit pass-by-reference for read data
 *
 * Returns:
 * Arduino Wire response code; should be passed to user.
 */
int MAX31725::read_two_bytes(uint8_t _reg, uint16_t& _data) {
    // Set register pointer on chip
    Wire.beginTransmission(i2cAddress);
    Wire.write(_reg);
    int status_ = Wire.endTransmission(false); // Send restart as per datasheet.

    // If failure, quit and report.
    if(status_ != 0) {
        return status_;
    }

    Wire.requestFrom(i2cAddress, 2);
    _data = (Wire.read() << 8) | (Wire.read());

    return 0;
}
