
#include <user_config.h>
#include <SmingCore.h>
#include <SmingCore/Network/TelnetServer.h>
#include <AppSettings.h>
#include <globals.h>
#include <Network.h>
//#include <MyGateway.h>
#include <HTTP.h>
#include <controller.h>

FTPServer      ftp;
TelnetServer   telnet;
static boolean first_time = TRUE;
int            isNetworkConnected = FALSE;
//char           convBuf[MAX_PAYLOAD*2+1];

/*
void incomingMessage(const MyMessage &message)
{
    // Pass along the message from sensors to serial line
    Debug.printf("APP RX %d;%d;%d;%d;%d;%s\n",
                  message.sender, message.sensor,
                  mGetCommand(message), mGetAck(message),
                  message.type, message.getString(convBuf));

    if (GW.getSensorTypeString(message.type) == "VAR2") 
    {
        Debug.printf("received pong\n");
    }

    if (mGetCommand(message) == C_SET)
    {
        String type = GW.getSensorTypeString(message.type);
        String topic = message.sender + String("/") +
                       message.sensor + String("/") +
                       "V_" + type;
        controller.notifyChange(topic, message.getString(convBuf));
    }

    return;
}
*/
void startFTP()
{
  if (!fileExist("index.html"))
      fileSetContent("index.html", "<h3>Please connect to FTP and upload files from folder 'web/build' (details in code)</h3>");

  // Start FTP server
  ftp.listen(21);
  ftp.addUser("admin", "12345678"); // FTP account
  } //

int updateSensorStateInt(int node, int sensor, int type, int value)
{
//MyMessage myMsg;
//myMsg.set(value);
//GW.sendRoute(GW.build(myMsg, node, sensor, C_SET, type, 0));
//rfPacketsTx++;
  } // 

int updateSensorState(int node, int sensor, int value)
{
  updateSensorStateInt(node, sensor, 2 , value);
  } //

Timer heapCheckTimer;
void heapCheckUsage()
{
  controller.notifyChange("memory", String(system_get_free_heap_size()));    
  } //

//void i2cChangeHandler(String object, String value)
//{
//  controller.notifyChange(object, value);
//  } //

// Will be called when system initialization was completed
void startServers()
{
  HTTP.begin(); //HTTP must be first so handlers can be registered
  heapCheckTimer.initializeMs(60000, heapCheckUsage).start(true);
  startFTP();
  telnet.listen(23);
  controller.begin(); // gpiod
  } //

// Will be called when WiFi station was connected to AP
void wifiCb(bool connected)
{
  isNetworkConnected = connected;
  if (connected) {
    Debug.println("--> I'm CONNECTED");
    if (first_time) 
      first_time = FALSE;
    }
  } //

