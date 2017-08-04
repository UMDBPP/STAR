#ifndef BMX_H
#define BMX_H

//Not sure if they need to be included or not, so they're here if they should be
#include <stdint.h>
#include <Arduino.h>
#include "../DataStructures/Vector.h"

#define BMX_MAX_SAMPLES 5 //Maximum samples for supersampling

//I2C Addresses
#define BMX_ACCEL_ADDRESS 0x18
#define BMX_GYRO_ADDRESS 0x68
#define BMX_MAG_ADDRESS 0x10

#define BMX_ACCEL_DEVICE_ID 0xfa
#define BMX_GYRO_DEVICE_ID 0x0f
#define BMX_MAG_DEVICE_ID 0x32

//Register definitions
//Accelerometer
#define BMX_ACCEL_ID 0x00
#define BMX_ACCEL_ACCD_X_LSB 0x02
#define BMX_ACCEL_ACCD_X_MSB 0x03
#define BMX_ACCEL_ACCD_Y_LSB 0x04
#define BMX_ACCEL_ACCD_Y_MSB 0x05
#define BMX_ACCEL_ACCD_Z_LSB 0x06
#define BMX_ACCEL_ACCD_Z_MSB 0x07
#define BMX_ACCEL_ACCD_TEMP 0x08
#define BMX_ACCEL_INT_STATUS_0 0x09
#define BMX_ACCEL_INT_STATUS_1 0x0a
#define BMX_ACCEL_INT_STATUS_2 0x0b
#define BMX_ACCEL_INT_STATUS_3 0X0c
#define BMX_ACCEL_FIFO_STATUS 0x0e
#define BMX_ACCEL_PMU_RANGE 0x0f
#define BMX_ACCEL_PMU_BW 0x10
#define BMX_ACCEL_PMU_LPW 0x11
#define BMX_ACCEL_PMU_LOW_PWR 0x12
#define BMX_ACCEL_ACCD_HBW 0x13
#define BMX_ACCEL_BGW_SOFTRESET 0x14
#define BMX_ACCEL_INT_EN_0 0x16
#define BMX_ACCEL_INT_EN_1 0x17
#define BMX_ACCEL_INT_EN_2 0x18
#define BMX_ACCEL_INT_MAP_0 0x19
#define BMX_ACCEL_INT_MAP_1 0x1a
#define BMX_ACCEL_INT_MAP_2 0x1b
#define BMX_ACCEL_INT_SRC 0x1e
#define BMX_ACCEL_INT_OUT_CTRL 0x20
#define BMX_ACCEL_INT_RST_LATCH 0x21
#define BMX_ACCEL_INT_0 0x22
#define BMX_ACCEL_INT_1 0x23
#define BMX_ACCEL_INT_2 0x24
#define BMX_ACCEL_INT_3 0x25
#define BMX_ACCEL_INT_4 0x26
#define BMX_ACCEL_INT_5 0x27
#define BMX_ACCEL_INT_6 0x28
#define BMX_ACCEL_INT_7 0x29
#define BMX_ACCEL_INT_8 0x2a
#define BMX_ACCEL_INT_9 0x2b
#define BMX_ACCEL_INT_A 0x2c
#define BMX_ACCEL_INT_B 0x2d
#define BMX_ACCEL_INT_C 0x2e
#define BMX_ACCEL_INT_D 0x2f
#define BMX_ACCEL_FIFO_CONFIG_0 0x30
#define BMX_ACCEL_PMU_SELF_TEST 0x32
#define BMX_ACCEL_TRIM_NVM_CTRL 0x33
#define BMX_ACCEL_BGW_SPI3_WDT 0x34
#define BMX_ACCEL_OFC_CTRL 0x36
#define BMX_ACCEL_OFC_SETTING 0x37
#define BMX_ACCEL_OFC_X_OFFSET 0x38
#define BMX_ACCEL_OFC_Y_OFFSET 0x39
#define BMX_ACCEL_OFC_Z_OFFSET 0x3a
#define BMX_ACCEL_TRIM_GP0 0x3c
#define BMX_ACCEL_TRIM_GP1 0x3c
#define BMX_ACCEL_FIFO_CONFIG_1 0x3e
#define BMX_ACCEL_FIFO_DATA 0x3f

