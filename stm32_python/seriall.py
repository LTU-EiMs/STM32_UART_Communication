import serial
import time

# Replace '/dev/ttyACM0' with the appropriate serial port on your system
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

try:
    while True:
        # Send a message over the serial port
        ser.write(b"Hello, world!\n")
        print("Sent: Hello, world!")

        # Read data from the serial port
        received_data = ser.readline().decode().strip()
        print("Received:", received_data)

        time.sleep(1)  # Wait for 1 second before sending the next message

except KeyboardInterrupt:
    # Close the serial port when the program is terminated
    ser.close()