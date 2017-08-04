// when _IS_UT_ is defined this file doesn't include it's normal 
// header file which allows the UT to define what it needs
// to for the test
#ifndef _IS_UT_
#include "CmdQueue.h"
#endif

Queue <CCSDS_Cmd_t> cmd_queue;
// Note: this doesn't appear to work if its passed into functions
// so its defined as a global

bool Queue_Enabled_Flag = false;

int8_t load_cmdseq(char *fileName, uint16_t page_num){
/*
 * Loads a command sequence, first attempting from the SD and then
 * falling back on loading from the flash if that fails. Handles errors
 * in helper functions.
 * 
 * Inputs: 
 * filename - filename on SD to read commands from
 * cmd_queue - Queue to read commands into
 * 
 * Output:
 * file_pos - position in file of last error
 * 
 * Return:
 * 0 if load completed successfully or the error code of the last error encountered. 
 */

  int8_t load_stat;
  uint16_t file_pos;

  // load from SD
  load_stat = load_cmds_sd(fileName, file_pos);

  // handle errors here for now
  if(load_stat == ERROR_SDLOAD_OPENFILE){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Cannot open file");
  }
  else if(load_stat == ERROR_SDLOAD_SHORTTIME){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Not enough bytes available to read timestamp");
  }
  else if(load_stat == ERROR_SDLOAD_SHORTHEAD){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Not enough bytes available to read pkt header");
  }
  else if(load_stat == ERROR_SDLOAD_SHORTPKT){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Not enough bytes available to read pkt");
  }
  else if(load_stat == ERROR_SDLOAD_CMDCHKSUM){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Command checksum doesn't validate");
  }
  else{
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SDLOAD> Successful load from SD");
  }

  // if fail, load from flash
  if(load_stat != 0){
    load_stat = load_cmds_flash(page_num, file_pos);
  }

  if(load_stat == ERROR_FLASHLOAD_NOTIMP){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <FLASHLOAD> flash load function not implemented ");
  }
  else{
    //sendTxtMsg(SERIAL_DEBUG, "INFO: <FLASHLOAD> Successful load from flash");
  }

  // if fail, log but execute anyways
  if(load_stat != 0){
    disable_cmd_queue();
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <CMDLOAD> Command loading failed... disabled queue");
  }
  
  return load_stat;
}

int8_t load_cmds_sd(char *fileName, uint16_t &file_pos){
/*
 * Loads a command sequence from the SD card. Errors will be returned and a FileLoad 
 * Error packet will be issued.
 * 
 * Inputs: 
 * cmd_queue - Queue to read commands into
 * filename - filename on SD to read commands from
 * 
 * Output:
 * file_pos - position in file of last error
 * 
 * Return:
 * 0 if load completed successfully or the error code of the last error encountered. 
 * 
 */

  // Initialize variables
  int8_t load_stat = ERROR_SDLOAD_SUCCESS;
  uint8_t cmd_buf[MAX_CMD_LEN];
  uint8_t pkt_len = 0;
  uint16_t file_idx = 0; // tracks current position in file

  // attempt to open the file
  File cmdFile = SD.open(fileName);
  
  char buf[100];
  sprintf(buf,"SDLOAD: <DEBUG> Found file with length %d",cmdFile.available());
  sendTxtMsg(SERIAL_DEBUG, buf);

  // if the file can't be opened, there's no point in continuing, so error
  if (!cmdFile) {
    send_fileload_error(ERROR_SDLOAD_OPENFILE, file_idx);
    return ERROR_SDLOAD_OPENFILE;
  }

  // infiloop protection counter
  // will decrement once for each command read, 
  // read loop will end if counter reaches zero
  int16_t ctr = MAX_CMDS_IN_QUEUE;
  
  // read the entire file
  while (ctr--) {
    
    // initialize a temporary command to read data into
    CCSDS_Cmd_t tmp_cmd;

    // if there are more bytes available than the length of a timestamp, 
    // we can read a timestamp
    if(cmdFile.available() > sizeof(tmp_cmd.timestamp)){
       cmdFile.read((uint32_t*)&tmp_cmd.timestamp,sizeof(tmp_cmd.timestamp));

       // during testing (on win10 desktop machine) it was noted that 
       // timestamps were read with opposite endian so this was added
       // it may need to be removed on the embedded system
       tmp_cmd.timestamp = __builtin_bswap32(tmp_cmd.timestamp);
       file_idx += sizeof(tmp_cmd.timestamp);
    }
    else{
      //send_fileload_error(ERROR_SDLOAD_SHORTTIME, file_idx);
      load_stat = ERROR_SDLOAD_SHORTTIME;
      file_pos = file_idx;
      break;
    }
    
    // if there are more bytes available that the length of a header, 
    // we can read a header
    if(cmdFile.available() > sizeof(CCSDS_PriHdr_t)){
       cmdFile.read((uint8_t*)&tmp_cmd.bytes,sizeof(CCSDS_PriHdr_t));
       file_idx += sizeof(CCSDS_PriHdr_t);
    }
    else{
      //send_fileload_error(ERROR_SDLOAD_SHORTHEAD, file_idx);
      load_stat = ERROR_SDLOAD_SHORTHEAD;
      file_pos = file_idx;
      break;
    }
    
    // read the packet length
    pkt_len = getPacketLength(tmp_cmd.bytes);
    
    sprintf(buf,"SDLOAD: <DEBUG> Found pkt with length %d",pkt_len);
    sendTxtMsg(SERIAL_DEBUG, buf);

    // if there are more than bytes available than the remaining bytes 
    // of the pkt, we can read the rest of the pkt
    if(cmdFile.available() >= pkt_len-sizeof(CCSDS_PriHdr_t)){
       cmdFile.read((uint8_t*)&tmp_cmd.bytes+sizeof(CCSDS_PriHdr_t),pkt_len-sizeof(CCSDS_PriHdr_t));
       file_idx += pkt_len-sizeof(CCSDS_PriHdr_t);
    }
    else{
      //send_fileload_error(ERROR_SDLOAD_SHORTPKT, file_idx);
      load_stat = ERROR_SDLOAD_SHORTPKT;
      file_pos = file_idx;
      break;
    }
    
    // validate command checksum
    if(!validateChecksum(tmp_cmd.bytes)){
      load_stat = ERROR_SDLOAD_CMDCHKSUM;
      file_pos = file_idx;
    }
    
    // put it in the command queue
    cmd_queue.push(tmp_cmd);
    sendTxtMsg(SERIAL_DEBUG, "SDLOAD: <DEBUG> Added pkt");
    
    // stop reading if we've reached the end of the file
    if(cmdFile.available()==0){
      break;
    }
  }

  if(ctr < 0){
    sendTxtMsg(SERIAL_DEBUG, "SDLOAD: <ERROR> Aborted load SD due to InifLoop");
    load_stat = ERROR_SDLOAD_INFILOOP;
    file_pos = file_idx;
  }
  cmdFile.close();
  return load_stat;
}

