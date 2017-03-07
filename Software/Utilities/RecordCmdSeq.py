## import the serial library
import serial
import time

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False
run = True

locations=['COM11']

for device in locations:
    try:
        print("Trying...",device)
        ser = serial.Serial(device, 57600)
        break
    except:
        print("Failed to connect on",device)

## loop until the arduino tells us it is ready
while not connected:
    serin = ser.read()
    connected = True
    
print("Connected...")

## open text file to store the current 
##gps co-ordinates received from the rover    
text_file = open("cmds.cmd", 'w')
## read serial data from arduino and 
## write it to the text file 'position.txt'
while run:
    try:
        pkt_len = 0
        bytes_waiting = 0
        # if we've received a header
        if ser.inWaiting():
            print("Begin packet read...")
            msg = []

            for i in range(0,7):
                msg.append(ord(ser.read(1)))

            pkt_buf = bytearray(msg)

            print(" ".join("%02x" % b for b in pkt_buf))
            
            #get the desired timestamp from the user
            exec_time = int(input("Enter desired execution time (in ms) of command:"))
            exec_time_bytes = [int(exec_time >> i & 0xff) for i in (24,16,8,0)]

            # concat the time and message
            write_buf = exec_time_bytes
            write_buf.extend(pkt_buf)

            print("Writing to file: ")
            print(" ".join("%02x" % b for b in write_buf))
            
            #write the whole packet
            for i in write_buf:
                text_file.write(str(chr(i).encode('utf8')))

            # write tot he file
            text_file.flush()

    except (KeyboardInterrupt, SystemExit):
        print("Closing...")
        run = False
    except:
        # report error and proceed
        raise
        #
## close the serial connection and text file
text_file.close()
ser.close()


