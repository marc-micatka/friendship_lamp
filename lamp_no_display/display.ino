//************************ DISPLAY STUFF *******************************/
void display_state(){
  switch(displayed_state){
    case 0:
      select_fx();
      break;
    case 1:
      display_fx();
      break;
  }
}

void select_fx(){
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
    case 4:
      wipe();
      break;
  }
}

void display_fx(){
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
