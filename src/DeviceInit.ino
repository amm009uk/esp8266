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

#ifdef WEMOS
#endif

} // deviceInit()
