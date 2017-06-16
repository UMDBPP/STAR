#ifndef _starduino_h_
#define _starduino_h_

/* Include libraries */
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

/* Define Parameters */
// used for setting size of buffers which will hold packets
#define MAX_CMD_LEN 100

// used for setting size of queue storage for commands
// Note: Not all commands are allowed to be queued (ex, file 
//  commands), so the size of this buffer only has to hold 
//  the largest queued command.
#define MAX_QUEUE_CMD_LEN 9

// used for setting size of buffer to hold output packets being created
#define MAX_TLM_LEN 255

// cycles that a partial packet will be kept before being discarded
#define MAX_READ_CYCLES_STALENESS 15

// cycles that a partial packet will be kept before being discarded
#define DESIRED_CYCLE_HZ 30

// define the max number of commands allowed in a queue
#define MAX_CMDS_IN_QUEUE 10

// define the number of bytes of a file sent in each packet
#define FILE_BYTES_PER_PKT 100

// alias this to the name StaticQueue expects
#define QUEUE_LEN 10//MAX_CMDS_IN_QUEUE

/* Include project libraries */
#include "src/CCSDS/CCSDS.h"
#include "src/CCSDS/ccsds_utilities.h"
#include "src/Queue/StaticQueue.h"

/* Define aliases */
// Software Reset Register
#define RESET_REGISTER *(unsigned int volatile *)0xE000ED0C

// Serials
#define SERIAL_DEBUG Serial // FIXME
#define SERIAL_CTU Serial
#define SERIAL_PI Serial // FIXME

// Pins
#define PIN_SDCHIPSELECT 53 // FIXME
#define PIN_LEDPWM 14 // FIXME
#define PIN_MOTORPWM 14 // FIXME
#define PIN_BIAS 14 // FIXME
#define PIN_CUTOFF 14 // FIXME

// APIDs
#define APID_STAR_CMD 100
#define APID_STAR_FILELOADERR 110
#define APID_STAR_TXTMSG 120
#define APID_STAR_CMDRESP 130
#define APID_STAR_STATUS 140
#define APID_STAR_FILEINFO 150
#define APID_STAR_FILECHKSUM 160
#define APID_STAR_BEGIN_FILESEND 170
#define APID_STAR_FILEDATA 171
#define APID_STAR_END_FILESEND 172

// FcnCodes
#define FCNCODE_NOOP_CMD 9
#define FCNCODE_LED_CMD 10
#define FCNCODE_BIAS_CMD 11 
#define FCNCODE_SHAKE_CMD 12
#define FCNCODE_CUTOFF_CMD 13
#define FCNCODE_TLM_CMD 14
#define FCNCODE_QUEUE_CMD 15
#define FCNCODE_RESETMET_CMD 16
#define FCNCODE_FILEINFO_CMD 17
#define FCNCODE_REBOOT_CMD 18
#define FCNCODE_SENDFILE_CMD 19
#define FCNCODE_DELFILE_CMD 20
#define FCNCODE_RCVFILE_CMD 22

// Filenames1
#define FILENAME_INTERFACE_LOG "intf_log.txt"
#define FILENAME_SENSOR_LOG "sens_log.txt"
#define FILENAME_SYNC_LOG "sync_log.txt"
#define FILENAME_CMDSEQ "cmds.cmd"

// Errors
#define ERROR_SDLOAD_OPENFILE -1
#define ERROR_SDLOAD_CMDCHKSUM -3
#define ERROR_FLASHLOAD_NOTIMP -5
#define ERROR_SDLOAD_SHORTTIME -6
#define ERROR_SDLOAD_SHORTHEAD -7
#define ERROR_SDLOAD_SHORTPKT -8
#define ERROR_SDLOAD_INFILOOP -9
#define ERROR_SDLOAD_SUCCESS 0
#define ERROR_FLASHLOAD_SUCCESS 0
#define ERROR_OPENFILEIDX_DIR -1
#define ERROR_OPENFILEIDX_NOEXIST -2
#define ERROR_OPENFILEIDX_SUCCESS 1

// Status
#define STATUS_INITALIZED 1

// Cmd Response
#define CMDRESP_BAD_FCNCODE 2 
#define CMDRESP_BAD_APID 3 
#define CMDRESP_NOT_CMD 4
#define CMDRESP_BAD_CHKSUM 5

#if MAX_QUEUE_CMD_LEN > MAX_CMD_LEN
  #error "MAX_QUEUE_CMD_LEN should not be greater than MAX_CMD_LEN"
#endif

/* Define Datatypes */
typedef struct CCSDS_Cmd_t {
  uint32_t timestamp;
  uint8_t bytes[MAX_QUEUE_CMD_LEN];
}CCSDS_Cmd_t;

typedef struct Cmd_Pkt_Buff_t {
  uint16_t buf_size;
  uint16_t end_pos;
  uint8_t cycles_since_last_read;
  uint8_t bytes[MAX_CMD_LEN];
}Cmd_Pkt_Buff_t;

#endif

