//************************ CHECK BUTTONS *******************************/
void check_buttons(){
  int readingA = digitalRead(BUTTON_A);
  int readingB = digitalRead(BUTTON_B);
  int readingC = digitalRead(BUTTON_C);
  int readingD = digitalRead(BUTTON_D);
  
  //************Check Button A*************
  // If the switch changed, due to noise or pressing:
  if (readingA != lastButtonStateA) {
    // reset the debouncing timer
    lastDebounceTimeA = millis();
  }

  if ((millis() - lastDebounceTimeA) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingA != buttonStateA) {
      buttonStateA = readingA;
      lastActivity = millis();

      // only toggle the LED if the new button state is HIGH
      if (buttonStateA == HIGH) {   
        Serial.println("A");
        a_button_action();
      }
    }
  }
  lastButtonStateA = readingA;

    //************Check Button B*************
  // if the button pin reads LOW, the button is pressed (negative/ground switch)
  if (readingB == LOW){
  // mark the button as active, and start the timer
    if (BActive == false){
      BActive = true;
      BTimer = millis();
      lastActivity = millis();
    }
  
    // calculate the button press duration by subtracting the button time from the boot time
    BDuration = millis() - BTimer;
    
    // mark the button as long-pressed if the button press duration exceeds the long press threshold
    if ((BDuration > longPressThresholdB) && (BLongPress == false)){
      BLongPress = true;
      Serial.println("B - Long Press");
    //  Reset everything, return home
      displayed_state = 0;
      curr_pos_fx = 0;  
      curr_pos_friends = 0;  
      disp_length = 3;
      displayed_min = 0;
      
    }
    if (BDuration > longPressThresholdB){
      BCurrentLongPress = true;
      
      
    }
  }
  // button either hasn't been pressed, or has been released
  else{
    BCurrentLongPress = false;
    // if the button was marked as active, it was recently pressed
    if (BActive == true){
      // reset the long press active state
      if (BLongPress == true){
        BLongPress = false;
      }
    
    // we either need to debounce the press (noise) or register a normal/short press
    else{
      // if the button press duration exceeds our bounce threshold, then we register a short press
      if (BDuration > debounceDelay){
        Serial.println("B - Short Press");
        b_button_action();        
      }
    }
    // reset the button active status
    BActive = false;
    
    }
  }
  
  //************Check Button C*************
  // If the switch changed, due to noise or pressing:
  if (readingC != lastButtonStateC) {
    // reset the debouncing timer
    lastDebounceTimeC = millis();
  }

  if ((millis() - lastDebounceTimeC) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readingC != buttonStateC) {
      buttonStateC = readingC;
      lastActivity = millis();

      // only toggle the LED if the new button state is HIGH
      if (buttonStateC == HIGH) {
        Serial.println("C");   
        c_button_action();
      }
    }
  }
  lastButtonStateC = readingC;
  
  //************Check Big Button*************
  // if the button pin reads LOW, the button is pressed (negative/ground switch)
  if (readingD == LOW)
  {
  // mark the button as active, and start the timer
    if (DActive == false)
    {
      DActive = true;
      lastActivity = millis();
      DTimer = millis();
    }
  
    // calculate the button press duration by subtracting the button time from the boot time
    DDuration = millis() - DTimer;

   
    // mark the button as long-pressed if the button press duration exceeds the long press threshold
    if ((DDuration > longPressThresholdD) && (DLongPress == false))
    {
      DLongPress = true;
      Serial.println("D - Long Press");
      //Long Press Action

      
      
    }
    if (DDuration > longPressThresholdD){
      DCurrentLongPress = true;
      
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = analogRead(A_PIN);
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex = readIndex + 1;
      // if we're at the end of the array...
      if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
      }

      // calculate the average:
      int neo_min = 10;
      int neo_max = 255;
      average = total / numReadings;
      neo_brightness = 250 * (average - pot_min)/(pot_max - pot_min);
    }
  }
  // button either hasn't been pressed, or has been released
  else
  {
    DCurrentLongPress = false;
    // if the button was marked as active, it was recently pressed
    if (DActive == true)
    {
      // reset the long press active state
      if (DLongPress == true)
      {
        DLongPress = false;
      }
    
    // we either need to debounce the press (noise) or register a normal/short press
    else
    {
      // if the button press duration exceeds our bounce threshold, then we register a short press
      if (DDuration > debounceDelay)
      {
        //D - Short Press
//        Serial.println("D - Short Press");
        
      }
    }
    
    // reset the button active status
    DActive = false;
    }
  }
}

void a_button_action(){
  switch(displayed_state) {
      case 2:
        curr_pos_friends = ((curr_pos_friends - 1) % friend_len + friend_len) % friend_len;
        break;
      case 3:
        curr_pos_fx = ((curr_pos_fx - 1) % fx_len + fx_len) % fx_len;
        break;   
      case 4:
        curr_pos_send = ((curr_pos_send - 1) % send_len + send_len) % send_len;
        break;

      case 7:
          displayed_state = 0;
          curr_pos_fx = 0;  
          curr_pos_friends = 0;  
          disp_length = 3;
          displayed_min = 0;
          break;
  }
}

void b_button_action(){
  
  display.clearDisplay(); 

  if (displayed_state == 6){
    displayed_state = 0;
    curr_pos_friends = 0;  
    disp_length = 3;
    displayed_min = 0;
  }

  else if (displayed_state == 7){
    displayed_state = 0;
    curr_pos_friends = 0;  
    disp_length = 3;
    displayed_min = 0;
  }
  
 
  if (displayed_state > num_of_states){
    displayed_state ++;
    displayed_state = displayed_state % num_of_states;
    curr_pos_fx = 0;  
    curr_pos_friends = 0;  
    disp_length = 3;
    displayed_min = 0;
  }
  
}

void c_button_action(){

  switch(displayed_state) {
    case 2:
      curr_pos_friends = (curr_pos_friends + 1) % friend_len;
      break;
      
    case 3:
      curr_pos_fx = (curr_pos_fx + 1) % fx_len;
      break;

    case 4:
      curr_pos_send = (curr_pos_send + 1) % send_len;
      break;

    case 7:
      displayed_state = 0;
      curr_pos_fx = 0;  
      curr_pos_friends = 0;  
      disp_length = 3;
      displayed_min = 0;
      break;
  }
}

void d_button_action(){
  
}
