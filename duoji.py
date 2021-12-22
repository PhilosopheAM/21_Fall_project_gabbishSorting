#import RPi.GPIO as GPIO  # 当实际应用时，取消注释即可，包括下面各行中有GPIO的语句
import threading
import time


class duoji(object):
    '''
    def __init__(self, pin_number: int):
        self.pin = pin_number
        self.pwm_motor = GPIO.PWM(self.pin, 50)

#        GPIO.setmode(GPIO.BOARD)s
        
    def change_pin(self, pin_number):
        self.pin = pin_number

    def open_cover(self):
        GPIO.setup(self.pin, GPIO.OUT)
        # Open the cover
        self.pwm_motor.start(10)
        GPIO.output(self.pin, GPIO.HIGH)

        for i in range(10, 60, 1):
            m = float(i / 10)
            self.pwm_motor.ChangeDutyCycle(m)
            time.sleep(0.02)

    def close_cover(self):

        # Close the cover
        for i in range(59, 10, -1):
            m = float(i / 10)
            self.pwm_motor.ChangeDutyCycle(m)
            time.sleep(0.02)

            self.pwm_motor.stop()
    '''
    @staticmethod
    def rest_time(self, time_seconds: int):
        ''' The seconds to wait '''
        time.sleep(time_seconds)

    def print_status(self):
        print("Duoji is working for 1s")
        time.sleep(1)
        print('Duoji is working for 2s')
        time.sleep(1)
        print('Duoji is working for 3s')
        time.sleep(1)
        print("The pin we use for motor now is {self.pin}") # A bug occurs, we use a simple print sentence to substitute it.

    def duoji_Test(self):
        aa = threading.Thread(target= self.print_status)
        aa.start()
