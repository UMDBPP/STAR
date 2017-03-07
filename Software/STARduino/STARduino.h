#ifndef _starduino_h_
#define _starduino_h_

/* Include libraries */
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

/* Include project libraries */
#include "CCSDS/CCSDS.h"
#include "CCSDS/ccsds_utilities.h"
#include "Queue/QueueList.h"

/* Define aliases */
// Serials
#define SERIAL_DEBUG Serial // FIXME
#define SERIAL_CTU Serial1 // FIXME
#define SERIAL_PI Serial2 // FIXME

// Pins
#define PIN_SDCHIPSELECT 13 // FIXME
#define PIN_LEDPWM 13 // FIXME
#define PIN_MOTORPWM 13 // FIXME
#define PIN_BIAS 13 // FIXME
#define PIN_CUTOFF 13 // FIXME

// APIDs
#define APID_STAR_CMD 100
#define APID_STAR_FILELOADERR 110
#define APID_STAR_TXTMSG 120
#define APID_STAR_CMDRESP 130

// FcnCodes
#define FCNCODE_NOOP_CMD 9
#define FCNCODE_LED_CMD 10
#define FCNCODE_BIAS_CMD 11 
#define FCNCODE_SHAKE_CMD 12
#define FCNCODE_CUTOFF_CMD 13
#define FCNCODE_TLM_CMD 14
#define FCNCODE_QUEUE_CMD 15
#define FCNCODE_RESETMET_CMD 16

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
#define ERROR_SDLOAD_LONGCMD -2

/* Define Parameters */
#define MAX_CMD_LEN 9
#define MAX_TLM_LEN 255

/* Define Datatypes */
typedef struct CCSDS_Cmd_t {
  uint32_t timestamp;
  uint8_t bytes[MAX_CMD_LEN];
}CCSDS_Cmd_t;


#endif

