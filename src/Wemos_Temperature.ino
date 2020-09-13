#ifdef TEMP_SENSOR
void getDHTdata() {

  now = millis();
  if ( now - lastMsg <= (tempPolling * 1000) ) {                     // Only get DHT data according to polling interval
    return;
  }
  lastMsg = now;

  temp_c  = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temp_c) || isnan(humidity)) {                            // Check if any DHT read failed and exit early to try again
		#ifdef SERIAL_DEBUG
    debugln(F("Failed to read data from DHT sensor!"));
		#endif
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(temp_c, humidity, false);
  temp_state = (char*)String(hic).c_str();                           // Float --> String

  #ifdef SERIAL_DEBUG
  debug(F("Humidity: "));          debug(String(humidity).c_str());
  debug(F("\tTemperature: "));     debug(String(temp_c).c_str()); debug("*C");
  debug(F(" ---> Heat index: "));  debugln(String(hic).c_str());
	#endif

  MQTTclient.publish(mqtt_tempTopic, String(hic).c_str());

  delay(10);

} // getDHTdata()
#endif