#ifndef PRESSURE_H
#define PRESSURE_H

#include <stdint.h>

enum PRESSURE_SENSOR_STATUS { NORMAL, COMMAND_MODE, STALE_DATA, DIAGNOSTIC_FAULT };

class HoneywellPressureI2C {
    public:
        HoneywellPressureI2C();
        ~HoneywellPressureI2C();

        void recieve_data();
        uint16_t get_pressure() { return pressureCounts; }
        uint16_t get_temperature() { return tempCounts; }
        PRESSURE_SENSOR_STATUS get_status() { return status; }

    private:
        int pressureCounts;
        int tempCounts;
        PRESSURE_SENSOR_STATUS status;
        
};

#endif // PRESSURE_H
