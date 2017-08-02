#ifndef _commands_h_
#define _commands_h_

#include <stdint.h>
#include "STARduino.h"
#include "CmdQueue.h"
#include "Utility.h"
#include "Messaging.h"

/* Define prototypes */
void execute_command(uint8_t Pkt_Buff[]);
void execute_queued_command(uint8_t Pkt_Buff[]);
void execute_CTU_command(Cmd_Pkt_Buff_t &_Pkt_Buff);
void execute_PI_command(Cmd_Pkt_Buff_t &_Pkt_Buff);
void execute_DEBUG_command(Cmd_Pkt_Buff_t &_Pkt_Buff);

// Command execution 
void set_LED(uint8_t);
void set_bias(uint8_t);
void set_shaker(uint8_t);
void set_cutoff(uint8_t);
void send_tlm(uint8_t);
void send_cmd_response(uint8_t, uint8_t);
void send_status_msg(uint8_t);
void find_fileinfo(uint8_t);
void send_file(uint8_t);
void delete_file(uint8_t);
void get_file_checksum(uint8_t);
void write_file(char filename[13],uint8_t Pkt_Buff[], uint16_t filesize);

#endif



