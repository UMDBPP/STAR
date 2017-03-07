
#include "CmdQueue.h"

QueueList <CCSDS_Cmd_t> cmd_queue;
// Note: this doesn't appear to work if its passed into functions
// so its defined as a global

bool Queue_Enabled_Flag = true;

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
 * 
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
  else if(load_stat == ERROR_SDLOAD_LONGCMD){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Pkt header indicates a command which is too long");
  }
  else if(load_stat == ERROR_SDLOAD_CMDCHKSUM){   
    sendTxtMsg(SERIAL_DEBUG, "ERROR: <SDLOAD> Command checksum doesn't validate");
  }

  // if fail, load from flash
  if(load_stat != 0){
    load_stat = load_cmds_flash(page_num, file_pos);
  }
/*
  if(load_stat == ERROR_FLASHLOAD_NOTIMP){   
    SERIAL_DEBUG.println("ERROR: <FLASHLOAD> flash load function is not implemented ");
  }
*/
  // if fail, log but execute anyways
  if(load_stat != 0){
    disable_cmd_queue();
    //SERIAL_DEBUG.print("ERROR: <CMDLOAD> Command loading failed... disbled queue");
  }
  
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
  int8_t load_stat = 0;
  uint8_t cmd_buf[MAX_CMD_LEN];
  uint8_t pkt_len = 0;
  //uint8_t pkt_idx = 0;
  uint16_t file_idx = 0;


  // attempt to open the file
  File cmdFile = SD.open(fileName);

  // if the file can't be opened, there's no point in continuing, so error
  if (!cmdFile) {
    send_fileload_error(ERROR_SDLOAD_OPENFILE, file_idx);
    return ERROR_SDLOAD_OPENFILE;
  }

  // read the entire file
  while (1) {

    // initialize a temporary command to read into
    CCSDS_Cmd_t tmp_cmd;

    // if there are more bytes available than the length of a timestamp, 
    // we can read a timestamp
    if(cmdFile.available() > sizeof(tmp_cmd.timestamp)){
       cmdFile.read((uint32_t*)&tmp_cmd.timestamp,sizeof(tmp_cmd.timestamp));
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
    CCSDS_WR_LEN(getPrimaryHeader(tmp_cmd.bytes),pkt_len);

    // if the packet is too long, record an error
    if(pkt_len > MAX_CMD_LEN){
      //send_fileload_error(ERROR_SDLOAD_LONGCMD, file_idx);
      load_stat = ERROR_SDLOAD_LONGCMD;
      file_pos = file_idx;
    }      

    // if there are more than bytes available than the remaining bytes 
    // of the pkt, we can read the rest of the pkt
    if(cmdFile.available() > pkt_len-sizeof(CCSDS_PriHdr_t)){
       cmdFile.read(tmp_cmd.bytes,pkt_len-sizeof(CCSDS_PriHdr_t));
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

    // stop reading if we've reached the end of the file
    if(cmdFile.available()<0){
      break;
    }
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
 
  // Need more info to implement
  send_fileload_error(ERROR_FLASHLOAD_NOTIMP, 0);
  return ERROR_FLASHLOAD_NOTIMP;
}

bool check_inject_cmd(uint32_t MET){
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
     return false;
  }
   
  // get next command from queue but don't remove it from the queue
  CCSDS_Cmd_t tmp = cmd_queue.peek();

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
 CCSDS_Cmd_t tmp = cmd_queue.pop();

 Pkt_Buff = tmp.bytes;
 
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
  Queue_Enabled_Flag = false;
}

