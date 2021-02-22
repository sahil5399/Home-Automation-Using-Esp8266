# Home-Automation Using Esp8266
An easy to make home automation project based on a popular wifi micro controller ESP8266 12-E module.
There are two different version of this project which have different functionalities. 
- Version 1: A HTTP API and a web app for controlling all the switches of the project
- Version 2: A upnp device which can be controlled by Alexa


## Parts Required
- ESP 12E micro controller or ESP 12-E based development board
- 5V relay modules 6 channel
- AMS1117 Voltage regulator
- 240V AC to 5V DC converter
- Wires
- Registers 10k
- Capacitor 100uf
- USB to ttl Converted ( Required to program ESP ). You can also use arduino for the same

## Circuit Diagram

[Circuit Diagram](CircuitDiagram.png)

## Connection
Connections are same for both the version, only the code is changed
- Connect all the relay data pins with the ESP pin number 4,14,12,5,13,1
- Connect the Relay power and Ground with 5V power supply
- Connect the ESP power and groound with 3.3V power supply
- Connect All the COM pin of the relay module with neutral
- Connect Relay NC pin on the neutral pin of the plug
- Connect the live and ground wire of the plus directly from the mains

## Code
### [Version 1](version1.ino)
Once you upload the code, ESP8266 will act as a server.
After ESP is connected to the wifi, find the ip address of the ESP device and paste the IP address on the browser.
You will be asked for username and password. 
The default username and password is admin:esp8266
you can change the username and password by changing the valus of the www_username and www_passwword variables. 
You will get a controll panel with 6 switches to turn on and off the switch.

To control the switch using an http API, you can issue a get request in the following format
`http://<ip-address>/control?relay=<Replay PIN Number>&action=<1/0>`
If you want to turn on the relay, you can set the value of action in the http parameter as 1
If you want to turn off the relay, you can set the value of action in the http parameter as 0
Note: Dont forget to supply the username and password with the http get request 

### [Version 2](version2.ino)
In this version, we are using [FauxmoESP by vintlabs](https://github.com/vintlabs/fauxmoESP) 
This library uses [ESPAsyncTCP](https://github.com/me-no-dev/AsyncTCP) library by [me-no-dev](https://github.com/me-no-dev)
After installing all the dependencies and uploading the code on our esp, we can issue discover device command on our alexa enabled devices to discover our virtual device and enable us to control them using voice commands.

## References
- https://tinkerman.cat/post/emulate-wemo-device-esp8266

## Future work
- The UI of web app in version 1 can be updated using css frameworks 
