// ASM Keypad class
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

void
Buttons::expanderWrite(byte _data) {
  Wire.beginTransmission(expander);
  Wire.write(_data);
  Wire.endTransmission();
}

byte
Buttons::expanderRead() {
  byte _data;
  Wire.requestFrom(expander, 1);
  if(Wire.available()) {
    _data = Wire.read();
  }
  return _data;
}

uint8_t
Buttons::pressed(void)
{
  uint8_t         newButton;

  // find what is currently being pressed
  newButton = _scanDebounced();

  // If the new state differs from the previous state, the
  // new state is interesting and we will report it.
  if (newButton != _currentButton) {
    _currentButton = newButton;

    // if we are reporting an actual button press, give
    // audio feedback
    if (0 != newButton)
      beep.play(BEEP_KEY);

    return(newButton);
  }

  // State has not changed, report "no button press"
  /// @note implement auto-repeat here
  return(0);
}

// Scan the buttons
uint8_t
Buttons::_scan(void)
{
  uint8_t scanCode;

  uint8_t code;
  code = expanderRead();
  scanCode = 0;
  if (!(code & 4))
    scanCode = B_DOWN;
  if (!(code & 8))
    scanCode = B_LEFT;
  if (!(code & 16)) {
    scanCode = B_OK;
    //		expanderWrite(B11111101); // Turns the red LED off
  }
  if (!(code & 32))
    scanCode = B_UP;
  if (!(code & 64)) {
    scanCode = B_CANCEL;
    //		expanderWrite(B11111100); // Turns the red LED on
  }
  if (!(code & 128))
    scanCode = B_RIGHT;

  return(scanCode);
}

uint8_t
Buttons::_scanDebounced(void)
{
  uint8_t         scanCode;

  // get initial state, bail immediately if no buttons pressed
  if (0 == (scanCode = _scan())) {
    //		_scanCode = 0;
    _scanStart = millis();
    return(0);
  }

  // if the state changed, bail
  if (scanCode != _scanCode) {
    _scanCode = scanCode;
    _scanStart = millis();
    _lastPress = millis()-BUTTON_REPEAT_TIMER+BUTTON_DEBOUNCE_TIMER;
    return(0);
  }

  // if we've pressed a button recently, ignore
  if ((millis() - _lastPress) < BUTTON_REPEAT_TIMER) {
    return(0);
  }

  // if the state has remained the same for long enough return the button code
  if ((millis() - _scanStart) >= BUTTON_DEBOUNCE_TIMER) {
    _scanStart = millis();
    _lastPress = millis();

    //		// Some test code, play a beep
    //		if (scanCode == B_CANCEL)
    //			beep.play(BEEP_BADKEY);
    //		if (scanCode == B_OK)
    //			beep.play(BEEP_KEY);

    // Return what was pressed
    return(scanCode);
  }

  // None of the above
  return(0);
}




