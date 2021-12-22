# -*- coding: utf-8 -*-
# A list stores the location of the pictures need to be used in the GUI.
from PyQt5 import  QtGui
from PyQt5.QtGui import QIcon

# At the first time, we use dict.item() function to give us pairs of key, value and set them into dict with function update()s
# But we met some troubles when we want to call the dict and find the value for specific key.
# Now we want to use map() function
# Well, it works when I declare variables by using a {}.
# So it teaches us that using {} to declare a dictionary.
Pollution_pictures_dict = {
            '垃圾分类污染图1': "background-image:url(GUI_resources/垃圾分类污染图1.png)",
            '垃圾分类污染图2': "background-image:url(GUI_resources/垃圾分类污染图2.png)",
            '垃圾分类污染图3': "background-image:url(GUI_resources/垃圾分类污染图3.png)",
            '垃圾分类污染图4': "background-image:url(GUI_resources/垃圾分类污染图4.png)"}

icon_Pollution_pictures_dict = {}

for key, values in Pollution_pictures_dict.items():
    icon_a = QIcon()
    icon_a.addPixmap(QtGui.QPixmap(values), QtGui.QIcon.Normal, QIcon.On)
    icon_Pollution_pictures_dict.update({key: icon_a})


RelaxingScreen_pictures_dict = {
            'faceChanging_1': "background-image:url(GUI_resources/faceChanging 1.jpg)",
            'faceChanging_2': "background-image:url(GUI_resources/faceChanging 2.jpg)",
            'faceChanging_3': "background-image:url(GUI_resources/faceChanging 3.jpg)",
            'faceChanging_4': "background-image:url(GUI_resources/faceChanging 4.png)"}

icon_RelaxingScreen_pictures_dict = {}

for i, j in RelaxingScreen_pictures_dict.items():
    icon_a = QIcon()
    icon_a.addPixmap(QtGui.QPixmap(j), QtGui.QIcon.Normal, QIcon.On)
    icon_RelaxingScreen_pictures_dict.update({i: icon_a})

Button_pictures_dict = {
            'Recyclable_button': "GUI_resources/新可回收标志.png",
            'Nonrecyclable_button': "GUI_resources/新不可回收标志.png",
            'Dustman_button': "GUI_resources/清洁模式标志.png",
            'Close_cover': "GUI_resources/关闭按钮.png"
}

icon_Button_pictures_dict = {}

for key, values in Button_pictures_dict.items():
    icon_a = QIcon()
    icon_a.addPixmap(QtGui.QPixmap(values), QtGui.QIcon.Normal, QIcon.On)
    icon_Button_pictures_dict.update({key: icon_a})



