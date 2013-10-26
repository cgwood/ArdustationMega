// ASM Keypad class
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

void Buttons::expanderWrite(byte _data) {
	Wire.beginTransmission(expander);
	Wire.write(_data);
	Wire.endTransmission();
}

byte Buttons::expanderRead() {
	byte _data;
	Wire.requestFrom(expander, 1);
	if (Wire.available()) {
		_data = Wire.read();
	}
	return _data;
}

uint8_t Buttons::pressed(void) {
	uint8_t newButton;

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

		return (newButton);
	}

	// State has not changed, report "no button press"
	/// @note implement auto-repeat here
	return (0);
}

// Scan the buttons
uint8_t Buttons::_scan(void) {
	uint8_t id;
	int16_t orientation;
	uint8_t scanCode;
	//		expanderWrite(B11111101); // Turns the red LED off
	//		expanderWrite(B11111100); // Turns the red LED on

	id = KEYPAD_ORIENTATION;
	nvram.load_setting(&id, &orientation);

	uint8_t code;
	code = expanderRead();
	scanCode = 0;

	switch (orientation) {
	default:
	case KP_CONN_RIGHT:
		if (!(code & 128))
			scanCode = B_RIGHT;
		if (!(code & 4))
			scanCode = B_DOWN;
		if (!(code & 8))
			scanCode = B_LEFT;
		if (!(code & 32))
			scanCode = B_UP;
		if (!(code & 16))
			scanCode = B_OK;
		if (!(code & 64))
			scanCode = B_CANCEL;

		break;

	case KP_CONN_DOWN:
		if (!(code & 128))
			scanCode = B_DOWN;
		if (!(code & 4))
			scanCode = B_LEFT;
		if (!(code & 8))
			scanCode = B_UP;
		if (!(code & 32))
			scanCode = B_RIGHT;
		if (!(code & 16))
			scanCode = B_OK;
		if (!(code & 64))
			scanCode = B_CANCEL;

		break;

	case KP_CONN_LEFT:
		if (!(code & 128))
			scanCode = B_LEFT;
		if (!(code & 4))
			scanCode = B_UP;
		if (!(code & 8))
			scanCode = B_RIGHT;
		if (!(code & 32))
			scanCode = B_DOWN;
		if (!(code & 16))
			scanCode = B_OK;
		if (!(code & 64))
			scanCode = B_CANCEL;

		break;

	case KP_CONN_UP:
		if (!(code & 128))
			scanCode = B_UP;
		if (!(code & 4))
			scanCode = B_RIGHT;
		if (!(code & 8))
			scanCode = B_DOWN;
		if (!(code & 32))
			scanCode = B_LEFT;
		if (!(code & 16))
			scanCode = B_OK;
		if (!(code & 64))
			scanCode = B_CANCEL;

		break;
	}

	return (scanCode);
}

uint8_t Buttons::_scanDebounced(void) {
	uint8_t scanCode;

	// get initial state, bail immediately if no buttons pressed
	if (0 == (scanCode = _scan())) {
		//		_scanCode = 0;
		_scanStart = millis();
		_scanCodeLast = 0;
		_buttonHolding = 0;

		return (0);
	}

	// if the state changed, save and exit
	if (scanCode != _scanCode) {
		_scanCode = scanCode;
		_scanStart = millis();
		_lastPress = millis() - nvram.nv.keypadRepeatDelay + BUTTON_DEBOUNCE_TIMER;
		return (0);
	}

	// Check if we've registered this button press already
	if (scanCode == _scanCodeLast) {
		// Check to see if we're holding the button down
		if (_buttonHolding) {
			// If it's been long enough since the last repeat of the button, repeat again
			// But only perform repeats on left, right, up and down
			if (scanCode <= B_RIGHT && millis() - _lastPress >= nvram.nv.keypadRepeatDelay) {
				_lastPress = millis();
				return (scanCode);
			}
			else {
				return (0);
			}
		}
		else {
			// If we've held it down long enough, flag the _buttonHolding as true
			if (millis() - _lastPress >= nvram.nv.keypadHoldDelay) {
				_lastPress = millis();
				_buttonHolding = 1;
				if (scanCode > B_RIGHT) {
					// Not a directional code, invoke hold behaviour
					if (scanCode == B_OK)
						return (B_OK_HOLD);
					else if (scanCode == B_CANCEL)
						return (B_CANCEL_HOLD);
					else
						return 0;
				}
				else
					return (scanCode);
			}
			else {
				return (0);
			}
		}
	}

	// if the state has remained the same for long enough return the button code
	if ((millis() - _scanStart) >= BUTTON_DEBOUNCE_TIMER) {
		_scanStart = millis();
		_lastPress = millis();

		// Save the scan code
		_scanCodeLast = _scanCode;

		// Return what was pressed
		return (scanCode);
	}

	// None of the above
	return (0);
}

