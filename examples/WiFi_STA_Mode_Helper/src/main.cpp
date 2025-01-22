/****************************************************************************************
* This is a modified version of the ESP32/8266 WiFi Station Mode example project.
* To implement this into a project, include the ESPWiFiSTAHelper.h file
*  and call the setupWiFi() function.
*
* The helper defaults to using DHCP for IP configuration.
* If you need to use a static IP address:
* - find the line 'bool USE_STATIC_IP' in the helper file,
* - set USE_STATIC_IP to true,
* - configure the IP space to your needs.
* 
* LED blink states:
* - fast  > attempting to connect
* - slow  > connected, no internet
* - solid > connected, internet access
****************************************************************************************/

#include <Arduino.h>
#include "ESPWiFiSTAHelper.h"  // include the STA Mode WiFi helper file


void setup() {
  Serial.begin(115200);
  Serial.println("\nRunning setup functions.\n");

  setupWiFi();    // set up Wi-Fi

  Serial.println("\nSetup completed.\n");
}


void loop() {
  handleBuiltInLED();  // handle LED blinking based on internet access
}
