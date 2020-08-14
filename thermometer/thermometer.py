import os
import time

read_period = int(os.environ.get('THERM_READ_PERIOD', '2'))
read_period = int(os.environ.get('THERM_SERVER_ADDRESS', '2'))

while True:
    with open('/app/dev/therm', 'r') as file:
        content = file.readlines();
        crc_line = content[0].strip()
        temp_line = content[1].strip()

        if crc_line.endswith('YES'):
            temp_reading = int(temp_line.split("=")[1]) / 1000
            print("Temperature: " + str(temp_reading))
        else:
            print("Error: unable to read temperature: " + crc_line)

    time.sleep(read_period)



