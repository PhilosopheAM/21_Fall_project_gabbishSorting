 '''
这一文件是总控制程序，它的工作原理为：
	创建线程池
	调用GUI绘制程序
	调用面部识别（关于这一功能还存在疑问，但是还是先试验一下线程流畅性）
	GUI界面的交互按钮将在被唤醒时开启舵机/电机控制程序以及时间控制程序
	GUI界面始终保持运行，监听用户唤醒操作
''''
import threading
import GUI
import face_detection

def test():
	print(threading.active_count()))
	print(threading.enumerate())
	print(threading.current_thread())

if __name__ == '__main__':

	screen_GUI = threading.Thread(target = 1.Screen_Function)
	screen_GUI.start()
	
	face_recognition = threading.Thread(target = 2.Face_Detection)
	face_recognition.start()
	# 需要注意人脸识别这里，看一看它要吃多少计算资源
	