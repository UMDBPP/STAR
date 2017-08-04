#include "EZGPIO.h"
#include<Wire.h>




#define TEST_ALL_SENSORS
//#define LOG_TEST

#ifdef TEST_ALL_SENSORS
  //#define ADXL_TEST
  //#define MAX_TEMP_TEST
  //#define POWER_SENSOR_TEST
  //#define BMX_ACCEL_TEST
  //#define BMX_GYRO_TEST
  #define BMX_MAG_TEST
#endif


int LEDPIN = 9;
int BIASPIN = 28;
int PIKILL = 16;
int EXTKILL = 13;
int SDCSPIN = 22;

#ifdef ADXL_TEST
  #include "src/Sensors/ADXL375.h"
  ADXL375 adxl;
#endif

#ifdef MAX_TEMP_TEST
  #include "src/Sensors/MAX31725Temp.h"
  MAX31725 tempSensor;
#endif

#ifdef POWER_SENSOR_TEST
  #include "src/Sensors/PowerSensors.h"
  VoltageSense voltageSensor;
  CurrentSense currentSensor;
#endif

#ifdef BMX_ACCEL_TEST
  #include "src/Sensors/BMX055.h"
  BMX055_ACCEL bmxAccel;
#endif


#ifdef BMX_GYRO_TEST
  #include "src/Sensors/BMX055.h"
  BMX055_GYRO bmxGyro;
#endif


#ifdef BMX_GYRO_TEST
  #include "src/Sensors/BMX055.h"
  BMX055_GYRO bmxGyro;
#endif

#ifdef BMX_MAG_TEST
  #include "src/Sensors/BMX055.h"
  BMX055_MAG bmxMag;
#endif

#ifdef LOG_TEST
  #include <SPI.h>
  #include<SD.h>
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
    delay(3000);
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Began");
  
  // Pull "Kill" ports high:
  DIRSET_A = (1 << PIKILL);
  OUTSET_A = (1 << PIKILL);
  DIRSET_A = (1 << EXTKILL);
  OUTSET_A = (1 << EXTKILL);

  DIRSET_B = (1 << SDCSPIN);
  OUTCLR_B = (1 << SDCSPIN);

#ifdef LOG_TEST
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

    uint8_t data = 1 << 3;;
    adxl.write_register(0x2D, data);
    adxl.read_register(0x2D , data);
    Serial.print("Register: 0x");
    Serial.println(data, HEX);

    
    int16_t rawXAcc = adxl.get_x_accel();
    int16_t rawYAcc = adxl.get_y_accel();
    int16_t rawZAcc = adxl.get_z_accel();
    
    Serial.print("Raw ADXL X acceleration: ");
    Serial.println(rawXAcc);
    Serial.print("Raw ADXL Y acceleration: ");
    Serial.println(rawYAcc);
    Serial.print("Raw ADXL Z acceleration: ");
    Serial.println(rawZAcc);

    Vector3<float> convertedAdxlAcceleration = adxl.convert_data();

    Serial.print("Converted ADXL X acceleration: ");
    Serial.println(convertedAdxlAcceleration.x);
    Serial.print("Converted ADXL Y acceleration: ");
    Serial.println(convertedAdxlAcceleration.y);
    Serial.print("Convertedaw ADXL Z acceleration: ");
    Serial.println(convertedAdxlAcceleration.z);
    
  #endif
  
  #ifdef MAX_TEMP_TEST
    
    tempSensor.recieve_data();
    uint8_t tempConfigRegister = tempSensor.read_config_register();
    uint16_t rawTemp = tempSensor.get_temperature();
    float convertedTemp = tempSensor.convert_temp();
    Serial.print("MAX31725 Configuration Register: ");
    Serial.println(tempConfigRegister);
    Serial.print("Raw MAX31725 temperature: ");
    Serial.println(rawTemp);
    Serial.print("Converted MAX31725 Temperature: ");
    Serial.println(convertedTemp);
  #endif 


  #ifdef POWER_SENSOR_TEST
    uint16_t rawVoltage = voltageSensor.read_voltage();
    uint16_t rawCurrent = currentSensor.read_current();
    Serial.print("Raw voltage: ");
    Serial.println(rawVoltage);
    Serial.print("Raw current: ");
    Serial.println(rawCurrent);
  #endif

  #ifdef BMX_ACCEL_TEST
    bmxAccel.recieve_data();
      
    int16_t bmxRawXAcc = bmxAccel.get_x_accel();
    int16_t bmxRawYAcc = bmxAccel.get_y_accel();
    int16_t bmxRawZAcc = bmxAccel.get_z_accel();
    int16_t bmxTemp = bmxAccel.get_temp();
    
    Serial.print("Raw BMX X acceleration: ");
    Serial.println(bmxRawXAcc);
    Serial.print("Raw BMX Y acceleration: ");
    Serial.println(bmxRawYAcc);
    Serial.print("Raw BMX Z acceleration: ");
    Serial.println(bmxRawZAcc);
    Serial.print("Raw BMX temp: ");
    Serial.println(bmxTemp);
  #endif

  #ifdef BMX_GYRO_TEST
    bmxGyro.recieve_data();
      
    int16_t xRate = bmxGyro.get_x_gyro();
    int16_t yRate = bmxGyro.get_y_gyro();
    int16_t zRate = bmxGyro.get_z_gyro();
    
    Serial.print("Raw BMX X rate: ");
    Serial.println(xRate);
    Serial.print("Raw BMX Y rate: ");
    Serial.println(yRate);
    Serial.print("Raw BMX Z rate: ");
    Serial.println(zRate);
  #endif

  #ifdef BMX_MAG_TEST
    bmxMag.recieve_data();
    uint8_t powerControlReg = 0x4B;
    uint8_t opReg = 0x4C;
    uint8_t powerConrolValue = 0x01;
    uint8_t val = 0x0;
    bmxMag.write_register(powerControlReg, powerConrolValue);

    uint8_t outVal = 255;
    Serial.println("==================");
        Serial.println(outVal);
        uint8_t result = bmxMag.read_register(powerControlReg, outVal);
    Serial.println(result);
    Serial.println(outVal);
      
    int16_t xMag = bmxMag.get_x_mag();
    int16_t yMag = bmxMag.get_y_mag();
    int16_t zMag = bmxMag.get_z_mag();
    
    Serial.print("Raw BMX X mag: ");
    Serial.println(xMag);
    Serial.print("Raw BMX Y mag: ");
    Serial.println(yMag);
    Serial.print("Raw BMX Z mag: ");
    Serial.println(zMag);
  #endif

  #ifdef LOG_TEST
   File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Herpaderp");
    dataFile.close();
    // print to the serial port too:
    Serial.println("Herpaderp"0);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  #endif

  delay(200);

}