//Gyroscope 
#define BMX_GYRO_ID 0x00
#define BMX_GYRO_RATE_X_LSB 0x02
#define BMX_GYRO_RATE_X_MSB 0x03
#define BMX_GYRO_RATE_Y_LSB 0x04
#define BMX_GYRO_RATE_Y_MSB 0x05
#define BMX_GYRO_RATE_Z_LSB 0x06
#define BMX_GYRO_RATE_Z_MSB 0x07
#define BMX_GYRO_INT_STATUS_0 0x09
#define BMX_GYRO_INT_STATUS_1 0x0a
#define BMX_GYRO_INT_STATUS_2 0x0b
#define BMX_GYRO_INT_STATUS_3 0x0c
#define BMX_GYRO_FIFO_STATUS 0x0e
#define BMX_GYRO_RANGE 0x0f
#define BMX_GYRO_BW 0x10
#define BMX_GYRO_LMP1 0x11
#define BMX_GYRO_LMP2 0x12
#define BMX_GYRO_RATE_HBW 0x13
#define BMX_GYRO_BGW_SOFTRESET 0x14
#define BMX_GYRO_INT_EN_0 0x15
#define BMX_GYRO_INT_EN_1 0x16
#define BMX_GYRO_INT_MAP_0 0x17
#define BMX_GYRO_INT_MAP_1 0x18
#define BMX_GYRO_INT_MAP_2 0X19
#define BMX_GYRO_INT_RST_LATCH 0x21
#define BMX_GYRO_HIGH_X_TH 0x22
#define BMX_GYRO_HIGH_X_DUR 0x23
#define BMX_GYRO_HIGH_Y_TH 0x24
#define BMX_GYRO_HIGH_Y_DUR 0x25
#define BMX_GYRO_HIGH_Z_TH 0x26
#define BMX_GYRO_HIGH_Z_DUR 0x27
#define BMX_GYRO_SOC 0x31
#define BMX_GYRO_A_FOC 0x32 
#define BMX_GYRO_TRIM_NVM_CTRL 0x33
#define BMX_GYRO_BGW_SPI3_WDT 0x34
#define BMX_GYRO_OFC1 0x36
#define BMX_GYRO_OFC2 0x37
#define BMX_GYRO_OFC3 0x38
#define BMX_GYRO_OFC4 0x39
#define BMX_GYRO_TRIM_GP0 0x3a
#define BMX_GYRO_TRIM_GP1 0x3b
#define BMX_GYRO_BIST 0x3c
#define BMX_GYRO_FIFO_CONFIG_0 0x3d
#define BMX_GYRO_FIFO_CONFIG_1 0x3e

//Magnetometer
#define BMX_MAG_Z_REP 0x52
#define BMX_MAG_XY_REP 0x51
#define BMX_MAG_HIGH_THRESH 0x50
#define BMX_MAG_LOW_THRESH 0x4f

//Not quite so sure what to name these ones, every (few) byte has a different value
#define BMX_MAG_CHANNEL_XYZ 0x4e
#define BMX_MAG_HIGH_INT 0x4d
#define BMX_MAG_RATE 0x4c
#define BMX_MAG_PWR_CTRL_RST_SPI 0x4b
#define BMX_MAG_INTERRUPT 0x4a

#define BMX_MAG_RHALL_MSB 0x49
#define BMX_MAG_RHALL_LSB 0x48
#define BMX_MAG_Z_MSB 0x47
#define BMX_MAG_Z_LSB 0x46
#define BMX_MAG_Y_MSB 0x45
#define BMX_MAG_Y_LSB 0x44
#define BMX_MAG_X_MSB 0x43
#define BMX_MAG_X_LSB 0x42
#define BMX_MAG_ID 0x40

//Sample Rates

//Accelerometer
//I'm not quite sure these are the right things. but I didn't find anything clser
//Set bandwidth by changing te 0x10 register bit 
#define BMX_ACCEL_BANDWIDTH_7_81Hz 0b1000
#define BMX_ACCEL_BANDWIDTH_15_63Hz 0b1001
#define BMX_ACCEL_BANDWIDTH_31_25Hz 0b1010
#define BMX_ACCEL_BANDWIDTH_62_5Hz 0b1011
#define BMX_ACCEL_BANDWIDTH_125Hz 0b1100
#define BMX_ACCEL_BANDWIDTH_250Hz 0b1101
#define BMX_ACCEL_BANDWIDTH_500Hz 0b1110
#define BMX_ACCEL_BANDWIDTH_1000Hz 0b1111

//Gyroscope
#define BMX_GYRO_SAMPLE_100Hz_BANDWIDTH_32Hz 0x0111
#define BMX_GYRO_SAMPLE_200Hz_BANDWIDTH_64Hz 0x0110
#define BMX_GYRO_SAMPLE_100Hz_BANDWIDTH_12Hz 0x0101
#define BMX_GYRO_SAMPLE_200Hz_BANDWIDTH_23Hz 0x0100
#define BMX_GYRO_SAMPLE_400Hz_BANDWIDTH_47Hz 0x0011
#define BMX_GYRO_SAMPLE_1000Hz_BANDWIDTH_116Hz 0X0010
#define BMX_GYRO_SAMPLE_2000Hz_BANDWIDTH_230Hz 0X0001
#define BMX_GYRO_SAMPLE_2000Hz_BANDWIDTH_523Hz 0X0000

