
#include <..\include\main.h> // Static values

/*----------------------------------------- Global variables -----------------------------------------*/

WiFiClient WiFiClnt;                  // The WiFi client object
PubSubClient MQTTclient(WiFiClnt);    // MQTT client object
ESP8266WebServer httpServer(80);      // WebServer on port 80
ESP8266HTTPUpdateServer httpUpdater;  // OTA updates
char deviceID[15] = "              "; // User specified name stored in configuration file

/* MQTT Settings */
char mqtt_server[25] = "                        ";
int mqtt_port;
char mqtt_user[10] = "         ";
char mqtt_password[10] = "         ";
char mqtt_inTopic1[25] = "                        ";
char mqtt_outTopic1[25] = "                        ";
char mqtt_inTopic2[25] = "                        ";
char mqtt_outTopic2[25] = "                        ";
char mqtt_inTopic3[25] = "                        ";
char mqtt_outTopic3[25] = "                        ";
char mqtt_inTopic4[25] = "                        ";
char mqtt_outTopic4[25] = "                        ";
long lastReconnectAttempt = 0;
long now; // Hold current millisecs device has been running

int state1 = 0; // Initial state of power (FOR BASIC or LIGHT)
int state2 = 0; // Initial state of power (DUAL)
int state3 = 0; // Initial state of power (4CH)
int state4 = 0; // Initial state of power (4CH)

#ifdef SONOFF_BASIC
boolean btnState = false; // Hold current state of relay
#endif

#ifdef SONOFF_LIGHT
//boolean ledState = false;
boolean buttonState = false;
boolean lastButtonState = false;    //
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers
#endif

#ifdef SONOFF_DUAL
#endif

#ifdef SONOFF_4CH
#endif

#ifdef WEMOS
#endif

/* 
** ------------------------------------------ Start of code ------------------------------------------ **
*/

void setup()
{

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  delay(5000);
  debugln(F("************ Setup() started **************"));
#endif

  if (!LittleFS.begin())
  { // Load filesystem
#ifdef SERIAL_DEBUG
    debugln(F("File system failed to mount"));
#endif
    return;
  }

  //LittleFS.format();                                                 // Wipe file system for a new device (don't forget to comment once done)

  if (loadConfig())
  { // Load config file
#ifdef SERIAL_DEBUG
    outputData();
#endif
  }
  else
  {
    return;
  }

  if (loadState())
  { // Load previous state/s so relays can be pre-set
#ifdef SERIAL_DEBUG
    debugln(F("..State data start"));
    debug(F("....state 1: "));
    debugln(String(state1));
    debug(F("....state 2: "));
    debugln(String(state2));
    debug(F("....state 3: "));
    debugln(String(state3));
    debug(F("....state 4: "));
    debugln(String(state4));
    debugln(F("..State data end"));
#endif
  }
  else
  {
    return;
  }

  deviceInit(); // Set device status to saved/previous state

  delay(10);

  /*
** Setup WiFi and Web Access
*/
  WiFi.hostname(deviceID); // Set the hostname on network
  connectWiFi();

  httpServer.onNotFound(handleNotFound);
  httpServer.on("/", handleRoot);
  httpServer.on("/saveChanges", handlesaveChanges);
  httpServer.on("/reset", handleReset);
  httpUpdater.setup(&httpServer, WWW_USER, WWW_PASSWD);
  httpServer.begin(); // Start Web Server
  delay(10);

  MQTTclient.setServer(mqtt_server, mqtt_port); // Initialse MQTT client
  MQTTclient.setCallback(callback);             // Callback service for receiving MQTT messages

  delay(10);

#ifdef SERIAL_DEBUG
  debugln(F("************ Setup() finished *************"));
#endif

} // setup()

void connectWiFi()
{
  WiFi.disconnect();

#ifdef SERIAL_DEBUG
  debug(F("Connect WiFi: "));
  debug(ssid);
  debug(F("/"));
  debug(password);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  for (int i = 0; i < 30; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
#ifdef SERIAL_DEBUG
      debug(F("."));
#endif
      delay(500);
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
#ifdef SERIAL_DEBUG
    debug(F("connected with IP "));
    debugln(WiFi.localIP().toString());
#endif
  }
  else
  {
    debugln("");
  }

} // connectWiFi()

void myReboot()
{
  delay(1000);
  ESP.restart();
  //delay(50);
} //myReboot()

void loop()
{

  //
  // Reconnect WiFi if down
  //
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

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

  if (MQTTclient.connected())
  {
    MQTTclient.loop(); // If MQTT Client connected check for MQTT activity
  }

  httpServer.handleClient(); // Give time to Web Server

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

  yield();

} // loop()

void outputData()
{

#ifdef SERIAL_DEBUG
  debug(F("....Version:        "));
  debugln(version);
  debug(F("....deviceID:       "));
  debugln(deviceID);
  debug(F("....MQTT Server:    "));
  debugln(mqtt_server);
  debug(F("....MQTT Port:      "));
  debugln(String(mqtt_port));
  debug(F("....MQTT User:      "));
  debugln(mqtt_user);
  debug(F("....MQTT Password:  "));
  debugln(mqtt_password);
  debug(F("....MQTT inTopic1:  "));
  debugln(mqtt_inTopic1);
  debug(F("....MQTT outTopic1: "));
  debugln(mqtt_outTopic1);
  debug(F("....MQTT inTopic2:  "));
  debugln(mqtt_inTopic2);
  debug(F("....MQTT outTopic2: "));
  debugln(mqtt_outTopic2);
  debug(F("....MQTT inTopic3:  "));
  debugln(mqtt_inTopic3);
  debug(F("....MQTT outTopic3: "));
  debugln(mqtt_outTopic3);
  debug(F("....MQTT inTopic4:  "));
  debugln(mqtt_inTopic4);
  debug(F("....MQTT outTopic4  "));
  debugln(mqtt_outTopic4);
#endif
}

void debug(String message)
{
  Serial.print(message);
}

void debugln(String message)
{
  Serial.println(message);
}
