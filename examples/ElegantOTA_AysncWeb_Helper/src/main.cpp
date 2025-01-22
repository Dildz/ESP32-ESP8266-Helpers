/***************************************************************************************************************
This is a customised usecase example of the ElegantOTA library for ESP32 & ESP8266 boards.

To incorpurate this in any other sketch:
 - include ElegantOTAHelper.h in the main sketch,
 - include the ESPWiFiHelper.h file and select the desired Wi-Fi operating mode (SoftAP or STA)
    and configure the network settings accoring to the selected mode & your preferences.

 - In Setup():
  > call setupWiFi() to setup Wi-Fi in either SoftAP or STA mode,
  > call setupOTA() to setup ElegantOTA & AsyncWebServer.
 - In Loop():
  > call the handleBuiltInLED() function if WiFi is configured for STA mode (indicates internet access),
  > call the ElegantOTA.loop() function in the loop() function to allow for reboots after updates.

To upload a new sketch (firmware) via WiFi:
 - build the sketch and find the firmware.bin file in the .pio/build/nodemcuv2 or /esp32dev folder,
 - open a web browser and navigate to http://[assigned.esp.ip.address]/update and upload the firmware.bin file.


**NOTE**
You will might find that the project does not compile if using an ESP8266 board.
To fix this:
 - navigate to the .pio/libdeps/esp8266 directory and find the ElegantOTA library src folder,
 - open ElegantOTA.h file and find the line: #define ELEGANTOTA_USE_ASYNC_WEBSERVER,
 - change the value from 0 to 1.

No change to the ElegantOTA.h file is needed if using an ESP32 board.
***************************************************************************************************************/

#include <Arduino.h>            // include if using PlatformIO
#include "ElegantOTAHelper.h"   // include the ElegantOTAHelper.h file
#include "ESPWiFiHelper.h"      // include a WiFiHelper file of choise


void setup() {
  Serial.begin(115200);
  Serial.println("\nRunning setup functions.\n");

  setupWiFi();    // set up Wi-Fi
  setupOTA();     // setup ElegantOTA & AsyncWebServer

  Serial.println("\nSetup completed.\n");
}


void loop() {
  handleBuiltInLED();   // handles LED blinking in STA mode if connected with no internet
  ElegantOTA.loop();    // handles rebooting after OTA update
}
