/****************************************************************************************
* This is a modified version of the ESP32/8266 WiFi SoftAP Mode example project.
* To implement this into a project, include the ESPWiFiSoftAPHelper.h file
*  and call the setupWiFi() function.
*
* Modify the SSID, password, IP address, and subnet mask as needed.
* 
* Built-In LED states:
* - off  > attempting to setup AP
* - solid > AP ready for clients
****************************************************************************************/

#include <Arduino.h>
#include "ESPWiFiSoftAPHelper.h"  // include the STA Mode WiFi helper file


void setup() {
  Serial.begin(115200);
  Serial.println("\nRunning setup functions.\n");

  setupWiFi();    // set up Wi-Fi

  Serial.println("\nSetup completed.\n");
}


void loop() {
  whosConnected();  // handle who's connected to the AP check
}
