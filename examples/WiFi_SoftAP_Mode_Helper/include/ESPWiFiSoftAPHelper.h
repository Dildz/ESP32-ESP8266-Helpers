/****************************************************************************************
* ESP SoftAP Helper
* This helper file consolidates the following functions:
* 1. Set up Wi-Fi Soft Access Point (AP) with custom settings,
* 2. Configure AP IP address and subnet mask,
* 3. Print number of connected devices every 10 seconds.
*
* To use this helper:
* - Include this file in your project,
* - Modify the SSID info, password, and AP IP configuration as needed,
* - In main setup() > call setupSoftAP() function,
* - In main loop() > call whosConnected() function.
****************************************************************************************/

#ifndef ESPWiFiSoftAPHelper_h
#define ESPWiFiSoftAPHelper_h

#ifdef ESP32    // for ESP32 boards
#include <WiFi.h>

#elif defined(ESP8266)    // for ESP8266 boards
#include <ESP8266WiFi.h>
#endif


// Configuration for SoftAP
const char* ssid = "ESP8266";       // Wi-Fi AP network name
const char* password = "12345678";  // Wi-Fi AP password (minimum 8 characters)

IPAddress IP(192, 168, 10, 1);      // Desired AP IP address
IPAddress subnet(255, 255, 255, 0); // Subnet mask

long lastCheckMS = 0;   // variable to track the last check connected devices time
bool isActive = false;  // Wi-Fi AP status


void setupWiFi() {
  // Start configuring the SoftAP
  Serial.println("Configuring Wi-Fi SoftAP...");

  if (!WiFi.softAPConfig(IP, IP, subnet)) {   // device IP | gateway IP | subnet mask
    Serial.println("Failed to configure network! Check your IP configuration.");
    return;
  }

  // Start the SoftAP with the provided credentials
  if (WiFi.softAP(ssid, password)) {
    Serial.println("SoftAP configured successfully!");
    Serial.print("\nNetwork Name: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());

    digitalWrite(LED_BUILTIN, LOW);     // turn on the LED (active LOW)
    isActive = true;                    // update the AP status
  } else {
    Serial.println("Failed to start SoftAP! Check your setup.");
  }
}


// Function to handle connected devices printing
void whosConnected() {
  const long CHECK_PERIOD = 10000;  // ms for checking connected devices period
  static long lastCheckMS = 0;      // static to retain value between calls
  long currentMS = millis();        // get the current time

  if (isActive) {
    if (currentMS - lastCheckMS >= CHECK_PERIOD) {
      int numStations = WiFi.softAPgetStationNum();  // get the number of connected stations
      Serial.printf("Number of connected devices = %d\n", numStations);

#ifdef ESP32    // for ESP32 boards
      wifi_sta_list_t stationList;
      esp_wifi_ap_get_sta_list(&stationList);  // get the list of connected stations
      for (int i = 0; i < stationList.num; i++) {
        wifi_sta_info_t station = stationList.sta[i];
        Serial.printf("Connected device MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                      station.mac[0], station.mac[1], station.mac[2],
                      station.mac[3], station.mac[4], station.mac[5]);
        Serial.println();
      }
#endif

#ifdef ESP8266  // for ESP8266 boards
      struct station_info* station = wifi_softap_get_station_info();  // get station info
      while (station) {
        Serial.printf("Connected device MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                      station->bssid[0], station->bssid[1], station->bssid[2],
                      station->bssid[3], station->bssid[4], station->bssid[5]);

        station = STAILQ_NEXT(station, next);  // move to the next connected station
        Serial.println();
      }
      wifi_softap_free_station_info();  // free the station info memory
#endif

      lastCheckMS = currentMS;  // update the last checked time
    }
  }
}

#endif
