void    myReboot();
void    connectWiFi();
void    debug(String message);
void    debugln(String message);

bool    loadSonoffConfig();
bool    saveSonoffConfig();
bool    loadSonoffState();
bool    saveSonoffState();

bool    loadWemosConfig();
bool    saveWemosConfig();

void    handleNotFound();
void    handleRoot();
void    handlesaveChanges();
void    handleReset();

void    deviceInit();

boolean MQTTconnect();//
void    callback(char* topic, byte* payload, unsigned int length);

#ifdef SONOFF_BASIC
void    relayToggle();
void    relayControl(String msg);
#endif

#ifdef SONOFF_LIGHT
void    relayToggle();
void    relayControl(String msg);
#endif

#ifdef SONOFF_DUAL
void    relayControl(String msg);
void    relayControl(char* topic, String msgContents);
#endif

#ifdef SONOFF_4CH
void    relayControl();
#endif

#ifdef TEMP_SENSOR
void    getDHTdata();
#endif