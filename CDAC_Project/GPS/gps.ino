#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Define the RX and TX pins for Software Serial
#define RX_PIN 4  // D2 (GPIO4)
#define TX_PIN 5  // D1 (GPIO5)

TinyGPSPlus gps;
SoftwareSerial ss(RX_PIN, TX_PIN);

void setup() {
  // Start hardware serial communication for debugging
  Serial.begin(115200);
  // Start software serial communication with GPS
  ss.begin(9600);
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    gps.encode(ss.read());
    
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      Serial.print("Altitude= "); 
      Serial.println(gps.altitude.meters());
      Serial.print("Date: ");
      Serial.print(gps.date.year()); 
      Serial.print("/"); 
      Serial.print(gps.date.month()); 
      Serial.print("/"); 
      Serial.print(gps.date.day()); 
      Serial.print(" Time: "); 
      Serial.print(gps.time.hour()); 
      Serial.print(":");
      Serial.print(gps.time.minute()); 
      Serial.print(":");
      Serial.println(gps.time.second()); 
      Serial.println();
    }
  }
}
