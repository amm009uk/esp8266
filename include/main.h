
//
const static  String version = "2.1";                                // Version control
//
#include <Arduino.h>
#include <LittleFS.h>                                                // LittleFS support
#include <ArduinoJson.h>                                             // Read, write JSON format
#include <ESP8266WiFi.h>                                             // ESP8266 Core WiFi Library
#include <ESP8266WebServer.h>                                        // Local WebServer used to serve the configuration portal
#include <ESP8266HTTPUpdateServer.h>                                 // OTA Updater
#include <PubSubClient.h>                                            // MQTT Client Publish and Subscribe
#include <DNSServer.h>                                               // Local DNS Server used for redirecting all requests to the configuration portal

#include <..\include\functions.h> //	Predefined functions
#include <..\include\user.h> //	Contains custom	settings that	MUST be	modified
#ifdef TEMP_SENSOR
#include <DHT.h> // DHT sensor library by Adafruit
#endif


/******************************************************************************************************/
  
#ifdef SONOFF_BASIC
#define LED_PIN								13																		 // LED indicator
#define RELAY_PIN							12																		 // Relay for power switch
#define TOGGLE_PIN					  00																		 // On board button
#define EXT_PIN   					  14																		 // GPIO14 external switch
#endif

#ifdef SONOFF_LIGHT
#define LED_PIN 	    				13																		 // WiFi LED indicator
#define RELAY_PIN							12																		 // Relay and LED for touch switch
#define TOUCH_PIN 					  00																		 // Touch button
#endif

#ifdef SONOFF_DUAL
#define BLUE_LED_PIN 	    	  13																		 // WiFi LED indicator
#define RELAY01_PIN    	      12																		 // Relay 1 (GREEN LED)
#define RELAY02_PIN   	    	05																		 // Relay 2 (RED LED)
#endif

#ifdef SONOFF_4CH
#endif

#ifdef WEMOS
#define BUILTIN_LED D4 // Built-in LED on the Wemos D1
#endif

#ifdef TEMP_SENSOR
#define DHTTYPE DHT22                                                // DHT 22 (AM2302), AM2321
#define DHT_PIN D1                                                   // Pin for Temperature sensor Wemos D1 mini pro
#endif
#ifdef MOTION_SENSOR
#define MOTION_PIN D2                                                // Pin for motion sensor
#endif
#ifdef REED_SENSOR1
#define REED_PIN1 D5                                                 // Pin for door/window sensor 1
#endif
#ifdef REED_SENSOR2 
#define REED_PIN2 D6                                                 // Pin for door/window sensor 2
#endif 
#ifdef REED_SENSOR3
#define REED_PIN3 D7                                                 // Pin for door/window sensor 3
#endif 
#ifdef REED_SENSOR4
#define REED_PIN4 D8                                                 // Pin for door/window sensor 4
#endif


#define MQTT_DELAY_RECONNECT  10000                                  // Delay between MQTT reconnects

#define SONOFF_CONFIG         "/sonoff_config.json"                  // Configuration file sonoff messages
#define WEMOS_CONFIG          "/wemos_config.json"                   // Configuration file for wemos devices
#define SONOFF_STATE          "/sonoff_state.json"                   // Relay state file holding relay on/off status

#define IP_REQUEST            "IP/Request"                           // MQTT IP Request Message
#define IP_REPLY              "IP/Reply/"                            // MQTT IP Reply Message
