#include "DRV8846.h"
//struct tc_module tc_instance; // TODO

uint8_t setup_DRV() {
    
    disable_DRV_output(); // For safety
    sleep_DRV(); // For safety
    set_DRV_dir_forward();
    if(set_DRV_decay_time(1, 1)) { // Both high for AutoTune
        return 1; // Return if error
    }
    
    if(set_DRV_curr_limit(255, 255)) { // Both Hi-Z for 50% limit (0.25 A)
        return 1; // Return if error
    }
    
    if(set_DRV_step_mode(255, 1)) { // Hi-Z, 1 for 32 microstep, rising edge only
        return 1; // Return if error
    }
    /*
    // TC setup code, from Atmel ASF examples
    struct tc_config config_tc;
    tc_get_config_defaults(&config_tc);
    config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
    config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
    config_tc.counter_16_bit.compare_capture_channel[0] = 1488; // 8 MHz clock frequency, 168 step/s, 32 microstep/step
    config_tc.pwm_channel[0].enabled = true;
    config_tc.pwm_channel[0].pin_out = PWM_OUT_PIN;
    config_tc.pwm_channel[0].pin_mux = PWM_OUT_MUX;
    if(tc_init(&tc_instance, PWM_MODULE, &config_tc) != STATUS_OK) {
        return 1; // Return if error
    }
    tc_enable(&tc_instance); // Start pwn train running
    */
    wake_DRV(); // Wake up, but output is still disabled
    
    return DRV_is_fault(); // Check DRV Fault status
}

/*
 * Enables the DRV8846 output stage.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t enable_DRV_output(void) {
    // Enable low, so unset to enable
    return set_TCA_output_pin_low(DRV_NENBL_PORT_TCA, DRV_NENBL_PIN_TCA);
}

/*
 * Disables the DRV8846 output stage; does not disable internal indexer.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t disable_DRV_output(void) {
    // Enable low, so set to disable
    return set_TCA_output_pin_high(DRV_NENBL_PORT_TCA, DRV_NENBL_PIN_TCA);
}

/*
 * Wakes the DRV8846.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t wake_DRV(void) {
    // Enable low, so unset to wake
    return set_TCA_output_pin_low(DRV_NSLEEP_PORT_TCA, DRV_NSLEEP_PIN_TCA);
}

/*
 * Puts the DRV8846 in sleep mode.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t sleep_DRV(void) {
    // Enable low, so set to sleep
    return set_TCA_output_pin_high(DRV_NSLEEP_PORT_TCA, DRV_NSLEEP_PIN_TCA);
}

/*
 * Sets the DRV8846 drive direction to forward.
 * The direction of forward has no physical significance, but it is gauranteed to be opposite of reverse.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t set_DRV_dir_forward(void) {
    // Direction is arbitrary, but data sheet info given for DIR high
    return set_TCA_output_pin_high(DRV_DIR_PORT_TCA, DRV_DIR_PIN_TCA);
}

/*
 * Sets the DRV8846 drive direction to reverse.
 * The direction of reverse has no physical significance, but it is gauranteed to be opposite of forward.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t set_DRV_dir_reverse(void) {
    // Direction is arbitrary, but data sheet info given for DIR high
    return set_TCA_output_pin_low(DRV_DIR_PORT_TCA, DRV_DIR_PIN_TCA);
}

/*
 * Sets the current limit scaling percentage on the DRV8846. Refer to page
 * 14 of the DRV8846 datasheet for more information. If either input is invalid,
 * has no effect.
 * 
 * Inputs:
 * I0: the value to be given to the DRV on the I0 line for computing the 
 * current limit scaling percentage. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 * I1: the value to be given to the DRV on the I1 line for computing the 
 * current limit scaling percentage. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t set_DRV_curr_limit(uint8_t I0, uint8_t I1) {
    uint8_t error1 = process_three_level_output(DRV_I0_I1_PORT_TCA, DRV_I0_PIN_TCA, I0);
    uint8_t error2 = process_three_level_output(DRV_I0_I1_PORT_TCA, DRV_I1_PIN_TCA, I1);
    return (error1 || error2); // Failure on either is a failure
}

/*
 * Sets the microstepping mode on the DRV8846. Refer to page
 * 11 of the DRV8846 datasheet for more information. If either input is invalid,
 * has no effect.
 * 
 * Inputs:
 * M0: the value to be given to the DRV on the M0 line for setting the
 * microstepping mode. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 * M1: the value to be given to the DRV on the M1 line for setting the
 * microstepping mode. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t set_DRV_step_mode(uint8_t M0, uint8_t M1) {
    uint8_t error1 = process_three_level_output(DRV_M0_M1_PORT_TCA, DRV_M0_PIN_TCA, M0);
    uint8_t error2 = process_three_level_output(DRV_M0_M1_PORT_TCA, DRV_M1_PIN_TCA, M1);
    return (error1 || error2); // Failure on either is a failure
}

/*
 * Sets the decay time on the DRV8846. Refer to page
 * 17 of the DRV8846 datasheet for more information. If either input is invalid,
 * has no effect. If the Autotune/Adaptive Decay system is enabled, both inputs 
 * should be set to 1 (logical high).
 * 
 * Inputs:
 * DEC0: the value to be given to the DRV on the DEC0 line for determining the
 * decay time. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 * DEC1: the value to be given to the DRV on the DEC1 line for determining the
 * decay time. 0 for low, 1 for high, 255 for Hi-Z.
 * All other inputs invalid.
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t set_DRV_decay_time(uint8_t DEC0, uint8_t DEC1) {
    uint8_t error1 = process_three_level_output(DRV_DEC0_DEC1_PORT_TCA, DRV_DEC0_PIN_TCA, DEC0);
    uint8_t error2 = process_three_level_output(DRV_DEC0_DEC1_PORT_TCA, DRV_DEC1_PIN_TCA, DEC1);
    return (error1 || error2); // Failure on either is a failure
}

/*
 * Checks the fault output of the DRV8846.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * true if the DRV8846 is reporting a fault condition, false otherwise
 */
