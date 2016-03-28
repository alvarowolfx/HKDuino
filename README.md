# HKDuino project
This is my personal DIY home automation project consisting of these modules:

* Central/Controller - RaspberryPi running [HomeAssistant](https://home-assistant.io/).
* Gateway - Arduino [MySensors](http://www.mysensors.org) Serial  Gateway. See [Serial Gateway](http://www.mysensors.org/build/serial_gateway).
* Nodes - Arduino + NRF24L01+ + Wharever I want.

I'm sharing the code for those who are interested in studying these tecnologies.

All Arduino projects was developed using [PlatformIO](http://platformio.org) for libraries dependencies. 

## Pre requirements
- Raspberry Pi or wharever Linux machine you are using as central - Python 3.4 with HomeAssistant installed. Basically you can install with :
```shell
pip3 install homeassistant
```
- Arduino.
- NRF24L01 radio modules.
- Relays
- RGB Leds

## Local enviroment
- Python 
- Install PlatformIO CLI - [here](http://docs.platformio.org/en/latest/installation.html) 
```shell
pip install -U platformio
```
- Run init command on each project:
```shell
# I use CLion soo,  ide = clion
platformio init --ide {ide}
```
- Install MySensors Library with this command:
```shell
platformio lib install 548
```

## Configuring and Runing HomeAssistant
- Configuring HomeAssistant configuration.yml (coming soon...)
- Init HomeAssistant on server:
```shell
hass
```

## Deploying to Arduino
- Deploy sketch on your arduinos:
```shell

# In my case I already configured tree types of boads. Arduino Uno(uno), Arduino Nano(nano) and Arduino Duemilanove(duemi).
platformio run -t upload -e {env}
``` 

