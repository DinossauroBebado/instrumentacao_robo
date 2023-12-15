import serial
import keyboard
import time

# Replace with your HC-05 Bluetooth module's address and baud rate
bluetooth_address = "00:23:09:01:16:CC" # Replace with your HC-05 address
baud_rate = 9600

# Initialize the serial connection
ser = serial.Serial(bluetooth_address, baud_rate, timeout=1)

def send_command(command):
    ser.write(command.encode('utf-8'))

try:
    while True:
        if keyboard.is_pressed('w'):
            send_command('F')
        elif keyboard.is_pressed('s'):
            send_command('B')
        elif keyboard.is_pressed('a'):
            send_command('L')
        elif keyboard.is_pressed('d'):
            send_command('R')
        else:
            send_command('S')

        time.sleep(0.1)  # Adjust the sleep time as needed

except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