void send_fileload_error(int8_t error_code, uint16_t filepos){
/*
 * Sends a message indicating an error with a fileread operation
 * 
 * Inputs: 
 * error_code - code representing which error occured
 * filepos - position in file where error occured
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */

  uint8_t payloadLength = 0;
  uint8_t payload_buff[MAX_TLM_LEN];
  
  payloadLength = addIntToTlm(error_code, payload_buff, payloadLength); // Add the value of the table to message
  payloadLength = addIntToTlm(filepos, payload_buff, payloadLength); // Add the value of the table to message

  sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILELOADERR, payload_buff, payloadLength);

}

int8_t load_cmds_flash(uint16_t page_num, uint16_t &filepos){
/*
 * Loads a command sequence from the flash memory. Errors will be returned and a FileLoad 
 * Error packet will be issued.
 * 
 * Inputs: 
 * cmd_queue - Queue to read commands into
 * filename - number of the page of flash memory which contains the commands
 * 
 * Output:
 * file_pos - position in file of last error
 * 
 * Return:
 * 0 if load completed successfully or the error code of the last error encountered. 
 * 
 */
 
  // initalize load status
  int8_t load_stat = ERROR_FLASHLOAD_SUCCESS;
 
  // set it to fail
  load_stat = ERROR_FLASHLOAD_NOTIMP;
  // Need more info to implement
  send_fileload_error(ERROR_FLASHLOAD_NOTIMP, 0);
  
  return load_stat;
}

bool time_for_queued_cmd(uint32_t MET){
/*
 * Checks if its time to execute a command from the queue
 * 
 * Inputs: 
 * cmd_queue - Queue to read commands from
 * MET - current mission elapsed time
 * 
 * Globals:
 * cmd_queue - queue of commands to execute
 * 
 * Output:
 * none
 * 
 * Return:
 * true if a command should be executed, false otherwise
 * 
 */

  // if the queue is disabled, return false
  if(!Queue_Enabled_Flag){
    return false;
  }
  
  // if queue is empty, it can't be time for a command, so return
  // false
  if(cmd_queue.isEmpty()){
     disable_cmd_queue();
     return false;
  }
   
  // get next command from queue but don't remove it from the queue
  CCSDS_Cmd_t tmp;
  cmd_queue.peek(tmp);

  // return if current time is greater than next command's timestamp
  return MET >= tmp.timestamp;

}

void inject_cmd(uint8_t Pkt_Buff[]){
/*
 * Removes the next command from the queue and places it in a buffer
 * to be excecuted.
 * 
 * Inputs: 
 * cmd_queue - Queue to read commands from
 * Pkt_Buff - buffer to be exeucted
 * 
 * Output:
 * none
 * 
 * Return:
 * true if a command should be executed, false otherwise
 * 
 */
 
 // get next command from queue
 CCSDS_Cmd_t tmp;
 cmd_queue.pop(tmp);
 
 sendTxtMsg(SERIAL_DEBUG, "INFO: <INJCHECK> Injecting command");
 memcpy(Pkt_Buff,tmp.bytes,getPacketLength(tmp.bytes));

 return;
}

void enable_cmd_queue(){
/*
 * Enables the command queue. 
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * N/A
 * 
 */
  Queue_Enabled_Flag = true;
}
void disable_cmd_queue(){
/*
 * Disables the command queue. 
 * 
 * Inputs: 
 * none
 * 
 * Output:
 * none
 * 
 * Return:
 * N/A
 * 
 */
  sendTxtMsg(SERIAL_DEBUG, "INFO: <CMDQUEUE> Disabled command queue");
  Queue_Enabled_Flag = false;
}
