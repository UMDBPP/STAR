#ifndef _ccsds_utilities_h_
#define _ccsds_utilities_h_

#include <stdint.h>
#include <string.h> // memcpy
#include "CCSDS.h"

#define CCSDS_CMD_PKT 1
#define CCSDS_TLM_PKT 0

CCSDS_PriHdr_t getPrimaryHeader(uint8_t _packet[]);
uint8_t validateChecksum(uint8_t _packet[]);

uint16_t getAPID(uint8_t _packet[]);
void setAPID(uint8_t _packet[], uint16_t APID);

uint8_t getPacketType(uint8_t _packet[]);
uint8_t getCmdFunctionCode(uint8_t _packet[]);
void setSecHdrFlg(uint8_t _packet[], uint8_t SHDR);
void setPacketType(uint8_t _packet[], uint8_t Type);
void setVer(uint8_t _packet[], uint8_t ver);
void setSeqCtr(uint8_t _packet[], uint16_t seqctr);
void setSeqFlg(uint8_t _packet[], uint8_t seqflg);

uint16_t getPacketLength(uint8_t _packet[]);
void setPacketLength(uint8_t _packet[], uint16_t Len);

void setTlmTimeSec(uint8_t _packet[], uint32_t sec);
void setTlmTimeSubSec(uint8_t _packet[], uint16_t subsec);

uint8_t extractStrFromTlm(char str[], uint8_t _packet[], uint8_t start_pos, uint8_t len);
uint8_t addStrToTlm(char *s, uint8_t payload[], uint8_t start_pos);

CCSDS_CmdSecHdr_t getCmdHeader(uint8_t _packet[]);

template<typename T> uint8_t addIntToTlm(const T& val, uint8_t payload[], uint8_t start_pos) {
  for(uint8_t i = 0; i < sizeof(T); i++) { // Loop through each byte
  	uint8_t shiftBits = 8 * (sizeof(T) - (i + 1)); // Number of bits to right shift
  	payload[start_pos + i] = (val >> shiftBits) & 0xFF; // Assign value
  }
  return start_pos + sizeof(T); // Return new array index.
}


template<typename T> uint8_t extractFromTlm(T& extractedVal, uint8_t data[], uint8_t start_pos) {
  uint8_t tmp[sizeof(T)];

  for(uint8_t i = 0; i < sizeof(T); i++) {
  	tmp[sizeof(T) - (i + 1)] = data[start_pos + i]; // endianness...
    //tmp[i] = data[start_pos + i]; // endianness...
  }
  memcpy(&extractedVal,&tmp[0],sizeof(T));
  return start_pos + sizeof(T);
}

#endif