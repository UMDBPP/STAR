#ifndef _messaging_h_
#define _messaging_h_

#include <stdint.h>
#include "STARduino.h"
#include "Utility.h"

void set_msg_logfile(File logfile);
void sendTxtMsg(Stream &_serial, const char str[]);
int sendTlmMsg(Stream &_serial, uint16_t _APID, uint8_t _payload[], uint16_t _payload_size);
void log_sent_pkt(uint8_t pkt_buf[], uint16_t pkt_size);

#endif
