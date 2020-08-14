## Raspberry PI thermometer

A raspberry PI thermometer that reads the temperature and writes it
to the console.

It is compatible with the [`w1_therm`](https://www.kernel.org/doc/Documentation/w1/slaves/w1_therm)
devices.

### Requirements

This requires the `w1-gpio` to be enabled and `docker` and `docker-compose` to be installed.

The recommended way:
1. Enable `w1-gpio`
   ```
   echo -e "dtoverlay=w1-gpio\n" | sudo tee -a /boot/config.txt
   sudo reboot
   ```
2. Install `docker` using the [convenience script](https://docs.docker.com/engine/install/debian/#install-using-the-convenience-script)
   ```
   curl -fsSL https://get.docker.com -o get-docker.sh
   sudo sh get-docker.sh
   sudo usermod -aG docker ${USER}
   ```
3. Install `docker-compose` requirements
   ```
   sudo apt-get install -y libffi-dev libssl-dev python3 python3-pip
   ```
4. Install `docker-compose` using [pip](https://docs.docker.com/compose/install/#install-using-pip)
   ```
   sudo pip3 -v install docker-compose
   ```

### Usage

1. Clone this repo
2. Run the following command
```
THERMOMETER_DEVICE_PATH=$(echo /sys/devices/w1_bus_master1/28-*/w1_slave) docker-compose up --build
```