//Magnetometer 
#define BMX_MAG_SAMPLE_10Hz 0x000b
#define BMX_MAG_SAMPLE_2Hz 0x001b
#define BMX_MAG_SAMPLE_6Hz 0x010b
#define BMX_MAG_SAMPLE_8Hz 0x011b
#define BMX_MAG_SAMPLE_15Hz 0x100b
#define BMX_MAG_SAMPLE_20Hz 0x101b
#define BMX_MAG_SAMPLE_25Hz 0x110b
#define BMX_MAG_SAMPLE_30Hz 0x111b

//Converstion factors
#define BMX_ACCEL_TEMP_CONVERSION_FACTOR .5f //.5K / LSB
//This gets defined as a ratio between decimal value and angular rate. Not sure what it needs to be in, even though that doesn't seem right either
#define BMX_GYRO_CONVERSION_FACTOR .061f //degrees/sec, based on the decimal value in the register
//Can't honestly tell if this is being done at the BMX055 software level or if we need it, so I'm putting it in here
#define BMX_MAG_CONVERSION_FACTOR .0625f //.0625uT / LSB 

#define BMX_ACCEL_CONVERSION_FACTOR .0098f; // TODO

//Class for interacting with the BMX055 Accelerometer
//Provides methods for setup and reading data, as well as converisons
class BMX055_ACCEL {
	public:
		BMX055_ACCEL();
		BMX055_ACCEL(uint8_t);
		~BMX055_ACCEL();

		//Exposed single-byte read/write functions
		uint8_t read_register(uint8_t _addr, uint8_t& _data);
		uint8_t write_register(uint8_t _addr, uint8_t& _value);
		
		uint8_t recieve_data();
		Vector3<float> convert_data();

		uint8_t set_sample_rate(uint8_t);
		uint8_t set_accel_resolution(uint8_t);

		void set_number_samples(uint8_t);

		//getters
		int16_t get_x_accel() {return rawAccel.x;}
		int16_t get_y_accel() {return rawAccel.y;}
		int16_t get_z_accel() {return rawAccel.z;}
        int16_t get_temp() {return temp;}
       
		Vector3<int16_t> get_offset() {return offsetAccel;}
		Vector3<int16_t> get_accel() {return rawAccel;}

	private:
		Vector3<int16_t> rawAccel;
		uint8_t numSamples;
		Vector3<int16_t> offsetAccel;
        int16_t temp;
};	


//Class for interacting with the BMX055 Magnetometer
//Provides methods for setup and reading data, as well as converisons
class BMX055_GYRO {
	public:
		BMX055_GYRO();
		BMX055_GYRO(uint8_t);
		~BMX055_GYRO();

		// Exposed single-byte read/write functions
		uint8_t read_register(uint8_t _addr, uint8_t& _data);
		uint8_t write_register(uint8_t _addr, uint8_t& _value);

		uint8_t recieve_data();
		Vector3<float> convert_data();

		uint8_t set_sample_rate(uint8_t);

		void set_number_samples(uint8_t);

		//Getters

		int16_t get_x_gyro() {return rawGyro.x;}
		int16_t get_y_gyro() {return rawGyro.y;}
		int16_t get_z_gyro() {return rawGyro.z;}
		Vector3<int16_t> get_gyro() {return rawGyro;}

	private:
		Vector3<int16_t> rawGyro;
		uint8_t numSamples;
};


class BMX055_MAG {
	public:
		BMX055_MAG();
		BMX055_MAG(uint8_t);
		~BMX055_MAG();
        
        void begin();

		// Exposed single-byte read/write functions
		uint8_t read_register(uint8_t _addr, uint8_t& _data);
		uint8_t write_register(uint8_t _addr, uint8_t& _value);

		uint8_t recieve_data();
		Vector3<float> convert_data();

		uint8_t set_sample_rate(uint8_t);

		void set_number_samples(uint8_t);

		//Getters

		int16_t get_x_mag() {return rawMag.x;}
		int16_t get_y_mag() {return rawMag.y;}
		int16_t get_z_mag() {return rawMag.z;}
		Vector3<int16_t> get_mag() {return rawMag;}
        uint16_t get_hall() {return hall;}

	private:
		Vector3<int16_t> rawMag;
		uint8_t numSamples;
		uint16_t hall;
};
#endif //BMX_H