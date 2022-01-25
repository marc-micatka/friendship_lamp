// *************************** CONFIGURATION STUFF ************************** //
#include "config.h"
const char* owner = "ENTER OWNER NAME HERE"; 

// *************************** DISPLAY STUFF ************************** //
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

int char_height = 7;
int char_width = 5;
int display_width = 128;
int display_height = 64;

// display timeout for inactivity (5 minutes)
unsigned long timeout = 5*60*1000;
unsigned long lastActivity = millis();

// 'bulb', 32x32px
const unsigned char epd_bitmap_bulb [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 
  0x02, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x80, 0x00, 0x87, 0xf1, 0x00, 0x00, 0x18, 0x18, 0x00, 
  0x00, 0x20, 0x04, 0x00, 0x00, 0x60, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x80, 0x01, 0x00, 
  0x3c, 0x80, 0x01, 0x3c, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 
  0x00, 0xc2, 0x41, 0x00, 0x00, 0x43, 0xc2, 0x00, 0x00, 0x22, 0x46, 0x00, 0x01, 0x32, 0x4c, 0x80, 
  0x03, 0x12, 0x88, 0xc0, 0x04, 0x09, 0x90, 0x60, 0x08, 0x08, 0x10, 0x10, 0x00, 0x08, 0x70, 0x00, 
  0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x0c, 0x10, 0x00, 
  0x00, 0x01, 0xe0, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00
};

// *************************** NEOPIXEL STUFF ******************`******** //
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long lastUpdate = 0 ; // for millis() when last update occoured
unsigned long previousTime = 0;
  
int bright_min = 10;
int bright_max = 150;
int bright_range = 2 * bright_max;
int b = bright_min;
int neo_brightness = 150;
bool count_up = true;

// *************************** IO and WIFI STUFF ************************** //
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish lamp_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/PUT YOUR FEED NAME HERE");
Adafruit_MQTT_Subscribe lamp_sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/PUT YOUR FEED NAME HERE");

// Keep alive time stuff
unsigned long previous_keepalive_time = 0;

//   SETUP WIFI
WiFiManager wifiManager;

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

// ******** FINITE STATE STUFF ******** //
/* Display States
 * 1 - Welcome Screen
 * 2 - Select Friends
 * 3 - Select FX
 * 4 - Select Color
 * 5 - Send
 * 6 - RECEIVING MESSAGE
 */
int displayed_state = 1;
int num_of_states = 5;

// *************************** SENDING AND RECEIVING STUFF ************************** //
// **** SENDING STUFF
String send_opts[] = { 
  "Send", 
  "Cancel"
};
int send_len = sizeof(send_opts)/sizeof(send_opts[0]);
int curr_pos_send = 0;  

String send_msgs[] = {
  "good vibes",
  "much love",
  "love galore",
  "virtual love",
  "endless hearts",
  "butterflies",
  "sunshine",
  "positivity"  
};
int send_msg_len = sizeof(send_msgs)/sizeof(send_msgs[0]);
bool msg_gen = false;
int idx = 0;

// ******** FRIEND STUFF ******** //
//String friends[] = { 
//  "Natalie", 
//  "Norah", 
//  "Sarah",
//  "All Friends"
//};
String friends[] = { 
  "Sarah"
};
int friend_len = sizeof(friends)/sizeof(friends[0]);
int curr_pos_friends = 0;  

// ******** FX STUFF ******** //
String fx[] = { 
  "Solid", 
  "Pulse", 
  "Rainbow",
  "Color Wipe"
};
int fx_len = sizeof(fx)/sizeof(fx[0]);
int curr_pos_fx = 0;  

// Number of FX to display at once
int disp_length = 3;
int displayed_min = 0;
int displayed_max = displayed_min + disp_length;

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
unsigned long debounceDelay = 20;    // the debounce time; increase if the output flickers

int buttonStateA = HIGH;             // the current reading from the input pin
int lastButtonStateA = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTimeA = 0;  // the last time the output pin was toggled

//int buttonStateB = HIGH;             // the current reading from the input pin
//int lastButtonStateB = HIGH;   // the previous reading from the input pin
//unsigned long lastDebounceTimeB = 0;  // the last time the output pin was toggled

