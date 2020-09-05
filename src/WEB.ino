void handleNotFound()
{

#ifdef SERIAL_DEBUG
  debugln(F("Running handleNotFound()"));
#endif
  httpServer.send(404, "text/plain", "404: Not found");

} // handleNotFound()

void handleRoot()
{ // Send the main page

#ifdef SERIAL_DEBUG
  debugln(F("Running handleRoot()"));
#endif

  if (!httpServer.authenticate(WWW_USER, WWW_PASSWD))
  {                                            // Check user + password
    return httpServer.requestAuthentication(); // Request user + password
  }

#ifdef SERIAL_DEBUG
  outputData();
#endif

  String page; // Store web page

  // Open HTML file
  File webFile = LittleFS.open("mainPage.html", "r");
  if (!webFile)
  {
#ifdef SERIAL_DEBUG
    debugln(F("ERROR: No web file found!!!"));
#endif

    delay(10);
  }

  // Read HTML file
  while (webFile.available())
  {
    page += char(webFile.read());
  }

  webFile.close();

  //
  // Update web Page with data
  //
  if (MQTTclient.connected())
    page.replace("{{_MQTT}}", "Yes");
  else
    page.replace("{{_MQTT}}", "No");

  page.replace("{{_version}}", version);
  page.replace("{{_deviceID}}", deviceID);
  page.replace("{{_mqtt_server}}", mqtt_server);
  page.replace("{{_mqtt_port}}", String(mqtt_port));
  page.replace("{{_mqtt_user}}", mqtt_user);
  page.replace("{{_mqtt_password}}", mqtt_password);
  page.replace("{{_mqtt_inTopic1}}", mqtt_inTopic1);
  page.replace("{{_mqtt_outTopic1}}", mqtt_outTopic1);
  page.replace("{{_mqtt_inTopic2}}", mqtt_inTopic2);
  page.replace("{{_mqtt_outTopic2}}", mqtt_outTopic2);
  page.replace("{{_mqtt_inTopic3}}", mqtt_inTopic3);
  page.replace("{{_mqtt_outTopic3}}", mqtt_outTopic3);
  page.replace("{{_mqtt_inTopic4}}", mqtt_inTopic4);
  page.replace("{{_mqtt_outTopic4}}", mqtt_outTopic4);

  httpServer.send(200, "text/html", page);

  delay(10);

#ifdef SERIAL_DEBUG
  debugln(F("HTML loaded"));
#endif

} // handleRoot()

void handlesaveChanges()
{

#ifdef SERIAL_DEBUG
  debugln(F("Saving changes..."));
#endif

  if (httpServer.args() != 14)
  {
#ifdef SERIAL_DEBUG
    debug(F("Wrong number of args received from HTTP POST: "));
    debugln(String(httpServer.args()));
    for (uint8_t i = 0; i < httpServer.args(); i++)
    {
      debug((String)i);
      debug("  ");
      debugln(httpServer.arg(i).c_str());
    }
#endif
    return;
  }

  strcpy(deviceID, httpServer.arg(0).c_str());
  strcpy(mqtt_server, httpServer.arg(1).c_str());
  mqtt_port = httpServer.arg(2).toInt();
  strcpy(mqtt_user, httpServer.arg(3).c_str());
  strcpy(mqtt_password, httpServer.arg(4).c_str());
  strcpy(mqtt_inTopic1, httpServer.arg(5).c_str());
  strcpy(mqtt_outTopic1, httpServer.arg(6).c_str());
  strcpy(mqtt_inTopic2, httpServer.arg(7).c_str());
  strcpy(mqtt_outTopic2, httpServer.arg(8).c_str());
  strcpy(mqtt_inTopic3, httpServer.arg(9).c_str());
  strcpy(mqtt_outTopic3, httpServer.arg(10).c_str());
  strcpy(mqtt_inTopic4, httpServer.arg(11).c_str());
  strcpy(mqtt_outTopic4, httpServer.arg(12).c_str());

  saveConfig();

  httpServer.send(200, "text/html", "<META http-equiv=\"refresh\" content=\"15;URL=/\">Changes saved! Rebooting please wait this page will refresh...");

  myReboot();

} // handlesaveChanges()

void handleReset()
{ // Reset device

  httpServer.send(200, "text/html", "<META http-equiv=\"refresh\" content=\"15;URL=/\">Device has been wiped! Rebooting please wait this page will refresh...");
  delay(10);

  if (LittleFS.format())
  {
    delay(10);
#ifdef SERIAL_DEBUG
    debugln(F("File System Formated"));
#endif
  }
  else
  {
#ifdef SERIAL_DEBUG
    debugln(F("File System Formatting Error"));
#endif
  }

  myReboot();

} // handleReset()
