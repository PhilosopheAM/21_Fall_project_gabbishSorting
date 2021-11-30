# 这一部分一飞说还有工作需要更新。因此不进行调用，仅预留位置
import RPi.GPIO as GPIO
import time

out1 = 11
out2 = 13
ENA = 3
ENB = 5

i = 0
y = 0

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(out1, GPIO.OUT)
GPIO.setup(out2, GPIO.OUT)
GPIO.setup(ENA, GPIO.OUT)
GPIO.setup(ENB, GPIO.OUT)

try:
    while (1):
        GPIO.output(out1, GPIO.LOW)
        GPIO.output(out2, GPIO.LOW)
        GPIO.output(ENA, 255)
        GPIO.output(ENB, 255)
        x = int(input())
        if x > 0 and x <= 400:
            for y in range(x, 0, -1):
                if i == 0:
                    GPIO.output(out1, GPIO.HIGH)
                    GPIO.output(out2, GPIO.LOW)
                    time.sleep(0.03)
                    # time.sleep(1)
                elif i == 1:
                    GPIO.output(out1, GPIO.HIGH)
                    GPIO.output(out2, GPIO.HIGH)
                    time.sleep(0.03)
                    # time.sleep(1)
                elif i == 2:
                    GPIO.output(out1, GPIO.LOW)
                    GPIO.output(out2, GPIO.HIGH)
                    time.sleep(0.03)
                    # time.sleep(1)
                elif i == 3:
                    GPIO.output(out1, GPIO.LOW)
                    GPIO.output(out2, GPIO.LOW)
                    time.sleep(0.03)
                    # time.sleep(1)
                if i == 4:
                    i = 0
                    continue
                i = i + 1


except KeyboardInterrupt:
    GPIO.output(ENA, False)
    GPIO.output(ENB, False)
    GPIO.cleanup()