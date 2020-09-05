# ESP8266

### This project assumes you know what a "ESP8266" device is and how to upload code to it

-------------------------------------------------------------------------------------------------------------
## Features

- Custom firmware to take control of the ESP8266 and integrate with MQTT Broker. The Broker would typically integrate with your Home Automation system 

- One firmware to support multiple devices:
    1. SonOff Basic
    2. SonOff Dual   *(not complete)*
    3. Sonoff 4CH    *(not complete)*
    4. Sonoff Light  *(not complete)*
    5. One motion, one temperature & up to four reed switches (door/window type)  *(not complete)*

- Device will respond to:
    1. MQTT messages
    2. Onboard switches
    3. GPIO-14 switch if connected  

- WiFi or MQTT drop outs handled automatically

- MQTT parameters set over HTTP

- OTA updates

-------------------------------------------------------------------------------------------------------------
## Version
1.1 Initial version  

-------------------------------------------------------------------------------------------------------------
## Device Setup
**For ease of initial setup, enable Serial output within **User.h** and monitor output. You will get the device IP and be able to monitor activity before final deployment**  

1. Set your WiFi SSID and PASSWORD in **User.h**. *Only place you can do this*

1. Set your ESP8266 Web access USER and PASSWORD in **User.h**. *Only place you can do this*

2. Flash the file system *LittleFS in this case*

3. Flash firmware

4. Use must power on/off after flashing firmware due a bug within esp8266 causing reboot to hang  

5. Once device is connected to WiFi and Broker, get its IP from serial output and connect to it using Browser
       *User/Password are stored in **sonoff/src/User.h** *

4. Configure device parameters on web page  
       - The MQTT section should be self explanatory  
       - The Inbound message is received by the device containing either **ON** or **OFF**  
       - The Outbound message is sent by the device containing the action carried out either **ON** or **OFF**  
       - Once all parameters are correct, save settings and device will reboot and reconnect to WiFi and MQTT Broker  

**Above steps should be done over USB-->Serial interface until device is fully functioning**  

5. Test device and once ok, turn off debugging within **User.h** and upload firmware one last time  


-------------------------------------------------------------------------------------------------------------
## Finding device IP Address
To get the device IP address you have the following options:  

  1. Look in your router to see WiFi clients  
    
  2. (a) If already connected to WiFi and MQTT Broker, you can send a blank MQTT message **IP/Request**  
     (b) Device will respond with a MQTT message **IP/Reply** and IP address in the payload  

-------------------------------------------------------------------------------------------------------------
## Credits
I am simply reusing other peoples amazing work for instance the following libraries:  
  - [PubSubClient](https://github.com/knolleary/pubsubclient)  
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)  

AND OF COURSE the many examples on github  

My development environment is with VSCode + PlatformIO