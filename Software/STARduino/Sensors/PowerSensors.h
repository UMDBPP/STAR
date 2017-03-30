#ifndef POWER_SENSE_H
#define POWER_SENSE_H

#include <stdint.h>

// Maximum samples for sensor supersampling (currently not used)
#define VOLTAGE_MAX_SAMPLES 10
#define CURRENT_MAX_SAMPLES 10

// Analog Pins for sensors
#define voltage_sensePin A0
#define current_sensePin A4

//number of samples for sensor supersampling 
#define voltage_numSamples
#define current_numSamples 

//Constants for converting sensor readings to usable units
#define voltage_constant 1
#define current_constant 1


// Voltage reading class for analog VSENSE
// Supports software supersampling
class VoltageSense {
    public:
        VoltageSense(int _sensePin, uint8_t _numSamples = 1);
        ~VoltageSense();

        uint16_t read_voltage();

        void set_number_samples(uint8_t _numSamples) { numSamples = _numSamples; }
        uint8_t get_number_samples() { return numSamples; }
		void set_sense_pin(int _sensePin) {sensePin = _sensePin;}

    private:
        uint8_t numSamples;
		int sensePin;

};

// Class to read analog current sensor (Comes off 5V reg)
// Supports software supersampling
class CurrentSense {
    public:
        CurrentSense(int _sensePin, uint8_t _numSamples = 1);
        ~CurrentSense();

        uint16_t read_current();

        void set_number_samples(uint8_t _numSamples) { numSamples = _numSamples; }
        uint8_t get_number_samples() { return numSamples; }
		void set_sense_pin(int _sensePin) {sensePin = _sensePin;}
		
    private:
        uint8_t numSamples;
		int sensePin;

};

#endif // POWER_SENSE_H
