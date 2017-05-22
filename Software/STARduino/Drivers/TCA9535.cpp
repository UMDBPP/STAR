#include "TCA9535.h"

/*
 * Reads the TCA9535 register corresponding to the command byte; returns the value in the register
 * 
 * Inputs:
 * command_byte: the command byte of the register in question, as defined by the TCA9535 data sheet
 *
 * Output:
 * result: the value read from the TCA9535 register
 *
 * Return:
 * Meaning is dependent on the register identified by the command byte; refer to the TCA9535 datasheet.
 */
uint8_t read_TCA_register(uint8_t command_byte) {
    Wire.beginTransmission(TCA_ADDRESS);
    Wire.send(command_byte);
    Wire.requestFrom(TCA_ADDRESS,1);
    uint8_t result = Wire.read();
    Wire.endTransmission();
    return result;
}

/*
 * Sets the TCA9535 register corresponding to the command byte to the given data value
 * 
 * Inputs:
 * command_byte: the command byte of the register in question, as defined by the TCA9535 data sheet
 * data: the byte to write to the register
 *
 * Output:
 * none
 */
 void write_TCA_register(uint8_t command_byte, uint8_t data) {
    Wire.beginTransmission(TCA_ADDRESS);
    Wire.send(command_byte);
    Wire.send(data);
    Wire.endTransmission();
}

/*
 * Reads the TCA9535 input 0 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 input 0 register
 *
 * Return:
 * An 8-bit bitfield representing the logical values on each of the 8 pins on the TCA's input 0 port
 */
uint8_t read_TCA_input_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_IN_0);
}

/*
 * Reads the TCA9535 input 1 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 input 1 register
 *
 * Return:
 * An 8-bit bitfield representing the logical values on each of the 8 pins on the TCA's input 1 port
 */
uint8_t read_TCA_input_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_IN_1);
}

/*
 * Reads the TCA9535 output 0 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 output 0 register
 *
 * Return:
 * An 8-bit bitfield representing the (commanded) logical values on each of the 8 pins on the TCA's output 0 port
 */
uint8_t read_TCA_output_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_OUT_0);
}

/*
 * Reads the TCA9535 output 1 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 output 1 register
 *
 * Return:
 * An 8-bit bitfield representing the (commanded) logical values on each of the 8 pins on the TCA's output 1 port
 */
 uint8_t read_TCA_output_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_OUT_1);
}

/*
 * Reads the TCA9535 configuration 0 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 configuration 0 register
 *
 * Return:
 * An 8-bit bitfield representing whether each pin on the TCA's port 0 is an input (1) or an output (0)
 */
 uint8_t read_TCA_config_0(void) {
    return read_TCA_register(TCA_CMD_BYTE_CFG_0);
}

/*
 * Reads the TCA9535 configuration 1 register; returns the value in the register
 * 
 * Inputs:
 * none
 *
 * Output:
 * result: the value read from the TCA9535 configuration 1 register
 *
 * Return:
 * An 8-bit bitfield representing whether each pin on the TCA's port 1 is an input (1) or an output (0)
 */uint8_t read_TCA_config_1(void) {
    return read_TCA_register(TCA_CMD_BYTE_CFG_0);
}

/*
 * Sets the TCA9535 output 0 register to the given data value
 * 
 * Inputs:
 * data: the byte to write to the register
 *
 * Output:
 * none
 */
void write_TCA_output_0(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_OUT_0, data);
}

/*
 * Sets the TCA9535 output 1 register to the given data value
 * 
 * Inputs:
 * data: the byte to write to the register
 *
 * Output:
 * none
 */
void write_TCA_output_1(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_OUT_1, data);
}

/*
 * Sets the TCA9535 configuration 0 register to the given data value
 * 
 * Inputs:
 * data: the byte to write to the register
 *
 * Output:
 * none
 */
void write_TCA_config_0(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_CFG_0, data);
}

/*
 * Sets the TCA9535 configuration 1 register to the given data value
 * 
 * Inputs:
 * data: the byte to write to the register
 *
 * Output:
 * none
 */
 void write_TCA_config_1(uint8_t data) {
    write_TCA_register(TCA_CMD_BYTE_CFG_1, data);
}
