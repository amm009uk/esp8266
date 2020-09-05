# ESP8266 over multiple devices

## This project assumes the following:
      ### you know what a ESP8266 device is and how to upload code to it
      ### you have MQTT Broker running
      ### you want to integrate the ESP8266 with your Home Automation system through MQTT messages

-------------------------------------------------------------------------------------------------------------
## Features

- Custom firmware to control ESP8266 and integrate with MQTT Broker. The Broker would typically integrate with your Home Automation system 

- One firmware to support multiple devices:
    - SonOff Basic
    - SonOff Dual   *(not complete)*
    - Sonoff 4CH    *(not complete)*
    - Sonoff Light  *(not complete)*
    - One motion, one temperature & up to four reed switches (door/window type)  *(not complete)*

- Device will respond to:
    - MQTT messages
    - Onboard switches
    - GPIO-14 switch if connected  

- WiFi or MQTT drop outs handled automatically

- MQTT parameters set over HTTP

- OTA updates

-------------------------------------------------------------------------------------------------------------
## Version
  
1.1 Initial version  

-------------------------------------------------------------------------------------------------------------
## Device Setup
  
**For ease of initial setup, enable Serial output within **User.h** and monitor output. You will get the device IP and be able to monitor activity before final deployment**  

1. Set your WiFi SSID/PASSWORD in **User.h** - *Only place you can do this*

2. Set your ESP8266 Web access USER/PASSWORD in **User.h** - *Only place you can do this*

3. Flash the file system - *LittleFS in this case*

4. Flash firmware

5. Use must power on/off after flashing firmware due a bug within esp8266 causing reboot to hang  

6. Once device is connected to WiFi and Broker, get IP from serial output and connect using Browser

7. Configure device parameters on web page  
       - The MQTT section should be self explanatory  
       - The Inbound message is received by the device containing either **ON** or **OFF**  
       - The Outbound message is sent by the device containing the action carried out, either **ON** or **OFF**  
       - Once all parameters are correct, save settings and device will reboot and connect to WiFi + Broker  

**Above steps should be done over USB-->Serial interface until device is fully functioning**  

8. Test device and once ok, turn off debugging within **User.h** and upload firmware one last time  
       - You may upload on serial or using OTA

-------------------------------------------------------------------------------------------------------------
## Finding device IP Address
To get the device IP after you have deployed, you have the following options:  

  1. Look in your router to see WiFi clients and try and figure it out
    
  2. (a) Send a blank MQTT message **IP/Request**  
     (b) Device will respond with MQTT message **IP/Reply** and IP address is in the payload  

-------------------------------------------------------------------------------------------------------------
## Credits
I am simply reusing other peoples amazing work for instance the following libraries:  
  - [PubSubClient](https://github.com/knolleary/pubsubclient)  
  - [ArduinoJson](https://github.com/bblanchon/ArduinoJson)  

AND OF COURSE the many examples on github  

My development environment is with VSCode + PlatformIO