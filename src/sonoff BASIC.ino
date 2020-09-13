#ifdef SONOFF_BASIC
void relayToggle()
{

	if (!digitalRead(RELAY_PIN))
		relayControl("ON");
	else
		relayControl("OFF");

	saveSonoffState();

	delay(10);
} // relayToggle()

void relayControl(String msg)
{

	if (msg == "ON")
	{
		digitalWrite(LED_PIN, LOW);
		digitalWrite(RELAY_PIN, HIGH);
		MQTTclient.publish(mqtt_outTopic1, "ON");
		state1 = 1;
	}
	else if (msg == "OFF")
	{
		digitalWrite(LED_PIN, HIGH);
		digitalWrite(RELAY_PIN, LOW);
		MQTTclient.publish(mqtt_outTopic1, "OFF");
		state1 = 0;
	}

	delay(10);
} // relayControl()
#endif
