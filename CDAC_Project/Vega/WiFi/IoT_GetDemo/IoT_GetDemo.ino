/*
 @file   IoT_GetDemo.ino
 @brief  Wi-Fi NINA Module communicates with the ARIES IoT v1 Board via the SPI Protocol
 @detail To control the internal LED in Aries IoT board from Adafruit cloud server using Wi-Fi 

 * Reference Arduino code: https://www.electronics-lab.com/project/getting-started-with-adafruit-io-iot-cloud-platform/
 * Reference ARIES IoT board: https://vegaprocessors.in/devboards/ariesiot.html
 * Reference ARIES IoT pin diagram: https://vegaprocessors.in/files/ARIES-IoT-Pin-out.pdf
 * 
 * Required Libraries : VEGA_WiFiNINA (Version 1.0.1)         
 * 
 * Note:
 * No external connections required for testing this code using ARIES IoT v1
 * 
 * Steps to do in Adafruit cloud platform:
 * 1) Visit https://io.adafruit.com/ and login or create an account if you do not already have.
 * 2) Click the feeds button,select create a "new feed”.
 * 3) Enter the name of the feed and a description and click the create button. (example feed Name = onoff)
 * 4) Click on "Dashbords"
 * 6) select “create a new dashboard“ for creating the dashboard ( Example dashboard name -> onoffDash)
 * 7) Click on the dashboard you have created, it should open a new window. On the window, click on the “gear” Icon to start adding blocks (widgets) to the dashboard.
 * 8) Clicking on any block will enable you to set the properties of the block. Select the feed to whose data will be displayed on that block and hit the next button.
 * 9) Here in this example, for internal LED control, select the "toggle" widget from the available blocks.
 * 10)Select the feed name to connect with the block in the dashboard,Click on "Next Step"
 * 11) Give "Button On Text" value as 1 and "Button off Text" value as 0 and click on "Create block"
 * 
 * Steps to do in the Code:
 * 1) Replace the values in ssid[] with your wi-Fi hotspot username
 * 2) Replace the values in pass[] with your wi-Fi hotspot password
 * 3) Replace the values in IO_USERNAME  with your Adafruit username
 * 4) Replace the values in IO_FEEDNAME  with your Adafruit feed name
 * 5) Replace the values in adafruitKey[]  with your Adafruit key
 * 6) Replace the values in wifiClient.print() in line 99  with your Adafruit credentials
 * 
 * After completing above steps properly, run this code and then you can control the intenal LED from Adafruit cloud server
 * 
 * Example Adafruit credentials:
 * Username -> mydhily
 * Feed Name -> onoff
 * Adafruit key -> aio_UBnp43gUiHdyCK4mMgkXVmKkZ2hM
 * Adafruit Dashboard Name -> onoffDash
 * 
*/

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
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(1000); // Try to connect every 5 seconds
  }

  Serial.println("Connected to Wi-Fi!");

  #else
  Serial.println("Choose the correct board! " );
  #endif
}

void loop() {
  
}
