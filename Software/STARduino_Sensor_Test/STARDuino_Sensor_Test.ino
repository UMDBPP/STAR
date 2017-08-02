#include "EZGPIO.h"
#include "src/Sensors/ADXL375.h"
#include "src/Sensors/MAX31725Temp.h"
#include "src/Sensors/HoneywellPressure.h"
#include "src/Sensors/PowerSensors.h"
#include <SPI.h>
#include<SD.h>

#define TEST_ALL_SENSORS
#define TEST_LOGGING

#ifdef TEST_ALL_SENSORS
  #define ADXL_TEST
  #define MAX_TEMP_TEST
  #define PRESSURE_SENSOR_TEST
  #define POWER_SENSOR_TEST
#endif


int LEDPIN = 9;
int BIASPIN = 28;
int PIKILL = 16;
int EXTKILL = 13;
int SDCSPIN = 22;

#ifdef ADXL_TEST
  ADXL375 adxl;
#endif

#ifdef MAX_TEMP_TEST
  MAX31725 tempSensor;
#endif

#ifdef PRESSURE_SENSOR_TEST
  HoneywellPressureI2C pressureSensor;
#endif

#ifdef POWER_SENSOR_TEST
  VoltageSense voltageSensor;
  CurrentSense currentSensor;
#endif 

/* Define Functions */
void setup() {
  /*
     Initalizes the STAR payload. Opens the serial connections, opens log files,
     loads queued commands, and initializes sensors.

     Inputs:
     none

     Output:
     none

     Return:
     none

  */
  Serial.begin(9600);
  
  // Pull "Kill" ports high:
  DIRSET_A = (1 << PIKILL);
  OUTSET_A = (1 << PIKILL);
  DIRSET_A = (1 << EXTKILL);
  OUTSET_A = (1 << EXTKILL);

  DIRSET_B = (1 << SDCSPIN);
  OUTCLR_B = (1 << SDCSPIN);

#ifdef TEST_LOGGING
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  else
  {
    Serial.println("card initialized.");
  }
  #endif
  
  //DIRSET_A = (1 << BIASPIN);
  //DIRSET_A = (1 << LEDPIN);
  
}

void loop() {

  #ifdef ADXL_TEST

    adxl.recieve_data();
    uint16_t rawXAcc = adxl.get_x_accel();
    uint16_t rawYAcc = adxl.get_y_accel();
    uint16_t rawZAcc = adxl.get_z_accel();
    Serial.print("Raw ADXL X acceleration: ");
    Serial.println(rawXAcc);
    Serial.print("Raw ADXL Y acceleration: ");
    Serial.println(rawYAcc);
    Serial.print("Raw ADXL Z acceleration: ");
    Serial.println(rawZAcc);

    Vector3<uint16_t> convertedAdxlAcceleration = adxl.get_accel();

    Serial.print("Converted ADXL X acceleration: ");
    Serial.println(convertedAdxlAcceleration.x);
    Serial.print("Converted ADXL Y acceleration: ");
    Serial.println(convertedAdxlAcceleration.y);
    Serial.print("Convertedaw ADXL Z acceleration: ");
    Serial.println(convertedAdxlAcceleration.z);
    
  #endif
  
  #ifdef MAX_TEMP_TEST
    
    tempSensor.recieve_data();
    uint16_t rawTemp = tempSensor.get_temperature();
    float convertedTemp = tempSensor.convert_temp();
    Serial.print("Raw MAX31725 temperature: ");
    Serial.println(rawTemp);
    Serial.print("Converted MAX31725 Temperature: ");
    Serial.println(convertedTemp);
  #endif 

  #ifdef PRESSURE_SENSOR_TEST
    pressureSensor.recieve_data();
    uint16_t rawHoneywellTemperature = pressureSensor.get_temperature();
    uint16_t rawPressure = pressureSensor.get_pressure();
    float convertedHoneywellTemperature = pressureSensor.convert_temp();
    float convertedPressure = pressureSensor.convert_pressure();
    Serial.print("Raw Honeywell temperature: ");
    Serial.println(rawHoneywellTemperature);
    Serial.print("Raw Honeywell pressure: ");
    Serial.println(rawPressure);
    Serial.print("Converted Honeywell Temperature: ");
    Serial.println(convertedHoneywellTemperature);
    Serial.print("Converted Presure: ");
    Serial.println(convertedPressure);
  #endif

  #ifdef POWER_SENSOR_TEST
    uint16_t rawVoltage = voltageSensor.read_voltage();
    uint16_t rawCurrent = currentSensor.read_current();
    Serial.print("Raw voltage: ");
    Serial.println(rawVoltage);
    Serial.print("Raw current: ");
    Serial.println(rawCurrent);
  #endif

  #ifdef LOG_TEST


  #endif

  

}





