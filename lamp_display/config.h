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
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "ArduinoJson.h"
#include "AdafruitIO_WiFi.h"

//************************ Adafruit IO Setup *******************************/
#define IO_USERNAME    "mmicatka"
#define IO_KEY         "aio_UlUl53OygcqBIR7DMLB8XXTMl878"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "mmicatka"
#define AIO_KEY         "aio_UlUl53OygcqBIR7DMLB8XXTMl878" // Put Your adafruit ID here
#define MQTT_KEEP_ALIVE 65535

//************************ Wifi Setup *******************************/
#define AP_NAME    "Friendship Lamp"
#define AP_PASSWORD "friendship_forever"

//************************ Adafruit Neopixels *******************************/
#define LED_PIN    14
#define LED_COUNT 16

// *************************** BUTTON STUFF ************************** //
#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2
#define BUTTON_D 12

#define A_PIN A0 // Input for the potentiometer
