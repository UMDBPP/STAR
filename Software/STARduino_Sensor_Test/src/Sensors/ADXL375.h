#ifndef ADXL_H
#define ADXL_H

#include <stdint.h>
#include <Arduino.h>
#include "../DataStructures/Vector.h"

#define ADXL_ADDRESS 0x1D

#define ADXL_MAX_SAMPLES 5 // Maximum samples for supersampling
#define ADXL_DEVICE_ID 0b11100101

// Register definitions.
// In header to allow user access.
#define ADXL_ID_REG 0x00
#define ADXL_SHOCK_THRESH_REG 0x1d
#define ADXL_X_OFFSET_REG 0x1e
#define ADXL_Y_OFFSET_REG 0x1f
#define ADXL_Z_OFFSET_REG 0x20
#define ADXL_SHOCK_DUR_REG 0x21
#define ADXL_SHOCK_LATENT_REG 0x22
#define ADXL_SHOCK_WINDOW_REG 0x23
#define ADXL_ACTIVITY_THRESH_REG 0x24
#define ADXL_INACTIVITY_THRESH_REG 0x25
#define ADXL_INACTIVE_TIME_REG 0x26
#define ADXL_ACT_INACT_CTL_REG 0x27
#define ADXL_SHOCK_AXES_REG 0x2a
#define ADXL_ACT_SHOCK_STATUS_REG 0x2b
#define ADXL_BW_RATE_REG 0x2c
#define ADXL_POWER_CTL_REG 0x2d
#define ADXL_INT_ENABLE_REG 0x2e
#define ADXL_INT_MAP_REG 0x2f
#define ADXL_INT_SOURCE_REG 0x30
#define ADXL_DATA_FORMAT 0x31
#define ADXL_DATA_X0_REG 0x32
#define ADXL_DATA_X1_REG 0x33
#define ADXL_DATA_Y0_REG 0x34
#define ADXL_DATA_Y1_REG 0x35
#define ADXL_DATA_Z0_REG 0x36
#define ADXL_DATA_Z1_REG 0x37
#define ADXL_FIFO_CTL_REG 0x38
#define ADXL_FIFO_STATUS_REG 0x39

// Sample rates
#define ADXL_SAMPLE_3200Hz 0b1111
#define ADXL_SAMPLE_1600Hz 0b1110
#define ADXL_SAMPLE_800Hz 0b1101
#define ADXL_SAMPLE_400Hz 0b1100
#define ADXL_SAMPLE_200Hz 0b1011
#define ADXL_SAMPLE_100Hz 0b1010
#define ADXL_SAMPLE_50Hz 0b1001
#define ADXL_SAMPLE_25Hz 0b1000
#define ADXL_SAMPLE_12_5Hz 0b0111
#define ADXL_SAMPLE_6_25Hz 0b0110
#define ADXL_SAMPLE_3_13Hz 0b0101
#define ADXL_SAMPLE_1_56Hz 0b0100
#define ADXL_SAMPLE_0_78Hz 0b0011
#define ADXL_SAMPLE_0_39Hz 0b0010
#define ADXL_SAMPLE_0_20Hz 0b0001
#define ADXL_SAMPLE_0_10Hz 0b0000

#define ADXL_CONVERSION_FACTOR 0.049f // 49 milli g per LSB
#define ADXL_MAX_SAMPLES 5 // Max number of samples for supersampling

// Class for interacting with the ADXL375 high-g accelerometer.
// Provides methods for setup and reading data, and also conversions.
// Also supports supersampling of data.
class ADXL375 {
    public:
        ADXL375();
        ADXL375(uint8_t);
        ~ADXL375();
        
        void begin();

        // Exposed single-byte read/write functions
        // Many registers in ADXL375, not all will have dedicated functions.
        uint8_t read_register(uint8_t _addr, uint8_t& _data);
        uint8_t write_register(uint8_t _addr, uint8_t& _value);

        uint8_t recieve_data();
        Vector3<float> convert_data();
        
        uint8_t set_sample_rate(uint8_t);

        void set_number_samples(uint8_t);

        // Getters
        int16_t get_x_accel() { return rawAccel.x; }
        int16_t get_y_accel() { return rawAccel.y; }
        int16_t get_z_accel() { return rawAccel.z; }
        Vector3<int16_t> get_accel() { return rawAccel; }

    private:
        Vector3<int16_t> rawAccel;
        uint8_t numSamples;
};

#endif // ADXL_H
