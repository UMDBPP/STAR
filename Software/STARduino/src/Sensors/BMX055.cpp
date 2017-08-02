#include <Wire.h>
#include "BMX055.h"

BMX055_ACCEL::BMX055_ACCEL() {
	Wire.begin();
	this->numSamples = BMX_MAX_SAMPLES;
}

BMX055_ACCEL::BMX055_ACCEL(uint8_t samples) {
	Wire.begin();
	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
		numSamples = samples;
	} else {
		numSamples = BMX_MAX_SAMPLES;
	}
}

uint8_t BMX055_ACCEL::read_register(uint8_t _addr, uint8_t& _data) {
	Wire.beginTransmission(BMX_ACCEL_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) { // Verify command byte was sent successfully
		return 1;
	}

	if(Wire.endTransmission()) { //Wire.endTransmission fails if not 1
		return 1;
	}

	Wire.requestFrom(BMX_ACCEL_ADDRESS, 1);
	_data = Wire.read();
	return 0;
}

uint8_t BMX055_ACCEL::write_register(uint8_t _addr, uint8_t& _value) {
	Wire.beginTransmission(BMX_ACCEL_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) {
		return 1;
	}
	bytesSent = Wire.write(_value);
	return (!bytesSent || Wire.endTransmission()); //byteSent should be 1, Wire.endTransmission should be 0
}

uint8_t BMX055_ACCEL::recieve_data() {
	//32 bit to prevent overflow
	uint32_t runningX = 0;
	uint32_t runningY = 0;
	uint32_t runningZ = 0;

	for(uint8_t i = 0; i < numSamples; i++) {

		Wire.beginTransmission(BMX_ACCEL_ADDRESS);

		while(Wire.available()) {
			Wire.read();
		}

		Wire.requestFrom(BMX_ACCEL_ADDRESS, 7);

		if(Wire.available() != 7) {
			return 1;
		}

		uint8_t x0 = Wire.read(); //BMX_ACCEL_ACCD_X_LSB
		uint8_t x1 = Wire.read(); //BMX_ACCEL_ACCD_X_MSB
		uint8_t y0 = Wire.read(); //BMX_ACCEL_ACCD_Y_LSB
		uint8_t y1 = Wire.read(); //BMX_ACCEL_ACCD_Y_MSB
		uint8_t z0 = Wire.read(); //BMX_ACCEL_ACCD_Z_LSB
		uint8_t z1 = Wire.read(); //BMX_ACCEL_ACCD_Z_MSB
		uint8_t temp = Wire.read(); //BMX_ACCEL_ACCD_TEMP
		uint8_t xOffset = Wire.read(); //BMX_ACCEL_OFC_X_OFFSET
		uint8_t yOffset = Wire.read(); //BMX_ACCEL_OFC_Y_OFFSET
		uint8_t zOffset = Wire.read(); //BMX_ACCEL_OFC_Z_OFFSET

		uint16_t x = ((x1 << 8) | (0xF & (x0 >> 4)));
		uint16_t y = ((y1 << 8) | (0xF & (y0 >> 4)));
		uint16_t z = ((z1 << 8) | (0xF & (z0 >> 4)));

		runningX += x;
		runningY += y;
		runningZ += z;
	}

	uint16_t x = runningX / numSamples;
	uint16_t y = runningY / numSamples;
	uint16_t z = runningZ / numSamples;

	rawAccel = Vector3<uint16_t>(x, y, z);
	offsetAccel = Vector3<uint16_t>(xOffset, yOffset, zOffset);

	return 0;
}

Vector3<float> BMX055_ACCEL::convert_data() {
	float x_float = BMX_ACCEL_CONVERSION_FACTOR*this->rawAccel.x - BMX_ACCEL_CONVERSION_FACTOR*this->offsetAccel.x;
	float y_float = BMX_ACCEL_CONVERSION_FACTOR*this->rawAccel.y - BMX_ACCEL_CONVERSION_FACTOR*this->offsetAccel.y;
	float z_float = BMX_ACCEL_CONVERSION_FACTOR*this->rawAccel.z - BMX_ACCEL_CONVERSION_FACTOR*this->offsetAccel.z;

	return Vector3<float>(x_float, y_float, z_float);
}

uint8_t BMX055_ACCEL::set_sample_rate(uint8_t rate) {
	rate = rate & 0b1111; //Rate is only four bits

	uint8_t register_val;

	uint8_t return_code = read_register(BMX_ACCEL_PMU_BW, register_val); //Read existing register value

	if(return_code != 0) {
		return return_code;
	}

	register_val &= ~(0b1111); //Unset bottom four bits to set new rate
	register_val |= rate;

	return_code = write_register(BMX_ACCEL_PMU_BW, register_val);

	return return_code;
}

