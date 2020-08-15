import os
import time
import requests

class ThermometerReader:
    def __init__(self):
        pass

    def read(self):
        with open('/app/dev/therm', 'r') as file:
            content = file.readlines();
            crc_line = content[0].strip()
            temp_line = content[1].strip()

            if crc_line.endswith('YES'):
                return int(temp_line.split("=")[1]) / 1000            
            else:
                print("Error: unable to read temperature: " + crc_line)

class ThermometerLog:
    def __init__(self, url, token):
        self.url = url
        self.token = token

    def log(self, temp_reading):
        data = {
            'temperature': temp_reading
        }
        headers = {
            'Authorization': ('Bearer ' + self.token)
        }
        response = requests.post(self.url, json=data, headers=headers)
        response.raise_for_status()

def get_required_env(key):
    if key not in os.environ:
        raise Exception('The ' + key + ' must be specified')

    return os.environ.get(key)

def get_secret(secret):
    with open('/run/secrets/' + secret) as f:
        return f.readline().strip();

def main():
    read_period = int(os.environ.get('THERM_READ_PERIOD', '2'))
    location = get_required_env('THERM_LOCATION')
    server_address = get_required_env('THERM_SERVER_ADDRESS')
    url = server_address + '/rooms/' + location + '/log'
    token = get_secret('therm_auth_secret')

    reader = ThermometerReader()
    thermometer_logger = ThermometerLog(url, token)

    while True:
        try:
            start_time = time.time()
            temp_reading = reader.read()
            print("Temperature: " + str(temp_reading))
            thermometer_logger.log(temp_reading)
        
            remaining = read_period - (time.time() - start_time)
            if (remaining > 0):
                time.sleep(read_period)

        except requests.HTTPError as err:
            print("Request failed: " + str(err))
        except OSError as err:
            print("Unable to read temperature: " + str(err))

if __name__ == "__main__":
    main()

