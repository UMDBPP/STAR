#include <stdio.h>      /* printf */
#include <stdint.h>      /* printf */
#include <assert.h>     /* assert */
#include <string.h> // memcpy
#include <iostream>
#include <fstream> // for ifstream

#include "../CCSDS/ccsds_utilities.h"
#include "../Queue/StaticQueue.h"

// flag indicating that we're testing
#define _IS_UT_

#define SERIAL_DEBUG 0
#define MAX_TLM_LEN 255
#define MAX_CMD_LEN 9
#define APID_STAR_FILELOADERR 0
#define ERROR_SDLOAD_OPENFILE -1
#define ERROR_SDLOAD_CMDCHKSUM -3
#define ERROR_FLASHLOAD_NOTIMP -5
#define ERROR_SDLOAD_SHORTTIME -6
#define ERROR_SDLOAD_SHORTHEAD -7
#define ERROR_SDLOAD_SHORTPKT -8
#define ERROR_SDLOAD_LONGCMD -2
#define ERROR_SDLOAD_INFILOOP -9

/* Define Datatypes */
typedef struct CCSDS_Cmd_t {
  uint32_t timestamp;
  uint8_t bytes[8];
}CCSDS_Cmd_t;

typedef uint8_t Stream;


#include "Stub_SD.cpp"


// stubbed sendTxtMsg
void sendTxtMsg(Stream _serial, const char str[]){
  printf(str);
  printf("\n");
}

int sendTlmMsg(Stream _serial, uint16_t _APID, uint8_t _payload[], uint16_t _payload_size){
  
}

int8_t load_cmdseq(char *fileName, uint16_t page_num);
int8_t load_cmds_sd(char *fileName, uint16_t &file_pos);
int8_t load_cmds_flash(uint16_t page_num, uint16_t &file_pos);
void send_fileload_error(int8_t error_code, uint16_t filepos);
bool time_for_queued_cmd(uint32_t MET);
void inject_cmd(uint8_t Pkt_Buff[]);
void enable_cmd_queue();
void disable_cmd_queue();

#include "../CmdQueue.cpp"

bool test_enable_queue_flag(){
  
  assert(Queue_Enabled_Flag == true);
  disable_cmd_queue();
  assert(Queue_Enabled_Flag == false);
  enable_cmd_queue();
  assert(Queue_Enabled_Flag == true);
  return 1;
}

bool test_inject_cmd(){
  uint8_t Pkt_Buff[25];

  CCSDS_Cmd_t tmp_pop;
  CCSDS_Cmd_t tmp_push = {.timestamp = 10, .bytes = {0x10, 0x64, 0xC0, 0x00, 0x00, 0x01, 0x09, 0x43}};

  // empty the queue
  while(!cmd_queue.isEmpty()){
    cmd_queue.pop(tmp_pop);
  }

  // verify there's nothing in the queue
  assert(cmd_queue.getSize() == 0);

  // add a test element into the queue
  cmd_queue.push(tmp_push);

  // inject the command into the buffer
  inject_cmd(Pkt_Buff);

  // confirm that its in the buffer
  assert(Pkt_Buff[0] == 0x10);
  assert(Pkt_Buff[1] == 0x64);
  assert(Pkt_Buff[2] == 0xC0);
  assert(Pkt_Buff[3] == 0x00);
  assert(Pkt_Buff[4] == 0x00);
  assert(Pkt_Buff[5] == 0x01);
  assert(Pkt_Buff[6] == 0x09);
  assert(Pkt_Buff[7] == 0x43);
  
  // empty the queue
  while(!cmd_queue.isEmpty()){
    cmd_queue.pop(tmp_pop);
  }
  
  return 1;
}

bool test_time_for_cmd(){
  uint8_t Pkt_Buff[100];

  CCSDS_Cmd_t tmp_pop;
  CCSDS_Cmd_t tmp_push = {.timestamp = 10, .bytes = {1, 2, 3, 4, 5, 6, 7, 8}};
  
  // empty the queue
  while(!cmd_queue.isEmpty()){
    cmd_queue.pop(tmp_pop);
  }
  
  // verify there's nothing in the queue
  assert(cmd_queue.getSize() == 0);
   
  // add a test element into the queue
  cmd_queue.push(tmp_push);
  
  // confirm that it doesn't trigger early
  assert(time_for_queued_cmd(0) == 0);
  assert(time_for_queued_cmd(5) == 0);
  
  // check that it triggers at time
  assert(time_for_queued_cmd(10) == 1);
  
  // check that it triggers after time
  assert(time_for_queued_cmd(11) == 1);
  assert(time_for_queued_cmd(15) == 1);
  assert(time_for_queued_cmd(65535) == 1);
  
  // empty the queue
  while(!cmd_queue.isEmpty()){
    cmd_queue.pop(tmp_pop);
  }
  
  return 1;
}

bool test_load_cmd_sd(){
  
  int8_t status = 0;
  uint16_t file_pos = 0;
  
  // verify that correct error message is thrown when file doesn't exist
  char fileName1[] = "non_existant_cmds.cmd";
  status = load_cmds_sd(fileName1, file_pos);
  assert(status == ERROR_SDLOAD_OPENFILE);
  
  // verify a command with bad checksum is detected
  char fileName2[] = "ut_cmds_badchksum.cmd";
  status = load_cmds_sd(fileName2, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_CMDCHKSUM);
  
  // verify a command with bad checksum is detected
  char fileName3[] = "ut_cmds_badchksum.cmd";
  status = load_cmds_sd(fileName3, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_CMDCHKSUM);
  
  // verify a command with short timestamp is detected
  char fileName4[] = "ut_cmds_shorttime.cmd";
  status = load_cmds_sd(fileName4, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_SHORTTIME);
  
  // verify a command with short header is detected
  char fileName5[] = "ut_cmds_shorthead.cmd";
  status = load_cmds_sd(fileName5, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_SHORTHEAD);
  
  // verify a command with short pkt is detected
  char fileName6[] = "ut_cmds_shortpkt.cmd";
  status = load_cmds_sd(fileName6, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_SHORTPKT);
  
  // verify a command with a long cmd is detected
  char fileName7[] = "ut_cmds_long_cmd.cmd";
  status = load_cmds_sd(fileName7, file_pos);
  printf("Status: %d \n",status);
  assert(status == ERROR_SDLOAD_LONGCMD);

}

int main(){
  
  if(test_enable_queue_flag()){
    printf("Queue functions passed!! \n");
  }
  if(test_inject_cmd()){
    printf("Inject cmd functions passed!  \n");
  }
  if(test_time_for_cmd()){
    printf("Time for cmd functions passed!  \n");
  }
  if(test_load_cmd_sd()){
    printf("Load cmd SD function passed!  \n");
  }
}