uint8_t BMX055_ACCEL::set_accel_resolution(uint8_t rate) {
	rate = rate & 0b1111; //Rate is only four bits

	uint8_t register_val;

	uint8_t return_code = read_register(BMX_ACCEL_PMU_RANGE, register_val); //Read existing register value

	if(return_code != 0) {
		return return_code;
	}

	register_val &= ~(0b1111); //Unset bottom four bits to set new rate
	register_val |= rate;

	return_code = write_register(BMX_ACCEL_PMU_RANGE, register_val);

	//not sure exactly how return_code will return, but this sets the conversion factor for accelerometer
	if return_code == 0b0011 {
		BMX_ACCEL_CONVERSION_FACTOR = .0098f //.98 mg / LSB
	}

	else if return_code == 0b0101 {
		BMX_ACCEL_CONVERSION_FACTOR = .0195f // 1.95 mg / LSB
	}

	else if return_code == 0b1000 {
		BMX_ACCEL_CONVERSION_FACTOR = .0391f //3.91 mg / LSB
	}

	else if return_code == 0b1100 {
		BMX_ACCEL_CONVERSION_FACTOR = .0781f //7.81 mg / LSB
	}

	return return_code;
}

/*
 * Set new supersampling value. Includes bounds check.
 *
 * Inputs: New number of samples for supersampling.
 */
 void BMX055_ACCEL::set_number_samples(uint8_t samples) {
 	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
 		numSamples = samples;
 	} 
 	else {
 		numSamples = numSamples;
 	}
 }








 BMX055_GYRO::BMX055_GYRO() {
	Wire.begin();
	this->numSamples = BMX_MAX_SAMPLES;
}

BMX055_GYRO::BMX055_GYRO(uint8_t samples) {
	Wire.begin();
	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
		numSamples = samples;
	} else {
		numSamples = BMX_MAX_SAMPLES;
	}
}

uint8_t BMX055_GYRO::read_register(uint8_t _addr, uint8_t& _data) {
	Wire.beginTransmission(BMX_GYRO_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) { // Verify command byte was sent successfully
		return 1;
	}

	if(Wire.endTransmission()) { //Wire.endTransmission fails if not 1
		return 1;
	}

	Wire.requestFrom(BMX_GYRO_ADDRESS, 1);
	_data = Wire.read();
	return 0;
}

uint8_t BMX055_GYRO::write_register(uint8_t _addr, uint8_t& _value) {
	Wire.beginTransmission(BMX_GYRO_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) {
		return 1;
	}
	bytesSent = Wire.write(_value);
	return (!bytesSent || Wire.endTransmission()); //byteSent should be 1, Wire.endTransmission should be 0
}

uint8_t BMX055_GYRO::recieve_data() {
	//32 bit to prevent overflow
	uint32_t runningX = 0;
	uint32_t runningY = 0;
	uint32_t runningZ = 0;

	for(uint8_t i = 0; i < numSamples; i++) {

		Wire.beginTransmission(BMX_GYRO_ADDRESS);

		while(Wire.available()) {
			Wire.read();
		}

		Wire.requestFrom(BMX_GYRO_ADDRESS, 7);

		if(Wire.available() != 7) {
			return 1;
		}

		uint8_t x0 = Wire.read(); //BMX_GYRO_RATE_X_LSB
		uint8_t x1 = Wire.read(); //BMX_GYRO_RATE_X_MSB
		uint8_t y0 = Wire.read(); //BMX_GYRO_RATE_Y_LSB
		uint8_t y1 = Wire.read(); //BMX_GYRO_RATE_Y_MSB
		uint8_t z0 = Wire.read(); //BMX_GYRO_RATE_Z_LSB
		uint8_t z1 = Wire.read(); //BMX_GYRO_RATE_Z_MSB
\
		uint16_t x = ((x1 << 8) | x0);
		uint16_t y = ((y1 << 8) | y0);
		uint16_t z = ((z1 << 8) | z0);

		runningX += x;
		runningY += y;
		runningZ += z;
	}

	uint16_t x = runningX / numSamples;
	uint16_t y = runningY / numSamples;
	uint16_t z = runningZ / numSamples;

	rawGyro = Vector3<uint16_t>(x, y, z);

	return 0;
}

Vector3<float> BMX055_GYRO::convert_data() {
	float x_float = BMX_GYRO_CONVERSION_FACTOR*this->rawGyro.x;
	float y_float = BMX_GYRO_CONVERSION_FACTOR*this->rawGyro.y;
	float z_float = BMX_GYRO_CONVERSION_FACTOR*this->rawGyro.z;

	return Vector3<float>(x_float, y_float, z_float);
}

uint8_t BMX055_ACCEL::set_sample_rate(uint8_t rate) {
	rate = rate & 0b1111; //Rate is only four bits

	uint8_t register_val;

	uint8_t return_code = read_register(BMX_GYRO_BW, register_val); //Read existing register value

	if(return_code != 0) {
		return return_code;
	}

	register_val &= ~(0b1111); //Unset bottom four bits to set new rate
	register_val |= rate;

	return_code = write_register(BMX_GYRO_BW, register_val);

	return return_code;
}

