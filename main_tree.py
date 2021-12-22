import four_buttons
import sys,os
import multiprocessing
# sys.path.append("./") # 这一句的添加是为了调用上层文件夹下的文件
import face_detection
import motor
import duoji
import cleaner
from PyQt5 import QtCore, QtGui, QtWidgets

def create_GUI(motor_a: motor.motor_substitute_temp, duoji_a:duoji.duoji, duoji_recyclable:duoji.duoji,cleaner_a:cleaner.cleaner):
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = four_buttons.Ui_MainWindow(MainWindow)  # 调用主函数
    ui.setupUi(MainWindow, motor_a, duoji_a,duoji_recyclable, cleaner_a)  # 初始化
    MainWindow.show() #显示
    sys.exit(app.exec_())   #点击关闭才退出

if __name__ == '__main__':
    multiprocessing.freeze_support() # Windows plug
    # 依次创建 电机motor， 舵机duoji 类对象
    motor_b = motor.motor_substitute_temp() # 注意这里只是一个替代函数，实际工作时需要替换
    duoji_unreusable = duoji.duoji(29) # GPIO lead is pin 29, set for un-reusable rubbish bin.
    duoji_recyclable = duoji.duoji(31) # GPIO lead is pin 31, set for recyclable rubbish bin.
    cleaner_b = cleaner.cleaner()
    # heater_a = Heating.Heating_substitute()
    # 新建了一个进程池，并告诉程序咱们的树莓派有四个核心所以创建四个线程
    pool = multiprocessing.Pool(processes=4)
    #每隔0.5s做一次人脸识别
    # pool.apply_async(face_detection.face_detect,(1,)) #windows测试时换成另外一个方法
    # 代替人脸识别的同步进程测试方法
    pool.apply_async(face_detection.light_shinning)
    print('Hello boys and girls!')
    pool.apply(func = create_GUI, args=(motor_b, duoji_unreusable, duoji_recyclable, cleaner_b))

    pool.close()
    pool.join()