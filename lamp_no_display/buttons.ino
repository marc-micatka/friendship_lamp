//************************ CHECK BUTTONS - Long Press*******************************//
void check_buttons_long(){
  // Blue == cycle through effects
  int readingBLUE = digitalRead(BUTTON_BLUE);
  // Green == Send
  int readingGREEN = digitalRead(BUTTON_GREEN);

  //************Check Blue Button*************
  // if the button pin reads LOW, the button is pressed (negative/ground switch)
  if (readingBLUE == LOW)
  {
    
  // mark the button as active, and start the timer
    if (blueActive == false)
    {
      blueActive = true;
      blueTimer = millis();
    }
  
    // calculate the button press duration by subtracting the button time from the boot time
    blueDuration = millis() - blueTimer;
    
    
    // mark the button as long-pressed if the button press duration exceeds the long press threshold
    if ((blueDuration > longPressThreshold) && (blueLongPress == false))
    {
      blueLongPress = true;
      Serial.println("Blue - Long Press");
      //Double Press Action
      // Turn LED on/off
      if (LED_BLUE_Status == LOW){
        digitalWrite(LED_BLUE, HIGH);
        LED_BLUE_Status = HIGH;
      }
      
      else if (LED_BLUE_Status == HIGH){
        digitalWrite(LED_BLUE, LOW);
        LED_BLUE_Status = LOW;
      }
      // Turn LED on/off
      if (LED_GREEN_Status == LOW){
        digitalWrite(LED_GREEN, HIGH);
        LED_GREEN_Status = HIGH;
      }
      
      else if (LED_GREEN_Status == HIGH){
        digitalWrite(LED_GREEN, LOW);
        LED_GREEN_Status = LOW;
      }
    }
    if (blueDuration > longPressThreshold){
      blueCurrentLongPress = true;
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = analogRead(A_PIN);
      // add the reading to the total:
      total = total + readings[readIndex];
    //  // advance to the next position in the array:
      readIndex = readIndex + 1;
    //  // if we're at the end of the array...
      if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }

      // calculate the average:
      int neo_min = 10;
      int neo_max = 255;
      average = total / numReadings;
      neo_brightness = 255 - 250 * (average - pot_min)/(pot_max - pot_min);
      strip.setBrightness(neo_brightness);
      solid(strip.Color(255, 255, 255));
      Serial.println(neo_brightness);
    }
  }
  // button either hasn't been pressed, or has been released
  else
  {
    blueCurrentLongPress = false;
    // if the button was marked as active, it was recently pressed
    if (blueActive == true)
    {
      // reset the long press active state
      if (blueLongPress == true)
      {
        blueLongPress = false;
      }
    
    // we either need to debounce the press (noise) or register a normal/short press
    else
    {
      // if the button press duration exceeds our bounce threshold, then we register a short press
      if (blueDuration > debounceThreshold)
      {
        Serial.println("Blue - Short Press");
        // Cycle through FX
        if (displayed_state == 0){
          curr_pos_fx = (curr_pos_fx + 1) % fx_len;
        }
        Serial.println(curr_pos_fx);
      }
    }
    
    // reset the button active status
    blueActive = false;
    }
  }
  
  //************Check Green Button*************
  // if the button pin reads LOW, the button is pressed (negative/ground switch)
  if (readingGREEN == LOW)
  {
  // mark the button as active, and start the timer
    if (greenActive == false)
    {
      greenActive = true;
      greenTimer = millis();
    }
  
    // calculate the button press duration by subtracting the button time from the boot time
    greenDuration = millis() - greenTimer;
  
    // mark the button as long-pressed if the button press duration exceeds the long press threshold
    if ((greenDuration > longPressThreshold) && (greenLongPress == false))
    {
      greenLongPress = true;
      Serial.println("Green - Long Press");
      //Double Press Action
      //Reset display
      displayed_state = 0;
      curr_pos_fx = 0;
    }
  }
  // button either hasn't been pressed, or has been released
  else
  {
    // if the button was marked as active, it was recently pressed
    if (greenActive == true)
    {
      // reset the long press active state
      if (greenLongPress == true)
      {
        greenLongPress = false;
      }
    
    // we either need to debounce the press (noise) or register a normal/short press
    else
    {
      // if the button press duration exceeds our bounce threshold, then we register a short press
      if (greenDuration > debounceThreshold)
      {
        Serial.println("Green - Short Press");
        // clear neopixels
        wipe();
        
        if (displayed_state == 0){
          send_command();
        }
        
        displayed_state = 0;
      }
    }
    
    // reset the button active status
    greenActive = false;
    }
  }
}
