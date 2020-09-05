#ifdef SONOFF_LIGHT
void relayToggle()
{

	if (!digitalRead(RELAY_PIN))
	{
		relayControl("ON");
	}
	else
	{
		relayControl("OFF");
	}

} // relayToggle()

void relayControl(String msg)
{

	if (msg == "ON")
	{
		digitalWrite(LED_PIN, HIGH);
		digitalWrite(RELAY_PIN, HIGH);
		digitalWrite(TOUCH_PIN, LOW);
		MQTTclient.publish(mqtt_outTopic1, "ON");
		state1 = 1;
		saveState();
	}
	else if (msg == "OFF")
	{
		digitalWrite(LED_PIN, LOW);
		digitalWrite(RELAY_PIN, LOW);
		digitalWrite(TOUCH_PIN, HIGH);
		MQTTclient.publish(mqtt_outTopic1, "OFF");
		state1 = 0;
		saveState();
	}

} // relayControl()
#endif
