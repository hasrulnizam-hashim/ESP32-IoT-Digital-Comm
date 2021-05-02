from time import sleep
import serial

serialPort = "/dev/ttyS0"
baudrate = 9600

port = serial.Serial(serialPort, baudrate=baudrate, timeout=1)

counter = 0
while 1:
	if counter<12:
		counter=counter+1
	else:
		counter=1
	port.write("id"+str(counter))
	sleep(0.5)
	if port.in_waiting>0:
		sleep(0.1)
		message=""
		while port.in_waiting>0:
			message+=port.read()
		print message
	sleep(0.5)


