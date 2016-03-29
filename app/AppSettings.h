/*
 * AppSettings.h
 *
 *  Created on: 13 ��� 2015 �.
 *      Author: Anakod
 */

#ifndef INCLUDE_APPSETTINGS_H_
#define INCLUDE_APPSETTINGS_H_

#include <SmingCore/SmingCore.h>
#include "globals.h"

#define APP_SETTINGS_FILE ".settings.conf" // leading point for security reasons :)

typedef enum
{
    apModeAlwaysOn = 1,
    apModeAlwaysOff,
    apModeWhenDisconnected
} eAppMode;

class ApplicationSettingsStorage
{
 public:
  bool               wired = false;
  String             ssid;
  String             password;
  eAppMode           apMode = apModeAlwaysOn;
  String             apPassword;

  String             portalUrl;
  String             portalData;

  bool               dhcp = true;

  IPAddress          ip;
  IPAddress          netmask;
  IPAddress          gateway;

  String             mqttUser;
  String             mqttPass;
  String             mqttServer;
  int                mqttPort = 1883;
  String             mqttClientId;
  String             mqttEvtPfx;
  String             mqttCmdPfx;

  bool               cpuBoost = true;
  bool               useOwnBaseAddress = true;

  String             cloudDeviceToken;
  String             cloudLogin;
  String             cloudPassword;

  String             webOtaBaseUrl;

  int                gpiodMode = 0;

  void               load();
  void               save();
  bool               exist() { return fileExist(APP_SETTINGS_FILE); }
  };

extern ApplicationSettingsStorage AppSettings;

#endif /* INCLUDE_APPSETTINGS_H_ */
