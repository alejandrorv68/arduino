# For automatically setting the ESP in bootloade mode, connect pins 2 and 3 from the Pi
# to RST and PRGM respectively, run this code and after the bootloader sequence 
# upload the file TestPRGMpin.ino (externally as a .bin or from arduino) to the ESP

#Libraries
import RPi.GPIO as GPIO
from time import sleep


GPIO.setmode(GPIO.BCM)	# Pin numbering mode
GPIO.setwarnings(False)

#Assign reset and program to gpios
rst = 2
prgm = 3

#Set us outputs
GPIO.setup(rst,GPIO.OUT)
GPIO.setup(prgm,GPIO.OUT)

#Bootloader sequence
GPIO.setup(rst,0)
GPIO.setup(prgm,0)
sleep(2)
GPIO.setup(rst,1)
sleep(0.5)
GPIO.setup(prgm,1)

#Upload .bin file [paste code here] (or upload using arduino externally) 
#Wait while software is being uploaded
sleep(40)


#Once program is uploaded, force a reset
GPIO.setup(rst,0)
sleep(.5)
GPIO.setup(rst,1)
sleep(1)	#wait for 1 second to pull prgm low. Otherwise crash. There is a delay in the .ino Setup() function to account for this
GPIO.setup(prgm,0)



#GPIO.setup(prgm,0)
#sleep(2)
#GPIO.setup(prgm,1)



#GPIO.cleanup()
