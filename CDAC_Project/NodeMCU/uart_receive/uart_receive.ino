#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial
const int rxPin = D7;  // Use D1 (GPIO5) as RX pin
const int txPin = D8;  // Use D2 (GPIO4) as TX pin (not used in this code)

// Create SoftwareSerial object with RX and TX
SoftwareSerial mySerial(rxPin, txPin);

void setup() {
    // Start the hardware serial port
    Serial.begin(9600);

    // Start the software serial port
    mySerial.begin(9600);

    Serial.println("NodeMCU Ready");
}

void loop() {
    if (mySerial.available()) {
        String received = mySerial.readString();
        Serial.print("Received: ");
        Serial.println(received);
    }
}
