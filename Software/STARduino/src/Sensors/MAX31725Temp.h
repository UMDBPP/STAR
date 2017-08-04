#ifndef TEMP_SENSE_H
#define TEMP_SENSE_H

#include <stdint.h>

#define TEMPERATURE_MAX_SAMPLES 1 // Max samples for supersampling (NOTE: supersampling OFF for now!)
#define TEMPERATURE_DEFAULT_ADDR 0x48 // Dafault I2C address for sensor (All addr bits to GND)

// Class to interact with MAX31725 temperature sensor.
// Supports multiple I2C addresses since MAX31725 can do that (and we have 2!)
// Also supports software supersampling.
class MAX31725 {
    public:
        MAX31725(uint8_t _samples = TEMPERATURE_MAX_SAMPLES);
        MAX31725(uint8_t _i2cAddress, uint8_t _samples = TEMPERATURE_MAX_SAMPLES);
        ~MAX31725();

        int write_config_register(uint8_t _newConfig);
        int write_thyst_register(uint16_t _newThyst);
        int write_tos_register(uint16_t _newTos);

        uint8_t read_config_register();
        uint8_t read_thyst_register(uint16_t& _regValue);
        uint8_t read_tos_register(uint16_t& _regValue);

        uint8_t recieve_data();
        float convert_temp();

        // Setters
        void set_i2c_address(uint8_t _newAddr) { i2cAddress = _newAddr; }
        void set_num_samples(uint8_t _newSamples);

        // Getters
        uint8_t get_i2c_addr() { return i2cAddress; }
        uint8_t get_num_samples() { return numSamples; }
        uint16_t get_temperature() { return rawTemperature; }
        uint8_t get_current_config() { return currentConfig; }

    private:
        uint8_t i2cAddress;
        uint8_t numSamples;
        uint16_t rawTemperature;
        uint8_t currentConfig;

        int write_two_bytes(uint8_t _reg, uint16_t _value);
        int read_two_bytes(uint8_t _reg, uint16_t& _data);

};

#endif // TEMP_SENSE_H
