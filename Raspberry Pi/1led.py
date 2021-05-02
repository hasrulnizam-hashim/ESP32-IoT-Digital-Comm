import RPi.GPIO as GPIO
from time import sleep

LED = 21

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(LED, GPIO.OUT)

while 1:
    print "LED ON"
    GPIO.output(LED,GPIO.HIGH)
    sleep(1)
    print "LED OFF"
    GPIO.output(LED,GPIO.LOW)
    sleep(1)
	