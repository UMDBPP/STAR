
#include "Messaging.h"

uint32_t _SentPktCtr = 0;
File logfile_interface_ptr;


void read_serial(Stream &_serial, Cmd_Pkt_Buff_t *_Pkt_Buff){
/*
 * Reads data from the serial into the current end of the buffer, 
 * updates the buffer length, and handles clearing the buffer when 
 * data has expired
 * 
 * Inputs: 
 * _serial - serial to read data from
 * _Pkt_Buff - data structure to update/read data into
 * 
 * Output:
 * _Pkt_Buff - updated data structure
 * 
 * Return:
 * none
 * 
 */
 
  // increment the counter which tracks how long since the last read
  _Pkt_Buff->cycles_since_last_read++;

  // if there's data in the buffer but its been a while since it was read,
  // assume it was a partial packet and "clear" it from the buffer to discard it
  // so that we can receive new commands

  if(_Pkt_Buff->end_pos > 0 && (_Pkt_Buff->cycles_since_last_read > MAX_READ_CYCLES_STALENESS)){

    char buf[100];
    sprintf(buf,"INFO: <READ_SERIAL> Have %d bytes of data, clearing buffer",_Pkt_Buff->end_pos);
    sendTxtMsg(SERIAL_DEBUG,buf);
    _Pkt_Buff->end_pos = 0;
  }

  uint8_t BytesRead = 0;
  
  // if data is available, attempt to read it
  if(BytesRead = _serial.available()){

    
    // don't read if reading would overflow the buffer
    if(BytesRead > _Pkt_Buff->end_pos + _Pkt_Buff->buf_size){
      sendTxtMsg(_serial,"ERROR: <READ_SERIAL> Buffer overflow imminent, not reading command");
    }
    else{
      
      // read the bytes directly into the current end of the buffer
      BytesRead = _serial.readBytes((char*)_Pkt_Buff->bytes +_Pkt_Buff->end_pos, BytesRead);

      // update packet reading variables
      _Pkt_Buff->end_pos += BytesRead;
      _Pkt_Buff->cycles_since_last_read = 0;

    }
  }
}

bool full_cmd_available(Cmd_Pkt_Buff_t _Pkt_Buff){
/*
 * Returns if a full command is available for execution
 * 
 * Inputs: 
 * _Pkt_Buff - data structure containing command
 * 
 * Output:
 * none 
 * 
 * Return:
 * flag indicating if there's a command to execute
 * 
 */

  // command is available if buffer isn't empty and length of packet in the buffer is 
  // shorter than the current length of the buffer
  return _Pkt_Buff.end_pos > 0 && (_Pkt_Buff.end_pos >= getPacketLength(_Pkt_Buff.bytes));
}

void set_msg_logfile(File &_logfile){
/*
 * Sets the logfile to log interface I/O to
 * 
 * Inputs: 
 * logfile - handle of file to log to
 * 
 * Output:
 * none 
 * 
 * Return:
 * none
 * 
 */
  
  logfile_interface_ptr = _logfile;
}

void sendTxtMsg(Stream &_serial, const char _str[]){
/*
 * Sends a telemetry packet containing text
 * 
 * Inputs: 
 * _serial - serial to write message to
 * _str - string to write into packet
 * 
 * Output:
 * none 
 * 
 * Return:
 * none
 * 
 */
 
  // if the string is too long, don't send it
  if(strlen(_str)+12 > MAX_TLM_LEN){
    return;
  }

 //_serial.println(_str);
  sendTlmMsg(_serial, APID_STAR_TXTMSG, (uint8_t*)_str, strlen(_str));

  // Not sure if we want this, but Cosmos appears to have trouble dealing with 
  // packets too closely spaced
  delay(50);
}


int sendTlmMsg(Stream &_serial, uint16_t _APID, uint8_t _payload[], uint16_t _payload_size){
/*
 * Sends a telemetry message
 * 
 * Inputs: 
 * _serial - Serial object to send message on
 * _APID - Application ID of message to send... identifies sender and format of message
 *    to recipient
 * _payload - Pointer to array of bytes to in message payload
 * _payload_size - Length of array of bytes containing message payload
 * 
 * Output:
 * none
 * 
 * Return:
 * 0 always.
 * 
 */

  uint8_t pkt_buf[MAX_TLM_LEN];

  // fill primary header fields
  setAPID(pkt_buf, _APID);
  setSecHdrFlg(pkt_buf, 1);
  setPacketType(pkt_buf, CCSDS_TLM_PKT);
  setVer(pkt_buf, 0);
  setSeqCtr(pkt_buf, _SentPktCtr);
  setSeqFlg(pkt_buf, 0x03);
  setPacketLength(pkt_buf, _payload_size+sizeof(CCSDS_TlmPkt_t));

  // fill secondary header fields
  setTlmTimeSec(pkt_buf, get_MET()/1000);
  setTlmTimeSubSec(pkt_buf, get_MET()%1000);

  // copy the packet data
  memcpy(pkt_buf+sizeof(CCSDS_TlmPkt_t), _payload, _payload_size);

  // update the payload_size to include the headers
  _payload_size += sizeof(CCSDS_TlmPkt_t);

  // send and log the packet
  _serial.write(pkt_buf,_payload_size);
  log_sent_pkt(pkt_buf,_payload_size);

  // update counter with this packet
  _SentPktCtr++;
  return 0;
}

void log_sent_pkt(uint8_t pkt_buf[], uint16_t pkt_size){
/*
 * Logs the sending of a packet.
 * 
 * Inputs: 
 * pkt_buf - arrray contains packet
 * pkt_size - length (in bytes) of the packet
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */
 
  if(logfile_interface_ptr){
    // log 'S' to show its a sent packet
    logfile_interface_ptr.print("S ");
  
    // print the time of the message
    print_datestamp(logfile_interface_ptr, get_MET());
    // print the packet
    logfile_interface_ptr.print(" ");
    
    char buf[5];
    // print the data in hex
    for(int i = 0; i < pkt_size; i++){
        sprintf(buf, "%02x, ", pkt_buf[i]);
        logfile_interface_ptr.print(buf);
    }
    logfile_interface_ptr.println();
    logfile_interface_ptr.flush();
  }
  else{
    // don't do anything because our logfile isn't open
    SERIAL_DEBUG.println("ERROR: <LOGPKT> LogFile isn't open!");
  }
}


