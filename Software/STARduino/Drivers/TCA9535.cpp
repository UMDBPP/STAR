#include "TCA9535.h"

// Reads the TCA register corresponding to the command byte; returns the value in the register
uint8_t read_TCA_register(uint8_t command_byte) {
    Wire.beginTransmission(TCA_ADDRESS);
    Wire.send(command_byte);
    Wire.requestFrom(TCA_ADDRESS,1);
    uint8_t result = Wire.read();
    Wire.endTransmission();
    return result;
}

// Writes the given data byte to the TCA register corresponding to the command byte
void write_TCA_register(uint8_t command_byte, uint8_t data) {
    Wire.beginTransmission(TCA_ADDRESS);
    Wire.send(command_byte);
    Wire.send(data);
    Wire.endTransmission();
}

// Returns the value in the TCA input 0 register
uint8_t read_TCA_input_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_IN_0);
}

// Returns the value in the TCA input 1 register
uint8_t read_TCA_input_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_IN_1);
}

// Returns the value in the TCA output 0 register
uint8_t read_TCA_output_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_OUT_0);
}

// Returns the value in the TCA output 1 register
uint8_t read_TCA_output_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_OUT_1);
}

// Returns the value in the TCA configuration 0 register
uint8_t read_TCA_config_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_CFG_0);
}

// Returns the value in the TCA configuration 1 register
uint8_t read_TCA_config_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_CFG_0);
}

// Sets the value in the TCA output 0 register
void write_TCA_output_0(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_OUT_0, data);
}

// Sets the value in the TCA output 1 register
void write_TCA_output_1(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_OUT_1, data);
}

// Sets the value in the TCA configuration 0 register
void write_TCA_config_0(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_CFG_0, data);
}

// Sets the value in the TCA configuration 1 register
void write_TCA_config_1(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_CFG_1, data);
}