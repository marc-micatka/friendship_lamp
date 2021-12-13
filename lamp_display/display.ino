//************************ DISPLAY STUFF *******************************/
void display_screen(){
  switch(displayed_state) {
    case 1:
      display_homescreen();
      break;
    case 2:
      display_friend_list(curr_pos_friends);
      break;
    case 3:
      display_fx_list(curr_pos_fx);
      break;
    case 4:
      display_send(curr_pos_send);
      break;
    case 5:
      send_message();
      break;
    case 6:
      receiving_message();
      break;
    case 7:
      display_timeout();
      break;
  }
}

void receiving_message(){
      // text display tests
  display.setTextSize(1);
  display.setCursor(5, 10);
  
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.print("Message from ");
  display.print(sender);
  display.print("!");
  display.display(); // actually display all of the above

//  Serial.println(fx_sent);
  switch(fx_sent) {
      case 0:
        solid(color_sent);
        break;
      case 1:
        pulse(color_sent, 1);
        break;
      case 2:
        rainbow();
        break;
      case 3:
        colorWipe(color_sent);
        break;
    }
}

void display_timeout(){
  display.fillRect(0, 0, display_width, display_height, SH110X_BLACK);
  display.display();
}

void send_message(){
  int16_t  x1, y1;
  uint16_t w, h;

  if (curr_pos_send == 1){
    display.display();
    display.clearDisplay();
  }
  else{
    if (send_command()){
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    display.getTextBounds("Sent!",0, 0, &x1, &y1, &w, &h);
    display.setCursor((display_width - w)/2, 20);
    display.print("Sent!");
  }
  else{
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    display.getTextBounds("Not Sent. Try Again!",0, 0, &x1, &y1, &w, &h);
    display.setCursor((display_width - w)/2, 30);
    display.print("Not Sent. Try Again!");
    }
    
    display.display();
    delay(1000);
    display.clearDisplay();
  }
    
  // Return home
  displayed_state = 1;
  
}

void display_send(int curr_pos){
  // text display tests
  // Keep displaying fx though cause it's fun
  switch(curr_pos_fx) {
    case 0:
      solid(displayed_color);
      break;
    case 1:
      pulse(displayed_color, 1);
      break;
    case 2:
      rainbow();
      break;
    case 3:
      colorWipe(displayed_color);
      break;
  }
  
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.setTextColor(SH110X_WHITE);
  display.print("Sending ");
  display.println(send_msgs[idx]);
  display.print(" to ");
  display.print(friends[curr_pos_friends]);
  
  for(int i = 0; i<send_len; i++) {
    
    if (i == curr_pos){
      display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    }
    else{
      display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    }
    display.setCursor(10, 30 + char_height + i * (char_height+ 2));
    display.print(send_opts[i]);
   }
  
  display.display(); // actually display all of the above
}


void display_fx_list(int curr_pos){
  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5, 5);
  display.println("Choose Color and FX:");
  
  // Wrap text and scroll through only 4 at once
  if (curr_pos < displayed_min){
    displayed_min --;
    displayed_min = min(displayed_min, curr_pos);
    displayed_max = displayed_min + disp_length;
    display.clearDisplay();
    
  } else if(curr_pos > displayed_max){
    displayed_max ++;
    displayed_max = max(displayed_max, curr_pos);
    displayed_min = displayed_max - disp_length;
    display.clearDisplay();
  }

  int spacing = 0;
  for(int j = displayed_min; j <= displayed_max; j++) {
    spacing ++;
    
    if (j == curr_pos){
      display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    }
    else{
      display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    }
    display.setCursor(10, char_height + spacing * (char_height+ 2));
    display.print(fx[j]);
   }
   
   display.display(); // actually display all of the above

  select_color();

  // Displayed the selected FX
  /*
   * String fx[] = { 
  "Solid", 
  "Pulse", 
  "Rainbow",
  "Color Wipe"
};
   */
  switch(curr_pos) {
    case 0:
      solid(displayed_color);
      break;
    case 1:
      pulse(displayed_color, 1);
      break;
    case 2:
      rainbow();
      break;
    case 3:
      colorWipe(displayed_color);
      break;
  }
}

void select_color(){
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
  average = total / numReadings;
  potVal = 100 * (average - pot_min)/(pot_max - pot_min);
  
  // Get the current color from the potentiometer
  choose_color(potVal);
}

void select_fx(){
  
  select_color();
  
  switch(curr_pos_fx) {
    case 0:
      solid(displayed_color);
      break;
    case 1:
      pulse(displayed_color, 1);
      break;
    case 2:
      rainbow();
      break;
    case 3:
      colorWipe(displayed_color);
      break;
  }
}

void display_homescreen(){
  int16_t  x1, y1;
  uint16_t w, h;
 
  select_fx();   
  
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  
  display.getTextBounds("Friendship",0, 0, &x1, &y1, &w, &h);
//  display.setCursor((display_width - w)/2, 15);
  display.setCursor((display_width - w)/2, 5);
  display.println("Friendship");
  
  display.getTextBounds("Lamp",0, 0, &x1, &y1, &w, &h);
//  display.setCursor((display_width - w)/2, 5 + h);
  display.setCursor(15, 15 + h);
  display.println("Lamp");

  display.drawBitmap(w + 30, 26, epd_bitmap_bulb, 32, 32, SH110X_WHITE);

  display.display();
}

void display_friend_list(int curr_pos){
  // text display tests
  select_fx();  
  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 5);
  display.println("Send To");

  for(int i = 0; i<friend_len; i++) {
    
    if (i == curr_pos){
      display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    }
    else{
      display.setTextColor(SH110X_WHITE, SH110X_BLACK);
    }
    display.setCursor(10, 10 + char_height + i * (char_height+ 2));
    display.print(friends[i]);
   }
   
   display.display(); // actually display all of the above
}
