#ifndef _utility_h_
#define _utility_h_

#include <stdint.h>
#include "STARduino.h"

void set_epoch();
uint32_t get_MET();
void print_datestamp(File write_file, uint32_t time_millis);


#endif
