/* Include project files */
#include "Utility.h"
#include "CmdQueue.h"
#include "Messaging.h"
#include "Commands.h"
#include "STARduino.h"

/* Declare Variables */
// Logfiles
File logfile_sensor;
File logfile_sync;
File logfile_interface;

// Command Buffers
Cmd_Pkt_Buff_t Debug_Pkt_Buff = {.buf_size = MAX_CMD_LEN, .end_pos = 0, .cycles_since_last_read = 0};
Cmd_Pkt_Buff_t CTU_Pkt_Buff = {.buf_size = MAX_CMD_LEN, .end_pos = 0, .cycles_since_last_read = 0};
Cmd_Pkt_Buff_t PI_Pkt_Buff = {.buf_size = MAX_CMD_LEN, .end_pos = 0, .cycles_since_last_read = 0};
uint8_t Queued_Pkt_Buff[MAX_QUEUE_CMD_LEN];

// Cycle info
uint32_t last_cycle_start_time = 0;

/* Define Functions */
void setup() {
/*
 * Initalizes the STAR payload. Opens the serial connections, opens log files,
 * loads queued commands, and initializes sensors.
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * none 
 * 
 */

  // disable the watchdog timer immediately in case it was on because of a 
  // commanded reboot
  wdt_disable();
 
  // setup comms
  SERIAL_DEBUG.begin(57600);
  SERIAL_CTU.begin(57600); // do not change without concurrence from CTU
  SERIAL_PI.begin(57600);
  
  pinMode(PIN_SDCHIPSELECT,OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(PIN_SDCHIPSELECT)) {
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDINIT> SDcard failed to init");
  }
  sendTxtMsg(SERIAL_DEBUG, "INIT: <SDINIT> SDcard init");
  
  // Open logfiles
  // Note: Because the interface automatically logs outgoing messages,
  // no messages can be sent until the logfile_input is opened
  logfile_interface = SD.open(FILENAME_INTERFACE_LOG, FILE_WRITE);
  //SERIAL_DEBUG.print("Logfile name: ");
    //SERIAL_DEBUG.print(logfile_interface.name());
    //SERIAL_DEBUG.print(", Logfile handle: ");
    //SERIAL_DEBUG.println(logfile_interface);
  set_msg_logfile(logfile_interface);
  logfile_sensor = SD.open(FILENAME_SENSOR_LOG, FILE_WRITE);
  logfile_sync = SD.open(FILENAME_SYNC_LOG, FILE_WRITE);
  
  // open log files
  sendTxtMsg(SERIAL_DEBUG, "INFO: <Init> LogFiles init");

  // setup sync pulse interrupt
  //sendTxtMsg(SERIAL_DEBUG, "INFO: <Init> Setup sync interrupt");

    
  // load command table
  char cmds_filename[sizeof(FILENAME_CMDSEQ)];
  strcpy(cmds_filename, FILENAME_CMDSEQ);
  uint16_t flash_page = 1;
  int8_t load_status = 0;

  load_status = load_cmdseq(cmds_filename, flash_page);

  if(load_status < 0){
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <Init> Failed to load cmd table");
  }

  // initialize sensors
  //sendTxtMsg(SERIAL_DEBUG, "INFO: <Init> Sensors initalized");

  sendTxtMsg(SERIAL_DEBUG, "INFO: <Init> Initialized!");
  send_status_msg(STATUS_INITALIZED);
}

void loop() {
/*
 * Executes the main loop of the STAR payload. Reads interfaces and executes commands,
 * reads and logs sensor data, logs software status.
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * none 
 * 
 */

  if(time_for_cycle(last_cycle_start_time)){
    
    last_cycle_start_time = get_MET();

    //sendTxtMsg(SERIAL_DEBUG, "DEBUG: <Loop> Loop Start");
    
    // check if time to execute next command in buffer
    if(time_for_queued_cmd(get_MET())){
      inject_cmd(Queued_Pkt_Buff);
      execute_queued_command(Queued_Pkt_Buff);
    }
    
    // read debug interface
    read_serial(SERIAL_DEBUG,&Debug_Pkt_Buff);
  
    // execute debug command
    if(full_cmd_available(Debug_Pkt_Buff)){
      //SERIAL_DEBUG.println();
      //SERIAL_DEBUG.println("Executing cmd1");
      execute_DEBUG_command(Debug_Pkt_Buff);
    }
     
    // read CTU interface
    read_serial(SERIAL_CTU,&CTU_Pkt_Buff);
  
    // execute CTU command
    if(full_cmd_available(CTU_Pkt_Buff)){
      execute_CTU_command(CTU_Pkt_Buff);
    }
  
    // read CTU interface
    read_serial(SERIAL_PI,&PI_Pkt_Buff);
  
    // execute CTU command
    if(full_cmd_available(PI_Pkt_Buff)){
      execute_PI_command(PI_Pkt_Buff);
    }
  
    // read sensor data
  
    // write to sensor log file
  
    // log FSW status
    
  }

}


  


