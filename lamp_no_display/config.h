#include <ArduinoMqttClient.h>

#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <AdafruitIO.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT_Client.h"
#include <ArduinoHttpClient.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include "ArduinoJson.h"
#include "AdafruitIO_WiFi.h"

//************************ Adafruit IO Setup *******************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "ENTER USERNAME HERE"
#define AIO_KEY         "ENTER YOUR KEY HERE" // Put Your adafruit ID here
#define MQTT_KEEP_ALIVE 65535

//************************ Wifi Setup *******************************/
#define AP_NAME    "Friendship Lamp" // this is the name of the wifi network the lamp creates
#define AP_PASSWORD "12345" // this is the login for the wifi network

//************************ Adafruit Neopixels *******************************/
#define LED_PIN    14
#define LED_COUNT 24

// *************************** BUTTON STUFF ************************** //
#define BUTTON_BLUE 5// A == cycle through effects
#define BUTTON_GREEN 4 // B == Send
#define LED_GREEN 0
#define LED_BLUE 2

#define A_PIN A0 // Input for the potentiometer

// *************************** RESOUCES ************************** //
// https://forum.arduino.cc/t/fastled-neopixel-examples-that-are-non-blocking/396845/5
