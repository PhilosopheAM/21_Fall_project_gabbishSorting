### Imports ###################################################################

from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import LEDs_control
import os


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
face_cascade = cv2.CascadeClassifier( '/home/pi/Desktop/lbpcascade_frontalface.xml' )
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
			LEDs_control.play_LEDs()
		else:
			LEDs_control.play_LEDs(False)

    
	#print("Found " + str( len( faces ) ) + " face(s)")
	

    # Draw a rectangle around every face and move the motor towards the face

	# to-do: remember to delete following codes when real using
    '''for ( x, y, w, h ) in faces:

        cv2.rectangle( image, ( x, y ), ( x + w, y + h ), ( 100, 255, 100 ), 2 )
        cv2.putText( image, "Face No." + str( len( faces ) ), ( x, y ), cv2.FONT_HERSHEY_SIMPLEX, 0.5, ( 0, 0, 255 ), 2 )

        tx = x + w/2
        ty = y + h/2

        if   ( cx - tx >  10 and xdeg <= 190 ):
            xdeg += 3
            os.system( "echo 0=" + str( xdeg ) + " > /dev/servoblaster" )
        elif ( cx - tx < -10 and xdeg >= 110 ):
            xdeg -= 3
            os.system( "echo 0=" + str( xdeg ) + " > /dev/servoblaster" )

        if   ( cy - ty >  10 and ydeg >= 110 ):
            ydeg -= 3
            os.system( "echo 1=" + str( ydeg ) + " 
	'''