bool DRV_is_fault(void) {
    return !(read_TCA_input_0() & (1 << DRV_NFAULT_PIN_TCA)); // Fault is low, so negate 
}

/*
 * Checks if the DRV8846 is set to the forward direction.
 * 
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * true if the DRV8846 is set to the forward direction, false otherwise
 */
bool DRV_is_fwd(void) {
    return (read_TCA_input_1() & (1 << DRV_DIR_PIN_TCA)); // Forward is high 
}

/*
 * Common method to abstract functionality of setting three-level ports.
 * Sets the given pin to high, low, or Hi-Z, as appropriate.
 * 
 * Inputs:
 * port: the port of the pin to set
 * pin: the pin to set
 * val: the value to set on the pin: 0 for logical low, 1 for logial high, 255 for Hi-Z
 * all other inputs are invalid
 *
 * Output:
 * none
 *
 * Return:
 * 0 for success, 1 for failure
 */
uint8_t process_three_level_output(uint8_t port, uint8_t pin, uint8_t val) {
    
    // Check for invalid input
    if((val > 1) && (val != 255))
    {
        return 1;
    }
    
    uint8_t error = 0; // Initialize to nominal condition
    
    if (val == 0) { // If the value is logical low
        
        // val = 0, so line is low
        error = set_TCA_output_pin_low(port, pin);
        
        if (!error) { // Check if previous operation succeeded
            // Not Hi-Z, so port is a TCA output (config bit is low)
            error = set_TCA_config_pin_low(port, pin);
        }
    }
    else if (val == 1) { // If the value is logical high
        
        // val = 1, so line is high
        error = set_TCA_output_pin_high(port, pin);
        
        if (!error) { // Check if previous operation succeeded
            // Not Hi-Z, so port is a TCA output (config bit is low)
            error = set_TCA_config_pin_low(port, pin);
        }
    }
    else { // val = 255
        // Hi-Z, so port is a TCA input (config bit is high)
        error = set_TCA_config_pin_high(port, pin);
    }
    
    return error;
}