#include <stdio.h>      /* printf */
#include <stdint.h>      /* printf */
#include <assert.h>     /* assert */
#include <string.h> // memcpy
#include <iostream>
#include <fstream> // for ifstream

#include "../CCSDS/ccsds_utilities.h"
#include "../Queue/StaticQueue.h"

#include "Stub_SD.cpp"


int main(){
  
  char fileName7[] = "non_existant_cmds.cmd";
  char fileName8[] = "ut_cmds_badchksum.cmd";
  
  // attempt to open the file
  File cmdFile = SD.open(fileName7);
  
  // make sure no existant file doesn't register as open
  assert(!cmdFile == 1);
  
  // make sure file length is correct
  assert(cmdFile.available() == -1);

  // attempt to open the file
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
  printf("4Reading cmds from '%s' \n",fileName8);
  uint8_t tmp8_1[4];
  printf("4.0Reading cmds from '%s' \n",fileName8);
  cmdFile.read(&tmp8_1,4);
  
  printf("4.5Reading cmds from '%s' \n",fileName8);

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
  
  // make sure file length is correct
  assert(!cmdFile == 1);
  
  // attempt to open the file
  printf("Reading cmds from '%s' \n",fileName8);
  cmdFile = SD.open(fileName8);
  
  assert(!cmdFile == 0);
  assert(cmdFile.available() > 0);
  
  uint32_t tmp32_2;
  uint8_t tmp_pkt[8];
  printf("REading cmds in \n");
  for (int i = 0; i < 10; i++){
    
    if(cmdFile.available() > 4){
      cmdFile.read(&tmp32_2,sizeof(uint32_t));
      //tmp32_2 = __builtin_bswap32(tmp32_2);
      printf("time: 0x%x \n",tmp32_2);
    }
    else{
      break;
    }
    if(cmdFile.available() > 6){
      cmdFile.read(&tmp_pkt,sizeof(tmp_pkt));
      for(int j = 0; j<9;j++){
        printf("0x%x ",tmp_pkt[j]);
      }
      printf("\n");
    }
    else{
      break;
    }
  }
}