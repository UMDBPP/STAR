#include "Commands.h"


void execute_command(uint8_t Pkt_Buff[]){
/*
 * Executes a command.
 * 
 * Inputs: 
 * Pkt_Buff - buffer containing the input command
 * 
 * Output:
 * none
 * 
 * Return:
 * none
 * 
 */

  // check the APID
  if(getAPID(Pkt_Buff) != APID_STAR_CMD){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Invalid command APID received ");
    send_cmd_response(getCmdFunctionCode(Pkt_Buff), 3);
    return;
  }

  // verify that the packet is a command
  if(getPacketType(Pkt_Buff) != CCSDS_CMD_PKT){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Packet recieved is not a command");
    send_cmd_response(getCmdFunctionCode(Pkt_Buff), 4);
    return;
  }

  // validate command checksum
  if(!validateChecksum(Pkt_Buff)){
    sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Command checksum doesn't validate");
    send_cmd_response(getCmdFunctionCode(Pkt_Buff), 5);
    return;
  }

  /*
   * The type of command is identified by the function code included in 
   * the header. In this case we've defined 2 commands.
   */
  switch(getCmdFunctionCode(Pkt_Buff)){
    case FCNCODE_NOOP_CMD:{
      /*  
       *   FcnCode FCNCODE_NOOP_CMD is a command to do nothing. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> NOOP command received");

      send_cmd_response(FCNCODE_NOOP_CMD, 1);

      break;
    }case FCNCODE_LED_CMD:{
      /*  
       *   FcnCode FCNCODE_LED_CMD is a command to control the illumination. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Brightness (1 byte) (0-255 corresponding to brightness)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> LED command received");

      // initalize variables needed for command execution
      uint8_t brightness_flag = 0;

      // extract the on_off_flag from the command
      extractFromTlm(brightness_flag, Pkt_Buff, 8);

      // command the LED
      set_LED(brightness_flag);
      break;
    }
    case FCNCODE_BIAS_CMD:{
      /*  
       *   FcnCode FCNCODE_BIAS_CMD is a command to control the Bias. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Bias_Enable (1 bytes) [1 = on, 0 = off]
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Bias command received");

      // initalize variables needed for command execution
      uint8_t enable_flag = 0;

      // extract the on_off_flag from the command
      extractFromTlm(enable_flag, Pkt_Buff, 8);

      // command the Bias
      set_bias(enable_flag);
      break;
    }
    case FCNCODE_SHAKE_CMD:{
      /*  
       *   FcnCode FCNCODE_SHAKE_CMD is a command to control the shaker. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Shake_Speed (1 bytes)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Shake command received");

      // initalize variables needed for command execution
      uint8_t shake_speed = 0;

      // extract the on_off_flag from the command
      extractFromTlm(shake_speed, Pkt_Buff, 8);

      // command the shaker
      set_shaker(shake_speed);
      break;
    }
    case FCNCODE_CUTOFF_CMD:{
      /*  
       *   FcnCode FCNCODE_CUTOFF_CMD is a command to enable the cutoff. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   Cutoff_Enable (1 bytes) [1 = cutoff, 0 = on]
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> Cutoff command received");

      // initalize variables needed for command execution
      uint8_t cutoff_enable = 0;

      // extract the on_off_flag from the command
      extractFromTlm(cutoff_enable, Pkt_Buff, 8);

      // command the LED
      set_cutoff(cutoff_enable);
      break;
    }
    case FCNCODE_TLM_CMD:{
      /*  
       *   FcnCode FCNCODE_TLM_CMD is a command to output telemetry. 
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   TlmRequested (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> TLM command received");

      // initalize variables needed for command execution
      uint8_t tlm_requested = 0;

      // extract the tlm_requested from the command
      extractFromTlm(tlm_requested, Pkt_Buff, 8);

      // command the LED
      send_tlm(tlm_requested);
      break;
    }
    case FCNCODE_QUEUE_CMD:{
      /*  
       *   FcnCode FCNCODE_QUEUE_CMD is a command to control the 
       *   execution of the automated command sequence.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   EnableQueue (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> QUEUE command received");

      bool queue_flag;
      
      // extract the tlm_requested from the command
      extractFromTlm(queue_flag, Pkt_Buff, 8);

      if(queue_flag){
        enable_cmd_queue();
      }
      else{
        disable_cmd_queue();
      }

      break;
    }
    case FCNCODE_RESETMET_CMD:{
      /*  
       *   FcnCode FCNCODE_RESETMET_CMD is a command reset the 
       *   MET (mission elapsed time) counter to zero.
       *   The function is defined with the following format:
       *   
       *   CCSDS Command Header (8 bytes)
       *   EnableQueue (1 byte)
       */
      sendTxtMsg(SERIAL_DEBUG,"INFO: <CMD> RESET_MET command received");

      set_epoch();

      break;
    }
    default:{
      sendTxtMsg(SERIAL_DEBUG,"ERROR: <CMD> Unrecognized function code");
      //SERIAL_DEBUG.println(getCmdFunctionCode(Pkt_Buff));
      send_cmd_response(getCmdFunctionCode(Pkt_Buff), 2);
    }
  }
}

void set_LED(uint8_t brightness){
  digitalWrite(PIN_LEDPWM, brightness);
  //SERIAL_DEBUG.print("Set LED to ");
  //SERIAL_DEBUG.println(brightness);
}

void set_bias(uint8_t enable){
  digitalWrite(PIN_BIAS, enable);
  //SERIAL_DEBUG.print("Set Bias to ");
  //SERIAL_DEBUG.println(enable);
}

void set_shaker(uint8_t shake_speed){

  // do math here to figure out what to command
  
  digitalWrite(PIN_MOTORPWM, shake_speed);
  //SERIAL_DEBUG.print("Set Shake speed to ");
  //SERIAL_DEBUG.println(shake_speed);
}

void set_cutoff(uint8_t enable){
  
  digitalWrite(PIN_CUTOFF, enable);
  //SERIAL_DEBUG.print("Set cutoff enable to ");
  //SERIAL_DEBUG.println(enable);
}

void send_tlm(uint8_t tlm_requested){
  // Do not implement for now
  // argument is a bitfield indicating which packets to output
}

void send_cmd_response(uint8_t fcncode, uint8_t stat){
  uint8_t payloadLength = 0;
  uint8_t payload_buff[MAX_TLM_LEN];
  
  payloadLength = addIntToTlm(fcncode, payload_buff, payloadLength); // Add the value of the table to message
  payloadLength = addIntToTlm(stat, payload_buff, payloadLength); // Add the value of the table to message

  sendTlmMsg(SERIAL_DEBUG, APID_STAR_CMDRESP, payload_buff, payloadLength);

}

