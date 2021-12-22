
# coding:utf-8
# Python中声明文件编码的注释，编码格式指定为utf-8
from socket import *
from time import ctime
import binascii
import RPi.GPIO as GPIO
import time
import threading

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

########电机驱动接口定义#################
ENA = 22  # //L298使能A
ENB = 20  # //L298使能B
IN1 = 17  # //电机接口1
IN2 = 27  # //电机接口2
IN3 = 21  # //电机接口3
IN4 = 26  # //电机接口4

#########电机初始化为LOW##########
GPIO.setup(ENA ,GPIO.OUT ,initial=GPIO.LOW)
GPIO.setup(IN1 ,GPIO.OUT ,initial=GPIO.LOW)
GPIO.setup(IN2 ,GPIO.OUT ,initial=GPIO.LOW)
GPIO.setup(ENB ,GPIO.OUT ,initial=GPIO.LOW)
GPIO.setup(IN3 ,GPIO.OUT ,initial=GPIO.LOW)
GPIO.setup(IN4 ,GPIO.OUT ,initial=GPIO.LOW)

####PWM初始化，并设置频率为200HZ####
GPIO.setup(ENA ,GPIO.OUT)  # 初始化
p1 = GPIO.PWM(ENA ,200)  # 200HZ
p1.start(50
         )  # 产生占空比为0.4的PWM信号，取值范围0-100
#########定义电机正转函数##########
def gogo():
    GPIO.output(ENA ,True)
    GPIO.output(IN1 ,True)
    GPIO.output(IN2 ,False)
    GPIO.output(IN3 ,True)
    GPIO.output(IN4 ,False)
#########定义电机反转函数##########
def back():
    GPIO.output(ENA ,True)
    GPIO.output(IN1 ,False)
    GPIO.output(IN2 ,True)


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
    time.sleep(3)

def downMove(): #向下
    back()
    time.sleep(3)
