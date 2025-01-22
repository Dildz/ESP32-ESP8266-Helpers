/****************************************************************************************
* This helper file consolidates the setup of ElegantOTA & AsyncWebServer for ESP32/8266 boards.
*
* To use this helper:
* - Include this file in your project.
* - Include ESPWiFiHelper.h in your project or setup Wi-Fi connection yourself in main.
* - In main setup() > call the setupOTA() function.
* - In main loop() > call the ElegantOTA.loop() function to allow for reboots after updates.
*
* >>IMPORTANT<<
* If using an ESP8266 board, set #define ELEGANTOTA_USE_ASYNC_WEBSERVER 0 to 1 in the ElegantOTA.h file.
****************************************************************************************/

#ifndef ELEGANTOTAHELPER_H
#define ELEGANTOTAHELPER_H

#ifdef ESP32                // for ESP32 boards
#include <AsyncTCP.h>

#elif defined(ESP8266)      // for ESP8266 boards
#include <ESPAsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>      // include the AsyncWebServer library
#include <ElegantOTA.h>             // set #define ELEGANTOTA_USE_ASYNC_WEBSERVER 0 to 1 in the ElegantOTA.h file
#include <LittleFS.h>

// Asyncronous web server, on port 80
AsyncWebServer server(80);


// Function to initialize Serial monitor, Wi-Fi, Built-In LED, server, and OTA
void setupOTA() {
    // Handle unknown requests
    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "404 - Page Not Found, oops!");
    });

    // Default landing page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        String message = "Hi! I am ESP8266, and look! No wires!!.\n\n";
        message += "Browse to http://[assigned.esp.ip.address]/update to update my firmware.";

        request->send(200, "text/plain", message);
    });

    // Setup the server
    ElegantOTA.begin(&server);
    server.begin();
    delay(500);
    Serial.println("\nHTTP server started");
    Serial.println("Open: http://[assigned.esp.ip.address]/update in a browser to update firmware");
}

#endif
