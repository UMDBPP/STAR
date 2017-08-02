#include "Commands.h"


void execute_queued_command(uint8_t _Pkt_Buff[]){
  /*
 * Executes a command from the queue. The following functions
 * are not permitted to be executed from the queue:
 * SEND_FILE
 * DEL_FILE
 * FILE_INFO
 * REBOOT
 * 
 * Inputs: 
 * Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 sendTxtMsg(SERIAL_DEBUG,"INFO: <QUEUECMD> Function filtering has not been implemented yet");
 execute_command(_Pkt_Buff);
}

void execute_DEBUG_command(Cmd_Pkt_Buff_t &_Pkt_Buff){
  /*
 * Executes a command from DEBUG. All commands are executable
 * from this interface.
 * 
 * Inputs: 
 * Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 //SERIAL_DEBUG.println("Executing cmd");
 execute_command(_Pkt_Buff.bytes);
 
 // reset the end position of the buffer (effectively "emptying" it)
 _Pkt_Buff.end_pos = 0;
}

void execute_CTU_command(Cmd_Pkt_Buff_t &_Pkt_Buff){
  /*
 * Executes a command from CTU. Only the SEND_TLM command
 * is executable from CTU. 
 * 
 * Inputs: 
 * Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 sendTxtMsg(SERIAL_DEBUG,"INFO: <CTUCMD> Function filtering has not been implemented yet");
 execute_command(_Pkt_Buff.bytes);
 
 // reset the end position of the buffer (effectively "emptying" it)
 _Pkt_Buff.end_pos = 0;
}

void execute_PI_command(Cmd_Pkt_Buff_t &_Pkt_Buff){
  /*
 * Executes a command from the PiZero. No commands are currently 
 * executable from the Pi.
 * 
 * Inputs: 
 * Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 sendTxtMsg(SERIAL_DEBUG,"INFO: <PICMD> Commands from the Pi are not executed");

 // reset the end position of the buffer (effectively "emptying" it)
 _Pkt_Buff.end_pos = 0;
}

void execute_command(uint8_t _Pkt_Buff[]){
/*
 * Executes a command.
 * 
 * Inputs: 
 * _Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */

  // check the APID
  if(getAPID(_Pkt_Buff) != APID_STAR_CMD){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Invalid command APID received ");
    send_cmd_response(getCmdFunctionCode(_Pkt_Buff), CMDRESP_BAD_APID);
    return;
  }

  // verify that the packet is a command
  if(getPacketType(_Pkt_Buff) != CCSDS_CMD_PKT){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Packet recieved is not a command");
    send_cmd_response(getCmdFunctionCode(_Pkt_Buff), CMDRESP_NOT_CMD);
    return;
  }

  // validate command checksum
  if(!validateChecksum(_Pkt_Buff)){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Command checksum doesn't validate");
    send_cmd_response(getCmdFunctionCode(_Pkt_Buff), CMDRESP_BAD_CHKSUM);
    return;
  }

  /*
   * The type of command is identified by the function code included in 
   * the header. In this case we've defined 2 commands.
   */
  switch(getCmdFunctionCode(_Pkt_Buff)){
    case FCNCODE_NOOP_CMD:{
      /*  
       *   FcnCode FCNCODE_NOOP_CMD is a command to do nothing. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> NOOP command received");

      send_cmd_response(FCNCODE_NOOP_CMD, 1);

      break;
    }case FCNCODE_LED_CMD:{
      /*  
       *   FcnCode FCNCODE_LED_CMD is a command to control the illumination. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Brightness (1 byte) (0-255 corresponding to brightness)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> LED command received");

      // initalize variables needed for command execution
      uint8_t brightness_flag = 0;

      // extract the on_off_flag from the command
      extractFromTlm(brightness_flag, _Pkt_Buff, 8);

      // command the LED
      set_LED(brightness_flag);
      break;
    }
    case FCNCODE_BIAS_CMD:{
      /*  
       *   FcnCode FCNCODE_BIAS_CMD is a command to control the Bias. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Bias_Enable (1 bytes) [1 = on, 0 = off]
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Bias command received");

      // initalize variables needed for command execution
      uint8_t enable_flag = 0;

      // extract the on_off_flag from the command
      extractFromTlm(enable_flag, _Pkt_Buff, 8);

      // command the Bias
      set_bias(enable_flag);
      break;
    }
    case FCNCODE_SHAKE_CMD:{
      /*  
       *   FcnCode FCNCODE_SHAKE_CMD is a command to control the shaker. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Shake_Speed (1 bytes)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Shake command received");

      // initalize variables needed for command execution
      uint8_t shake_speed = 0;

      // extract the on_off_flag from the command
      extractFromTlm(shake_speed, _Pkt_Buff, 8);

      // command the shaker
      set_shaker(shake_speed);
      break;
    }
    case FCNCODE_CUTOFF_CMD:{
      /*  
       *   FcnCode FCNCODE_CUTOFF_CMD is a command to enable the cutoff. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Cutoff_Enable (1 bytes) [1 = cutoff, 0 = on]
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Cutoff command received");

      // initalize variables needed for command execution
      uint8_t cutoff_enable = 0;

      // extract the on_off_flag from the command
      extractFromTlm(cutoff_enable, _Pkt_Buff, 8);

      // command the LED
      set_cutoff(cutoff_enable);
      break;
    }
    case FCNCODE_TLM_CMD:{
      /*  
       *   FcnCode FCNCODE_TLM_CMD is a command to output telemetry. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   TlmRequested (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> TLM command received");

      // initalize variables needed for command execution
      uint8_t tlm_requested = 0;

      // extract the tlm_requested from the command
      extractFromTlm(tlm_requested, _Pkt_Buff, 8);

      // command the LED
      send_tlm(tlm_requested);
      break;
    }
    case FCNCODE_QUEUE_CMD:{
      /*  
       *   FcnCode FCNCODE_QUEUE_CMD is a command to control the 
       *   execution of the automated command sequence.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   EnableQueue (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> QUEUE command received");

      bool queue_flag;
      
      // extract the tlm_requested from the command
      extractFromTlm(queue_flag, _Pkt_Buff, 8);

      if(queue_flag){
        enable_cmd_queue();
      }
      else{
        disable_cmd_queue();
      }

      break;
    }
    case FCNCODE_RESETMET_CMD:{
      /*  
       *   FcnCode FCNCODE_RESETMET_CMD is a command reset the 
       *   MET (mission elapsed time) counter to zero.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   EnableQueue (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> RESET_MET command received");

      set_epoch();

      break;
    }
    case FCNCODE_FILEINFO_CMD:{
      /*  
       *   FcnCode FCNCODE_FILEINFO_CMD is a command to return
       *   info on a file on the SD card.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   FileIdx (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> FILE_INFO command received");
      
      uint8_t fileidx = 0;
      
      // extract the tlm_requested from the command
      extractFromTlm(fileidx, _Pkt_Buff, 8);
      
      // respond to the command
      find_fileinfo(fileidx);
      
      break;
    }
    case FCNCODE_SENDFILE_CMD:{
      /*  
       *   FcnCode FCNCODE_SENDFILE_CMD is a command to send a file
       *   in packets.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   FileIdx (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> SEND_FILE command received");
      
      uint8_t fileidx = 0;
      
      // extract the tlm_requested from the command
      extractFromTlm(fileidx, _Pkt_Buff, 8);
      
      // respond to the command
      send_file(fileidx);
      
      break;
    }
    case FCNCODE_DELFILE_CMD:{
      /*  
       *   FcnCode FCNCODE_DELFILE_CMD is a command to delete a file
       *   from the SD card.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   FileIdx (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> DEL_FILE command received");
      
      uint8_t fileidx = 0;
      
      // extract the file index from the command
      extractFromTlm(fileidx, _Pkt_Buff, 8);
      
      // respond to the command
      delete_file(fileidx);
      
      break;
    }
    case FCNCODE_RCVFILE_CMD:{
      /*  
       *   FcnCode FCNCODE_RCVFILE_CMD is a command will receive part
       *   of a file and write it to the SD. The function is defined with the 
       *   following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   FileName (13 bytes)
       *   FileSize (2 bytes)
       *   FileData (100 bytes)
       *   
       *   Note: commands already contain a checksum, so the file being uploaded
       *   is not also checksummed
       *   Note: this does not yet support multi-packet file uploads
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> RCV_FILE command received");
      sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> RCV_FILE command is not yet implemented!");

      uint8_t pkt_pos = 8;
      char filename[13];
      uint16_t filesize = 0;

      // extract the file index from the command
      pkt_pos = extractStrFromTlm(filename, _Pkt_Buff, pkt_pos, 13);
      pkt_pos = extractFromTlm(filesize, _Pkt_Buff, pkt_pos);
      
      // respond to the command
      write_file(filename,_Pkt_Buff + pkt_pos,filesize);
      
      break;
    }
    case FCNCODE_REBOOT_CMD:{
      /*  
       *   FcnCode FCNCODE_REBOOT_CMD is a command to reboot 
       *   STARduino. The function is defined with the following 
       *   format:
       *   
       *   CCSDS Command Header (8 bytes)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> REBOOT command received");
      unsigned int reset_val = 0x05FA0004;
      RESET_REGISTER = reset_val;

      break;
    }
    default:{
      sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Unrecognized function code");
      send_cmd_response(getCmdFunctionCode(_Pkt_Buff), CMDRESP_BAD_FCNCODE);
    }
  }
}

void write_file(char _filename[13], uint8_t _Pkt_Buff[], uint16_t _filesize){
/*
 * Writes a buffer of data to a file on the SD card
 * 
 * Inputs: 
 * filename - name of the file to write to
 * Pkt_Buff - pointer to begining of data to write
 * filesize - number of bytes to write to file
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  File new_file = SD.open(_filename);

  new_file.write(_Pkt_Buff,_filesize);

  new_file.close();
}


void delete_file(uint8_t _fileidx){
/*
 * Sends a file via telemetry
 * 
 * Inputs: 
 * fileidx - index of file on SD card
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */

 
  File entry;
  File rootdir = SD.open("/");
  int8_t file_status = 0;
  char filename[13]; // max filename is 8 characters + 1 period + 3 letter extention + 1 null term

   file_status = open_fileidx(rootdir, entry, _fileidx);

  // send back message indicating it was a directory
  if(file_status == ERROR_OPENFILEIDX_DIR){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <DELFILE> Cannot delete a directory");
  }
  // send back message indicating the file doesn't exist
  else if(file_status == ERROR_OPENFILEIDX_NOEXIST){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <DELFILE> File doesn't exist, not deleted");
  }
  // otherwise, delete the file
  else{
  
    sprintf(filename,"%12s",entry.name());
    SD.remove(filename);
    sendTxtMsg(SERIAL_DEBUG,"INFO: <DELFILE> File deleted");
  }

  rootdir.close();
  entry.close();
  return;
 
}

void send_file(uint8_t _fileidx){
/*
 * Sends a file via telemetry
 * NOTE: This command is BLOCKING. No operations will 
 * happen until the file has been sent.
 * 
 * Inputs: 
 * fileidx - index of file on SD card
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  File entry;
  File rootdir = SD.open("/");
  int8_t file_status = 0;
  
  file_status = open_fileidx(rootdir, entry, _fileidx);

  
  // send back message indicating it was a directory
  if(file_status == ERROR_OPENFILEIDX_DIR){
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> File is a directory, can't send");
  }
  // send back message indicating the file doesn't exist
  else if(file_status == ERROR_OPENFILEIDX_NOEXIST){
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> File doesn't exist, can't send");
  }
  // otherwise, send the file
  else{

    // first send a begin_send message
    // this is actually just a FileInfo message with a different APID, but the 
    // ground system needs to handle it differently
    uint8_t payload_buff[FILE_BYTES_PER_PKT+12];
    uint8_t payloadLength = 0;  
    uint32_t filesize;
    uint8_t checkSum = 0xFF;  
    char filename[13]; // max filename is 8 characters + 1 period + 3 letter extention + 1 null term
    uint16_t bytes_available = 0;
    
    sprintf(filename,"%12s",entry.name());
    filesize = entry.size();
    payloadLength = addStrToTlm(filename, payload_buff, payloadLength);
    payloadLength = addIntToTlm(filesize, payload_buff, payloadLength);
    
    // iterate through file and calculate checksum
    while(entry.available()){
      checkSum ^= entry.read();
    }

    payloadLength = addIntToTlm(checkSum, payload_buff, payloadLength);
  
    sendTlmMsg(SERIAL_DEBUG, APID_STAR_BEGIN_FILESEND, payload_buff, payloadLength);

    // then send the packet data
    entry.seek(0);
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> sent beginfile");
    //loop through packets sending bytes
    for (uint16_t i = 0; i < floor(filesize / FILE_BYTES_PER_PKT); i++){

      // read file data into packet and send it
      entry.read(payload_buff,FILE_BYTES_PER_PKT);
      sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILEDATA, payload_buff, payloadLength);
      sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> sent 100 bytes");
    }

    // one last packet to finish file
    bytes_available = entry.available();
    if(bytes_available < FILE_BYTES_PER_PKT){
      entry.read(payload_buff,entry.available());
      sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILEDATA, payload_buff, bytes_available);
    }
    else{
      sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> Calculation error!");
    }
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> sent filedata");
    // then send a end_send message
    payloadLength = 0;
    payloadLength = addStrToTlm(filename, payload_buff, payloadLength);
    payloadLength = addIntToTlm(filesize, payload_buff, payloadLength);
    payloadLength = addIntToTlm(checkSum, payload_buff, payloadLength);
  
    sendTlmMsg(SERIAL_DEBUG, APID_STAR_END_FILESEND, payload_buff, payloadLength);
    sendTxtMsg(SERIAL_DEBUG, "INFO: <SENDFILE> sent endfile");
  }
  
  rootdir.close();
  entry.close();
  return;
}


void find_fileinfo(uint8_t _fileidx){
/*
 * Sends file information (name and size) via telemetry
 * 
 * Inputs: 
 * fileidx - index of file on SD card
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
  File entry;
  File rootdir = SD.open("/");
  int8_t file_status = 0;
  char filename[13]; // max filename is 8 characters + 1 period + 3 letter extention + 1 null term
  uint8_t payload_buff[13+4+1];
  uint8_t payloadLength = 0;  
  uint8_t checkSum = 0xFF;  
  uint32_t filesize;

  file_status = open_fileidx(rootdir, entry, _fileidx);

  // send back message indicating it was a directory
  if(file_status == ERROR_OPENFILEIDX_DIR){
    sprintf(filename,"%12s",entry.name());
    payloadLength = addStrToTlm(filename, payload_buff, payloadLength);
    payloadLength = addIntToTlm(0, payload_buff, payloadLength);
    payloadLength = addIntToTlm(checkSum, payload_buff, payloadLength);
    sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILEINFO, payload_buff, payloadLength);
  }
  // send back message indicating the file doesn't exist
  else if(file_status == ERROR_OPENFILEIDX_NOEXIST){
    strcpy(filename,"no_exist.noo");
    payloadLength = addStrToTlm(filename, payload_buff, payloadLength);
    payloadLength = addIntToTlm(0, payload_buff, payloadLength);
    payloadLength = addIntToTlm(checkSum, payload_buff, payloadLength);
    sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILEINFO, payload_buff, payloadLength);
  }
  // otherwise, send back the filename and length
  else{
  
    sprintf(filename,"%12s",entry.name());
    filesize = entry.size();
    payloadLength = addStrToTlm(filename, payload_buff, payloadLength);
    payloadLength = addIntToTlm(filesize, payload_buff, payloadLength);
    
    // iterate through file and calculate checksum
    while(entry.available()){
      checkSum ^= entry.read();
    }

    payloadLength = addIntToTlm(checkSum, payload_buff, payloadLength);
  
    sendTlmMsg(SERIAL_DEBUG, APID_STAR_FILEINFO, payload_buff, payloadLength);
  }

  rootdir.close();
  entry.close();
  return;
 
}

void set_LED(uint8_t _brightness){
/*
 * Sets LED to desired brightness
 * 
 * Inputs: 
 * brightness - brightness [normalized units]
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  digitalWrite(PIN_LEDPWM, _brightness);
  sendTxtMsg(SERIAL_DEBUG, "INFO: <LED> Set LED brightness.");
}

void set_bias(uint8_t _enable){
  digitalWrite(PIN_BIAS, _enable);

  if(_enable){
    sendTxtMsg(SERIAL_DEBUG, "INFO: <BIAS> Set bias to enabled.");
  }
  else{
    sendTxtMsg(SERIAL_DEBUG, "INFO: <BIAS> Set bias to disabled");
  }
}

void set_shaker(uint8_t _shake_speed){
/*
 * Sets shaker to a desired speed
 * 
 * Inputs: 
 * shake_speed - speed of shaking [units???]
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  // do math here to figure out what to command
  
  digitalWrite(PIN_MOTORPWM, _shake_speed);
  sendTxtMsg(SERIAL_DEBUG, "INFO: <SHAKE> Set shake speed");
}

void set_cutoff(uint8_t _enable){
/*
 * Sets status of cutoff circuits.
 * 
 * Inputs: 
 * enable - flag indicating cutoff status [1 = cutoff, 0 = connected]
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  digitalWrite(PIN_CUTOFF, _enable);
  if(_enable){
    sendTxtMsg(SERIAL_DEBUG, "INFO: <CUTOFF> Set cutoff to enabled");
  }
  else{
    sendTxtMsg(SERIAL_DEBUG, "INFO: <CUTOFF> Set cutoff to disabled");
  }
}

void send_tlm(uint8_t _tlm_requested){
  // Do not implement for now
  // argument is a bitfield indicating which packets to output
  sendTxtMsg(SERIAL_DEBUG, "ERROR: <SENDTLM> SEND_TLM is not yet implemented");
}

void send_cmd_response(uint8_t _fcncode, uint8_t _stat){
/*
 * Sends a message indicating the status of a command execution.
 * 
 * Inputs: 
 * fcncode - function code of command being reported
 * stat - flag indicating status
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
  uint8_t payloadLength = 0;
  uint8_t payload_buff[2];
  
  payloadLength = addIntToTlm(_fcncode, payload_buff, payloadLength); // Add the value of the table to message
  payloadLength = addIntToTlm(_stat, payload_buff, payloadLength); // Add the value of the table to message

  sendTlmMsg(SERIAL_DEBUG, APID_STAR_CMDRESP, payload_buff, payloadLength);

}

void send_status_msg(uint8_t _stat){
/*
 * Sends a message indicating the status of STAR.
 * 
 * Inputs: 
 * stat - flag indicating status
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
  uint8_t payloadLength = 0;
  uint8_t payload_buff[1];
  
  payloadLength = addIntToTlm(_stat, payload_buff, payloadLength); // Add the value of the table to message

  sendTlmMsg(SERIAL_DEBUG, APID_STAR_STATUS, payload_buff, payloadLength);

}

