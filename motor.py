import RPi.GPIO as GPIO
import time

out1 = 32
out2 = 37
out3 = 36
out4 = 38

ENA=22
ENB=40

inpin1 = 18


GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(out1, GPIO.OUT)
GPIO.setup(out2, GPIO.OUT)
GPIO.setup(out3, GPIO.OUT)
GPIO.setup(out4, GPIO.OUT)
GPIO.setup(ENA,GPIO.OUT)
GPIO.setup(ENB,GPIO.OUT)
GPIO.setup(inpin1, GPIO.IN)
GPIO.setup(inpin1, GPIO.LOW)

ENApwm=GPIO.PWM(ENA,200)
ENApwm.start(50)
ENBpwm=GPIO.PWM(ENB,200)
ENBpwm.start(50)


GPIO.output(out1, GPIO.HIGH)
GPIO.output(out2, GPIO.LOW)
GPIO.output(out3, GPIO.HIGH)
GPIO.output(out4, GPIO.LOW)

while (1):
    if GPIO.input(inpin1) == GPIO.HIGH:
        print("Yes")
        GPIO.output(out1, GPIO.LOW)
        GPIO.output(out2, GPIO.LOW)
        GPIO.output(out3, GPIO.LOW)
        GPIO.output(out4, GPIO.LOW)
        break
time.sleep(5)

GPIO.output(out1, GPIO.LOW)
GPIO.output(out2, GPIO.HIGH)
GPIO.output(out3, GPIO.LOW)
GPIO.output(out4, GPIO.HIGH)

while (1):
    if GPIO.input(inpin1) == GPIO.HIGH:
        print("Yes")
        GPIO.output(out1, GPIO.LOW)
        GPIO.output(out2, GPIO.LOW)
        GPIO.output(out3, GPIO.LOW)
        GPIO.output(out4, GPIO.LOW)
        break
