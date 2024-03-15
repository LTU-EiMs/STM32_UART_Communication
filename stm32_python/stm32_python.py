import tkinter as tk
import time
import random
from threading import Thread
import serial

weekday_names = {
    0: "Mon",
    1: "Tue",
    2: "Wed",
    3: "Thu",
    4: "Fri",
    5: "Sat",
    6: "Sun"
}

month_names = {
    1: "Jan",
    2: "Feb",
    3: "Mar",
    4: "Apr",
    5: "May",
    6: "Jun",
    7: "Jul",
    8: "Aug",
    9: "Sep",
    10: "Oct",
    11: "Nov",
    12: "Dec"
}

class SerialThread(Thread):
    def __init__(self, root):
        super().__init__()
        self.root = root
        self.serial_port = serial.Serial('/dev/ttyUSB0', 9600)  # Adjust the port and baud rate as needed
        self.running = True

    def run(self):
        while self.running:
            data = self.serial_port.readline().decode().strip()
            #print(data)
            fields = data.split(',')
            header = fields[0]
            if header == "SENSOR_DATA":
                temperature = float(fields[1])
                hours = fields[2]
                minutes = fields[3]
                seconds = fields[4]
                date = fields[5]
                month = fields[6]
                year = "20"+fields[7]
                button_status = fields[8]
                UID_con = fields[9] 
                name = fields[10] 

                # Update the UI with received data
                self.root.update_label(temperature, hours, minutes, seconds, date, month, year, button_status, UID_con, name)
            else:
                print("Invalid data format")
            time.sleep(0.1) 

    def stop(self):
        self.running = False

class MicrocontrollerApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Microcontroller Info")
        
        self.microcontroller_name_label = tk.Label(self, text="Microcontroller name:")
        self.microcontroller_name_label.grid(row=0, column=0, padx=10, pady=5, sticky="e")

        self.microcontroller_name_output = tk.Label(self, text="")
        self.microcontroller_name_output.grid(row=0, column=1, padx=10, pady=5, sticky="w")

        self.microcontroller_uid_label = tk.Label(self, text="Microcontroller UID:")
        self.microcontroller_uid_label.grid(row=1, column=0, padx=10, pady=5, sticky="e")

        self.microcontroller_uid_output = tk.Label(self, text="")
        self.microcontroller_uid_output.grid(row=1, column=1, padx=10, pady=5, sticky="w")

        self.new_name_label = tk.Label(self, text="New Microcontroller name:")
        self.new_name_label.grid(row=2, column=0, padx=10, pady=5, sticky="e")

        self.new_name_entry = tk.Entry(self)
        self.new_name_entry.grid(row=2, column=1, padx=10, pady=5)

        self.send_update_button = tk.Button(self, text="Update", command=self.send_update)
        self.send_update_button.grid(row=2, column=2, padx=10, pady=5)

        self.time_label = tk.Label(self, text="Current time in microcontroller:")
        self.time_label.grid(row=3, column=0, padx=10, pady=5, sticky="e")

        self.time_output = tk.Label(self, text="")
        self.time_output.grid(row=3, column=1, padx=10, pady=5, sticky="w")

        self.sync_time_button = tk.Button(self, text="Sync time", command=self.sync_time)
        self.sync_time_button.grid(row=3, column=2, padx=10, pady=5)

        self.temp_label = tk.Label(self, text="Current CPU temperature:")
        self.temp_label.grid(row=4, column=0, padx=10, pady=5, sticky="e")

        self.temp_output = tk.Label(self, text="")
        self.temp_output.grid(row=4, column=1, padx=10, pady=5, sticky="w")

        self.button_status_label = tk.Label(self, text="Button status:")
        self.button_status_label.grid(row=5, column=0, padx=10, pady=5, sticky="e")

        self.button_status_output = tk.Label(self, text="")
        self.button_status_output.grid(row=5, column=1, padx=10, pady=5, sticky="w")

        self.bad_message_button = tk.Button(self, text="Bad Message", command=self.send_bad_message)
        self.bad_message_button.grid(row=6, column=0, padx=10, pady=5, columnspan=3)

        self.serial_thread = SerialThread(self)
        self.serial_thread.start()

        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def on_close(self):
        self.serial_thread.stop()
        self.destroy()

    def sync_time(self):
        current_time=time.localtime()
        date=(str(current_time.tm_hour) +","+str(current_time.tm_min) +","+str(current_time.tm_sec) +","+ weekday_names[current_time.tm_wday] +","+month_names[current_time.tm_mon]+","+str(current_time.tm_mday)+","+str(current_time.tm_year-2000))
        print(date)
        message = f"COMMAND_DATE,{date}".encode()
        self.serial_thread.serial_port.write(message)
        #self.time_output.config(text=current_time)

    def update_label(self, temperature, hours, minutes, seconds, date, month, year, button_status, UID_con, name):
        self.microcontroller_uid_output.config(text=UID_con)
        self.temp_output.config(text=str(temperature) + "°C")
        self.time_output.config(text=f"{hours}:{minutes}:{seconds} {year}-{month}-{date}")
        self.button_status_output.config(text=button_status)
        self.microcontroller_name_output.config(text=name)

    def send_bad_message(self):
        # Send a predefined bad message over the serial port
        print("Sending bad message...")  # Print for debugging
        self.serial_thread.serial_port.write(b"BAD_MESSAGE\n")

    def send_update(self):
        new_name = self.new_name_entry.get()
        if new_name:
            # Construct a message to send to the serial port with the new name
            message = f"COMMAND_NAME,{new_name}".encode()
            self.serial_thread.serial_port.write(message)
            #print(f"Sent update: {message.decode().strip()}")

if __name__ == "__main__":
    app = MicrocontrollerApp()
    app.mainloop()
