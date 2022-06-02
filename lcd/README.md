# I2C Character lcd

## Install

1. install python3
   * `sudo apt-get install python3`
2. I2C enable
   * `sudo raspi-config`
   * Interface Options > P5 I2C > Enable
3. install smbus
   * `sudo apt-get install python3-smbus`
4. install i2c tools
   * `sudo apt-get install i2c-tools`
5. install dos2unix
   * `sudo apt-get install dos2unix`
6. WSL
   * `git config --global core.autocrlf false` : 자동으로 CRLF 로 바뀌는 것을 막음
7. clone
   * `git clone https://github.com/the-raspberry-pi-guy/lcd.git`

## Run

1. `cd lcd`
2. `sudo sh install.sh`
3. `python3 demo_lcd.py`

### Print Hello world!

```python
import drivers
from time import sleep

# Load the driver and set it to "display"
# If you use something from the driver library use the "display." prefix first
display = drivers.Lcd()

# Main body of code
try:
    while True:
        # Remember that your sentences can only be 16 characters long!
        print("print Helloworld")
        display.lcd_display_string("  Hello World!", 1)  # Write line of text to first line of display
        display.lcd_display_string("  * WELCOME * ", 2)  # Write line of text to second line of display
        sleep(2)                         
        display.lcd_display_string("  Hello World!", 1)                  # Give time for the message to be read
        display.lcd_display_string("    WELCOME   ", 2)   # Refresh the first line of display with a different message
        sleep(2)                                           # Give time for the message to be read                                          # Give time for the message to be read
except KeyboardInterrupt:
    # If there is a KeyboardInterrupt (when you press ctrl+c), exit the program and cleanup
    print("Cleaning up!")
    display.lcd_clear()
```

#### Reference

* [라즈베리파이 1602 I2C 캐릭터 LCD 사용 (Python, 파이썬)](https://velog.io/@dogakday/%EB%9D%BC%EC%A6%88%EB%B2%A0%EB%A6%AC%ED%8C%8C%EC%9D%B4-1602-I2C-%EC%BA%90%EB%A6%AD%ED%84%B0-LCE-%EC%82%AC%EC%9A%A9-Python-%ED%8C%8C%EC%9D%B4%EC%8D%AC)
