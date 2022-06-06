import drivers
from time import sleep
from socket import *

class SocketInfo():
    HOST = "192.168.0.14"
    PORT = 8888
    BUFSIZE = 50
    ADDR = (HOST, PORT)

class socketInfo(SocketInfo):
    HOST = "192.168.0.14"

def run():
    display = drivers.Lcd()
    display.lcd_backlight(1)
    display.lcd_display_string("Greetings Human!", 1)  
    display.lcd_display_string("Demo Pi Guy code", 2)  
    sleep(2)  
    s = socket(AF_INET, SOCK_STREAM)
    s.connect(socketInfo.ADDR)

    while(True):
        command = s.recv(socketInfo.BUFSIZE)
        command = str(command)
        print(command)
        command = command.replace('b\'', '')
        command = command.replace('\\x00N=\\xf6', '')
        command = command.replace('\'', '')
        display.lcd_display_string("EMOTION DETECTED!", 1)  # Write line of text to first line of display
        display.lcd_display_string(command, 2)  # Write line of text to second line of display
        
        sleep(1)
        display.lcd_clear()
    s.close()



if __name__ == '__main__':
    run()
