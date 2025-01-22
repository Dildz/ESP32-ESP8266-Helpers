# ESP32-ESP8266-Helpers

IDE: VSCode with PlatformIO

Collection of Helpers I created from various example projects &amp; tutorials that can be easiliy intergrated into almost any project with just a few lines of code.

They are designed to be as modular as possible while keeping main code debloated &amp; easier to maintain.

- ESPWiFiSTAHelper.h -- Station mode Wi-Fi setup. Edit network settings & include.

- ESPWiFiSoftAPHelper.h -- Soft Access Point mode Wi-Fi setup. Edit network settings & include.

- ESPWiFiHelper.h -- Combines both Station & Soft Access Point modes in one setup. Choose desired mode, edit network settings & include.

- ElegantOTAHelper.h -- Designed to be used in conjunction with one of the WiFiHelpers that best suits your needs.

Useage example projects included.

Notes:

- Tested on an ESP8266 NodeMCU only as I do not yet own an ESP32. When I get one I'll test & update accordingly.
- The example project platformio.ini files must be edited to include the corresponding ESP Ping dependancies for ESP32 if you are NOT using a ESP8266.
