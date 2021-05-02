import RPi.GPIO as GPIO
from time import sleep

PB = 20

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(PB, GPIO.IN, pull_up_down = GPIO.PUD_UP)

while 1:
    print GPIO.input(PB)
    sleep(1)
    