#ifdef REED_SENSOR1
void checkReedState(unsigned int PIN, char *topic, String &last_reed_state, String &reed_state)
{

  last_reed_state = reed_state; // Get previous state of door

  if (digitalRead(PIN) == HIGH)
  {
    reed_state = "CLOSED";
  }
  else
  {
    reed_state = "OPEN";
  }

  // If the state has changed then publish the change otherwise no need
  if (last_reed_state != reed_state)
  {
    MQTTclient.publish(topic, &reed_state[0]);
#ifdef SERIAL_DEBUG
    debug(F("Reed state change: "));
    debug(topic);
    debug(F("::"));
    debugln(reed_state.c_str());
#endif
  }

  delay(10);

} // checkReedState()
#endif