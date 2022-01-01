
# 这个程序开始即代表进入清洁模式
class cleaner(object):
    '''Create this class to give a cleaning mode to the program.

    Attribute:
        __is_cleaning: A protected attribute refers to the status of cleaning mode. 
    
    '''
    def __init__(self):

        self.__is_cleaning = False #定义了一个私有属性

    def is_cleanning_work_on(self):
        '''Use this method to check if the program is in the cleaning mode.'''
        return self.__is_cleaning

    def change_cleaning_status(self):
        '''Use this method to change the cleaning status.'''
        if self.__is_cleaning:
            self.__is_cleaning = False
        else:
            self.__is_cleaning = True
    def print_out_status(self):
        print("",self.is_cleanning_work_on())
        self.change_cleaning_status()
