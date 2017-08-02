#ifndef _DRV8846_H
#define _DRV8846_H

#include "TCA9535.h"

// Macros for mapping DRV functions to pins/ports on the TCA
#define DRV_M0_PIN_TCA 1 // M0 on pin 1, port 1 of TCA
#define DRV_M1_PIN_TCA 0 // M1 on pin 0, port 1 of TCA
#define DRV_I0_PIN_TCA 7 // I0 on pin 7, port 1 of TCA
#define DRV_I1_PIN_TCA 6 // I1 on pin 6, port 1 of TCA
#define DRV_DEC0_PIN_TCA 5 // DEC0 on pin 5, port 1 of TCA
#define DRV_DEC1_PIN_TCA 3 // DEC1 on pin 3, port 1 of TCA
#define DRV_DIR_PIN_TCA 2 // DIR on pin 2, port 1 of TCA
#define DRV_NFAULT_PIN_TCA 6 // NFAULT on pin 6, port 0 of TCA
#define DRV_NENBL_PIN_TCA 7 // NENBL on pin 7, port 0 of TCA
#define DRV_NSLEEP_PIN_TCA 4 // NSLEEP on pin 4, port 1 of TCA

// NFAULT and NENBL on port 0 of TCA
#define DRV_NFAULT_PORT_TCA 0
#define DRV_NENBL_PORT_TCA 0

// M0, M1, I0, I1, DEC0, DEC1, NSLEEP, and DIR on port 1 of TCA
#define DRV_M0_M1_PORT_TCA 1
#define DRV_I0_I1_PORT_TCA 1
#define DRV_DEC0_DEC1_PORT_TCA 1
#define DRV_NSLEEP_PORT_TCA 1
#define DRV_DIR_PORT_TCA 1

// TODO
#define PWM_PER 1488 // 8 MHz/(168 step/s * 32 div/step)
#define DRV_GLCK_GEN_ID 4
#define DRV_GLCK_ID 0x1A // GCLK_TCC0 (datasheet table 15-5)
#define DRV_GCLK_DIV_FACTOR 6 // 48 MHz / 8 = 8 MHz
#define DRV_OUT_PORT 0 // Port A
#define DRV_OUT_PIN 8 // Pin 8 on port A (logical; physical pin is 13)

uint8_t setup_DRV();

void config_PWM();
void start_PWM();
void stop_PWM();

uint8_t enable_DRV_output(void);
uint8_t disable_DRV_output(void);
uint8_t wake_DRV(void);
uint8_t sleep_DRV(void);

uint8_t set_DRV_dir_forward(void);
uint8_t set_DRV_dir_reverse(void);

uint8_t set_DRV_curr_limit(uint8_t, uint8_t);
uint8_t set_DRV_step_mode(uint8_t, uint8_t);
uint8_t set_DRV_decay_time(uint8_t, uint8_t);

bool DRV_is_fault(void);
bool DRV_is_fwd(void);

//uint8_t process_three_level_two_register(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
uint8_t process_three_level_output(uint8_t, uint8_t, uint8_t);

#endif // ndef _DRV8846_H
