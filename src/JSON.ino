bool loadConfig()
{ // Load JSON config file from LittleFS

  File configFile = LittleFS.open(CONFIGFILE, "r");
  if (!configFile)
  {
#ifdef SERIAL_DEBUG
    debugln(F("ERROR: No config file found!!!"));
#endif

    delay(10);
    return (true);
  }

  // Calculate buffer size to hold file contents
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(12) + 420;
  // Allocate buffer to store contents of file
  std::unique_ptr<char[]> buf(new char[bufferSize]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), bufferSize);

  DynamicJsonDocument doc(bufferSize);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error)
  {
    debug(F("ERROR: Parsing JSON config file: "));
    debugln(error.c_str());
    configFile.close();
    return false;
  }

  const char *t_deviceID = doc["deviceID"];
  strcpy(deviceID, t_deviceID);

  JsonObject MQTT = doc["MQTT"];

  const char *t_MQTT_server = MQTT["Server"];
  strcpy(mqtt_server, t_MQTT_server);
  mqtt_port = MQTT["Port"];
  const char *t_MQTT_user = MQTT["User"];
  strcpy(mqtt_user, t_MQTT_user);
  const char *t_MQTT_password = MQTT["Password"];
  strcpy(mqtt_password, t_MQTT_password);
  const char *t_MQTT_inTopic1 = MQTT["inTopic1"];
  strcpy(mqtt_inTopic1, t_MQTT_inTopic1);
  const char *t_MQTT_outTopic1 = MQTT["outTopic1"];
  strcpy(mqtt_outTopic1, t_MQTT_outTopic1);
  const char *t_MQTT_inTopic2 = MQTT["inTopic2"];
  strcpy(mqtt_inTopic2, t_MQTT_inTopic2);
  const char *t_MQTT_outTopic2 = MQTT["outTopic2"];
  strcpy(mqtt_outTopic2, t_MQTT_outTopic2);
  const char *t_MQTT_inTopic3 = MQTT["inTopic3"];
  strcpy(mqtt_inTopic3, t_MQTT_inTopic3);
  const char *t_MQTT_outTopic3 = MQTT["outTopic3"];
  strcpy(mqtt_outTopic3, t_MQTT_outTopic3);
  const char *t_MQTT_inTopic4 = MQTT["inTopic4"];
  strcpy(mqtt_inTopic4, t_MQTT_inTopic4);
  const char *t_MQTT_outTopic4 = MQTT["outTopic4"];
  strcpy(mqtt_outTopic4, t_MQTT_outTopic4);

  configFile.close();

#ifdef SERIAL_DEBUG
  debugln(F("Config file loaded"));
#endif

  delay(10);
  return (true);

} // loadConfig()

bool saveConfig()
{ // Save JSON config file to LittleFS

  LittleFS.remove(CONFIGFILE);

  File configFile = LittleFS.open(CONFIGFILE, "w");
  if (!configFile)
  {
#ifdef SERIAL_DEBUG
    debugln(F("Failed to open config file for writing"));
#endif
    return (false);
  }

#ifdef SERIAL_DEBUG
  outputData();
#endif

  const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(12) + 420;
  DynamicJsonDocument doc(capacity);
  //   DynamicJsonDocument doc(640);

  doc["deviceID"] = deviceID;

  JsonObject MQTT = doc.createNestedObject("MQTT");

  MQTT["Server"] = mqtt_server;
  MQTT["Port"] = mqtt_port;
  MQTT["User"] = mqtt_user;
  MQTT["Password"] = mqtt_password;
  MQTT["inTopic1"] = mqtt_inTopic1;
  MQTT["outTopic1"] = mqtt_outTopic1;
  MQTT["inTopic2"] = mqtt_inTopic2;
  MQTT["outTopic2"] = mqtt_outTopic2;
  MQTT["inTopic3"] = mqtt_inTopic3;
  MQTT["outTopic3"] = mqtt_outTopic3;
  MQTT["inTopic4"] = mqtt_inTopic4;
  MQTT["outTopic4"] = mqtt_outTopic4;

  if (serializeJson(doc, configFile) == 0)
  {
#ifdef SERIAL_DEBUG
    debugln(F("Failed to write config file"));
    delay(10);
#endif
    return (false);
  }

  configFile.close();

#ifdef SERIAL_DEBUG
  debugln(F("Config changes saved"));
#endif

  delay(10);
  return (true);

} // saveConfig()

bool loadState()
{ // Load JSON state file from LittleFS

  File stateFile = LittleFS.open(STATEFILE, "r");
  if (!stateFile)
  {
#ifdef SERIAL_DEBUG
    debugln(F("No state file found will use default values (zeroes)"));
#endif
    return (true);
  }

  // Calculate buffer size to hold file contents
  const size_t bufferSize = JSON_OBJECT_SIZE(4) + 40;
  // Allocate buffer to store contents of file
  std::unique_ptr<char[]> buf(new char[bufferSize]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  stateFile.readBytes(buf.get(), bufferSize);

  DynamicJsonDocument doc(bufferSize);
  DeserializationError error = deserializeJson(doc, buf.get());
  if (error)
  {
    debug(F("ERROR: Parsing JSON state file: "));
    debugln(error.c_str());
    stateFile.close();
    return false;
  }

  state1 = doc["state1"];
  ;
  state2 = doc["state2"];
  ;
  state3 = doc["state3"];
  ;
  state4 = doc["state4"];
  ;

  stateFile.close();

#ifdef SERIAL_DEBUG
  debugln(F("State file loaded"));
#endif

  delay(10);
  return (true);

} // loadState()

bool saveState()
{ // Save JSON state file to LittleFS

  LittleFS.remove(STATEFILE);

  File stateFile = LittleFS.open(STATEFILE, "w");
  if (!stateFile)
  {
#ifdef SERIAL_DEBUG
    debugln(F("Failed to open state file for writing"));
#endif
    return (false);
  }

#ifdef SERIAL_DEBUG
  debugln(F("..Contents to save"));
  debug(F("....State1: "));
  debugln(String(state1));
  debug(F("....State2: "));
  debugln(String(state2));
  debug(F("....State3: "));
  debugln(String(state3));
  debug(F("....State4: "));
  debugln(String(state4));
#endif

  const size_t capacity = JSON_OBJECT_SIZE(4) + 40;
  DynamicJsonDocument doc(capacity);
  //   DynamicJsonDocument doc(512);

  doc["state1"] = state1;
  doc["state2"] = state2;
  doc["state3"] = state3;
  doc["state4"] = state4;

  if (serializeJson(doc, stateFile) == 0)
  {
#ifdef SERIAL_DEBUG
    debugln(F("Failed to write state file"));
    delay(10);
#endif
    return (false);
  }

  stateFile.close();

#ifdef SERIAL_DEBUG
  debugln(F("State changes saved"));
#endif

  delay(10);
  return (true);

} // saveConfig()