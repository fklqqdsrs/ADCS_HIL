import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import serial
import serial.tools.list_ports
import threading
import re
import csv
import queue

# Global variable to track the current update count for validation
update_count = 0
NUM_REGISTERS = 600  # Number of registers to keep visible

# Function to generate valid values based on the current update count
def generate_valid_values(update_count):
    values = [i for i in range(NUM_REGISTERS)]

    # Apply the update logic based on the current update count
    for _ in range(update_count + 1):  # +1 to include the initial setup
        for i in range(NUM_REGISTERS):
            values[i] += (i % 3) + 1

    return set(values)  # Convert to a set for faster lookup

# Function to validate data based on the dynamically generated valid values
def validate_entry(value):
    # Regenerate valid values based on the current state
    valid_values = generate_valid_values(update_count)
    # Check if the value is in the set of valid values
    return "Valid" if value in valid_values else "Invalid"

# Function to handle UI updates from the queue
def update_ui(tree, data_queue):
    while not data_queue.empty():
        entry = data_queue.get()
        # Insert data into the treeview
        tree.insert("", tk.END, values=entry)
        # Scroll to the latest row to keep it in view
        tree.see(tree.get_children()[-1])

        # Limit the number of rows to the latest NUM_REGISTERS
        if len(tree.get_children()) > NUM_REGISTERS:
            tree.delete(tree.get_children()[0])  # Remove the oldest entry

    # Schedule the next UI update
    tree.after(100, update_ui, tree, data_queue)

# Function to read data from UART continuously
def read_uart(ser, data_queue, save_to_file, file_path):
    global update_count  # Use global variable to track updates
    received_count = 0  # Counter to track the number of values received

    while ser.is_open:
        try:
            # Read line from UART
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue

            # Use regex to parse the required fields from the line
            try:
                # Extracting only flags, register, value, and time
                pattern = r"flags:(\d+)\s+Register\s+\[(\d+),\s*(\d+)\]\s+Retrieved\s+Value:\s+(\d+),\s+Time:\s+(\d+)\s+ms"
                match = re.search(pattern, line)
                
                if match:
                    flags = int(match.group(1))
                    id = int(match.group(2))
                    sub_id = int(match.group(3))
                    value = int(match.group(4))
                    time = int(match.group(5))

                    # Validate the value
                    validation_status = validate_entry(value)

                    # Create an entry for the UI and CSV file
                    entry = (flags, id, sub_id, value, time, validation_status)

                    # Put the entry into the queue for UI update
                    data_queue.put(entry)

                    # Save to CSV file if selected
                    if save_to_file.get() and file_path.get():
                        with open(file_path.get(), 'a', newline='') as csvfile:
                            writer = csv.writer(csvfile)
                            writer.writerow(entry)

                    # Increment the count of received values
                    received_count += 1

                    # Regenerate valid values every 600 received values
                    if received_count >= 600:
                        received_count = 0  # Reset the count
                        update_count += 1  # Increment the global update count

                else:
                    print(f"Line format did not match: {line}")

            except (IndexError, ValueError) as e:
                print(f"Error parsing line: {line} -> {e}")

        except serial.SerialException as e:
            print(f"Serial exception: {e}")
            break

# Function to start reading data from UART
def start_reading(port, baudrate, tree, data_queue, start_btn, stop_btn, save_to_file, file_path):
    try:
        # Open serial port
        ser = serial.Serial(port, baudrate, timeout=1)
        start_btn.config(state=tk.DISABLED)
        stop_btn.config(state=tk.NORMAL)

        # Start a separate thread for reading UART data
        thread = threading.Thread(target=read_uart, args=(ser, data_queue, save_to_file, file_path), daemon=True)
        thread.start()

        # Save the serial object to stop reading later
        stop_btn.config(command=lambda: stop_reading(ser, start_btn, stop_btn))

    except serial.SerialException as e:
        messagebox.showerror("Serial Error", f"Failed to open port: {e}")

# Function to stop reading data from UART
def stop_reading(ser, start_btn, stop_btn):
    if ser.is_open:
        ser.close()
    start_btn.config(state=tk.NORMAL)
    stop_btn.config(state=tk.DISABLED)

# Function to select file for saving data
def select_file(file_path):
    file = filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("CSV files", "*.csv"), ("All files", "*.*")])
    file_path.set(file)

# Function to setup the UI
def setup_ui():
    root = tk.Tk()
    root.title("UART Test Register for CSP")

    # Frame for COM port and baudrate settings
    settings_frame = ttk.Frame(root)
    settings_frame.pack(pady=10)

    # Port selection
    port_label = ttk.Label(settings_frame, text="Port:")
    port_label.grid(row=0, column=0, padx=5, pady=5)
    port_var = tk.StringVar()
    port_combobox = ttk.Combobox(settings_frame, textvariable=port_var)
    port_combobox['values'] = [port.device for port in serial.tools.list_ports.comports()]
    port_combobox.grid(row=0, column=1, padx=5, pady=5)

    # Baudrate selection
    baudrate_label = ttk.Label(settings_frame, text="Baudrate:")
    baudrate_label.grid(row=0, column=2, padx=5, pady=5)
    baudrate_var = tk.IntVar(value=115200)
    baudrate_combobox = ttk.Combobox(settings_frame, textvariable=baudrate_var)
    baudrate_combobox['values'] = [9600, 19200, 38400, 57600, 115200]
    baudrate_combobox.grid(row=0, column=3, padx=5, pady=5)

    # Options for saving data
    save_frame = ttk.Frame(root)
    save_frame.pack(pady=10)

    save_to_file = tk.BooleanVar(value=False)
    save_checkbutton = ttk.Checkbutton(save_frame, text="Save to CSV", variable=save_to_file)
    save_checkbutton.grid(row=0, column=0, padx=5)

    file_path = tk.StringVar()
    file_button = ttk.Button(save_frame, text="Select File", command=lambda: select_file(file_path))
    file_button.grid(row=0, column=1, padx=5)

    # Buttons for start and stop
    button_frame = ttk.Frame(root)
    button_frame.pack(pady=10)

    # Queue for managing data between threads
    data_queue = queue.Queue()

    start_btn = ttk.Button(button_frame, text="Start", command=lambda: start_reading(port_var.get(), baudrate_var.get(), tree, data_queue, start_btn, stop_btn, save_to_file, file_path))
    start_btn.grid(row=0, column=0, padx=5)

    stop_btn = ttk.Button(button_frame, text="Stop", state=tk.DISABLED)
    stop_btn.grid(row=0, column=1, padx=5)

    # Treeview for displaying UART data
    columns = ("Flags", "ID", "Sub ID", "Value", "Time", "Validation")
    tree = ttk.Treeview(root, columns=columns, show="headings")
    tree.heading("Flags", text="Flags")
    tree.heading("ID", text="ID")
    tree.heading("Sub ID", text="Sub ID")
    tree.heading("Value", text="Value")
    tree.heading("Time", text="Time (ms)")
    tree.heading("Validation", text="Validation")

    # Scrollbar for the treeview
    scrollbar = ttk.Scrollbar(root, orient=tk.VERTICAL, command=tree.yview)
    tree.configure(yscroll=scrollbar.set)
    scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
    tree.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)

    # Start the UI update loop
    tree.after(100, update_ui, tree, data_queue)

    root.mainloop()

if __name__ == "__main__":
    setup_ui()
