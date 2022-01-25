// *************************** CONFIGURATION STUFF ************************** //
#include "config.h"
const char* owner = "Lamp Owner Name"; 
const char* all_friends = "All Friends"; 

// ******** FRIEND STUFF ******** //
// List your friends here
String friends[] = { 
  "friend A",
  "ALl Friends"
};

int friend_len = sizeof(friends)/sizeof(friends[0]);
int curr_pos_friends = 0;  

// *************************** NEOPIXEL STUFF ************************** //
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long patternInterval = 20 ; // time between steps in the pattern
unsigned long lastUpdate = 0 ; // for millis() when last update occoured
unsigned long previousTime = 0;
  
int bright_min = 10;
int bright_max = 150;
int bright_range = 2 * bright_max;
int b = bright_min;
int neo_brightness = 150;
bool count_up = true;
bool blueCurrentLongPress = false;

// *************************** IO and WIFI STUFF ************************** //
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish lamp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/PUT YOUR FEED NAME HERE");
Adafruit_MQTT_Subscribe lamp_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fPUT YOUR FEED NAME HERE");

// Keep alive time stuff
unsigned long previous_keepalive_time = 0;

long messageReceivedTime = -1000000;
int count = 0;
bool wifi_connected = false;
bool msg_recieved = false;

const int capacity = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(4);
StaticJsonDocument<capacity>doc;
void MQTT_connect();

const char* sender = "";
const char* receiver = "";
int fx_sent = 0;
uint32_t color_sent = strip.Color(0, 0, 0);

//   SETUP WIFI
WiFiManager wifiManager;

// ******** FINITE STATE STUFF ******** //
/* Display States
 * 0 - Sending Command
 * 2 - Receiving Command
 */
int displayed_state = 0;
int num_of_states = 2;

// ******** FX STUFF ******** //
String fx[] = { 
  "Solid", 
  "Pulse", 
  "Rainbow",
  "Color Wipe",
  "Off"
  
};
int fx_len = sizeof(fx)/sizeof(fx[0]);
int curr_pos_fx = 0;  

// ******** POTENTIOMETER STUFF ******** //
// Potentiometer range stuff
int potVal = 0;
int pot_min = 10;
int pot_max = 1016;
const int numReadings = 30;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// Colors to send/display
uint32_t displayed_color = strip.Color(0, 0, 0);

// *************************** BUTTON STUFF ************************** //
//initialising led
int LED_GREEN_Status = LOW;
int LED_BLUE_Status = LOW;

const unsigned long longPressThreshold = 2000; 
const unsigned long debounceThreshold = 50; // the threshold (in milliseconds) for a button press to be confirmed (i.e. not "noise")

boolean blueActive = false;
boolean blueLongPress = false;

boolean greenActive = false;
boolean greenLongPress = false;

unsigned long blueTimer = 0; // stores the time that the button was pressed (relative to boot time)
unsigned long blueDuration = 0; // stores the duration (in milliseconds) that the button was pressed/held down for

unsigned long greenTimer = 0; // stores the time that the button was pressed (relative to boot time)
unsigned long greenDuration = 0; // stores the duration (in milliseconds) that the button was pressed/held down for


unsigned long timePress_BLUE = 0;
unsigned long timePressLimit_BLUE = 0;
int clicks_BLUE = 0;

unsigned long timePress_GREEN = 0;
unsigned long timePressLimit_GREEN = 0;
int clicks_GREEN = 0;

unsigned long doubleClickTime = 500;

// Button A cycles through effects
int buttonStateGREEN = HIGH;             // the current reading from the input pin
int lastButtonStateGREEN = HIGH;   // the previous reading from the input pin

int buttonStateBLUE = HIGH;             // the current reading from the input pin
int lastButtonStateBLUE = HIGH;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTimeGREEN = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTimeBLUE = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//************************ Main Program Starts Here *******************************/
void MQTT_connect();

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  
  //entered config mode, make led toggle faster
  solid(strip.Color(255, 255, 0));
}

