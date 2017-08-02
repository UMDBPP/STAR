#ifndef _loadcmd_h_
#define _loadcmd_h_

#include <stdint.h>  /* for int datatypes */
#include "STARduino.h"
#include "Messaging.h"

int8_t load_cmdseq(char *fileName, uint16_t page_num);
int8_t load_cmds_sd(char *fileName, uint16_t &file_pos);
int8_t load_cmds_flash(uint16_t page_num, uint16_t &file_pos);
void send_fileload_error(int8_t error_code, uint16_t filepos);
bool time_for_queued_cmd(uint32_t MET);
void inject_cmd(uint8_t Pkt_Buff[]);
void enable_cmd_queue();
void disable_cmd_queue();


#endif
