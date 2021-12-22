from PyQt5 import QtCore, QtGui, QtWidgets
import  threading, multiprocessing,os,sys

def Make_welcome_page(main_page:QtWidgets.QWidget):




if __name__ == '__main__':
    multiprocessing.freeze_support()
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QWidget()
    MainWindow.setObjectName("MainWindow")
    MainWindow.setEnabled(True)
    MainWindow.resize(1280, 800)
    MainWindow.setCursor(QtGui.QCursor(QtCore.Qt.ArrowCursor))
    MainWindow.setWindowOpacity(1.0)  # set the opacity of the window. When set to 0, the window is invisible
    MainWindow.setStyleSheet("background-image:url(GUI_resources/新背景图片.png)")
    Make_welcome_page(MainWindow)
    MainWindow.show()  # 显示
    sys.exit(app.exec_())  # 点击关闭才退出