boolean BActive = false;
boolean BLongPress = false;
boolean BCurrentLongPress = false;
unsigned long BTimer = 0; // stores the time that the button was pressed (relative to boot time)
unsigned long BDuration = 0; // stores the duration (in milliseconds) that the button was pressed/held down for
unsigned long timePressB = 0;
unsigned long timePressLimitB = 0;
const unsigned long longPressThresholdB = 1000; 

int buttonStateC = HIGH;             // the current reading from the input pin
int lastButtonStateC = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTimeC = 0;  // the last time the output pin was toggled

boolean DActive = false;
boolean DLongPress = false;
boolean DCurrentLongPress = false;
unsigned long DTimer = 0; // stores the time that the button was pressed (relative to boot time)
unsigned long DDuration = 0; // stores the duration (in milliseconds) that the button was pressed/held down for
unsigned long timePressD = 0;
unsigned long timePressLimitD = 0;
const unsigned long longPressThresholdD = 2000; 

//************************ Main Program Starts Here *******************************/
//*********************************************************************************/
void MQTT_connect();

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  //entered config mode, make led toggle faster
  solid(strip.Color(255, 255, 0));
}
void setup() {
  Serial.begin(115200);
  while(! Serial);

  // SETUP BUTTONS
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);

  // SETUP the DISPLAY
  display.begin(0x3C, true); // Address 0x3C default
  display.clearDisplay();
  display.setRotation(1);
  display_homescreen();

  // Activate the Neopixels
  strip.begin();
  strip.setBrightness(neo_brightness);
  strip.show(); // Initialize all pixels to 'off'
  
  // Flash RED (just to confirm that power is connected)
  flash(strip.Color(255, 0, 0), 500, 3);

//  wifiManager.setBreakAfterConfig(true);
//  wifiManager.setConfigPortalBlocking(false);
//  wifiManager.setAPCallback(configModeCallback);
//  wifiManager.resetSettings(); //reset settings - for testing

  // WIFI STUFF
  if (!wifiManager.autoConnect(AP_NAME, AP_PASSWORD)) {
    Serial.println("Failed to connect to Network.");
    flash(strip.Color(255, 0, 0), 1000, 1);    
    delay(1000);
    ESP.restart();
  } 
  else { 
    wifi_connected = true;
    Serial.println("Connected to Network.");

    const String wifi_ssid = wifiManager.getWiFiSSID();
    const String wifi_pass = wifiManager.getWiFiPass();

    // Connect to WiFi access point.
    Serial.print("Connected to: ");
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
    // Setup MQTT subscription for feed
    mqtt.subscribe(&lamp_sub);
  }
  
  // set averages to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  select_color();

   // Flash Green three times
  flash(strip.Color(0, 255, 0), 500, 3);
  
  randomSeed(millis());
  displayed_state = 1;
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
  check_buttons();
  
  receive_command();

  if (millis() - lastActivity > timeout){
    // turning display off to save screen
    displayed_state = 7;
  } 

  if (!DCurrentLongPress){
      display_screen();
    }
  else{
    strip.setBrightness(neo_brightness);
    solid(strip.Color(255, 255, 255));
    strip.show();
  }
}

//************************ Main Program Ends Here *******************************/
//*********************************************************************************/

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
      fx_sent = doc["fx"];
      color_sent = doc["color"];
      
      if (strcmp(sender, owner) != 0){
        Serial.println("Receiving message");
        lastActivity = millis();
        displayed_state = 6;
        display.clearDisplay();
      }
    }
  }
}

bool send_command(){
  // Send Message
  char msg[128]; 
  
  doc["sender"] = owner;
  doc["receiver"] = friends[curr_pos_friends];
  doc["fx"] = curr_pos_fx;
  doc["color"] = displayed_color;
  
  serializeJson(doc, msg);
  Serial.println(msg);

  displayed_state = 0;
  
  if (! lamp_pub.publish(msg)) {
    Serial.println(F("Failed"));
    return false;
  } else {
    Serial.println(F("OK!"));
    return true;
    delay(1000);
    }
}
