//************************ NEOPIXEL EFFECTS *******************************/
void choose_color(int Ain){
  int R = 0;
  int G = 0;
  int B = 0;
  
  if(Ain <= 16.6){                     // 0 to 60°
    R = 100;                           // Red on.
    G = Ain * 6;                       // Green fading up.
    B = 0;                             // Blue off.
  };
  if((Ain > 16.6) && (Ain <= 33.3)){    // 60° to 120°
    R = 100 - (Ain - 16.6) * 6;        // Red fading down.
    G = 100;                           // Green on.
    B = 0;                             // Blue off.
  }
  if((Ain > 33.3) && (Ain <= 50)){      // 120° to 180°
    R = 0;                             // Red off.
    G = 100;                           // Green on.
    B = (Ain - 33.3) * 6;              // Blue fading up.
  }
  if((Ain > 50) && (Ain <= 66.6)){      // 180° to 240°
    R = 0;                             // Red off.
    G = 100 - (Ain - 50) * 6;          // Green fading down.
    B = 100;                           // Blue on.
  }
  if((Ain > 66.6) && (Ain <= 83.3)){    // 240° to 300°
    R = (Ain - 66.6) * 6;              // Red fading up.
    G = 0;                             // Green off.
    B = 100;                           // Blue on.
  }
  if((Ain > 83.3) && (Ain <= 100)){     // 300° to 360°
    R = 100;                           // Red on.
    G = 0;                             // Green off.
    B = 100 - (Ain - 86.6) * 6;        // Red fading down.
  }
  displayed_color = strip.Color(R, B, G);
}
//simple code to turn all of the Neopixels off
void wipe(){ // clear all LEDs
  for(int i=0;i<strip.numPixels();i++){
   strip.setPixelColor(i, strip.Color(0,0,0)); 
   }
   strip.show();
   
}

// display solid color
void solid(uint32_t color){
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
    strip.show();
}

// Some functions of our own for creating animated effects -----------------
void pulse(uint32_t color, long duration){
  unsigned long currentTime = millis();
  strip.clear();
  int wait = duration * 1000 / bright_range;

  if (currentTime - previousTime > wait){
    for(int c=0; c<strip.numPixels(); c++) {
      strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
    }

  if (b > bright_max){
    count_up = false;
  }

  if (b < bright_min){
    count_up = true;
  }
  
  if (count_up){
    b++;
  }

  if (!count_up){
    b--;
  }
//  Serial.println(b);

  strip.setBrightness(b);
  strip.show();
  }
}

//code to flash the Neopixels when a stable connection to Adafruit IO is made
void flash(uint32_t c, int wait, int iters) {
  for(int j=0; j<iters; j++){
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
    
    strip.show();
    delay(wait);
    strip.clear();
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) { // modified from Adafruit example to make it a state machine
  static int i =0;
  int wait = 50;
  unsigned long currentTime = millis();
  if (currentTime - previousTime > wait){
    strip.setPixelColor(i, c);
    strip.show();
    i++;
    if(i >= strip.numPixels()){
      i = 0;
      wipe(); // blank out strip
    }
    previousTime = millis(); // time for next change to the display
  }

}

// Rainbow pulse
void rainbow() { // modified from Adafruit example to make it a state machine
  static uint16_t j=0;
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
     j++;
  if(j >= 256) j=0;
  lastUpdate = millis(); // time for next change to the display
  
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() { // modified from Adafruit example to make it a state machine
  static uint16_t j=0;
    for(int i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  j++;
  if(j >= 256*5) j=0;
  lastUpdate = millis(); // time for next change to the display
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c) {
    static int j=0, q = 0;
    static boolean on = true;
     if(on){
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, displayed_color);    //turn every third pixel on
             }
     }
      else {
           for (int i=0; i < strip.numPixels(); i=i+3) {
               strip.setPixelColor(i+q, 0);        //turn every third pixel off
                 }
      }
     on = !on; // toggel pixelse on or off for next time
      strip.show(); // display
      q++; // update the q variable
      if(q >=3 ){ // if it overflows reset it and update the J variable
        q=0;
        j++;
        if(j >= 256) j = 0;
      }
  lastUpdate = millis(); // time for next change to the display    
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() { // modified from Adafruit example to make it a state machine
  static int j=0, q = 0;
  static boolean on = true;
     if(on){
            for (int i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
             }
     }
      else {
           for (int i=0; i < strip.numPixels(); i=i+3) {
               strip.setPixelColor(i+q, 0);        //turn every third pixel off
                 }
      }
     on = !on; // toggel pixelse on or off for next time
      strip.show(); // display
      q++; // update the q variable
      if(q >=3 ){ // if it overflows reset it and update the J variable
        q=0;
        j++;
        if(j >= 256) j = 0;
      }
  lastUpdate = millis(); // time for next change to the display    
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