void processInfoCommand(String commandLine, CommandOutput* out)
{
//  uint64_t rfBaseAddress = GW.getBaseAddress();

    out->printf("System information : ESP8266 based MySensors gateway\r\n");
    out->printf("Build time         : %s\n", build_time);
    out->printf("Version            : %s\n", build_git_sha);
    out->printf("Sming Version      : %s\n", SMING_VERSION);
    out->printf("ESP SDK version    : %s\n", system_get_sdk_version());
//    out->printf("MySensors version  : %s\n", GW.version());
    out->printf("\r\n");

    out->printf("Station SSID       : %s\n", AppSettings.ssid.c_str());
    out->printf("Station DHCP       : %s\n", AppSettings.dhcp ? "TRUE" : "FALSE");
    out->printf("Station IP         : %s\n", Network.getClientIP().toString().c_str());
    out->printf("\r\n");

    String apModeStr;
    if (AppSettings.apMode == apModeAlwaysOn)
        apModeStr = "always";
    else if (AppSettings.apMode == apModeAlwaysOff)
        apModeStr = "never";
    else
        apModeStr= "whenDisconnected";

    out->printf("Access Point Mode  : %s\n", apModeStr.c_str());
    out->printf("\r\n");

    out->printf("System Time        : ");
    out->printf(SystemClock.getSystemTimeString().c_str());
    out->printf("\r\n");

    out->printf("Free Heap          : %d\r\n", system_get_free_heap_size());
    out->printf("CPU Frequency      : %d MHz\r\n", system_get_cpu_freq());
    out->printf("System Chip ID     : %x\r\n", system_get_chip_id());
    out->printf("SPI Flash ID       : %x\r\n", spi_flash_get_id());
    out->printf("SPI Flash Size     : %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
    out->printf("\r\n");

//  out->printf("RF base address    : %02x", (rfBaseAddress >> 32) & 0xff);
//  out->printf("%08x\r\n", rfBaseAddress);
//  out->printf("\r\n");
  }

void processRestartCommand(String commandLine, CommandOutput* out)
{
  System.restart();
  }

void processRestartCommandWeb(void)
{
  System.restart();
  }

void processDebugCommand(String commandLine, CommandOutput* out)
{
  Vector<String> commandToken;
  int numToken = splitString(commandLine, ' ' , commandToken);

  if ((numToken != 2) || (commandToken[1] != "on" && commandToken[1] != "off")) {
    out->printf("usage : \r\n\r\n");
    out->printf("debug on  : Enable debug\r\n");
    out->printf("debug off : Disable debug\r\n");
    return;
    }

  if (commandToken[1] == "on")
    Debug.start();
  else
    Debug.stop();
  } // 

void processCpuCommand(String commandLine, CommandOutput* out)
{
    Vector<String> commandToken;
    int numToken = splitString(commandLine, ' ' , commandToken);

    if (numToken != 2 ||
        (commandToken[1] != "80" && commandToken[1] != "160"))
    {
        out->printf("usage : \r\n\r\n");
        out->printf("cpu 80  : Run at 80MHz\r\n");
        out->printf("cpu 160 : Run at 160MHz\r\n");
        return;
    }

    if (commandToken[1] == "80")
    {
        System.setCpuFrequency(eCF_80MHz);
        AppSettings.cpuBoost = false;
    }
    else
    {
        System.setCpuFrequency(eCF_160MHz);
        AppSettings.cpuBoost = true;
    }

    AppSettings.save();
}

void processBaseAddressCommand(String commandLine, CommandOutput* out)
{
    Vector<String> commandToken;
    int numToken = splitString(commandLine, ' ' , commandToken);

    if (numToken != 2 ||
        (commandToken[1] != "default" && commandToken[1] != "private"))
    {
        out->printf("usage : \r\n\r\n");
        out->printf("base-address default : Use the default MySensors base address\r\n");
        out->printf("base-address private : Use a base address based on ESP chip ID\r\n");
        return;
    }

    if (commandToken[1] == "default")
    {
        AppSettings.useOwnBaseAddress = false;
    }
    else
    {
        AppSettings.useOwnBaseAddress = true;
    }

    AppSettings.save();
    System.restart();
}

void processShowConfigCommand(String commandLine, CommandOutput* out)
{
  out->println(fileGetContent(".settings.conf"));
  }

void processAPModeCommand(String commandLine, CommandOutput* out)
{
  Vector<String> commandToken;
  int numToken = splitString(commandLine, ' ' , commandToken);

    if (numToken != 2 ||
        (commandToken[1] != "always" && commandToken[1] != "never" &&
         commandToken[1] != "whenDisconnected"))
    {
      out->printf("usage : \r\n\r\n");
      out->printf("apMode always           : Always have the AP enabled\r\n");
      out->printf("apMode never            : Never have the AP enabled\r\n");
      out->printf("apMode whenDisconnected : Only enable the AP when discnnected\r\n");
      out->printf("                          from the network\r\n");
      return;
    }

    if (commandToken[1] == "always")
    {
      AppSettings.apMode = apModeAlwaysOn;
    }
    else if (commandToken[1] == "never")
    {
      AppSettings.apMode = apModeAlwaysOff;
    }        
    else
    {
      AppSettings.apMode = apModeWhenDisconnected;
    }

  AppSettings.save();
  System.restart();
  }

void init()
{
  /* Make sure wifi does not start yet! */
  wifi_station_set_auto_connect(0);

  pinMode(OUTPUT0_PIN, OUTPUT);
  digitalWrite(OUTPUT0_PIN, HIGH);
  pinMode(OUTPUT1_PIN, OUTPUT);
  digitalWrite(OUTPUT1_PIN, HIGH);
  pinMode(INPUT0_PIN, INPUT);
  pinMode(INPUT1_PIN, INPUT);

    /* Make sure all chip enable pins are HIGH */
/*
#ifdef RADIO_SPI_SS_PIN
    pinMode(RADIO_SPI_SS_PIN, OUTPUT);
    digitalWrite(RADIO_SPI_SS_PIN, HIGH);
#endif
#ifdef SD_SPI_SS_PIN
    pinMode(SD_SPI_SS_PIN, OUTPUT);
    digitalWrite(SD_SPI_SS_PIN, HIGH);
#endif
#ifdef ETHERNET_SPI_SS_PIN
    pinMode(ETHERNET_SPI_SS_PIN, OUTPUT);
    digitalWrite(ETHERNET_SPI_SS_PIN, HIGH);
#endif
*/
  /* Mount the internal storage */
  int slot = rboot_get_current_rom();
  if (slot == 0) {
    Debug.printf("trying to mount spiffs at %x, length %d\n", 0x40300000, SPIFF_SIZE);
    spiffs_mount_manual(0x40300000, SPIFF_SIZE);
    }
  else {
    Debug.printf("trying to mount spiffs at %x, length %d\n", 0x40500000, SPIFF_SIZE);
    spiffs_mount_manual(0x40500000, SPIFF_SIZE);
    }

  Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
  Serial.systemDebugOutput(true); // Enable debug output to serial
  Serial.commandProcessing(true);
  Debug.start();

  // set prompt
  commandHandler.setCommandPrompt("WifiEbsEbr> ");

  // add new commands
  commandHandler.registerCommand(CommandDelegate("info",
                                                 "Show system information",
                                                 "System",
                                                 processInfoCommand));
  commandHandler.registerCommand(CommandDelegate("debug",
                                                 "Enable or disable debugging",
                                                 "System",
                                                 processDebugCommand));
  commandHandler.registerCommand(CommandDelegate("restart",
                                                 "Restart the system",
                                                 "System",
                                                 processRestartCommand));
  commandHandler.registerCommand(CommandDelegate("cpu",
                                                 "Adjust CPU speed",
                                                 "System",
                                                 processCpuCommand));
  commandHandler.registerCommand(CommandDelegate("apMode",
                                                 "Adjust the AccessPoint Mode",
                                                 "System",
                                                 processAPModeCommand));
  commandHandler.registerCommand(CommandDelegate("showConfig",
                                                 "Show the current configuration",
                                                 "System",
                                                 processShowConfigCommand));
  commandHandler.registerCommand(CommandDelegate("base-address",
                                                 "Set the base address to use",
                                                 "MySensors",
                                                 processBaseAddressCommand));
  AppSettings.load();

  // Start either wired or wireless networking
  Network.begin(wifiCb);

  // CPU boost
  if (AppSettings.cpuBoost)
    System.setCpuFrequency(eCF_160MHz);
  else
    System.setCpuFrequency(eCF_80MHz);

  // Run WEB server on system ready
  System.onReady(startServers);
  } // 
