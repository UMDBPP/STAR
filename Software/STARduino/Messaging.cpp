
#include "Messaging.h"

uint32_t _SentPktCtr = 0;
File logfile_interface;

void read_serial(Stream &_serial, Cmd_Pkt_Buff_t &_Pkt_Buff){

  // increment the counter which tracks how long since the last read
  _Pkt_Buff.cycles_since_last_read++;

  // if there's data in the buffer but its been a while since it was read,
  // assume it was a partial packet and "clear" it from the buffer to discard it
  // so that we can receive new commands
  if(_Pkt_Buff.end_pos > 0 && _Pkt_Buff.cycles_since_last_read > MAX_READ_CYCLES_STALENESS){
    _Pkt_Buff.end_pos = 0;
  }
  
  // if data is available, attempt to read it
  if(_serial.available()){

    // don't read if reading would overflow the buffer
    if(_serial.available() > _Pkt_Buff.end_pos + _Pkt_Buff.buf_size){
      sendTxtMsg(SERIAL_DEBUG,"ERROR: <READ_SERIAL> Buffer overflow imminent, not reading command");
    }
    else{
      uint8_t BytesRead = 0;
  
      BytesRead = SERIAL_DEBUG.readBytes((char*)_Pkt_Buff.bytes[_Pkt_Buff.end_pos+1], _serial.available());
      _Pkt_Buff.end_pos += BytesRead;
      _Pkt_Buff.cycles_since_last_read = 0;
    }
  }
}

bool full_cmd_available(Cmd_Pkt_Buff_t _Pkt_Buff){

  return _Pkt_Buff.end_pos > getPacketLength(_Pkt_Buff.bytes);
}

void set_msg_logfile(File logfile){
  logfile_interface = logfile;
}

void sendTxtMsg(Stream &_serial, const char str[]){

  // if the string is too long, don't send it
  if(strlen(str)+12 > MAX_TLM_LEN){
    return;
  }

  //_serial.print("Sending: '");
  //_serial.print(str);
  //_serial.print("' of length: ");
  //_serial.print(strlen(str));
  sendTlmMsg(_serial, APID_STAR_TXTMSG, (uint8_t*)str, strlen(str));
  delay(10);
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
  if(logfile_interface){
    // log 'S' to show its a sent packet
    logfile_interface.print("S ");
  
    // print the time of the message
    print_datestamp(logfile_interface, get_MET());
    // print the packet
    logfile_interface.print(" ");
    
    char buf[5];
    // print the data in hex
    for(int i = 0; i < pkt_size; i++){
        sprintf(buf, "%02x, ", pkt_buf[i]);
        logfile_interface.print(buf);
    }
    logfile_interface.println();
    logfile_interface.flush();
  }
}

uint8_t addStrToTlm(char *s, uint8_t payload[], uint8_t start_pos){

  memcpy(payload+start_pos,s,strlen(s));
  return start_pos + strlen(s);
}
