## Raspberry PI thermometer

A raspberry PI thermometer that reads the temperature and sends it to
a server over HTTP.

It is compatible with the [`w1_therm`] devices.

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

### Usage

1. Clone this repo
2. Run the following command, replacing <location> with the location of the thermometer, for example "living-room". This should only use a-z characters or hypens.
   ```
   THERM_SERVER_ADDRESS=https://<server>/ THERM_LOCATION=<location> THERM_DEVICE_PATH=$(echo /sys/devices/w1_bus_master1/28-*/w1_slave) docker-compose up --build
   ```

### Manual testing

1. Run the command and check the output:
   ```
   THERM_LOCATION=<location> THERM_DEVICE_PATH=$(echo /sys/devices/w1_bus_master1/28-*/w1_slave) docker-compose -f docker-compose.yml -f docker-compose.single-node-test.yml up --build
   ```

[`w1_therm`]: https://www.kernel.org/doc/Documentation/w1/slaves/w1_therm
[convenience script]: https://docs.docker.com/engine/install/debian/#install-using-the-convenience-script
[pip]: https://docs.docker.com/compose/install/#install-using-pip
