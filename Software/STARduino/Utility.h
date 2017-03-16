#ifndef _utility_h_
#define _utility_h_

#include <stdint.h>
#include "STARduino.h"

void set_epoch();
uint32_t get_MET();
void print_datestamp(File write_file, uint32_t time_millis);
bool time_for_cycle(uint32_t last_cycle_time);
int8_t open_fileidx(File &rootdir, File &entry, uint8_t _fileIdx);

#endif
