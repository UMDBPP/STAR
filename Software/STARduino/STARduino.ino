#define CUTOFF_TIME_MILLISECONDS 600000

#include "EZGPIO.h"
#include<Wire.h>
#include "src/Sensors/ADXL375.h"
#include "src/Sensors/MAX31725Temp.h"
#include "src/Sensors/PowerSensors.h"
#include "src/Sensors/BMX055.h"
#include <SPI.h>
#include<SD.h>


uint8_t LEDPIN = 9;
uint8_t BIASPIN = 28;
uint8_t PIKILL = 16;
uint8_t EXTKILL = 13;
uint8_t SDCSPIN = 22;

ADXL375 adxl;
MAX31725 tempSensor;
VoltageSense voltageSensor;
CurrentSense currentSensor;
BMX055_ACCEL bmxAccel;
BMX055_GYRO bmxGyro;
BMX055_MAG bmxMag;


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
  Wire.begin();
  
  for(int i = 0; i < 10; i ++)
  {
    Serial.print("Here: ");
    Serial.println(i);
    
  }
  
  
  Serial.println("Began");

  // Enable ESB; 
  
  DIRSET_A = (1 << EXTKILL);
  OUTSET_A = (1 << EXTKILL);
  DIRSET_A = (1 << LEDPIN);
  OUTSET_A = (1 << LEDPIN);
  DIRSET_A = (1 << BIASPIN);
  OUTSET_A = (1 << BIASPIN);

  DIRSET_B = (1 << SDCSPIN);
  
  OUTCLR_B = (1 << SDCSPIN);

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

  // Not all sensors require begin() functions
  adxl.begin();
  bmxMag.begin();

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println("MDSGC Rocksat-X Data Log");
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }

}

void loop() {
    

    uint32_t sensorReadingTime = millis();

    if(sensorReadingTime > CUTOFF_TIME_MILLISECONDS)
    {
      OUTCLR_A = (1 << LEDPIN);
      OUTCLR_A = (1 << EXTKILL);
      OUTCLR_A = (1 << BIASPIN);
    }
    
    uint16_t adcVoltageReading = voltageSensor.read_voltage();
    uint16_t adcCurrentReading = currentSensor.read_current();
  
    adxl.recieve_data();
    int16_t adxlAccelerometerXReading = adxl.get_x_accel();
    int16_t adxlAccelerometerYReading = adxl.get_y_accel();
    int16_t adxlAccelerometerZReading = adxl.get_z_accel();
    
    tempSensor.recieve_data();
    uint16_t maximTemperatureReading = tempSensor.get_temperature();
    
    bmxAccel.recieve_data();
    int16_t bmxAccelerometerXReading = bmxAccel.get_x_accel();
    int16_t bmxAccelerometerYReading = bmxAccel.get_y_accel();
    int16_t bmxAccelerometerZReading = bmxAccel.get_z_accel();
    int16_t bmxAccelerometerTemperatureReading = bmxAccel.get_temp();

    bmxGyro.recieve_data();
    int16_t bmxGyroscopeXReading = bmxGyro.get_x_gyro();
    int16_t bmxGyroscopeYReading = bmxGyro.get_y_gyro();
    int16_t bmxGyroscopeZReading = bmxGyro.get_z_gyro();

    bmxMag.recieve_data();
    int16_t bmxMagnetometerXReading = bmxMag.get_x_mag();
    int16_t bmxMagnetometerYReading = bmxMag.get_y_mag();
    int16_t bmxMagnetometerZReading =  bmxMag.get_z_mag();
    uint16_t bmxMagnetometerHallReading = bmxMag.get_hall();
    
    uint32_t portADir = PORT->Group[0].DIR.reg;
    uint32_t portBDir = PORT->Group[1].DIR.reg;
    uint32_t portAOut = PORT->Group[0].OUTSET.reg; // TODO HACK
    uint32_t portBOut = PORT->Group[1].OUTSET.reg; // TODO HACK
      
   File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(sensorReadingTime);
    dataFile.print(",");
    dataFile.print(portADir);
    dataFile.print(",");
    dataFile.print(portBDir);
    dataFile.print(",");
    dataFile.print(portAOut);
    dataFile.print(",");
    dataFile.print(portBOut);
    dataFile.print(",");
    dataFile.print(adcVoltageReading);
    dataFile.print(",");
    dataFile.print(adcCurrentReading);
    dataFile.print(",");
    dataFile.print(adxlAccelerometerXReading);
    dataFile.print(",");
    dataFile.print(adxlAccelerometerYReading);
    dataFile.print(",");
    dataFile.print(adxlAccelerometerZReading);
    dataFile.print(",");
    dataFile.print(maximTemperatureReading);
    dataFile.print(",");
    dataFile.print(bmxAccelerometerXReading);
    dataFile.print(",");
    dataFile.print(bmxAccelerometerYReading);
    dataFile.print(",");
    dataFile.print(bmxAccelerometerZReading);
    dataFile.print(",");
    dataFile.print(bmxAccelerometerTemperatureReading);
    dataFile.print(",");
    dataFile.print(bmxGyroscopeXReading);
    dataFile.print(",");
    dataFile.print(bmxGyroscopeYReading);
    dataFile.print(",");
    dataFile.print(bmxGyroscopeZReading);
    dataFile.print(",");
    dataFile.print(bmxMagnetometerXReading);
    dataFile.print(",");
    dataFile.print(bmxMagnetometerYReading);
    dataFile.print(",");
    dataFile.print(bmxMagnetometerZReading);
    dataFile.print(",");
    dataFile.print(bmxMagnetometerHallReading);
    dataFile.println();
    dataFile.close();
    
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  
}