void setup() {
  // put your setup code here, to run once:
  // initialize the LED pin as an output:
  pinMode(BUTTON_GREEN ,INPUT_PULLUP);
  pinMode(BUTTON_BLUE ,INPUT_PULLUP);
 
  // initialize the pushbutton pin as an input:
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_GREEN, LED_GREEN_Status);
  digitalWrite(LED_BLUE, LED_BLUE_Status);
    
  Serial.begin(115200);
// wait for serial monitor to open
  while(! Serial);
  
  // Activate the Neopixels
  strip.begin();
  strip.setBrightness(neo_brightness);
  strip.show(); // Initialize all pixels to 'off'
  
  // Flash RED (just to confirm that power is connected)
  flash(strip.Color(255, 0, 0), 1500, 1);

//  wifiManager.setBreakAfterConfig(true);
//  wifiManager.setConfigPortalBlocking(false);
//  wifiManager.setAPCallback(configModeCallback);

//  wifiManager.resetSettings(); //reset settings - for testing

  
  if (!wifiManager.autoConnect(AP_NAME, AP_PASSWORD)) {
    Serial.println("Failed to connect to Network.");
    flash(strip.Color(255, 0, 0), 1000, 1);    
    delay(1000);
    ESP.restart();
  } 
  else { 
    wifi_connected = true;
    Serial.println("Connected to Network...");
    wifiManager.getWiFiSSID();
    const String wifi_ssid = wifiManager.getWiFiSSID();
    const String wifi_pass = wifiManager.getWiFiPass();
  
    // Connect to WiFi access point.
    Serial.println(); Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);
  
    WiFi.begin(wifi_ssid, wifi_pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    // Setup MQTT subscription for onoff feed.
    mqtt.subscribe(&lamp_sub);
  
    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());
    
  }
  

  // set averages to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

   // Flash Green three times
  flash(strip.Color(0, 255, 0), 750, 3);

}

void loop() {
  if((millis() - previous_keepalive_time) > MQTT_KEEP_ALIVE * 1000) {
      previousTime = millis();   
      if(! mqtt.ping()) {
        mqtt.disconnect();
      }
    }
    MQTT_connect();
    delay(5);
    
    check_buttons_long();
    
    if (!blueCurrentLongPress){
      receive_command();
      display_state();
    }
    
}
//************************ IO CONNECTION *******************************/
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
//************************ CHECK FEED *******************************/
void receive_command(){
  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(10))) {
    // Check the lamp feed
    
    if (subscription == &lamp_sub) {
      
      
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, (char *)lamp_sub.lastread);
      
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      sender = doc["sender"];
      receiver = doc["receiver"];
      fx_sent = doc["fx"];
      color_sent = doc["color"];

      // You're not the sender AND the receiver is either YOU or ALL FRIENDS
      if (strcmp(sender, owner) != 0){
        if ((strcmp(owner, receiver) == 0) || strcmp(receiver, all_friends) == 0){
          Serial.println("Receiving message");
          Serial.println(fx_sent);
          Serial.println(color_sent);
          flash(strip.Color(0, 255, 0), 750, 3);    
          displayed_state = 1;
        }
      }
    }
  }
}

bool send_command(){
  // Send Message
  char msg[128]; 
  
  doc["sender"] = owner;
  doc["receiver"] = "mary";
  doc["fx"] = curr_pos_fx;
  doc["color"] = displayed_color;
  
  serializeJson(doc, msg);
  Serial.println(msg);

  displayed_state = 0;
    
  if (! lamp_pub.publish(msg)) {
    Serial.println(F("Failed"));
    flash(strip.Color(255, 0, 0), 750, 3);   
    return false;
  } else {
    Serial.println(F("OK!"));
    flash(strip.Color(0, 255, 0), 750, 3);    
    
    return true;
    }
}
