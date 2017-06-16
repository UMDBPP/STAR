#ifndef PRESSURE_H
#define PRESSURE_H

#include <stdint.h>

#define PRESSURE_I2C_ADDR 0x28 // I2C address of pressure sensor
#define PRESSURE_MIN_COUNTS 0x0666 // Minimum value sensor will return
#define PRESSURE_MAX_COUNTS 0x3999 // Maximum value sensor will return
#define PRESSURE_MIN_VAL 0.0 // Minimum converted pressure
#define PRESSURE_MAX_VAL 15.0 // Maximum converted pressure
#define PRESSURE_MAX_SAMPLES 5 // Maximum samples for supersampling sensor

// Status code enum for pressure sensor (First 2 bits of data are status)
// NOTE: This is an enum class, so to reference it, you MUST do PRESSURE_SENSOR_STATUS::NORMAL, etc.
// Enum class used to avoid enum naming conflicts.
enum class PRESSURE_SENSOR_STATUS { NORMAL, COMMAND_MODE, STALE_DATA, DIAGNOSTIC_FAULT, MISSING };

// Class to handle communication with Honeywell SSC pressure sensor over I2C.
// Supports software supersampling.
// Retrieves data in one block (as sensor sends it), separates into pressure, temp, and status
// Pressure, temp, status available individually through getters
// Conversions available as separate floating-point functions (so avoid unless needed!)
class HoneywellPressureI2C {
    public:
        HoneywellPressureI2C();
        HoneywellPressureI2C(uint8_t _samples);
        ~HoneywellPressureI2C();

        uint8_t recieve_data();
        float convert_temp();
        float convert_pressure();

        // Setter: numSamples
        void set_number_samples(uint8_t _samples);

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
