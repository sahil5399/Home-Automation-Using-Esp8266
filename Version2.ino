#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"


#define WIFI_SSID "" //Enter your Wifi SSID
#define WIFI_PASS "" //Enter your Wifi Password
fauxmoESP fauxmo;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

#define RELAY_ONE        4
#define RELAY_TWO        14
#define RELAY_THREE      5
#define RELAY_FOUR       12
#define RELAY_FIVE       13
#define RELAY_SIX        1

#define ID_ONE           "S1"
#define ID_TWO           "S2"
#define ID_THREE         "S3"
#define ID_FOUR          "S4"
#define ID_FIVE          "S5"
#define ID_SIX           "S6"

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // LEDs
    pinMode(RELAY_ONE, OUTPUT);
    pinMode(RELAY_TWO, OUTPUT);
    pinMode(RELAY_THREE, OUTPUT);
    pinMode(RELAY_FOUR, OUTPUT);
    pinMode(RELAY_FIVE, OUTPUT);
    pinMode(RELAY_SIX,OUTPUT);

    // Wifi
    wifiSetup();

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is requiSIX for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discoveSIX and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices - Note that many users have problems on ESP8266 with more than three devices
    fauxmo.addDevice(ID_ONE);
    fauxmo.addDevice(ID_TWO);
    fauxmo.addDevice(ID_THREE);
    fauxmo.addDevice(ID_FOUR);
    fauxmo.addDevice(ID_FIVE);    
    fauxmo.addDevice(ID_SIX);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value, unsigned int hue, unsigned int saturation, unsigned int ct) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d hue: %d saturation: %u ct: %u\n", device_id, device_name, state ? "ON" : "OFF", value, hue, saturation, ct);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, ID_ONE)==0) {
            digitalWrite(RELAY_ONE, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_TWO)==0) {
            digitalWrite(RELAY_TWO, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_THREE)==0) {
            digitalWrite(RELAY_THREE, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_FOUR)==0) {
            digitalWrite(RELAY_FOUR, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_FIVE)==0) {
            digitalWrite(RELAY_FIVE, state ? HIGH : LOW);
        }else if (strcmp(device_name,ID_SIX)==0){
          digitalWrite(RELAY_SIX, state ? HIGH : LOW  );
        }

    });
    ArduinoOTA.begin();

}

void loop() {
    ArduinoOTA.handle();
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_ONE, true, 255);

}
