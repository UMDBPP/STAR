
#include "ccsds_utilities.h"

CCSDS_PriHdr_t getPrimaryHeader(uint8_t _packet[]) {
/*
 * Returns a pointer to the primary header of the packet
 * 
 * Inputs: 
 * _packet - array contains packet
 * 
 * Output:
 * none
 * 
 * Return:
 * CCSDS_PriHdr_t pointer to array containing packet 
 * 
 */
	return *(CCSDS_PriHdr_t*)(_packet);
}

uint8_t validateChecksum(uint8_t _packet[]) {
/*
 * Validates the checksum contained within a CCSDS packet
 * 
 * Inputs: 
 * _packet - array contains packet
 * 
 * Output:
 * none
 * 
 * Return:
 * flag indicating if checksum is correct [1=correct, 0=not]
 * 
 */
	CCSDS_CmdPkt_t *header = (CCSDS_CmdPkt_t*) _packet;

	return CCSDS_ValidCheckSum(header);
}

uint16_t getAPID(uint8_t _packet[]) {
/*
 * Returns the APID from an array containing a CCSDS packet
 * 
 * Inputs: 
 * _packet - array contains packet
 * 
 * Output:
 * none
 * 
 * Return:
 * APID of packet
 * 
 */
	CCSDS_PriHdr_t header = getPrimaryHeader(_packet);

	return CCSDS_RD_APID(header);
}

void setAPID(uint8_t _packet[], uint16_t APID) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_APID((*header),APID);
}

uint8_t getPacketType(uint8_t _packet[]) {
	CCSDS_PriHdr_t header = getPrimaryHeader(_packet);

	return CCSDS_RD_TYPE(header);
}

uint8_t getCmdFunctionCode(uint8_t _packet[]) {
	CCSDS_CmdSecHdr_t shdr = getCmdHeader(_packet);

	return CCSDS_RD_FC(shdr);
}

void setSecHdrFlg(uint8_t _packet[], uint8_t SHDR) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_SHDR((*header),SHDR);
}

void setPacketType(uint8_t _packet[], uint8_t Type) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_TYPE((*header),Type);
}

void setVer(uint8_t _packet[], uint8_t ver) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_VERS((*header),ver);
}

void setSeqCtr(uint8_t _packet[], uint16_t seqctr) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_SEQ((*header),seqctr);
}

void setSeqFlg(uint8_t _packet[], uint8_t seqflg) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_SEQFLG((*header),seqflg);
}

uint16_t getPacketLength(uint8_t _packet[]) {
	CCSDS_PriHdr_t header = getPrimaryHeader(_packet);

	return CCSDS_RD_LEN(header);
}

void setPacketLength(uint8_t _packet[], uint16_t Len) {
	CCSDS_PriHdr_t *header = (CCSDS_PriHdr_t*)_packet;

	CCSDS_WR_LEN((*header), Len);
}

void setTlmTimeSec(uint8_t _packet[], uint32_t sec) {
	CCSDS_TlmSecHdr_t *header = (CCSDS_TlmSecHdr_t*)(_packet+sizeof(CCSDS_PriHdr_t));

	CCSDS_WR_SEC_HDR_SEC((*header), sec);
}

void setTlmTimeSubSec(uint8_t _packet[], uint16_t subsec) {
	CCSDS_TlmSecHdr_t *header = (CCSDS_TlmSecHdr_t*)(_packet+sizeof(CCSDS_PriHdr_t));

	CCSDS_WR_SEC_HDR_SUBSEC((*header), subsec);
}

CCSDS_CmdSecHdr_t getCmdHeader(uint8_t _packet[]) {
	return *(CCSDS_CmdSecHdr_t*)(_packet+sizeof(CCSDS_PriHdr_t)-1);
}

uint8_t extractStrFromTlm(char str[], uint8_t _packet[], uint8_t start_pos, uint8_t len){
  // FIXME: Needs to be tested
  
  memcpy(str,_packet + start_pos,len);
  return start_pos+len;
}

uint8_t addStrToTlm(char *_str, uint8_t _payload[], uint8_t _start_pos){
/*
 * Copies a string to a position in an array and returns the new length
 * 
 * Inputs: 
 * _str - string to add to array
 * _payload - array to copy _str into
 * _start_pos - position in array to copy _str into
 * 
 * Output:
 * none
 * 
 * Return:
 * new length of array
 * 
 */
 
  memcpy(_payload+_start_pos,_str,strlen(_str));
  return _start_pos + strlen(_str);
}