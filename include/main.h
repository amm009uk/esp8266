
//
const static  String version = "1.1";                                // Version control
//
#include <Arduino.h>
#include <LittleFS.h>                                                // LittleFS support
#include <ArduinoJson.h>                                             // Read, write JSON format
#include <ESP8266WiFi.h>                                             // ESP8266 Core WiFi Library
#include <ESP8266WebServer.h>                                        // Local WebServer used to serve the configuration portal
#include <ESP8266HTTPUpdateServer.h>                                 // OTA Updater
#include <PubSubClient.h>                                            // MQTT Client Publish and Subscribe
#include <DNSServer.h>                                               // Local DNS Server used for redirecting all requests to the configuration portal

#include <..\include\user.h>																				 //	Contains custom	settings that	MUST be	modified
#include <..\include\functions.h>																		 //	Predefined functions

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
#endif

#define MQTT_DELAY_RECONNECT  10000                                  // Delay between MQTT reconnects
#define CONFIGFILE            "/config.json"                         // Configuration file holding various values
#define STATEFILE             "/state.json"                          // Relay state file holding relay on/off status
#define IP_REQUEST            "IP/Request"                           // MQTT IP Request Message
#define IP_REPLY              "IP/Reply/"                            // MQTT IP Reply Message
