#include <stdio.h>      /* printf */
#include <stdint.h>      /* printf */
#include <assert.h>     /* assert */
#include <string.h> // memcpy
#include <iostream>
#include <fstream> // for ifstream
#include <stdlib.h>     /* malloc, free, rand */

#include "../CCSDS/ccsds_utilities.h"
#include "../Queue/StaticQueue.h"

/*
 * Include Stub_SD.cpp
 * While its not standard to include a cpp file, it seemed
 * like it would be a pain to figure out how to swap the 
 * header file that Stub_SD.cpp includes
 */
#include "Stub_SD.cpp"


int main(){
  
  // define filenames for reading
  char fileName7[] = "non_existant_cmds.cmd";
  char fileName8[] = "ut_cmds.cmd";

  // attempt to open the file
  File cmdFile = SD.open(fileName7);
  
  // make sure non-existant file doesn't register as open
  assert(!cmdFile == 1);
  
  // make sure file length is correct when file isn't open
  assert(cmdFile.available() == -1);
  
  // make sure read fails when file isn't open
  assert(cmdFile.read() == -1);

  // attempt to open a valid the file
  cmdFile = SD.open(fileName8);
  
  // make sure file registers as open
  assert(!cmdFile == 0);
  
  // make sure file length is correct
  assert(cmdFile.available() == 50);
  
  // read one byte
  uint8_t c = cmdFile.read();
  printf("Read 0x%x\n",c);
  
  // make sure file length is correct
  assert(cmdFile.available() == 49);
  
  // read several bytes
  uint8_t tmp8_1[4];
  cmdFile.read(&tmp8_1,4);
  
  printf("Read ");
  for (int i =0; i < sizeof(tmp8_1)/sizeof(tmp8_1[0]); i++){
    printf("0x%x ",tmp8_1[i]);
  }
  printf("\n");
  
  // make sure file length is correct
  assert(cmdFile.available() == 45);

  uint32_t tmp32;
  cmdFile.read(&tmp32,sizeof(uint32_t));
  tmp32 = __builtin_bswap32(tmp32);
  printf("Read 0x%x \n",tmp32);

  // make sure file length is correct
  assert(cmdFile.available() == 41);
  
  cmdFile.close();

  // make sure file length is correct
  assert(cmdFile.available() == -1);
  
  // make sure file bool operator is correct
  assert(!cmdFile == 1);
  
  // attempt to open the file
  printf("Reading cmds from '%s' \n",fileName8);
  cmdFile = SD.open(fileName8);
  
  // make sure file registers as open
  assert(!cmdFile == 0);
  
  // make sure file has bytes in it
  assert(cmdFile.available() > 0);
  
  uint32_t tmp32_2;
  uint8_t tmp_pkt[9];
  uint8_t cmd_size = 0;
  printf("REading cmds in \n");
  for (int i = 0; i < 10; i++){
    
    // read timestamp
    if(cmdFile.available() > 4){
      cmdFile.read(&tmp32_2,sizeof(uint32_t));
      tmp32_2 = __builtin_bswap32(tmp32_2);
      printf("time: 0x%x \n",tmp32_2);
    }
    else{
      break;
    }
    
    // read in different size commands
    // Note: this is defined based on the cmd file that
    // we're reading in for this test
    if(i==0){
      cmd_size = 8;
    } 
    else if(i ==1){
      cmd_size = 9;
    }
    else if(i == 2){
      cmd_size = 9;
    }
    else if(i == 3){
      cmd_size = 8;
    }
    // read command
    if(cmdFile.available() > cmd_size){
      cmdFile.read(&tmp_pkt,cmd_size);
      for(int j = 0; j<cmd_size;j++){
        printf("0x%x ",tmp_pkt[j]);
      }
      printf("\n");
    }
    else{
      break;
    }
    
    // validate the bytes we read are correct
    // Note: this is defined based on the cmd file that
    // we're reading in for this test
    if(i==0){
    assert(tmp32_2 == 0x0000131a);
    assert(tmp_pkt[0] == 0x10);
    assert(tmp_pkt[1] == 0x64);
    assert(tmp_pkt[6] == 0x09);
    assert(tmp_pkt[7] == 0x43);
    }
    else if(i==1){
      assert(tmp32_2 == 0x00002710);
      assert(tmp_pkt[0] ==0x10);
      assert(tmp_pkt[1] == 0x64);
      assert(tmp_pkt[6] == 0x0a);
      assert(tmp_pkt[8] == 0x01);
    }
    else if(i==2){
      assert(tmp32_2 == 0x00003a1a);
      assert(tmp_pkt[0] ==0x10);
      assert(tmp_pkt[1] == 0x64);
      assert(tmp_pkt[6] == 0x0c);
      assert(tmp_pkt[8] == 0x00);
    }
    else if(i==3){
      assert(tmp32_2 == 0x00004320);
      assert(tmp_pkt[0] ==0x10);
      assert(tmp_pkt[1] == 0x64);
      assert(tmp_pkt[6] == 0x09);
      assert(tmp_pkt[7] == 0x43);
    }
  }
}