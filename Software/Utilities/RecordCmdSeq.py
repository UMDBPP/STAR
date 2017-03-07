## import the serial library
import serial

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False
run = True

## establish connection to the serial port that your arduino 
## is connected to.

locations=['COM10']

for device in locations:
    try:
        print "Trying...",device
        ser = serial.Serial(device, 57600)
        break
    except:
        print "Failed to connect on",device

## loop until the arduino tells us it is ready
while not connected:
    serin = ser.read()
    connected = True

## open text file to store the current 
##gps co-ordinates received from the rover    
text_file = open("cmds.cmd", 'w')
## read serial data from arduino and 
## write it to the text file 'position.txt'
while run:
    try:
        if ser.inWaiting():
            x=ser.read()
            print(x) 
            text_file.write(x)
            if x=="\n":
                text_file.seek(0)
                text_file.truncate()
                text_file.flush()
    except (KeyboardInterrupt, SystemExit):
        print "Closing..."
        run = False
    except:
        # report error and proceed
        raise
        
## close the serial connection and text file
text_file.close()
ser.close()
