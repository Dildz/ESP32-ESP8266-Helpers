/****************************************************************************************
 * ESPWiFiHelper.h
 * 
 * This header file provides a simple way to configure and use either SoftAP (Access Point)
 * mode or Station (Client) mode for ESP8266/ESP32 boards. 
 * It supports both static IP configuration and DHCP for Station mode.
 * It also checks internet connectivity with a LED status:
 * - Solid: Wi-Fi connected and internet available
 * - Slow Blink: Wi-Fi connected but no internet
 * 
 * Usage:
 *
 * Include this header file in your project and configure the Wi-Fi modes & settings as needed.
 * 
 * 1. Select Wi-Fi Mode:
 *    - In the `wifiMode` variable, set either `WIFI_MODE_SOFTAP` (for SoftAP mode) or 
 *      `WIFI_MODE_STA` (for Station mode).
 * 
 * 2. Configure SoftAP (Access Point) Settings if using SoftAP mode:
 *    - Modify the `apSSID` (network name), `apPassword` (password), `apIP` (IP address), 
 *      and `apSubnet` (subnet mask) for your SoftAP network.
 * 
 * 3. Configure Station (Client) Settings if using STA mode:
 *    - Set `staSSID` (Wi-Fi network name) and `staPassword` (Wi-Fi network password).
 *    - If using Static IP, set `USE_STATIC_IP` to `true` and configure the `staticIP`, `gateway`, 
 *      `subnet`, and `dns` variables as needed.
 * 
 * 4. In the main.cpp file:
 *   - #include "ESPWiFiHelper.h"
 *   - In the `setup()` function, call the `setupWiFi()` function to initialize Wi-Fi based on the selected mode.
 *   - In the `loop()` function, call the `handleBuiltInLED()` function to handle LED status for no internet access.
 *     if STA mode is selected.
 * 
****************************************************************************************/

#ifndef ESPWiFiHelper_h
#define ESPWiFiHelper_h

#ifdef ESP32    // for ESP32 boards
#include <WiFi.h>
#include <ESP32Ping.h>

#elif defined(ESP8266)    // for ESP8266 boards
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#endif


// Wi-Fi Modes
#define WIFI_MODE_SOFTAP   0
#define WIFI_MODE_STA      1

/****************************************************
 ****************** MODE SELECTION ******************
 ** Replace with WIFI_MODE_SOFTAP or WIFI_MODE_STA **
 ****************************************************/

int wifiMode = WIFI_MODE_STA;   // WIFI_MODE_SOFTAP or WIFI_MODE_STA

// SoftAP Configuration
const char* apSSID = "ESP8266";       // SoftAP network name
const char* apPassword = "12345678";  // SoftAP password
IPAddress apIP(192, 168, 10, 1);      // AP IP address
IPAddress apSubnet(255, 255, 255, 0); // subnet mask for SoftAP

// STA Configuration
const char* staSSID = "YOUR_SSID_NAME";      // Wi-Fi network name
const char* staPassword = "YOUR_SSID_PW";    // Wi-Fi network password
const char* hostName = "ESP8266";            // change the hostname if needed

bool USE_STATIC_IP = false;             // static IP = true | DHCP = false
IPAddress staticIP(192, 168, 3, 10);    // static IP
IPAddress gateway(192, 168, 3, 1);      // router gateway
IPAddress subnet(255, 255, 255, 0);     // subnet mask
IPAddress dns(192, 168, 3, 1);          // DNS server

bool isConnected = false;   // Wi-Fi connection status
bool hasInternet = false;   // Internet connection status


// Setup Wi-Fi modes depending on the selected mode
void setupWiFi() {
  // Initialize the Built-In LED
  pinMode(LED_BUILTIN, OUTPUT);     // set the LED pin mode
  digitalWrite(LED_BUILTIN, HIGH);  // initialize off (active low)

/******************************************
 ************* SoftAP Mode ****************
 ******************************************/
  if (wifiMode == WIFI_MODE_SOFTAP) {

    Serial.println("Setting up SoftAP...");
    if (!WiFi.softAPConfig(apIP, apIP, apSubnet)) {
      Serial.println("Failed to configure SoftAP network! Check your IP configuration.");
      return;
    }
    if (WiFi.softAP(apSSID, apPassword)) {
      Serial.println("SoftAP configured successfully!");
      Serial.print("Network Name: ");
      Serial.println(apSSID);
      Serial.print("AP IP Address: ");
      Serial.println(WiFi.softAPIP());
      digitalWrite(LED_BUILTIN, LOW);  // Turn on LED (active low)
    } else {
      Serial.println("Failed to start SoftAP! Check your setup.");
    }
  }

/******************************************
 ******* Station Mode (Wi-Fi Client) ******
 ******************************************/
  if (wifiMode == WIFI_MODE_STA) {
    
    Serial.println("Connecting to Wi-Fi...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.persistent(false);
    WiFi.setAutoReconnect(true);
    WiFi.hostname(hostName);
    delay(100);

    if (USE_STATIC_IP) {
      if (!WiFi.config(staticIP, gateway, subnet, dns)) {
        Serial.println("Failed to configure static IP! Check the ESPWiFiHelper.h configuration.");
      } else {
        Serial.println("Static IP configuration successful.");
      }
    }

    WiFi.begin(staSSID, staPassword);
    Serial.print("Attempting to connect to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // Toggle LED for fast blinking
      delay(250);
    }

    Serial.println("\nWi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    isConnected = true;  // Set Wi-Fi connected flag

    // Check internet connectivity using ping
    Serial.println("\nPinging Google to test internet & DNS...");
    hasInternet = Ping.ping("www.google.com");

    if (hasInternet) {
      Serial.println("Ping successful! Internet is available.");
      digitalWrite(LED_BUILTIN, LOW);  // solid on LED (active low)
    } else {
      Serial.println("Ping failed! No internet or DNS issue.");
    }
  }
}


// Function to handle LED blinking for no internet access in STA mode
void handleBuiltInLED() {
  if (wifiMode == WIFI_MODE_STA) {
    const long BLINK_PERIOD = 1000;  // ms for slow blink
    static long lastBlinkMS = 0;     // last time the LED blinked
    static bool ledState = false;    // current state of the LED
    long currentMS = millis();       // get the current time

    if (isConnected && !hasInternet) {
      if (currentMS - lastBlinkMS >= BLINK_PERIOD) {
        digitalWrite(LED_BUILTIN, ledState);  // toggle the LED
        ledState = !ledState;                 // update LED state
        lastBlinkMS = currentMS;              // update the last blink time
      }
    }
  }
}

#endif  // ESPWiFiHelper_h