/*
 * Set new supersampling value. Includes bounds check.
 *
 * Inputs: New number of samples for supersampling.
 */
 void BMX055_ACCEL::set_number_samples(uint8_t samples) {
 	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
 		numSamples = samples;
 	} 
 	else {
 		numSamples = numSamples;
 	}
 }










 BMX055_MAG::BMX055_MAG() {
	Wire.begin();
	this->numSamples = BMX_MAX_SAMPLES;
}

BMX055_MAG::BMX055_MAG(uint8_t samples) {
	Wire.begin();
	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
		numSamples = samples;
	} else {
		numSamples = BMX_MAX_SAMPLES;
	}
}

uint8_t BMX055_MAG::read_register(uint8_t _addr, uint8_t& _data) {
	Wire.beginTransmission(BMX_MAG_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) { // Verify command byte was sent successfully
		return 1;
	}

	if(Wire.endTransmission()) { //Wire.endTransmission fails if not 1
		return 1;
	}

	Wire.requestFrom(BMX_MAG_ADDRESS, 1);
	_data = Wire.read();
	return 0;
}

uint8_t BMX055_MAG::write_register(uint8_t _addr, uint8_t& _value) {
	Wire.beginTransmission(BMX_MAG_ADDRESS);
	uint8_t bytesSent = Wire.write(_addr);
	if(bytesSent != 1) {
		return 1;
	}
	bytesSent = Wire.write(_value);
	return (!bytesSent || Wire.endTransmission()); //byteSent should be 1, Wire.endTransmission should be 0
}

uint8_t BMX055_MAG::recieve_data() {
	//32 bit to prevent overflow
	uint32_t runningX = 0;
	uint32_t runningY = 0;
	uint32_t runningZ = 0;

	for(uint8_t i = 0; i < numSamples; i++) {

		Wire.beginTransmission(BMX_MAG_ADDRESS);

		while(Wire.available()) {
			Wire.read();
		}

		Wire.requestFrom(BMX_MAG_ADDRESS, 7);

		if(Wire.available() != 7) {
			return 1;
		}

		uint8_t x0 = Wire.read(); //BMX_MAG_X_LSB
		uint8_t x1 = Wire.read(); //BMX_MAG_X_MSB
		uint8_t y0 = Wire.read(); //BMX_MAG_Y_LSB
		uint8_t y1 = Wire.read(); //BMX_MAG_Y_MSB
		uint8_t z0 = Wire.read(); //BMX_MAG_Z_LSB
		uint8_t z1 = Wire.read(); //BMX_MAG_Z_MSB
		uint8_t hall0 = Wire.read(); //BMX_MAG_RHALL_LSB
		uint8_t hall1 = Wire.read(); //BMX_MAG_RHALL_MSB

		uint16_t x = ((x1 << 8) | x0);
		uint16_t y = ((y1 << 8) | y0);
		uint16_t z = ((z1 << 8) | z0);
		uint16_t hall = ((hall1 << 8) | hall0);

		runningX += x;
		runningY += y;
		runningZ += z;
		runningHall += hall;
	}

	uint16_t x = runningX / numSamples;
	uint16_t y = runningY / numSamples;
	uint16_t z = runningZ / numSamples;
	uint16_t hall = runningHall / numSamples;

	rawMag = Vector3<uint16_t>(x, y, z);

	return 0;
}

Vector3<float> BMX055_MAG::convert_data() {
	//Haven't found how the Hall factor plays into this, but it does
	float x_float = BMX_MAG_CONVERSION_FACTOR*this->rawMag.x;
	float y_float = BMX_MAG_CONVERSION_FACTOR*this->rawMag.y;
	float z_float = BMX_MAG_CONVERSION_FACTOR*this->rawMag.z;

	return Vector3<float>(x_float, y_float, z_float);
}

uint8_t BMX055_ACCEL::set_sample_rate(uint8_t rate) {
	rate = rate & 0b1111; //Rate is only four bits

	uint8_t register_val;

	uint8_t return_code = read_register(BMX_MAG_RATE, register_val); //Read existing register value

	if(return_code != 0) {
		return return_code;
	}

	register_val &= ~(0b1111); //Unset bottom four bits to set new rate
	register_val |= rate;

	return_code = write_register(BMX_MAG_RATE, register_val);

	return return_code;
}

/*
 * Set new supersampling value. Includes bounds check.
 *
 * Inputs: New number of samples for supersampling.
 */
 void BMX055_ACCEL::set_number_samples(uint8_t samples) {
 	if ((samples <= BMX_MAX_SAMPLES) && (samples > 0)) {
 		numSamples = samples;
 	} 
 	else {
 		numSamples = numSamples;
 	}
 }