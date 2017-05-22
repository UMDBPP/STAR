#ifndef ADXL_H
#define ADXL_H

#include <stdint.h>
#include "DataStructures/Vector.h"

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

// Class for interacting with the ADXL375 high-g accelerometer.
// Provides methods for setup and reading data, and also conversions.
// Also supports supersampling of data.
class ADXL375 {
    public:
        ADXL375();
        ~ADXL375();

        // Exposed single-byte read/write functions
        // Many registers in ADXL375, not all will have dedicated functions.
        uint8_t read_register(uint8_t _addr, uint8_t& _data);
        uint8_t write_register(uint8_t _addr, uint8_t& _value);

        uint8_t recieve_data();
        Vector3<float> convert_data();

        // Getters
        uint16_t get_x_accel() { return rawAccel.x; }
        uint16_t get_y_accel() { return rawAccel.y; }
        uint16_t get_z_accel() { return rawAccel.z; }
        Vector3<uint16_t> get_accel() { return rawAccel; }

    private:
        Vector3<uint16_t> rawAccel;
};

#endif // ADXL_H
