void deviceInit()
{ // Initialise device

#ifdef SONOFF_BASIC
  // Initialise pins
  pinMode(LED_PIN, OUTPUT);   // LED
  pinMode(RELAY_PIN, OUTPUT); // Relay for power
  pinMode(TOGGLE_PIN, INPUT); // Onboard momentary switch
  pinMode(EXT_PIN, OUTPUT);   // Local switch (input and output)

  digitalWrite(EXT_PIN, HIGH); // Not sure why I have to do this but it won't work otherwise!

  if (state1 == 1)
  {
    relayControl("ON");
  }
  else
  {
    relayControl("OFF");
  }
#endif

#ifdef SONOFF_LIGHT           // Initialise pins
  pinMode(LED_PIN, OUTPUT);   // LED
  pinMode(RELAY_PIN, OUTPUT); // Relay for power
  pinMode(TOUCH_PIN, INPUT);  // Onboard momentary switch

  // Turn off power initially and initialise button
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(TOUCH_PIN, HIGH);
#endif

#ifdef SONOFF_DUAL // Initialise pins
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RELAY01_PIN, OUTPUT);
  pinMode(RELAY02_PIN, OUTPUT);
#endif

#ifdef SONOFF_4CH
#endif

  // Initialise PIN's
#ifdef WEMOS
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
#endif

#ifdef TEMP_SENSOR
  pinMode(DHT_PIN, INPUT);
  debugln(F("Temp Sensor started"));
  dht.begin(); // Start the DHT sensor
#endif

#ifdef MOTION_SENSOR
  pinMode(MOTION_PIN, INPUT);
#endif

#ifdef REED_SENSOR1
  pinMode(REED_PIN1, INPUT);
#endif

#ifdef REED_SENSOR2
  pinMode(REED_PIN2, INPUT);
#endif

#ifdef REED_SENSOR3
  pinMode(REED_PIN3, INPUT);
#endif

#ifdef REED_SENSOR4
  pinMode(REED_PIN4, INPUT);
#endif

  delay(10);

} // deviceInit()
