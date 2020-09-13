
void myReboot()
{
  delay(1000);
  ESP.restart();
  //delay(50);
} //myReboot()



void outputData()
{

#ifdef SERIAL_DEBUG
  debug(F("....Version:        "));  debugln(version);
  debug(F("....deviceID:       "));  debugln(deviceID);
  debug(F("....MQTT Server:    "));  debugln(mqtt_server);
  debug(F("....MQTT Port:      "));  debugln(String(mqtt_port));
  debug(F("....MQTT User:      "));  debugln(mqtt_user);
  debug(F("....MQTT Password:  "));  debugln(mqtt_password);
#endif

#if defined SERIAL_DEBUG && defined SONOFF
  debug(F("....MQTT inTopic1:  "));  debugln(mqtt_inTopic1);
  debug(F("....MQTT outTopic1: "));  debugln(mqtt_outTopic1);
  debug(F("....MQTT inTopic2:  "));  debugln(mqtt_inTopic2);
  debug(F("....MQTT outTopic2: "));  debugln(mqtt_outTopic2);
  debug(F("....MQTT inTopic3:  "));  debugln(mqtt_inTopic3);
  debug(F("....MQTT outTopic3: "));  debugln(mqtt_outTopic3);
  debug(F("....MQTT inTopic4:  "));  debugln(mqtt_inTopic4);
  debug(F("....MQTT outTopic4  "));  debugln(mqtt_outTopic4);
#endif

#if defined SERIAL_DEBUG && defined WEMOS
  debug(F("....MQTT Heat:      "));  debugln(mqtt_tempTopic);
  debug(F("....MQTT Motion:    "));  debugln(mqtt_motionTopic);
  debug(F("....MQTT SecurityA: "));  debugln(mqtt_reedTopic1);
  debug(F("....MQTT SecurityB: "));  debugln(mqtt_reedTopic2);
  debug(F("....MQTT SecurityC: "));  debugln(mqtt_reedTopic3);
  debug(F("....MQTT SecurityD: "));  debugln(mqtt_reedTopic4);
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
