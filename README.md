# Thermometer log

A platform for logging temperature using `w1_therm` thermometers.

## Temperature logbook

See `logbook`.

A server for storing the logs from the devices. It's designed to run on
Vercel. It uses AWS DynamoDB to store the data which can be created
using terraform (see `storage.tf`).

## Loggers

See `logger` for the thermometer loggers.

### `rasbpi_ds18b20` logger

A raspberry PI thermometer that reads the temperature and sends it to
a server over HTTP. It is compatible with the [`w1_therm`] devices.

### `black-pill+esp8266_ds18b20` logger

A logger for the [STM32F103C8T6 Black Pill] with ESP8266 WiFi Microchip.

This doesn't yet send the results to the logbook.

It is built with PlatformIO.

### Requirements

This requires the `w1-gpio` to be enabled and `docker` and `docker-compose` to be installed.

The recommended way:
1. Enable `w1-gpio`
   ```
   echo -e "dtoverlay=w1-gpio\n" | sudo tee -a /boot/config.txt
   sudo reboot
   ```
2. Install `docker` using the [convenience script]
   ```
   curl -fsSL https://get.docker.com -o get-docker.sh
   sudo sh get-docker.sh
   sudo usermod -aG docker ${USER}
   ```
3. Install `docker-compose` requirements
   ```
   sudo apt-get install -y libffi-dev libssl-dev python3 python3-pip
   ```
4. Install `docker-compose` using [pip]
   ```
   sudo pip3 -v install docker-compose
   ```
5. Set up a single-node swarm on the Raspberry Pi
   ```
   docker swarm init
   ```
6. Update the environment variables, replacing the `<..>`s with the
   correct values:
   ```
   THERM_DEV_PATH=/sys/devices/w1_bus_master1/28-<ID>/w1_slave
   THERM_SERVER_ADDRESS=<server-address>
   THERM_LOCATION=<location
   ```
7. Set up service secret:
   ```
   docker secret create therm_auth_secret -
   ```

### Usage

1. Clone this repo
2. Run the following command, replacing <location> with the location of the thermometer, for example "living-room". This should only use a-z characters or hypens.
   ```
   docker stack delpoy --compose-file docker-compse.yml --compose-file docker-compose.prod.yml thermometer
   ```

### Manual testing

```
docker-compose -f docker-compose.yml -f 
```

[`w1_therm`]: https://www.kernel.org/doc/Documentation/w1/slaves/w1_therm
[STM32F103C8T6 Black Pill]: https://stm32-base.org/boards/STM32F103C8T6-Black-Pill.html
[convenience script]: https://docs.docker.com/engine/install/debian/#install-using-the-convenience-script
[pip]: https://docs.docker.com/compose/install/#install-using-pip
