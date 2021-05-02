import RPi.GPIO as GPIO
import os
from time import sleep, strftime
from datetime import datetime
import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import subprocess

RST = None     
DC = 23
SPI_PORT = 0
SPI_DEVICE = 0

disp = Adafruit_SSD1306.SSD1306_128_64(rst=RST)

GPIO.setmode(GPIO.BCM)

shutdownPin = 18

GPIO.setup(shutdownPin, GPIO.IN, pull_up_down = GPIO.PUD_UP)

disp.begin()

disp.clear()
disp.display()

width = disp.width
height = disp.height
image = Image.new('1', (width, height))

draw = ImageDraw.Draw(image)

draw.rectangle((0,0,width,height), outline=0, fill=0)

padding = -2
top = padding
bottom = height-padding
x = 0

font = ImageFont.load_default()

temp = -1

while 1:
    """    if GPIO.input(shutdownPin) == False:
        disp.clear()
        disp.display()
        sleep(.1)
        os.system("sudo shutdown now") """
        
    draw.rectangle((0,0,width,height), outline=0, fill=0)

    cmd = "hostname -I | cut -d\' \' -f1"
    IP = subprocess.check_output(cmd, shell = True )
    temp = str(IP).find(".")
    date=datetime.now()

    draw.text((x, top),"    DATE & TIME     ",font=font, fill=255)
    draw.text((x, top+16)," IP: " + str(IP),  font=font, fill=255)
    if temp>-1:
        draw.text((x, top+24),date.strftime(" Date: %d %b %Y"),  font=font, fill=255)
        draw.text((x, top+32),date.strftime(" Time: %I:%M:%S %p"),  font=font, fill=255)
    else:
        draw.text((x, top+24),date.strftime(" Date: "),  font=font, fill=255)
        draw.text((x, top+32),date.strftime(" Time: "),  font=font, fill=255)

    disp.image(image)
    disp.display()
    sleep(.1)
