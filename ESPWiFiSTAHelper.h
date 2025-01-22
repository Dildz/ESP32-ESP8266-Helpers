/****************************************************************************************
* This helper file consolidates the following functions:
* 1. Connect to Wi-Fi network,
* 2. Configure static IP address (optional),
* 3. Check internet connectivity using ping,
* 4. Handle LED blinking states based on connectivity.
*
* To use this helper:
* - Include this file in your project,
* - Modify the SSID info, choose to use a Static IP or DHCP - if static, configure as needed,
* - In main setup() > call the setupWiFi() function,
* - In main loop() > call the handleLED() function.
****************************************************************************************/

#ifndef ESPWiFiSTAHelper_h
#define ESPWiFiSTAHelper_h

#ifdef ESP32    // for ESP32 boards
#include <WiFi.h>
#include <ESP32Ping.h>

#elif defined(ESP8266)    // for ESP8266 boards
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#endif

// Configuration for Wi-Fi and static IP (if applicable)
const char* ssid = "YOUR_SSID_NAME";      // Wi-Fi network name
const char* password = "YOUR_SSID_PW";    // Wi-Fi network password
const char* hostName = "ESP8266";         // change the hostname if needed

bool USE_STATIC_IP = false;   // static IP = true | DHCP = false

IPAddress staticIP(192, 168, 3, 10);    // static IP
IPAddress gateway(192, 168, 3, 1);      // router gateway
IPAddress subnet(255, 255, 255, 0);     // subnet mask
IPAddress dns(192, 168, 3, 1);          // DNS server

bool isConnected = false;   // Wi-Fi connection status
bool hasInternet = false;   // internet connection status


// Single function to handle Wi-Fi setup and LED states
void setupWiFi() {
  // Initialize the Built-In LED
  pinMode(LED_BUILTIN, OUTPUT);     // set the LED pin mode
  digitalWrite(LED_BUILTIN, HIGH);  // initialize the LED as off (active low)

  // Start Wi-Fi connection
  WiFi.mode(WIFI_STA);            // set Wi-Fi to Station mode
  WiFi.disconnect();              // disconnect from any previous connections
  WiFi.persistent(false);         // do not store Wi-Fi configuration in EEPROM
  WiFi.setAutoReconnect(true);    // reconnect if connection is lost
  WiFi.hostname(hostName);        // set a hostname
  delay(100);

  // Configure static IP if USE_STATIC_IP is true
  if (USE_STATIC_IP) {
    if (!WiFi.config(staticIP, gateway, subnet, dns)) {
      Serial.println("Failed to configure static IP! Check the ESPWiFiHelper.h configuration.");
    } else {
      Serial.println("Static IP configuration successful.");
    }
  }

  WiFi.begin(ssid, password);  // connect to Wi-Fi network

  Serial.print("Attempting to connect to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // toggle LED for fast blinking
    delay(250);
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  isConnected = true;  // set Wi-Fi is connected flag

  // Check internet connectivity using ping
  Serial.println("\nPinging Google to test internet & DNS...");
  hasInternet = Ping.ping("www.google.com");

  if (hasInternet) {
    Serial.println("Ping successful! Internet is available.");
    digitalWrite(LED_BUILTIN, LOW);  // solid LED (active low)
  } else {
    Serial.println("Ping failed! Either Wi-Fi is connected with no internet, or DNS is not working.");
  }
}

// Function to handle LED blinking for no internet access
void handleBuiltInLED() {
  const long BLINK_PERIOD = 1000;  // ms for slow blink
  long lastBlinkMS = 0;            // last time the LED blinked
  bool ledState = false;           // current state of the LED
  long currentMS = millis();       // get the current time

  if (isConnected && !hasInternet) {
    if (currentMS - lastBlinkMS >= BLINK_PERIOD) {
      digitalWrite(LED_BUILTIN, ledState);  // toggle the LED
      ledState = !ledState;                 // update LED state
      lastBlinkMS = currentMS;              // update the last blink time
    }
  }
}

#endif
