boolean MQTTconnect()
{

#ifdef SERIAL_DEBUG
  debug(F("Connect MQTT: "));
  debug(mqtt_server);
  debug(F(":"));
  debug(String(mqtt_port));
  debug(F("::"));
  debug(mqtt_user);
  debug(F("/"));
  debug(mqtt_password);
  debug(F("..."));
#endif

  if (MQTTclient.connect(deviceID, mqtt_user, mqtt_password))
  {

    MQTTclient.subscribe(IP_REQUEST); // We need this regardless of the type of device

    //
    // Subscribe to relevent messages with first one required by ANY device
    //

#if defined SONOFF_BASIC || defined SONOFF_LIGHT
    MQTTclient.subscribe(mqtt_inTopic1);
#endif

#ifdef SONOFF_DUAL
    MQTTclient.subscribe(mqtt_inTopic1);
    MQTTclient.subscribe(mqtt_inTopic2);
#endif

#ifdef SONOFF_4CH
    MQTTclient.subscribe(mqtt_inTopic1);
    MQTTclient.subscribe(mqtt_inTopic2);
    MQTTclient.subscribe(mqtt_inTopic3);
    MQTTclient.subscribe(mqtt_inTopic4);
#endif

//
// Now publish state of device so openHAB stays in sync with current state
//
#if defined SONOFF_BASIC || defined SONOFF_LIGHT
    if (!digitalRead(RELAY_PIN))
    {
      MQTTclient.publish(mqtt_outTopic1, "OFF");
    }
    else
    {
      MQTTclient.publish(mqtt_outTopic1, "ON");
    }
#endif

#ifdef SONOFF_DUAL
    if (!digitalRead(RELAY01_PIN))
    {
      MQTTclient.publish(mqtt_outTopic1, "OFF");
    }
    else
    {
      MQTTclient.publish(mqtt_outTopic1, "ON");
    }

    if (!digitalRead(RELAY02_PIN))
    {
      MQTTclient.publish(mqtt_outTopic2, "OFF");
    }
    else
    {
      MQTTclient.publish(mqtt_outTopic2, "ON");
    }
#endif

#ifdef SONOFF_4CH

#endif

#ifdef SERIAL_DEBUG
    debugln(F("connected"));
#endif
  }
  else
  {
#ifdef SERIAL_DEBUG
    debugln(F("connection failed"));
#endif
  }

  delay(10);
  return MQTTclient.connected();

} // MQTTconnect()

void callback(char *topic, byte *payload, unsigned int length)
{

#ifdef SERIAL_DEBUG
  debug(F("In MQTT Callback() message arrived... "));
  debugln(topic);
#endif

  //
  // Process IP_REQUEST messages
  //
  if (strcmp(topic, IP_REQUEST) == 0)
  { // Check if message is for IP request

    String replyMessage = IP_REPLY;                                  // Build the MQTT reply messsage name
    replyMessage.concat(deviceID);                                   // ...

    String Msg = WiFi.localIP().toString();                          // Build MQTT message payload contents

#ifdef SERIAL_DEBUG
    debug(F("MQTT Publish "));
    debug(replyMessage.c_str());
    debug(F(" with payload "));
    debugln(Msg.c_str());
#endif

    MQTTclient.publish(replyMessage.c_str(), Msg.c_str());           // Publish message to Broker

    delay(10);
    return;
  }

  //
  // Process regular messages
  //

  //
  // Get message contents
  //
#ifdef SONOFF
  String msgContents;
  char receivedChar;

  if ((strcmp(topic, mqtt_inTopic1) == 0) || (strcmp(topic, mqtt_inTopic2) == 0) || (strcmp(topic, mqtt_inTopic3) == 0) || (strcmp(topic, mqtt_inTopic4) == 0))
    {
      for (int i = 0; (i < (int)length); i++)
      {
        receivedChar = (char)payload[i];
        msgContents.concat(receivedChar);
      }

#ifdef SERIAL_DEBUG
    debug(F("..Payload: "));
    debugln(msgContents.c_str());
#endif
  }

#if defined SONOFF_BASIC || defined SONOFF_LIGHT
  relayControl(msgContents);
  saveSonoffState();
#endif

#if defined SONOFF_DUAL
  relayControl(topic, msgContents);
#endif

#endif

  delay(10);
} // callback()