### Arduino Setup

To Arduino chip used for this project is the ESP32 chip with built-in OLED screen made by heltec.
More Information about the chip can be found [here](http://www.heltec.cn/project/wifi-kit-32/?lang=en) 
The chip was programmed using the Arduino IDE. 
More information for installing the ARduino IDE can be found [here](https://www.arduino.cc/en/Main/Software) 
After the Arduino IDE is installed, you will also need to set up the Arduino IDE for esp32.  The instructions can be found [here] (https://robotzero.one/heltec-wifi-kit-32/) 
In order for the Arduino IDE to be able to commmunicate with the esp32 chip, a USB to UART driver must be installed on the machine.  The driver software can be found [here] (https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)  
Once that is done, the last thing that is needed is to add the  library needed for the SocketIO client.
To do that, Open the .ino file from this repo, then from the arduino menu, navigate to sketch-\>Include Library-\>add .ZIP library and then select the WebSocketIO folder contained in this repo.  
Before flashing the program to the chip, make sure the WiFi information is changed in the program and also the sensor\_ids are unique and flash to the 

#### Author
Cameron Eng
Peter Wu
