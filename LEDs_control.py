# coded by Koichi Tanaka
# encodig = UTF-8 in Python3
# Follow MIT license
# 旨在完成一份可以快速操作的灯光变换操作 Aim to make it easy to change the LEDs pattern to your preference
# Based on rpi_ws281x library and thanks for their work : )
# Hope you enjoy playing with these shiny gadgets :o 


import rpi_ws281x
import time

LED_Count = 5
LED_Pin = 18
LED_Brightness = 255
LED_Frequency = 800000
LED_DMA = 10
LED_Inverse = False
time_play = 1

def change_Count(x):
	global LED_Count
	LED_Count = x
	
def change_Pin(x):
	global LED_Pin 
	LED_Pin = x
	
def change_Brightness(x):
	global LED_Brightness
	LED_Brightness = x

def change_time(x):
	global 

def play_LEDs(play_as_default = True):
	'''
	global LED_Count
	global LED_Pin
	global LED_Brightness
	global LED_Frequency
	global LED_DMA
	global LED_Inverse
	global time_play
	'''
	strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN,LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS)
	
	if play_as_default :
		strip.begin()
		for i in range(0,strip.numPixels(),1):
			strip.setPixelColor(i,Color(219,58,52))
			# Defaultly, we set LEDs to red 
	else:
		strip.setBrightness(0)
		strip.begin()
		
		
		# Here we will make a muti-threads design
		while True(): # 这一句可能都不用谢
			strip.show()
			#time.sleep(time_play)

# If you want it to have a try
'''
if __name__ =='__main__':
	play_LEDs()
	'''