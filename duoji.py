# 这一部分一飞说还有工作需要更新。因此不进行调用，仅预留位置
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
pin = 8

GPIO.setup(pin, GPIO.OUT)

pwm_motor = GPIO.PWM(pin, 50)
pwm_motor.start(12.5)
GPIO.output(pin, GPIO.HIGH)

# In steering engine, pwm controls the angles rather than rotating speed

for i in range(25, 76, 1):
    m = float(i / 10)
    pwm_motor.ChangeDutyCycle(m)
    time.sleep(0.02)

time.sleep(2)

for i in range(75, 24, -1):
    m = float(i / 10)
    pwm_motor.ChangeDutyCycle(m)
    time.sleep(0.02)

pwm_motor.stop()
# for i in range(200):
#    delay(10)
