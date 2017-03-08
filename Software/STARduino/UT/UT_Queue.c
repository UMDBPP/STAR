#include <stdio.h>      /* printf */
#include <stdint.h>      /* printf */
#include <assert.h>     /* assert */
#include <string.h> // memcpy

#include "../Queue/StaticQueue.h"

/* Define Datatypes */
typedef struct CCSDS_Cmd_t {
  uint32_t timestamp;
  uint8_t bytes[8];
}CCSDS_Cmd_t;

/*
 * Testes the Queue class with an uint8_t array.
 * THIS DOES NOT WORK BECAUSE C++ POINTER MAGIC, don't do it
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 1 if success, fails internally otherwise
 */
int test_uint8_array_queue(){
  
  Queue <uint8_t[5]> testqueue;
  
  uint8_t tmp_peek[5] = {255, 255, 255, 255, 255};
  uint8_t tmp_pop[5] = {255, 255, 255, 255, 255};
  uint8_t tmp_push[5] = {255, 255, 255, 255, 255};

  // test that queue starts out empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue starts out not full
  assert(testqueue.isFull() == 0);
  
  // test that the size of an empty queue is zero
  assert(testqueue.getSize() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
  
  for(uint8_t i=0; i<10; i++){
      
    // test that queue is not full
    assert(testqueue.isFull() == 0);
    
    for (uint8_t j = 0; j < 6; j++){
      tmp_push[j] = i;
    }
    printf("i: %d, tmp_peek[1:5]: %d  %d  %d  %d  %d\n",i, tmp_push[0], tmp_push[1], tmp_push[2], tmp_push[3], tmp_push[4]);

    // test that pushing succeeds
    assert(testqueue.push(tmp_push) == 1);
    
    // test that peek with no arguments always returns
    // first value
    assert(testqueue.peek(tmp_peek) == 1);
    printf("i: %d, tmp_peek[1:5]: %d  %d  %d  %d  %d\n",i, tmp_peek[0], tmp_peek[1], tmp_peek[2], tmp_peek[3], tmp_peek[4]);
    for (uint8_t j = 0; j < 6; j++){
      assert(tmp_peek[j] == i);
    }
    
    // test that peek with argument returns the correct
    // value
    assert(testqueue.peek(tmp_peek,i) == 1);
    for (uint8_t j = 0; j < 6; j++){
      assert(tmp_peek[j] == i);
    }
    
    // test that the queue is not empty
    assert(testqueue.isEmpty() == 0);
    
    // test that getSize is correct
    assert(testqueue.getSize() == i+1);
   
  }
  
  // test that queue is full
  assert(testqueue.isFull() == 1);
  
  for (uint8_t j = 0; j < 6; j++){
      assert(tmp_peek[j] == 1);
    }
  // test that pushing to a full queue fails
  assert(testqueue.push(tmp_push) == 0);
  
  // test that a full queue is not empty
  assert(testqueue.isEmpty() == 0);
  
  // test that peek with an out of range value fails
  assert(testqueue.peek(tmp_peek,20) == 0);
  
  for(uint8_t i=0; i<10; i++){
    
    // test that peeking at the next value succeeds
    assert(testqueue.peek(tmp_peek) == 1);
    
    // test that peeking returns the next value
    for (uint8_t j = 0; j < 6; j++){
      assert(tmp_peek[j] == i);
    }
    
    // test that popping the next value succeeds
    assert(testqueue.pop(tmp_pop) == 1);
    
    // test that popping returns the next value
    for (uint8_t j = 0; j < 6; j++){
      assert(tmp_peek[j] == i);
    }
  }
   
   // test that queue is empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue is not full
  assert(testqueue.isFull() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
   
   
  return 1;
}


/*
 * Testes the Queue class with an uint8_t.
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 1 if success, fails internally otherwise
 */
int test_uint8_queue(){
  
  Queue <uint8_t> testqueue;
  
  uint8_t tmp_peek = 255;
  uint8_t tmp_pop = 255;

  // test that queue starts out empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue starts out not full
  assert(testqueue.isFull() == 0);
  
  // test that the size of an empty queue is zero
  assert(testqueue.getSize() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
  
  for(uint8_t i=0; i<10; i++){
      
    // test that queue is not full
    assert(testqueue.isFull() == 0);
    
    // test that pushing succeeds
    assert(testqueue.push(i) == 1);
    
    // test that peek with no arguments always returns
    // first value
    assert(testqueue.peek(tmp_peek) == 1);
    //printf("i: %d, tmp_peek: %d\n",i, tmp_peek);
    assert(tmp_peek == 0);
    
    // test that peek with argument returns the correct
    // value
    assert(testqueue.peek(tmp_peek,i) == 1);
    //printf("i: %d, tmp_peek: %d\n",i, tmp_peek);
    assert(tmp_peek == i);
    
    // test that the queue is not empty
    assert(testqueue.isEmpty() == 0);
    
    // test that getSize is correct
    assert(testqueue.getSize() == i+1);
    //printf("size = %d\n",testqueue.getSize());
   
  }
  
  // test that queue is full
  assert(testqueue.isFull() == 1);
  
  // test that pushing to a full queue fails
  assert(testqueue.push(1) == 0);
  //printf("size = %d\n",testqueue.getSize());
  
  // test that a full queue is not empty
  assert(testqueue.isEmpty() == 0);
  
  // test that peek with an out of range value fails
  assert(testqueue.peek(tmp_peek,20) == 0);
  
  for(uint8_t i=0; i<10; i++){
    
    // test that peeking at the next value succeeds
    assert(testqueue.peek(tmp_peek) == 1);
    
    // test that peeking returns the next value
    assert(tmp_peek == i);
    
    // test that popping the next value succeeds
    assert(testqueue.pop(tmp_pop) == 1);
    
    // test that popping returns the next value
    assert(tmp_pop == i);
  }
   
   // test that queue is empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue is not full
  assert(testqueue.isFull() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
   
   
  return 1;
}

/*
 * Testes the Queue class with an uint32_t.
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 1 if success, fails internally otherwise
 */
int test_uint32_queue(){
  
  Queue <uint32_t> testqueue;
  
  uint32_t tmp_peek = 65535;
  uint32_t tmp_pop = 65535;

  // test that queue starts out empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue starts out not full
  assert(testqueue.isFull() == 0);
  
  // test that the size of an empty queue is zero
  assert(testqueue.getSize() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
  
  for(uint8_t i=0; i<10; i++){
      
    // test that queue is not full
    assert(testqueue.isFull() == 0);
    
    // test that pushing succeeds
    assert(testqueue.push(i) == 1);
    
    // test that peek with no arguments always returns
    // first value
    assert(testqueue.peek(tmp_peek) == 1);
    assert(tmp_peek == 0);
    
    // test that peek with argument returns the correct
    // value
    assert(testqueue.peek(tmp_peek,i) == 1);
    assert(tmp_peek == i);
    
    // test that the queue is not empty
    assert(testqueue.isEmpty() == 0);
    
    // test that getSize is correct
    assert(testqueue.getSize() == i+1);
   
  }
  
  // test that queue is full
  assert(testqueue.isFull() == 1);
  
  // test that pushing to a full queue fails
  assert(testqueue.push(1) == 0);
  
  // test that a full queue is not empty
  assert(testqueue.isEmpty() == 0);
  
  // test that peek with an out of range value fails
  assert(testqueue.peek(tmp_peek,20) == 0);
  
  for(uint8_t i=0; i<10; i++){
    
    // test that peeking at the next value succeeds
    assert(testqueue.peek(tmp_peek) == 1);
    
    // test that peeking returns the next value
    assert(tmp_peek == i);
    
    // test that popping the next value succeeds
    assert(testqueue.pop(tmp_pop) == 1);
    
    // test that popping returns the next value
    assert(tmp_pop == i);
  }
   
   // test that queue is empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue is not full
  assert(testqueue.isFull() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
   
   
  return 1;
}

/*
 * Testes the Queue class with an CCSDS_Cmd_t struct.
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * 1 if success, fails internally otherwise
 */
int test_struct_queue(){
   
  // declare queue
  Queue <CCSDS_Cmd_t> testqueue;
  
  // declare variables for pushing/peeking
  CCSDS_Cmd_t tmp_peek;
  CCSDS_Cmd_t tmp_pop;
  CCSDS_Cmd_t tmp_push = { .timestamp = 0, .bytes = {0, 0, 0, 0, 0, 0, 0, 0}};
    
  // test that queue starts out empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue starts out not full
  assert(testqueue.isFull() == 0);
  
  // test that the size of an empty queue is zero
  assert(testqueue.getSize() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);

  for(uint8_t i=0; i<10; i++){
      
    // test that queue is not full
    assert(testqueue.isFull() == 0);
    
    tmp_push.timestamp = i;
    tmp_push.bytes[1] = i;
    
    // test that pushing succeeds
    assert(testqueue.push(tmp_push) == 1);
    
    // test that peek with no arguments always returns
    // first value
    assert(testqueue.peek(tmp_peek) == 1);
    assert(tmp_peek.timestamp == 0);
    assert(tmp_peek.bytes[1] == 0);
    
    // test that peek with argument returns the correct
    // value
    assert(testqueue.peek(tmp_peek,i) == 1);
    assert(tmp_peek.timestamp == i);
    assert(tmp_peek.bytes[1] == i);
    
    // test that the queue is not empty
    assert(testqueue.isEmpty() == 0);
    
    // test that getSize is correct
    assert(testqueue.getSize() == i+1);
   
  }
  
  // test that queue is full
  assert(testqueue.isFull() == 1);
  
  // test that pushing to a full queue fails
  assert(testqueue.push(tmp_push) == 0);
  
  // test that a full queue is not empty
  assert(testqueue.isEmpty() == 0);
  
  // test that peek with an out of range value fails
  assert(testqueue.peek(tmp_peek,20) == 0);
  
  for(uint8_t i=0; i<10; i++){
    
    // test that peeking at the next value succeeds
    assert(testqueue.peek(tmp_peek) == 1);

    // test that peeking returns the next value
    assert(tmp_peek.timestamp == i);
    assert(tmp_peek.bytes[1] == i);
    
    // test that popping the next value succeeds
    assert(testqueue.pop(tmp_pop) == 1);
    
    // test that popping returns the next value
    assert(tmp_peek.timestamp == i);
    assert(tmp_peek.bytes[1] == i);
  }
   
   // test that queue is empty
  assert(testqueue.isEmpty() == 1);
  
  // test that queue is not full
  assert(testqueue.isFull() == 0);
  
  // test the peek and pop fail with empty queue
  assert(testqueue.peek(tmp_peek) == 0);
  assert(testqueue.peek(tmp_peek,0) == 0);
  assert(testqueue.peek(tmp_peek,1) == 0);
  assert(testqueue.pop(tmp_pop) == 0);
   
   
  return 1;
}

/*
 * Runs the unit tests for the various variable types.
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * none
 */
int main(){
  
  if(test_uint8_queue()){
    printf("Passed with uint8_t queue!!\n");
  }
  
  if(test_uint32_queue()){
    printf("Passed with uint32_t queue!!\n");
  }
  
  // this doesnt work, see function definition
  //if(test_uint8_array_queue()){
  //  printf("Passed with uint8_t array queue!!\n");
  //}
  
  if(test_struct_queue()){
    printf("Passed with struct queue!!\n");
  }
}