### Imports ###################################################################
# 事实上这个文件由于没有导入需要的库，所以无法实际调用，我们调用了另外一个方法来表现它
# from picamera.array import PiRGBArray
# from picamera import PiCamera
# import time
# import cv2
# import LEDs_control
import os
import time

### Setup #####################################################################
def face_detect():
	# Center coordinates
	cx = 160
	cy = 120

	os.system( "echo 0=150 > /dev/servoblaster" )
	os.system( "echo 1=150 > /dev/servoblaster" )

	xdeg = 150
	ydeg = 150

	# Setup the camera
	camera = PiCamera()
	camera.resolution = ( 320, 240 )
	camera.framerate = 60
	rawCapture = PiRGBArray( camera, size=( 320, 240 ) )

	# Load a cascade file for detecting faces
	# face_cascade = cv2.CascadeClassifier( '/usr/share/opencv/haarcascades/haarcascade_frontalface_default' )
	face_cascade = cv2.CascadeClassifier( '/home/pi/Desktop/lbpcascade_frontalface.xml' )# 需要更改训练集地址
	# to-do: remember to substitute the target files

	t_start = time.time()
	fps = 0


### Main ######################################################################

# Capture frames from the camera
	for frame in camera.capture_continuous( rawCapture, format="bgr", use_video_port=True ):

		image = frame.array

		# Use the cascade file we loaded to detect faces
		gray = cv2.cvtColor( image, cv2.COLOR_BGR2GRAY )
		faces = face_cascade.detectMultiScale( gray )
			

		if(len(faces)>= 1):
			# LEDs_control.play_LEDs()
			print('Face has been detected!!!') # 这里就要把LED控制程序扔进去了
		else:
			# LEDs_control.play_LEDs(False)
			print('There is no one!!!') # # false判断需要考虑设计，可能灯光并不变

#灯条闪烁同步进行，模拟异步处理
def light_shinning():
	print('------灯条闪烁程序------\n')
	print('Loading',end='')

	for _ in range(8):
		print('.',end='',flush=True)
		time.sleep(0.5)

	print('\n------灯条闪烁程序完成，该线程释放资源------\n')
