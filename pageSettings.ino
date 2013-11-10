/*
 * pageSettings.ino
 *
 *  Created on: 20 Oct 2013
 *      Author: Colin
 */


////////////////////////////////////////////////////////////////////////////////
// Settings page
////////////////////////////////////////////////////////////////////////////////

PageSettings::PageSettings() {
	uint8_t i;

	/// Copy the header and types to internal storage
	_scale = (uint8_t*) SettingScales;
	_decPos = (uint8_t*) SettingDPs;
	_settingsCount = ASM_SETTINGS_COUNT;

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;

	_linecount = LCD_ROWS-2;

	_settingsLCD = gText(0, 16, LCD_COLUMNS, _linecount, SystemFont5x7);
}

uint8_t PageSettings::_enter(void) {
	// Draw the header
	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	GLCD.Printf_P(PSTR("ASM Settings"));
	GLCD.SelectFont(System5x7);

	// Draw the content
	_drawLocal();
}

uint8_t PageSettings::_forceUpdate(uint8_t reason) {

	return 0;
}

uint8_t PageSettings::_redefine(void) {
}

uint8_t PageSettings::_refresh_med() {
	return 0;
}

uint8_t PageSettings::_refresh_slow() {
	return 0;
}

void PageSettings::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	_settingsLCD.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + _linecount; j++) { //Need to go from zero to read through the string
		// If there are less settings than available lines, quit at end of list
		if (j >= _settingsCount)
			break;

		if (j >= _stateFirstVal)
			_settingsLCD.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
            c = pgm_read_byte_near(SettingNames + i++);
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					_settingsLCD.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < _linecount; lineno++) {
		_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _settingsCount)
			break;

		// Load the value, either editing or live val
		if (i == (_state - 101))
			value = _value_temp;
		else {
			nvram.load_setting(&i,&value_local);
			value = (uint32_t) floor(value_local + 0.5);
		}

		char text_value[7];
		nvram.load_setting_text(&i, -32768, text_value);
		_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
		_settingsLCD.print(text_value);
	}
}

/// Draw the "choosing" marker
void PageSettings::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		_settingsLCD.write('>');
	} else if (_state > 100 && _state < 200) {
		_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		_settingsLCD.write('#');
	}
}

/// remove the "choosing" marker
void PageSettings::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		_settingsLCD.write(' ');
	}
}

void PageSettings::_alterLocal(float alterMag) {
	/// Bounds on values
	int16_t min,max;
	uint8_t j;
	j = _state - 101;
	nvram.get_setting_bounds(&j,&min,&max);

//	if (_value_temp + alterMag < min)
//		_value_temp = 0;
//	else if (_value_temp + alterMag > max)
//		_value_temp = max;
//	else
//		_value_temp += alterMag;

	_value_temp = constrain(_value_temp+alterMag,min,max);

	// Keep the encoder value updated
	if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp
				/ (pow(10, _scale[_state - 101] - _decPos[_state - 101]))); // * 100;
	}

	// kick the update function
	_updated = true;
}

void PageSettings::_redrawLocal(void) {
	//	uint8_t         c;
	uint8_t i;
	uint8_t j;
	//	uint8_t         lineno;
	char decBuf[PIDFIELDWIDTH];
	uint32_t value;
	float value_local;
	uint8_t lineno;

	// Check that the state is not in confirmation mode and a value is being pointed at
	if (_state < 200 && _state > 0) {
		if (_state > 100) {
			value_local = _value_temp;
			i = _state - 101;
		} else {
			i = _state - 1;
		}

		// Find the text version of the value
		char text_value[7];
		nvram.load_setting_text(&i, (int16_t)value_local, text_value);

		// Write the value
		lineno = i - _stateFirstVal;
		_settingsLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
		_settingsLCD.print(text_value);
	}
}

void PageSettings::_voidLocal(void) {
	// Reset the variable and redraw
	uint8_t j = (_state % 100) - 1;
	nvram.load_setting(&j,&_value_temp);
	_redrawLocal();

	// Reset _state
	_state = 0;

	// Will also have to redraw the value to its original

	// kick the update function
	_updated = true;
}

void PageSettings::_writeChanges(void) {
	uint8_t j;

	// Make the changes
	j = _state - 101;
	nvram.write_setting(&j,&_value_temp);

	// Reset _state
	_state = 0;
}

