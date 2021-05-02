import RPi.GPIO as GPIO
from time import sleep

Relay = 18

GPIO.setmode(GPIO.BCM)      #BCM if to use GPIO number, BOARD if to use Pin number
GPIO.setwarnings(False)
GPIO.setup(Relay, GPIO.OUT)

while 1:
    print "Relay ON"    #print relay ON
    GPIO.output(Relay,GPIO.HIGH)
    sleep(1)        #1 second
    print "Relay OFF"
    GPIO.output(Relay,GPIO.LOW)
    sleep(1)
	