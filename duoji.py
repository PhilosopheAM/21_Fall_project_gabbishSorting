import RPi.GPIO as GPIO
import time

pin = 3

GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin, GPIO.OUT)

pwm_motor = GPIO.PWM(pin, 50)
pwm_motor.start(12.5)
GPIO.output(pin, GPIO.HIGH)

for i in range(125, 74, -1):
    m = float(i / 10)
    pwm_motor.ChangeDutyCycle(m)
    time.sleep(0.02)

time.sleep(5)

for i in range(75, 125, 1):
    m = float(i / 10)
    pwm_motor.ChangeDutyCycle(m)
    time.sleep(0.02)

pwm_motor.stop()
# for i in range(200):
#    delay(10)
