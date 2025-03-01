/*
#include <SPI.h>
#include <WiFiNINA.h>
#include <sprintf.h>

#define VEGA_ARIES_IOT

char ssid[] = "Tata Play";       // Replace with your Wi-Fi network SSID
char pass[] = "1234567890";   // Replace with your Wi-Fi network password
  
WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  #if defined (VEGA_ARIES_IOT)
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Current status: ");
    Serial.println(WiFi.status());

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("SSID not available");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Connection failed");
        break;
      case WL_CONNECTION_LOST:
        Serial.println("Connection lost");
        break;
      case WL_DISCONNECTED:
        Serial.println("Disconnected from Wi-Fi");
        break;
      default:
        Serial.println("Connecting...");
        break;
    }
  }

  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  #else
  Serial.println("Choose the correct board!");
  #endif
}

void loop() {
  // Your code here
}
*/

/*

#include <SPI.h>
#include <WiFiNINA.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Check for the Wi-Fi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }

  Serial.println("WiFi module detected!");
}

void loop() {
  // Empty loop
}
*/


#include <SPI.h>
#include <WiFiNINA.h>

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Check for the Wi-Fi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // Don't continue
    while (true);
  }

  // Print the firmware version
  String fv = WiFi.firmwareVersion();
  Serial.print("Firmware version: ");
  Serial.println(fv);
}

void loop() {
  // Empty loop
}
