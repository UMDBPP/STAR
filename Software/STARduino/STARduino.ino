/* Include project files */
#include "Utility.h"
#include "CmdQueue.h"
#include "Messaging.h"
#include "Commands.h"
#include "STARduino.h"

/* Declare Variables */
File logfile_sensor;
File logfile_sync;

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
  File logfile_interface = SD.open(FILENAME_INTERFACE_LOG, FILE_WRITE);
  //set_msg_logfile(logfile_interface);
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
 
  //sendTxtMsg(SERIAL_DEBUG, "DEBUG: <Loop> Loop");

  // initalization
  uint8_t Pkt_Buff[100];
  uint8_t BytesRead;
  
  // check if time to execute next command in buffer
  if(time_for_queued_cmd(get_MET())){
    inject_cmd(Pkt_Buff);
    execute_command(Pkt_Buff);
  }
  
  // read debug interface
  if(SERIAL_DEBUG.available()){
    BytesRead = SERIAL_DEBUG.readBytes((char*)Pkt_Buff, SERIAL_DEBUG.available());
    // FIXME: Need to add logic to combine partial packets received over multiple 
    // timesteps
    execute_command(Pkt_Buff);
  }
   
  // read CTU interface
  if(SERIAL_CTU.available()){
    BytesRead = SERIAL_CTU.readBytes((char*)Pkt_Buff, SERIAL_CTU.available());
    // FIXME: If comms with CTU is un-descoped, create a reduced version of 
    // execute_command() which only allows execution of the send_tlm command so 
    // that CTU can't accidentally command STAR in any other way
    execute_command(Pkt_Buff);
  }

  // read PI interface
  if(SERIAL_PI.available()){
    BytesRead = SERIAL_PI.readBytes((char*)Pkt_Buff, SERIAL_PI.available());
    // FIXME: If comms with PI is un-descoped, create a reduced version of 
    // execute_command() which only allows execution of a command to log the 
    // frame number so that the Pi can't accidentally command STAR in any other way
    execute_command(Pkt_Buff);
  }

  // read sensor data

  // write to sensor log file

  // log FSW status

  // delay until next cycle
  delay(10);

}


  


