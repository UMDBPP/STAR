
#include "ccsds_utilities.h"

CCSDS_PriHdr_t getPrimaryHeader(uint8_t _packet[]) {
	return *(CCSDS_PriHdr_t*)(_packet);
}

uint8_t validateChecksum(uint8_t _packet[]) {
	CCSDS_CmdPkt_t *header = (CCSDS_CmdPkt_t*) _packet;

	return CCSDS_ValidCheckSum(header);
}

uint16_t getAPID(uint8_t _packet[]) {
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