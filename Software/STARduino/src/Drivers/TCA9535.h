#ifndef _TCA9535_H
#define _TCA9535_H

#include<Wire.h>

// I2C address of the TCA9535
#define TCA_ADDRESS 0x27

// Command byte definitions from the TCA9535 datasheet
#define TCA_CMD_BYTE_IN_0 0x00 // Command byte for input port 0
#define TCA_CMD_BYTE_IN_1 0x01 // Command byte for input port 1
#define TCA_CMD_BYTE_OUT_0 0x02 // Command byte for output port 0
#define TCA_CMD_BYTE_OUT_1 0x03 // Command byte for output port 0
#define TCA_CMD_BYTE_CFG_0 0x06 // Command byte for configuration register 0
#define TCA_CMD_BYTE_CFG_1 0x07 // Command byte for configuration register 0

uint8_t read_TCA_register(uint8_t);

uint8_t read_TCA_input_0(void);
uint8_t read_TCA_input_1(void);

uint8_t read_TCA_output_0(void);
uint8_t read_TCA_output_1(void);

uint8_t read_TCA_config_0(void);
uint8_t read_TCA_config_1(void);

uint8_t write_TCA_register(uint8_t, uint8_t);;

uint8_t write_TCA_output_0(uint8_t);
uint8_t write_TCA_output_1(uint8_t);

uint8_t write_TCA_config_0(uint8_t);
uint8_t write_TCA_config_1(uint8_t);

uint8_t set_TCA_output_pin_high (uint8_t, uint8_t);
uint8_t set_TCA_output_pin_low (uint8_t, uint8_t);

uint8_t set_TCA_config_pin_high (uint8_t, uint8_t);
uint8_t set_TCA_config_pin_low (uint8_t, uint8_t);

#endif // ndef _TCA9535_H