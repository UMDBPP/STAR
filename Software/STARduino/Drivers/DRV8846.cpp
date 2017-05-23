#include "DRV8846.h"

void setup_DRV();

void enable_DRV(void) {
    uint8_t port_0 = read_TCA_output_0();
    port_0 &= (~1 << DRV_NENBL_PIN_TCA);
    
    
}

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