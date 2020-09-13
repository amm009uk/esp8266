#include <..\include\main.h> // Static values

/*----------------------------------------- Global variables -----------------------------------------*/

WiFiClient WiFiClnt;                        // The WiFi client object
PubSubClient MQTTclient(WiFiClnt);          // MQTT client object
ESP8266WebServer httpServer(80);            // WebServer on port 80
ESP8266HTTPUpdateServer httpUpdater;        // OTA updates
char deviceID[30] = "                    "; // User specified name stored in configuration file

/*
* MQTT Settings
*/
char mqtt_server[25]   = "                        ";
int  mqtt_port;
char mqtt_user[10]     = "         ";
char mqtt_password[10] = "         ";

long lastReconnectAttempt = 0;                                       // Previous reconnect attempt
long now;                                                            // Hold current millisecs device has been running

#ifdef SONOFF
char mqtt_inTopic1[25]  = "                        ";
char mqtt_outTopic1[25] = "                        ";
char mqtt_inTopic2[25]  = "                        ";
char mqtt_outTopic2[25] = "                        ";
char mqtt_inTopic3[25]  = "                        ";
char mqtt_outTopic3[25] = "                        ";
char mqtt_inTopic4[25]  = "                        ";
char mqtt_outTopic4[25] = "                        ";

int state1 = 0;                                                      // Initial state of power (FOR BASIC, LIGHT, DUAL and 4CH)
int state2 = 0;                                                      // Initial state of power (DUAL and 4CH)
int state3 = 0;                                                      // Initial state of power (4CH)
int state4 = 0;                                                      // Initial state of power (4CH)

boolean btnState = false;                                            // Hold current state of relay

//boolean ledState = false;
boolean buttonState = false;
boolean lastButtonState = false;                                     //
unsigned long lastDebounceTime = 0;                                  // the last time the output pin was toggled
unsigned long debounceDelay = 50;                                    // the debounce time; increase if the output flickers
#endif


#ifdef WEMOS
char mqtt_tempTopic[40];
char mqtt_motionTopic[40];
char mqtt_reedTopic1[40];
char mqtt_reedTopic2[40];
char mqtt_reedTopic3[40];
char mqtt_reedTopic4[40];
#endif

#if defined WEMOS && defined TEMP_SENSOR
    int tempPolling = 10; // Delay in secs for sleep timer (from config file)
char *temp_state = (char *)"UNDEFINED";
long lastMsg = 0;
float temp_c;
float humidity;
//static char celsiusTemp[7];
//static char fahrenheitTemp[7];
//static char humidityTemp[7];
DHT dht(DHT_PIN, DHTTYPE);                                           // Initialize DHT sensor
#endif

#if defined WEMOS && defined MOTION_SENSOR
String motion_state = "UNDEFINED";
String last_motion_state = "";
#endif

#if defined WEMOS && defined REED_SENSOR1
String reed_state1 = "UNDEFINED";
String last_reed_state1 = "";
#endif

#if defined WEMOS && defined REED_SENSOR2
String reed_state2 = "UNDEFINED";
String last_reed_state2 = "";
#endif

#if defined WEMOS && defined REED_SENSOR3
String reed_state3 = "UNDEFINED";
String last_reed_state3 = "";
#endif

#if defined WEMOS && defined REED_SENSOR4
String reed_state4 = "UNDEFINED";
String last_reed_state4 = "";
#endif

