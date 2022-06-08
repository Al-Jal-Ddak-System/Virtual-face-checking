import drivers
from time import sleep
from socket import *
import sys

def run():
    display = drivers.Lcd()
    display.lcd_backlight(1)

    while(True):
        f = open("../server/lcd.txt", "r")
        line = f.readline()
        if line is not None:
            display.lcd_clear()
            display.lcd_display_string("EMOTION DETECTED", 1)
            display.lcd_display_string(line, 2)
            sleep(1)
        f.close()




if __name__ == '__main__':
    run()