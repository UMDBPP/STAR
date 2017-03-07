#ifndef _commands_h_
#define _commands_h_

#include <stdint.h>
#include "STARduino.h"
#include "CmdQueue.h"
#include "Utility.h"

/* Define prototypes */
void execute_command(uint8_t Pkt_Buff[]);

// Command execution 
void set_LED(uint8_t brightness);
void set_bias(uint8_t enable);
void set_shaker(uint8_t shake_speed);
void set_cutoff(uint8_t enable);
void send_tlm(uint8_t tlm_requested);
void send_cmd_response(uint8_t fcncode, uint8_t stat);

#endif



