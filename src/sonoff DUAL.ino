#ifdef SONOFF_DUAL
void relayControl(char *topic, String msgContents)
{

	if ((strcmp(topic, mqtt_inTopic1) == 0))
	{
		if (msgContents == "ON")
		{
			digitalWrite(RELAY01_PIN, HIGH);
			MQTTclient.publish(mqtt_outTopic1, "ON");
			state1 = 1;
			saveState();
		}
		else if (msgContents == "OFF")
		{
			digitalWrite(RELAY01_PIN, LOW);
			MQTTclient.publish(mqtt_outTopic1, "OFF");
			state1 = 0;
			saveState();
		}
	}

	if ((strcmp(topic, mqtt_inTopic2) == 0))
	{
		if (msgContents == "ON")
		{
			digitalWrite(RELAY02_PIN, HIGH);
			digitalWrite(BLUE_LED_PIN, LOW);
			MQTTclient.publish(mqtt_outTopic1, "ON");
			state2 = 1;
			saveState();
		}
		else if (msgContents == "OFF")
		{
			digitalWrite(RELAY02_PIN, LOW);
			digitalWrite(BLUE_LED_PIN, HIGH);
			MQTTclient.publish(mqtt_outTopic2, "OFF");
			state2 = 0;
			saveState();
		}
	}

} // relayControl()
#endif