/* 
** ------------------------------------------ Initial Setup ------------------------------------------ **
*/
void setup()
{

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(1000);
  debugln(F("************ Setup() started **************"));
#endif

  if (!LittleFS.begin())                                             // Load filesystem
  { 
#ifdef SERIAL_DEBUG
    debugln(F("File system failed to mount"));
#endif
    return;
  }

  //LittleFS.format();                                                 // Wipe file system for a new device (don't forget to comment once done)

#ifdef SONOFF
  if (loadSonoffConfig())                                            // Load config file
  { 
    outputData();
    loadSonoffState();
  }
  else
  {
    return;
  }
#endif

#ifdef WEMOS
  if (loadWemosConfig())                                             // Load config file
  {
#ifdef SERIAL_DEBUG
    outputData();
#endif
  }
  else
  {
    return;
  }
#endif

  //
  // Setup WiFi and Web Access
  //
  WiFi.hostname(deviceID); // Set the hostname on network
  connectWiFi();

  httpServer.onNotFound(handleNotFound);
  httpServer.on("/", handleRoot);
  httpServer.on("/saveChanges", handlesaveChanges);
  httpServer.on("/reset", handleReset);
  httpUpdater.setup(&httpServer, WWW_USER, WWW_PASSWD);
  httpServer.begin(); // Start Web Server
  delay(10);

  MQTTclient.setServer(mqtt_server, mqtt_port);                      // Initialse MQTT client
  MQTTclient.setCallback(callback);                                  // Callback service for receiving MQTT messages

  delay(10);

  deviceInit();                                                      // Set device initial status
  
  delay(10);

#ifdef SERIAL_DEBUG
  debugln(F("************ Setup() finished *************"));
#endif

} // setup()

/* 
** -------------------------------------------  Code Loop -------------------------------------------- **
*/
void loop()
{
  yield();

  //
  // Reconnect WiFi if down
  //
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

  yield();

  //
  // Non-blocking connect to MQTT if WiFi is connected.
  // This allows other parts of the loop to run whilst no MQTT connection
  //
  if ((WiFi.status() == WL_CONNECTED) && !MQTTclient.connected())
  {
    now = millis();
    if ((now - lastReconnectAttempt) > MQTT_DELAY_RECONNECT)
    { // Attempt MQTT conncection if we tried over DELAY
      lastReconnectAttempt = now;
      // Attempt to reconnect MQTT
      if (MQTTconnect())
      {
        // Client connected
        lastReconnectAttempt = 0;
      }
    }
  }

  yield();

  if (MQTTclient.connected())
  {
    MQTTclient.loop(); // If MQTT Client connected check for MQTT activity
  }

  yield();

  httpServer.handleClient(); // Give time to Web Server

  yield();

#ifdef SONOFF_BASIC
  if (!digitalRead(TOGGLE_PIN)) // Onboard toggle button
    relayToggle();

  if (!digitalRead(EXT_PIN) && (!btnState))
  { // External button one way
    btnState = true;
    relayToggle();
  }

  if (digitalRead(EXT_PIN) && (btnState))
  { // External button other way
    btnState = false;
    relayToggle();
  }
#endif

#ifdef SONOFF_LIGHT
  // Get the current button state
  int reading = digitalRead(TOUCH_PIN);

  //De-bounce the button
  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // If the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;

      // Only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH)
        relayToggle();
    }
  }
  // save the reading.  Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
#endif

#ifdef SONOFF_DUAL

#endif

#ifdef SONOFF_4CH

#endif

#ifdef TEMP_SENSOR
  getDHTdata();
#endif

#ifdef MOTION_SENSOR
  checkMotionState(MOTION_PIN, mqtt_motionTopic, last_motion_state, motion_state);
#endif

#ifdef REED_SENSOR1
  checkReedState(REED_PIN1, mqtt_reedTopic1, last_reed_state1, reed_state1);
#endif

#ifdef REED_SENSOR2
  checkReedState(REED_PIN2, mqtt_reedTopic2, last_reed_state2, reed_state2);
#endif

#ifdef REED_SENSOR3
  checkReedState(REED_PIN3, mqtt_reedTopic3, last_reed_state3, reed_state3);
#endif

#ifdef REED_SENSOR4
  checkReedState(REED_PIN4, mqtt_reedTopic4, last_reed_state4, reed_state4);
#endif

  yield();

} // loop()