uint8_t PageSettings::_interact(uint8_t buttonid) {
	_clearMarker();

	switch (buttonid) {
	case B_UP:
		// Navigation
		if (_state == 0) {
			if (_stateFirstVal > 0) {
				_stateFirstVal--;
				_drawLocal();
			}
		} else if (_state > 1 && _state < 100) {
			if (_state == (_stateFirstVal + 1)) {
				_stateFirstVal--;
				_drawLocal();
			}
			_state--;
		}
		// Editing
		else if (_state > 100 && _state < 200) {
			if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
				if (_value_temp == 0)
					_alterLocal(1);
			} else {
				_alterLocal(1 * pow(10, _scale[_state - 101] - _decPos[_state - 101]));
			}
			_redrawLocal();
		}
		// Confirming
		else if (_state > 200)
			return 0;
		break;
	case B_DOWN:
		// Navigation
		if (_state == 0) {
			if (_stateFirstVal < _settingsCount - _linecount) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _settingsCount) {
			if (_state == (_stateFirstVal + _linecount)) {
				_stateFirstVal++;
				_drawLocal();
			}
			_state++;
		}
		// Editing
		else if (_state > 100 && _state < 200) {
			if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
				if (_value_temp == 1)
					_alterLocal(-1);
			} else {
				_alterLocal(-1 * pow(10, _scale[_state - 101] - _decPos[_state - 101]));
			}
			_redrawLocal();
		}
		// Confirming
		else if (_state > 200)
			return 0;
		break;
	case B_OK:
		if (_state == 0) {
			// Allow selection if the value is available
			if (1)
				_state = _stateFirstVal + 1;
		} else if (_state < 100) {
			// Allow editing if the value is available
			if (1) {
				uint8_t j;
				j = _state - 1;
				nvram.load_setting(&j,&_value_temp);

				// Configure the encoder
				if (_scale[_state - 1] == 99 && 99 == _decPos[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder = (int) (_value_temp / (pow(10, _scale[_state - 1] - _decPos[_state - 1])));
					/// Bounds on values
					int16_t min,max;
					nvram.get_setting_bounds(&j,&min,&max);
					rotary.configure(&_value_encoder, max, min, -4);
				}

				// Copy the value to temp for editing
//                    memcpy(&_value_temp, _value_live[_state-1], sizeof(_value_temp));
				//_value_temp.valueID = _value_live[_state-1].valueID;
				//_value_temp.value = _value_live[_state-1].value;
				// Update the state
				_state += 100;
			}
		} else if (_state > 100 && _state < 200) {
			// Save the value
			//_leave(OK);
			_writeChanges();
//			_state += 100;
			return 0; // Leave before we draw the marker again
		} else if (_state > 200) { // Should never happen...
			// Save the value
			//_leave(OK);
			_writeChanges();
			return 0; // Leave before we draw the marker again
		}
		break;
	case B_LEFT:
		// Navigation
//		if (_state == 0)
//			Pages::move(-1);
		// Editing
		if (_state > 100 && _state < 200
				&& !(_scale[_state - 101] == 99 && 99 == _decPos[_state - 101])) {
			_alterLocal(-10 * pow(10, _scale[_state - 101] - _decPos[_state - 101]));
			_redrawLocal();
		}
		// Confirming
		else if (_state > 200)
			return 0;
		break;
	case B_RIGHT:
		// Navigation
//		if (_state == 0)
//			Pages::move(1);
		// Editing
		if (_state > 100 && _state < 200
				&& !(_scale[_state - 101] == 99 && 99 == _decPos[_state - 101])) {
			_alterLocal(10 * pow(10, _scale[_state - 101] - _decPos[_state - 101]));
			_redrawLocal();
		}
		// Confirming
		else if (_state > 200)
			return 0;
		break;
	case B_CANCEL:
		if (_state == 0) {
			Pages::move(0);
			return 0;         // avoid drawing the cursor
		} else {
			if (_state > 200) {
				_drawLocal();
				_state = 0;
			} else if (_state > 100)
				// Don't save the changes to local variable
				_voidLocal();
			else
				_state = 0;
		}
		break;
	case B_ENCODER:
		if (_state > 100 && _state < 200) {
			if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
				_value_temp = _value_encoder;
			} else {
				_value_temp = _value_encoder
						* pow(10, _scale[_state - 101] - _decPos[_state - 101]);
			}
			_redrawLocal();
//			Serial.println(_value_encoder);
//			Serial.println(_value_temp);
		}
		break;
	}
	_paintMarker();
}







