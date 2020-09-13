void connectWiFi()
{
  WiFi.disconnect();

#ifdef SERIAL_DEBUG
  debug(F("Connect WiFi: "));
  debug(ssid);
  debug(F("/"));
  debug(password);
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  for (int i = 0; i < 30; i++)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
#ifdef SERIAL_DEBUG
      debug(F("."));
#endif
      delay(500);
    }
  }

  if (WiFi.status() == WL_CONNECTED)
  {
#ifdef SERIAL_DEBUG
    debug(F("connected with IP "));
    debugln(WiFi.localIP().toString());
#endif
  }
  else
  {
    debugln("");
  }

} // connectWiFi()