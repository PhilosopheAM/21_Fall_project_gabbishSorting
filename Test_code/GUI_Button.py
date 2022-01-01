# Define a specific button for this GUI program.
# This kind of button inherits from QPushButton class and it has unique attributes and functions to change its status.

import  PyQt5


class Button(PyQt5.QtWidgets.QPushButton):
    ''' A button designed for the use of GUI.
        Inherit from Qt.QtWidgets.QPushButton class.

    Attributes:
        upper_clicked_times: The most times this button been clicked before it works and recover.
        clicked_times: Once clicked, the clicked_times will add one, unless the recover_status function is called.
    '''
    def __init__(self, parent, upper_clicked_times = 2):
        '''
        Args:
            upper_clicked_times: The most times this button been clicked before it works and recover.
        '''
        super(Button, self).__init__(parent)
        self.clicked_times = 0
        self.upper_clicked_times = upper_clicked_times


