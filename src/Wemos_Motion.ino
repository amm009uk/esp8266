#ifdef MOTION_SENSOR
void checkMotionState(unsigned int PIN, char *topic, String &last_motion_state, String &motion_state)
{

  last_motion_state = motion_state; //get previous state of motion

  if (digitalRead(PIN) == HIGH)
  {
    motion_state = (char *)"OPEN";
  }
  else
  {
    motion_state = (char *)"CLOSED";
  }

  // If the state has changed only then publish the change
  if (last_motion_state != motion_state)
  {
    MQTTclient.publish(topic, &motion_state[0]);
#ifdef SERIAL_DEBUG
    debug(F("Motion state change: ")); debugln((char *)motion_state.c_str());
#endif
  }

  delay(10);

} // checkMotionState()
#endif
