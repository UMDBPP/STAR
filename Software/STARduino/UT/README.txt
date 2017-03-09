/************************************/
compile UT_Queue (on Win10):
>> g++ UT_Queue.c -o UT_Queue
expect clean compile

Run UT_Queue:
>> UT_Queue

Expected result: PASSING (2017-03-09)
Passed with uint8_t queue!!
Passed with uint32_t queue!!
Passed with struct queue!!

/************************************/
compile UT_Stub_SD (on Win10):
>> g++ UT_Stub_SD.cpp -o UT_Stub_SD
expect warnings about "anonymous type with no linkage for TlmHeader_u and CmdHEader_u"

Expected result: PASSING (2017-03-09)
There are a fair number of messages printed with this test

/************************************/
compile UT_CmdQueue (on Win10):
>> g++ UT_CmdQueue.cpp ../ccsds/ccsds_utilities.cpp ../ccsds/ccsds.cpp -o UT_CmdQueue
expect warnings about "anonymous type with no linkage for TlmHeader_u and CmdHEader_u"

Expected result: PASSING (2017-03-09)
There are a fair number of messages printed with this test

