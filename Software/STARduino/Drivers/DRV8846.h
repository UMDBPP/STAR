#ifndef _DRV8846_H
#define _DRV8846_H

// Macros for mapping DRV functions to pins/ports on the TCA
#define DRV_M0_PIN_TCA 6 // M0 on pin 6, port 0 of TCA
#define DRV_M1_PIN_TCA 0 // M1 on pin 0, port 1 of TCA
#define DRV_I0_PIN_TCA 7 // I0 on pin 7, port 1 of TCA
#define DRV_I1_PIN_TCA 6 // I1 on pin 6, port 1 of TCA
#define DRV_DEC0_PIN_TCA 5 // DEC0 on pin 5, port 1 of TCA
#define DRV_DEC1_PIN_TCA 3 // DEC1 on pin 3, port 1 of TCA
#define DRV_DIR_PIN_TCA 2 // DIR on pin 2, port 1 of TCA
#define DRV_NFAULT_PIN_TCA 1 // NFAULT on pin 1, port 1 of TCA
#define DRV_NENBL_PIN_TCA 7 // NENBL on pin 7, port 0 of TCA
#define DRV_NSLEEP_PIN_TCA 4 // NSLEEP on pin 4, port 1 of TCA


void setup_DRV();

void enable_DRV(void);
void disable_DRV(void);
void wake_DRV(void);
void sleep_DRV(void);

void set_DRV_dir_forward(void);
void set_DRV_dir_reverse(void);

void set_DRV_curr_limit(uint8_t, uint8_t);
void set_DRV_step_mode(uint8_t, uint8_t);
void set_DRV_decay_time(uint8_t, uint8_t);

bool DRV_is_fault(void);
bool DRV_is_fwd(void);

bool set_DRV_freq(uint16_t);
bool stop_DRV();

#endif // ndef _DRV8846_H
