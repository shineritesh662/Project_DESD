import time

def read_accelerometer_data():
    with open("/home/puneeth/Downloads/CDAC_Project/adxl_data.txt", "r") as file:
        data = file.read().strip()
    return data

while True:
    data = read_accelerometer_data()
    print(f"Sending data: {data}")
    # Code to send the data over the network (e.g., using sockets)
    time.sleep(1)