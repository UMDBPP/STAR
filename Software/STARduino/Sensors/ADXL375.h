#ifndef ADXL_H
#define ADXL_H

#include <stdint.h>

#define ADXL_MAX_SAMPLES 5

// Class for interacting with the ADXL375 high-g accelerometer.
// Provides methods for setup and reading data, and also conversions.
// Also supports supersampling of data.
class ADXL375 {
    public:
        ADXL375();
        ~ADXL375();
};

#endif // ADXL_H
