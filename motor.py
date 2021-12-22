
# coding:utf-8
# Python中声明文件编码的注释，编码格式指定为utf-8
from socket import *
from time import ctime
import binascii
# import RPi.GPIO as GPIO # Need to recover when use
import time
import threading

class motor_substitute_temp(object):
    def __init__(self):
        self.pin_number = 100

    def print_status(self):
        print('The motor uses pin number {self.pin_number}')
        print("Motor is working for 1s")
        time.sleep(1)
        print("Motor is working for 2s")
        time.sleep(1)
        print("Motor is working for 3s")
        time.sleep(1)
        print('Motor\'s work has been done.')
        
    def motor_Test(self):
        aa = threading.Thread(target = self.print_status)
        aa.start()

# Windows上测试无法导入GPIO包因此使用motor_substitute_temp作测试
class Motor(object):
    def __init__(self) -> None:
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        ########电机驱动接口定义#################
        self.ENA = 22  # //L298使能A
        self.ENB = 20  # //L298使能B
        self.IN1 = 17  # //电机接口1
        self.IN2 = 27  # //电机接口2
        self.IN3 = 21  # //电机接口3
        self.IN4 = 26  # //电机接口4

    def initial_other(self):
        #########电机初始化为LOW##########
        GPIO.setup(self.ENA ,GPIO.OUT ,initial=GPIO.LOW)
        GPIO.setup(self.IN1 ,GPIO.OUT ,initial=GPIO.LOW)
        GPIO.setup(self.IN2 ,GPIO.OUT ,initial=GPIO.LOW)
        GPIO.setup(self.ENB ,GPIO.OUT ,initial=GPIO.LOW)
        GPIO.setup(self.IN3 ,GPIO.OUT ,initial=GPIO.LOW)
        GPIO.setup(self.IN4 ,GPIO.OUT ,initial=GPIO.LOW)

        ####PWM初始化，并设置频率为200HZ####
        GPIO.setup(self.ENA ,GPIO.OUT)  # 初始化
        p1 = GPIO.PWM(self.ENA ,200)  # 200HZ
        p1.start(50)  # 产生占空比为0.4的PWM信号，取值范围0-100

        #########定义电机正转函数##########
    def gogo(self):
        GPIO.output(self.ENA ,True)
        GPIO.output(self.IN1 ,True)
        GPIO.output(self.IN2 ,False)
        GPIO.output(self.IN3 ,True)
        GPIO.output(self.IN4 ,False)
    #########定义电机反转函数##########
    def back(self):
        GPIO.output(self.ENA ,True)
        GPIO.output(self.IN1 ,False)
        GPIO.output(self.IN2 ,True)


        #########定义电机停止函数##########
        def stop():
            # GPIO.output(ENA,False)
            p1.stop # 停止PWM信号
            GPIO.output(ENB ,False)
            GPIO.output(IN1 ,False)
            GPIO.output(IN2 ,False)
            GPIO.output(IN3 ,False)
            GPIO.output(IN4 ,False)

        def upMove(): #向上
            gogo()
            time.sleep(10)

        def downMove(): #向下
            back()
            time.sleep(10)
