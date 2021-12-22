#encoding = utf-8

import time
class mutithreads_try:

    def time_calculate(self):
        initial = 0
        time_initial = time.time()

        for i in range(10000000):
            i += 1

        time.sleep(5)

        print(time.time() - time_initial)
