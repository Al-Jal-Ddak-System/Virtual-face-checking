import drivers
from time import sleep
from socket import *

class SocketInfo():
    HOST = "192.168.0.7"
    PORT = 8888
    BUFSIZE = 50
    ADDR = (HOST, PORT)

class socketInfo(SocketInfo):
    HOST = "192.168.0.7"

def run():
    display = drivers.Lcd()
    display.lcd_backlight(1)
    csock = socket(AF_INET, SOCK_STREAM)
    csock.connect(socketInfo.ADDR)

    while(True):
        command = csock.recv(socketInfo.BUFSIZE)
        command = str(command)
        command = command.replace('b\'', '')
        command = command.replace('\\x00N=\\xf6', '')
        command = command.replace('\'', '')
        display.lcd_display_string("SERVER MESSAGE", 1)  # Write line of text to first line of display
        display.lcd_display_string(command, 2)  # Write line of text to second line of display
        print(command)
        
        sleep(1)
        display.lcd_clear()
    csock.close()



if __name__ == '__main__':
    run()