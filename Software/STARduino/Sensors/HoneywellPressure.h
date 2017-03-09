#ifndef PRESSURE_H
#define PRESSURE_H

#include <stdint.h>

enum PRESSURE_SENSOR_STATUS { NORMAL, COMMAND_MODE, STALE_DATA, DIAGNOSTIC_FAULT };

class HoneywellPressureI2C {
    public:
        HoneywellPressureI2C();
        ~HoneywellPressureI2C();

        void recieve_data();

        // Setter: numSamples
        void set_number_samples(uint8_t _samples) { numSamples = _samples; }

        // Getters for all stored values
        uint16_t get_pressure() { return pressureCounts; }
        uint16_t get_temperature() { return tempCounts; }
        uint8_t get_number_samples() { return numSamples; }
        PRESSURE_SENSOR_STATUS get_status() { return status; }

    private:
        uint16_t pressureCounts;
        uint16_t tempCounts;
        uint8_t numSamples;
        PRESSURE_SENSOR_STATUS status;
        
};

#endif // PRESSURE_H
