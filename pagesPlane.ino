// Created 2011 By Colin G http://www.diydrones.com/profile/ColinG

// These defines are the values for bitmasking logging stuff
#define MASK_LOG_ATTITUDE_FAST 0
#define MASK_LOG_ATTITUDE_MED 2
#define MASK_LOG_GPS 4
#define MASK_LOG_PM 8
#define MASK_LOG_CTUN 16
#define MASK_LOG_NTUN 32
#define MASK_LOG_MODE 64
#define MASK_LOG_RAW 128
#define MASK_LOG_CMD 256
// bits in log_bitmask
#define LOGBIT_ATTITUDE_FAST	(1<<0)
#define LOGBIT_ATTITUDE_MED	(1<<1)
#define LOGBIT_GPS		(1<<2)
#define LOGBIT_PM		(1<<3)
#define LOGBIT_CTUN		(1<<4)
#define LOGBIT_NTUN		(1<<5)
#define LOGBIT_MODE		(1<<6)
#define LOGBIT_RAW		(1<<7)
#define LOGBIT_CMD		(1<<8)

#define LINECOUNT 8

////////////////////////////////////////////////////////////////////////////////
// Parameters page
////////////////////////////////////////////////////////////////////////////////

PagePlaneParameters::PagePlaneParameters() {
	uint8_t i;

	/// Copy the header and types to internal storage
//	_textHeader = ParamNamesPlane;
	_scale = (uint8_t*) ParamScales;
	_decPos = (uint8_t*) ParamDPs;
	_paramCount = PARAM_COUNT_PLANE;

	// Set the parameter IDs - requires parameter IDs to be consecutive (later something more robust?)
	for (i = 0; i < _paramCount; i++) {
		_Types[i] = i;
	}

	/// Initially, no values are available
	for (i = 0; i < _paramCount; i++) {
		_avail[i] = 1;
	}

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;
}

uint8_t PagePlaneParameters::_enter(void) {
	_drawLocal();
}

uint8_t PagePlaneParameters::_forceUpdate(uint8_t reason) {
	if (reason == R_PARAM) {
		// Exit edit mode
		_state = 0;

		// Redraw the entire page
		_drawLocal();
	}

	return 0;
}

uint8_t PagePlaneParameters::_redefine(void) {
}

uint8_t PagePlaneParameters::_refresh_med() {
	return 0;
}

uint8_t PagePlaneParameters::_refresh_slow() {
	return 0;
}

void PagePlaneParameters::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	lcd.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + LINECOUNT; j++) { //Need to go from zero to read through the string
		// If there are less parameters than available lines, quit at end of list
		if (j >= _paramCount)
			break;

		if (j >= _stateFirstVal)
			lcd.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
//              c = pgm_read_byte_near(_textHeader + i++);
			if (uav.type == MAV_TYPE_FIXED_WING) {
				c = pgm_read_byte_near(ParamNamesPlane + i++);
			} else if (uav.type == MAV_TYPE_GROUND_ROVER) {
				c = pgm_read_byte_near(ParamNamesRover + i++);
			} else {
				c = pgm_read_byte_near(ParamNamesPlane + i++);
			}
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					lcd.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < LINECOUNT; lineno++) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _paramCount)
			break;

//		if (_avail[i] == 1) {
		if (uav.param_plane_avail[i] == 1) {
			// Load the value, either editing or live val
			if (i == (_state - 101))
				value = _value_temp;
			else {
				j = _Types[i];
				//nvram.load_param(&j,&value_local);
				value_local = uav.param_plane[j];
				value = (uint32_t) floor(value_local + 0.5);
			}
			//value = _value_live[i].value;

			// Is it an On / Off value?
			if (99 == _decPos[i] && 99 == _scale[i]) {
				if (value > 0) {
					decBuf[0] = 'N';
					decBuf[1] = 'O';
					j = 2;
				} else {
					decBuf[0] = 'F';
					decBuf[1] = 'F';
					decBuf[2] = 'O';
					j = 3;
				}

				// Display the data
				//lcd.write(' ');
				while (j-- > 0)
					lcd.write(decBuf[j]);
				lcd.write(' ');
			} else {
				GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
				GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
				GLCD.print("  ");
//              // Scale the value and fix for the number of decimal places
//              value *= pow(10,_decPos[i] - _scale[i]);
//              decBuf[0] = '0' + (value % 10);
//              value /= 10;
//              for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//                if (j == _decPos[i]) {
//                  decBuf[j] = '.';
//                } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//                  decBuf[j] = ' ';
//                } else {
//                  decBuf[j] = '0' + (value % 10);
//                  value /= 10;
//                }
//              }
			}
		} else {
			// Data unavailable, display dashes
			for (j = 0; j < 16 - PARAMNAMEFIELDWIDTH; j++)
				decBuf[j] = '-';

			// Display the data
			lcd.write(' ');
			while (j-- > 0)
				lcd.write(decBuf[j]);
			lcd.write(' ');
		}
	}

	// redraw the "choosing" marker
//        _paintMarker();
}

/// Draw the "choosing" marker
void PagePlaneParameters::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write('>');
	} else if (_state > 100 && _state < 200) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		lcd.write('#');
	}
}

/// remove the "choosing" marker
void PagePlaneParameters::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write(' ');
	}
}

void PagePlaneParameters::_alterLocal(float alterMag) {
	// Is it an on off?
//  if (99 == _scale[_state-101] && 99 == _decPos[_state-101]) {
//	Serial.println(alterMag, DEC);
//    if (alterMag > 0) {
//      switch (_value_temp) {
//        case 0x50: _value_temp = LOGBIT_ATTITUDE_FAST; break;
//        case 0x51: _value_temp = LOGBIT_ATTITUDE_MED;  break;
//        case 0x52: _value_temp = LOGBIT_GPS;           break;
//        case 0x53: _value_temp = LOGBIT_PM;            break;
//        case 0x54: _value_temp = LOGBIT_CTUN;          break;
//        case 0x55: _value_temp = LOGBIT_NTUN;          break;
//        case 0x56: _value_temp = LOGBIT_MODE;          break;
//        case 0x57: _value_temp = LOGBIT_RAW;           break;
//        case 0x58: _value_temp = LOGBIT_CMD;           break;
//      }
//    }
//    else
//      _value_temp = 0;
//  }
//  else {
	// We don't do negative values here
	if (_value_temp + alterMag < 0)
		_value_temp = 0;
	else
		_value_temp += alterMag;
//  }

	// Keep the encoder value updated
	if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp
				/ (pow(10, _scale[_state - 101] - _decPos[_state - 101]))); // * 100;
	}

//	Serial.println(_value_temp);

	// kick the update function
	_updated = true;
}

void PagePlaneParameters::_redrawLocal(void) {
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

		// Write the value
		lineno = i - _stateFirstVal;
		GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		// Is it an On / Off value?
		if (99 == _decPos[i] && 99 == _scale[i]) {
			if (value_local > 0) {
				GLCD.print("ON ");
			} else {
				GLCD.print("OFF");
			}
		} else {
			GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
			GLCD.print("  ");
		}

//    value = (uint32_t)(floor(value_local+0.5));
//    value *= pow(10,_decPos[i] - _scale[i]);
//    Serial.println(value);
//    decBuf[0] = '0' + (value % 10);
//    value /= 10;
//    for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//      if (j == _decPos[i]) {
//        decBuf[j] = '.';
//      } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//        decBuf[j] = ' ';
//      } else {
//        decBuf[j] = '0' + (value % 10);
//        value /= 10;
//      }
//    }
//
//    // Display the data
//    while (j-- > 0)
//      lcd.write(decBuf[j]);

	}
}

void PagePlaneParameters::_voidLocal(void) {
	// Reset the variable and redraw
	int j = _Types[(_state % 100) - 1];
	_value_temp = uav.param_plane[j];
	_redrawLocal();

	// Reset _state
	_state = 0;

	// Will also have to redraw the value to its original

	// kick the update function
	_updated = true;
}

void PagePlaneParameters::_uploadConfirm(void) {
	uint8_t c;
	uint8_t row;
	const prog_char *str;
	str = confirmMessage;

	lcd.ClearArea();
	row = 0;

	for (;;) {
		c = pgm_read_byte_near(str++);
		if (0 == c)
			break;
		if ('\n' == c) {
			lcd.CursorTo(0, ++row);
			continue;
		}
		// emit
		lcd.write(c);
	}
}

void PagePlaneParameters::_uploadLocal(void) {
	uint8_t i,j;
	char str_param_id[16];

	// Send the value that we edited
	j = _Types[_state - 201];

	// First initialise the string to be empty
	for (i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
	strcpy_P(str_param_id, (char*)pgm_read_word(&(paramTable_plane[j])));

	// Upload the value
	gcs3.param_set(j, _value_temp, str_param_id);

	// Reset _state
	_state = 0;

	// kick the update function
	_updated = true;
}

uint8_t PagePlaneParameters::_interact(uint8_t buttonid) {
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
			if (_stateFirstVal < _paramCount - LINECOUNT) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _paramCount) {
			if (_state == (_stateFirstVal + LINECOUNT)) {
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
			if (_avail[_stateFirstVal])
				_state = _stateFirstVal + 1;
		} else if (_state < 100) {
			// Allow editing if the value is available
			if (_avail[(_state - 1)]) {
				uint8_t j;
				j = _Types[_state - 1];
				//nvram.load_param(&j,&_value_temp);
				_value_temp = uav.param_plane[j];

				// Configure the encoder
				if (_scale[_state - 1] == 99 && 99 == _decPos[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder =
							(int) (_value_temp
									/ (pow(10,
											_scale[_state - 1]
													- _decPos[_state - 1]))); // * 100;
					rotary.configure(&_value_encoder, 1000, 0, -4);
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
			_state += 100;
			_uploadConfirm();
			return 0; // Leave before we draw the marker again
		} else if (_state > 200) {
			// Save the value
			//_leave(OK);
			_uploadLocal();
			return 0; // Leave before we draw the marker again
		}
		break;
	case B_LEFT:
		// Navigation
		if (_state == 0)
			Pages::move(-1);
		// Editing
		else if (_state > 100 && _state < 200
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
		if (_state == 0)
			Pages::move(1);
		// Editing
		else if (_state > 100 && _state < 200
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



////////////////////////////////////////////////////////////////////////////////
// Control Tuning Parameters page
////////////////////////////////////////////////////////////////////////////////

PagePlaneParametersCTUN::PagePlaneParametersCTUN() {
	uint8_t i;

	/// Copy the header and types to internal storage
	_textHeader = ParamNamesPlaneCTUN;
	_scale = (uint8_t*) ParamScalesCTUN;
	_decPos = (uint8_t*) ParamDPsCTUN;
	_paramCount = PARAM_COUNT_PLANE_CTUN;

	// Set the parameter IDs - requires parameter IDs to be consecutive (later something more robust?)
	for (i = 0; i < _paramCount; i++) {
		_Types[i] = i;
	}

	/// Initially, no values are available
	for (i = 0; i < _paramCount; i++) {
		_avail[i] = 1;
	}

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;
}

uint8_t PagePlaneParametersCTUN::_enter(void) {
	_drawLocal();
}

uint8_t PagePlaneParametersCTUN::_forceUpdate(uint8_t reason) {
	if (reason == R_PARAM) {
		// Exit edit mode
		_state = 0;

		// Redraw the entire page
		_drawLocal();
	}

	return 0;
}

uint8_t PagePlaneParametersCTUN::_redefine(void) {
}

uint8_t PagePlaneParametersCTUN::_refresh_med() {
	return 0;
}

uint8_t PagePlaneParametersCTUN::_refresh_slow() {
	return 0;
}

void PagePlaneParametersCTUN::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	lcd.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + LINECOUNT; j++) { //Need to go from zero to read through the string
		// If there are less parameters than available lines, quit at end of list
		if (j >= _paramCount)
			break;

		if (j >= _stateFirstVal)
			lcd.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
            c = pgm_read_byte_near(_textHeader + i++);
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					lcd.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < LINECOUNT; lineno++) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _paramCount)
			break;

//		if (_avail[i] == 1) {
		if (uav.param_plane_ctun_avail[i] == 1) {
			// Load the value, either editing or live val
			if (i == (_state - 101))
				value = _value_temp;
			else {
				j = _Types[i];
				//nvram.load_param(&j,&value_local);
				value_local = uav.param_plane_ctun[j];
				value = (uint32_t) floor(value_local + 0.5);
			}
			//value = _value_live[i].value;

			// Is it an On / Off value?
			if (99 == _decPos[i] && 99 == _scale[i]) {
				if (value > 0) {
					decBuf[0] = 'N';
					decBuf[1] = 'O';
					j = 2;
				} else {
					decBuf[0] = 'F';
					decBuf[1] = 'F';
					decBuf[2] = 'O';
					j = 3;
				}

				// Display the data
				//lcd.write(' ');
				while (j-- > 0)
					lcd.write(decBuf[j]);
				lcd.write(' ');
			} else {
				GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
				GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
				GLCD.print("  ");
//              // Scale the value and fix for the number of decimal places
//              value *= pow(10,_decPos[i] - _scale[i]);
//              decBuf[0] = '0' + (value % 10);
//              value /= 10;
//              for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//                if (j == _decPos[i]) {
//                  decBuf[j] = '.';
//                } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//                  decBuf[j] = ' ';
//                } else {
//                  decBuf[j] = '0' + (value % 10);
//                  value /= 10;
//                }
//              }
			}
		} else {
			// Data unavailable, display dashes
			for (j = 0; j < 16 - PARAMNAMEFIELDWIDTH; j++)
				decBuf[j] = '-';

			// Display the data
			lcd.write(' ');
			while (j-- > 0)
				lcd.write(decBuf[j]);
			lcd.write(' ');
		}
	}

	// redraw the "choosing" marker
//        _paintMarker();
}

/// Draw the "choosing" marker
void PagePlaneParametersCTUN::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write('>');
	} else if (_state > 100 && _state < 200) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		lcd.write('#');
	}
}

/// remove the "choosing" marker
void PagePlaneParametersCTUN::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write(' ');
	}
}

void PagePlaneParametersCTUN::_alterLocal(float alterMag) {
	// Is it an on off?
//  if (99 == _scale[_state-101] && 99 == _decPos[_state-101]) {
//	Serial.println(alterMag, DEC);
//    if (alterMag > 0) {
//      switch (_value_temp) {
//        case 0x50: _value_temp = LOGBIT_ATTITUDE_FAST; break;
//        case 0x51: _value_temp = LOGBIT_ATTITUDE_MED;  break;
//        case 0x52: _value_temp = LOGBIT_GPS;           break;
//        case 0x53: _value_temp = LOGBIT_PM;            break;
//        case 0x54: _value_temp = LOGBIT_CTUN;          break;
//        case 0x55: _value_temp = LOGBIT_NTUN;          break;
//        case 0x56: _value_temp = LOGBIT_MODE;          break;
//        case 0x57: _value_temp = LOGBIT_RAW;           break;
//        case 0x58: _value_temp = LOGBIT_CMD;           break;
//      }
//    }
//    else
//      _value_temp = 0;
//  }
//  else {
	// We don't do negative values here
	if (_value_temp + alterMag < 0)
		_value_temp = 0;
	else
		_value_temp += alterMag;
//  }

	// Keep the encoder value updated
	if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp
				/ (pow(10, _scale[_state - 101] - _decPos[_state - 101]))); // * 100;
	}

//	Serial.println(_value_temp);

	// kick the update function
	_updated = true;
}

void PagePlaneParametersCTUN::_redrawLocal(void) {
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

		// Write the value
		lineno = i - _stateFirstVal;
		GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		// Is it an On / Off value?
		if (99 == _decPos[i] && 99 == _scale[i]) {
			if (value_local > 0) {
				GLCD.print("ON ");
			} else {
				GLCD.print("OFF");
			}
		} else {
			GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
			GLCD.print("  ");
		}

//    value = (uint32_t)(floor(value_local+0.5));
//    value *= pow(10,_decPos[i] - _scale[i]);
//    Serial.println(value);
//    decBuf[0] = '0' + (value % 10);
//    value /= 10;
//    for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//      if (j == _decPos[i]) {
//        decBuf[j] = '.';
//      } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//        decBuf[j] = ' ';
//      } else {
//        decBuf[j] = '0' + (value % 10);
//        value /= 10;
//      }
//    }
//
//    // Display the data
//    while (j-- > 0)
//      lcd.write(decBuf[j]);

	}
}

void PagePlaneParametersCTUN::_voidLocal(void) {
	// Reset the variable and redraw
	int j = _Types[(_state % 100) - 1];
	_value_temp = uav.param_plane_ctun[j];
	_redrawLocal();

	// Reset _state
	_state = 0;

	// Will also have to redraw the value to its original

	// kick the update function
	_updated = true;
}

void PagePlaneParametersCTUN::_uploadConfirm(void) {
	uint8_t c;
	uint8_t row;
	const prog_char *str;
	str = confirmMessage;

	lcd.ClearArea();
	row = 0;

	for (;;) {
		c = pgm_read_byte_near(str++);
		if (0 == c)
			break;
		if ('\n' == c) {
			lcd.CursorTo(0, ++row);
			continue;
		}
		// emit
		lcd.write(c);
	}
}

void PagePlaneParametersCTUN::_uploadLocal(void) {
	uint8_t i,j;
	char str_param_id[16];

	// Send the value that we edited
	j = _Types[_state - 201];

	// First initialise the string to be empty
	for (i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
	strcpy_P(str_param_id, (char*)pgm_read_word(&(paramTable_plane_ctun[j])));

	// Upload the value
	gcs3.param_set(j, _value_temp, str_param_id);

	// Reset _state
	_state = 0;

	// kick the update function
	_updated = true;
}

uint8_t PagePlaneParametersCTUN::_interact(uint8_t buttonid) {
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
			if (_stateFirstVal < _paramCount - LINECOUNT) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _paramCount) {
			if (_state == (_stateFirstVal + LINECOUNT)) {
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
			if (_avail[_stateFirstVal])
				_state = _stateFirstVal + 1;
		} else if (_state < 100) {
			// Allow editing if the value is available
			if (_avail[(_state - 1)]) {
				uint8_t j;
				j = _Types[_state - 1];
				//nvram.load_param(&j,&_value_temp);
				_value_temp = uav.param_plane_ctun[j];

				// Configure the encoder
				if (_scale[_state - 1] == 99 && 99 == _decPos[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder =
							(int) (_value_temp
									/ (pow(10,
											_scale[_state - 1]
													- _decPos[_state - 1]))); // * 100;
					rotary.configure(&_value_encoder, 1000, 0, -4);
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
			_state += 100;
			_uploadConfirm();
			return 0; // Leave before we draw the marker again
		} else if (_state > 200) {
			// Save the value
			//_leave(OK);
			_uploadLocal();
			return 0; // Leave before we draw the marker again
		}
		break;
	case B_LEFT:
		// Navigation
		if (_state == 0)
			Pages::move(-1);
		// Editing
		else if (_state > 100 && _state < 200
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
		if (_state == 0)
			Pages::move(1);
		// Editing
		else if (_state > 100 && _state < 200
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

////////////////////////////////////////////////////////////////////////////////
// Control Tuning Parameters page
////////////////////////////////////////////////////////////////////////////////

PagePlaneParametersNTUN::PagePlaneParametersNTUN() {
	uint8_t i;

	/// Copy the header and types to internal storage
	_textHeader = ParamNamesPlaneNTUN;
	_scale = (uint8_t*) ParamScalesNTUN;
	_decPos = (uint8_t*) ParamDPsNTUN;
	_paramCount = PARAM_COUNT_PLANE_NTUN;

	// Set the parameter IDs - requires parameter IDs to be consecutive (later something more robust?)
	for (i = 0; i < _paramCount; i++) {
		_Types[i] = i;
	}

	/// Initially, no values are available
	for (i = 0; i < _paramCount; i++) {
		_avail[i] = 1;
	}

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;
}

uint8_t PagePlaneParametersNTUN::_enter(void) {
	_drawLocal();
}

uint8_t PagePlaneParametersNTUN::_forceUpdate(uint8_t reason) {
	if (reason == R_PARAM) {
		// Exit edit mode
		_state = 0;

		// Redraw the entire page
		_drawLocal();
	}

	return 0;
}

uint8_t PagePlaneParametersNTUN::_redefine(void) {
}

uint8_t PagePlaneParametersNTUN::_refresh_med() {
	return 0;
}

uint8_t PagePlaneParametersNTUN::_refresh_slow() {
	return 0;
}

void PagePlaneParametersNTUN::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	lcd.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + LINECOUNT; j++) { //Need to go from zero to read through the string
		// If there are less parameters than available lines, quit at end of list
		if (j >= _paramCount)
			break;

		if (j >= _stateFirstVal)
			lcd.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
            c = pgm_read_byte_near(_textHeader + i++);
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					lcd.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < LINECOUNT; lineno++) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _paramCount)
			break;

//		if (_avail[i] == 1) {
		if (uav.param_plane_ntun_avail[i] == 1) {
			// Load the value, either editing or live val
			if (i == (_state - 101))
				value = _value_temp;
			else {
				j = _Types[i];
				//nvram.load_param(&j,&value_local);
				value_local = uav.param_plane_ntun[j];
				value = (uint32_t) floor(value_local + 0.5);
			}
			//value = _value_live[i].value;

			// Is it an On / Off value?
			if (99 == _decPos[i] && 99 == _scale[i]) {
				if (value > 0) {
					decBuf[0] = 'N';
					decBuf[1] = 'O';
					j = 2;
				} else {
					decBuf[0] = 'F';
					decBuf[1] = 'F';
					decBuf[2] = 'O';
					j = 3;
				}

				// Display the data
				//lcd.write(' ');
				while (j-- > 0)
					lcd.write(decBuf[j]);
				lcd.write(' ');
			} else {
				GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
				GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
				GLCD.print("  ");
//              // Scale the value and fix for the number of decimal places
//              value *= pow(10,_decPos[i] - _scale[i]);
//              decBuf[0] = '0' + (value % 10);
//              value /= 10;
//              for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//                if (j == _decPos[i]) {
//                  decBuf[j] = '.';
//                } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//                  decBuf[j] = ' ';
//                } else {
//                  decBuf[j] = '0' + (value % 10);
//                  value /= 10;
//                }
//              }
			}
		} else {
			// Data unavailable, display dashes
			for (j = 0; j < 16 - PARAMNAMEFIELDWIDTH; j++)
				decBuf[j] = '-';

			// Display the data
			lcd.write(' ');
			while (j-- > 0)
				lcd.write(decBuf[j]);
			lcd.write(' ');
		}
	}

	// redraw the "choosing" marker
//        _paintMarker();
}

/// Draw the "choosing" marker
void PagePlaneParametersNTUN::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write('>');
	} else if (_state > 100 && _state < 200) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		lcd.write('#');
	}
}

/// remove the "choosing" marker
void PagePlaneParametersNTUN::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write(' ');
	}
}

void PagePlaneParametersNTUN::_alterLocal(float alterMag) {
	// Is it an on off?
//  if (99 == _scale[_state-101] && 99 == _decPos[_state-101]) {
//	Serial.println(alterMag, DEC);
//    if (alterMag > 0) {
//      switch (_value_temp) {
//        case 0x50: _value_temp = LOGBIT_ATTITUDE_FAST; break;
//        case 0x51: _value_temp = LOGBIT_ATTITUDE_MED;  break;
//        case 0x52: _value_temp = LOGBIT_GPS;           break;
//        case 0x53: _value_temp = LOGBIT_PM;            break;
//        case 0x54: _value_temp = LOGBIT_NTUN;          break;
//        case 0x55: _value_temp = LOGBIT_NTUN;          break;
//        case 0x56: _value_temp = LOGBIT_MODE;          break;
//        case 0x57: _value_temp = LOGBIT_RAW;           break;
//        case 0x58: _value_temp = LOGBIT_CMD;           break;
//      }
//    }
//    else
//      _value_temp = 0;
//  }
//  else {
	// We don't do negative values here
	if (_value_temp + alterMag < 0)
		_value_temp = 0;
	else
		_value_temp += alterMag;
//  }

	// Keep the encoder value updated
	if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp
				/ (pow(10, _scale[_state - 101] - _decPos[_state - 101]))); // * 100;
	}

//	Serial.println(_value_temp);

	// kick the update function
	_updated = true;
}

void PagePlaneParametersNTUN::_redrawLocal(void) {
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

		// Write the value
		lineno = i - _stateFirstVal;
		GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		// Is it an On / Off value?
		if (99 == _decPos[i] && 99 == _scale[i]) {
			if (value_local > 0) {
				GLCD.print("ON ");
			} else {
				GLCD.print("OFF");
			}
		} else {
			GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
			GLCD.print("  ");
		}

//    value = (uint32_t)(floor(value_local+0.5));
//    value *= pow(10,_decPos[i] - _scale[i]);
//    Serial.println(value);
//    decBuf[0] = '0' + (value % 10);
//    value /= 10;
//    for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//      if (j == _decPos[i]) {
//        decBuf[j] = '.';
//      } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//        decBuf[j] = ' ';
//      } else {
//        decBuf[j] = '0' + (value % 10);
//        value /= 10;
//      }
//    }
//
//    // Display the data
//    while (j-- > 0)
//      lcd.write(decBuf[j]);

	}
}

void PagePlaneParametersNTUN::_voidLocal(void) {
	// Reset the variable and redraw
	int j = _Types[(_state % 100) - 1];
	_value_temp = uav.param_plane_ntun[j];
	_redrawLocal();

	// Reset _state
	_state = 0;

	// Will also have to redraw the value to its original

	// kick the update function
	_updated = true;
}

void PagePlaneParametersNTUN::_uploadConfirm(void) {
	uint8_t c;
	uint8_t row;
	const prog_char *str;
	str = confirmMessage;

	lcd.ClearArea();
	row = 0;

	for (;;) {
		c = pgm_read_byte_near(str++);
		if (0 == c)
			break;
		if ('\n' == c) {
			lcd.CursorTo(0, ++row);
			continue;
		}
		// emit
		lcd.write(c);
	}
}

void PagePlaneParametersNTUN::_uploadLocal(void) {
	uint8_t i,j;
	char str_param_id[16];

	// Send the value that we edited
	j = _Types[_state - 201];

	// First initialise the string to be empty
	for (i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
	strcpy_P(str_param_id, (char*)pgm_read_word(&(paramTable_plane_ntun[j])));

	// Upload the value
	gcs3.param_set(j, _value_temp, str_param_id);

	// Reset _state
	_state = 0;

	// kick the update function
	_updated = true;
}

uint8_t PagePlaneParametersNTUN::_interact(uint8_t buttonid) {
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
			if (_stateFirstVal < _paramCount - LINECOUNT) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _paramCount) {
			if (_state == (_stateFirstVal + LINECOUNT)) {
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
			if (_avail[_stateFirstVal])
				_state = _stateFirstVal + 1;
		} else if (_state < 100) {
			// Allow editing if the value is available
			if (_avail[(_state - 1)]) {
				uint8_t j;
				j = _Types[_state - 1];
				//nvram.load_param(&j,&_value_temp);
				_value_temp = uav.param_plane_ntun[j];

				// Configure the encoder
				if (_scale[_state - 1] == 99 && 99 == _decPos[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder =
							(int) (_value_temp
									/ (pow(10,
											_scale[_state - 1]
													- _decPos[_state - 1]))); // * 100;
					rotary.configure(&_value_encoder, 1000, 0, -4);
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
			_state += 100;
			_uploadConfirm();
			return 0; // Leave before we draw the marker again
		} else if (_state > 200) {
			// Save the value
			//_leave(OK);
			_uploadLocal();
			return 0; // Leave before we draw the marker again
		}
		break;
	case B_LEFT:
		// Navigation
		if (_state == 0)
			Pages::move(-1);
		// Editing
		else if (_state > 100 && _state < 200
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
		if (_state == 0)
			Pages::move(1);
		// Editing
		else if (_state > 100 && _state < 200
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

////////////////////////////////////////////////////////////////////////////////
// Control Tuning Parameters page
////////////////////////////////////////////////////////////////////////////////

PagePlaneParametersTECS::PagePlaneParametersTECS() {
	uint8_t i;

	/// Copy the header and types to internal storage
	_textHeader = ParamNamesPlaneTECS;
	_scale = (uint8_t*) ParamScalesTECS;
	_decPos = (uint8_t*) ParamDPsTECS;
	_paramCount = PARAM_COUNT_PLANE_TECS;

	// Set the parameter IDs - requires parameter IDs to be consecutive (later something more robust?)
	for (i = 0; i < _paramCount; i++) {
		_Types[i] = i;
	}

	/// Initially, no values are available
	for (i = 0; i < _paramCount; i++) {
		_avail[i] = 1;
	}

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;
}

uint8_t PagePlaneParametersTECS::_enter(void) {
	_drawLocal();
}

uint8_t PagePlaneParametersTECS::_forceUpdate(uint8_t reason) {
	if (reason == R_PARAM) {
		// Exit edit mode
		_state = 0;

		// Redraw the entire page
		_drawLocal();
	}

	return 0;
}

uint8_t PagePlaneParametersTECS::_redefine(void) {
}

uint8_t PagePlaneParametersTECS::_refresh_med() {
	return 0;
}

uint8_t PagePlaneParametersTECS::_refresh_slow() {
	return 0;
}

void PagePlaneParametersTECS::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	lcd.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + LINECOUNT; j++) { //Need to go from zero to read through the string
		// If there are less parameters than available lines, quit at end of list
		if (j >= _paramCount)
			break;

		if (j >= _stateFirstVal)
			lcd.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
            c = pgm_read_byte_near(_textHeader + i++);
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					lcd.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < LINECOUNT; lineno++) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _paramCount)
			break;

//		if (_avail[i] == 1) {
		if (uav.param_plane_tecs_avail[i] == 1) {
			// Load the value, either editing or live val
			if (i == (_state - 101))
				value = _value_temp;
			else {
				j = _Types[i];
				//nvram.load_param(&j,&value_local);
				value_local = uav.param_plane_tecs[j];
				value = (uint32_t) floor(value_local + 0.5);
			}
			//value = _value_live[i].value;

			// Is it an On / Off value?
			if (99 == _decPos[i] && 99 == _scale[i]) {
				if (value > 0) {
					decBuf[0] = 'N';
					decBuf[1] = 'O';
					j = 2;
				} else {
					decBuf[0] = 'F';
					decBuf[1] = 'F';
					decBuf[2] = 'O';
					j = 3;
				}

				// Display the data
				//lcd.write(' ');
				while (j-- > 0)
					lcd.write(decBuf[j]);
				lcd.write(' ');
			} else {
				GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
				GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
				GLCD.print("  ");
//              // Scale the value and fix for the number of decimal places
//              value *= pow(10,_decPos[i] - _scale[i]);
//              decBuf[0] = '0' + (value % 10);
//              value /= 10;
//              for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//                if (j == _decPos[i]) {
//                  decBuf[j] = '.';
//                } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//                  decBuf[j] = ' ';
//                } else {
//                  decBuf[j] = '0' + (value % 10);
//                  value /= 10;
//                }
//              }
			}
		} else {
			// Data unavailable, display dashes
			for (j = 0; j < 16 - PARAMNAMEFIELDWIDTH; j++)
				decBuf[j] = '-';

			// Display the data
			lcd.write(' ');
			while (j-- > 0)
				lcd.write(decBuf[j]);
			lcd.write(' ');
		}
	}

	// redraw the "choosing" marker
//        _paintMarker();
}

/// Draw the "choosing" marker
void PagePlaneParametersTECS::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write('>');
	} else if (_state > 100 && _state < 200) {
		lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		lcd.write('#');
	}
}

/// remove the "choosing" marker
void PagePlaneParametersTECS::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			lcd.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		lcd.write(' ');
	}
}

void PagePlaneParametersTECS::_alterLocal(float alterMag) {
	// Is it an on off?
//  if (99 == _scale[_state-101] && 99 == _decPos[_state-101]) {
//	Serial.println(alterMag, DEC);
//    if (alterMag > 0) {
//      switch (_value_temp) {
//        case 0x50: _value_temp = LOGBIT_ATTITUDE_FAST; break;
//        case 0x51: _value_temp = LOGBIT_ATTITUDE_MED;  break;
//        case 0x52: _value_temp = LOGBIT_GPS;           break;
//        case 0x53: _value_temp = LOGBIT_PM;            break;
//        case 0x54: _value_temp = LOGBIT_TECS;          break;
//        case 0x55: _value_temp = LOGBIT_TECS;          break;
//        case 0x56: _value_temp = LOGBIT_MODE;          break;
//        case 0x57: _value_temp = LOGBIT_RAW;           break;
//        case 0x58: _value_temp = LOGBIT_CMD;           break;
//      }
//    }
//    else
//      _value_temp = 0;
//  }
//  else {
	// We don't do negative values here
	if (_value_temp + alterMag < 0 && _state-101 != LIM_PITCH_MIN)
		_value_temp = 0;
	else
		_value_temp += alterMag;
//  }

	// Keep the encoder value updated
	if (_scale[_state - 101] == 99 && 99 == _decPos[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp
				/ (pow(10, _scale[_state - 101] - _decPos[_state - 101]))); // * 100;
	}

//	Serial.println(_value_temp);

	// kick the update function
	_updated = true;
}

void PagePlaneParametersTECS::_redrawLocal(void) {
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

		// Write the value
		lineno = i - _stateFirstVal;
		GLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		// Is it an On / Off value?
		if (99 == _decPos[i] && 99 == _scale[i]) {
			if (value_local > 0) {
				GLCD.print("ON ");
			} else {
				GLCD.print("OFF");
			}
		} else {
			GLCD.print(value_local * pow(10, -_scale[i]), _decPos[i]);
			GLCD.print("  ");
		}

//    value = (uint32_t)(floor(value_local+0.5));
//    value *= pow(10,_decPos[i] - _scale[i]);
//    Serial.println(value);
//    decBuf[0] = '0' + (value % 10);
//    value /= 10;
//    for (j=1;j<(16-PARAMNAMEFIELDWIDTH);j++) {
//      if (j == _decPos[i]) {
//        decBuf[j] = '.';
//      } else if ((0 == value) && (j > (_decPos[i] + 1))) {
//        decBuf[j] = ' ';
//      } else {
//        decBuf[j] = '0' + (value % 10);
//        value /= 10;
//      }
//    }
//
//    // Display the data
//    while (j-- > 0)
//      lcd.write(decBuf[j]);

	}
}

void PagePlaneParametersTECS::_voidLocal(void) {
	// Reset the variable and redraw
	int j = _Types[(_state % 100) - 1];
	_value_temp = uav.param_plane_tecs[j];
	_redrawLocal();

	// Reset _state
	_state = 0;

	// Will also have to redraw the value to its original

	// kick the update function
	_updated = true;
}

void PagePlaneParametersTECS::_uploadConfirm(void) {
	uint8_t c;
	uint8_t row;
	const prog_char *str;
	str = confirmMessage;

	lcd.ClearArea();
	row = 0;

	for (;;) {
		c = pgm_read_byte_near(str++);
		if (0 == c)
			break;
		if ('\n' == c) {
			lcd.CursorTo(0, ++row);
			continue;
		}
		// emit
		lcd.write(c);
	}
}

void PagePlaneParametersTECS::_uploadLocal(void) {
	uint8_t i,j;
	char str_param_id[16];

	// Send the value that we edited
	j = _Types[_state - 201];

	// First initialise the string to be empty
	for (i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
	strcpy_P(str_param_id, (char*)pgm_read_word(&(paramTable_plane_tecs[j])));

	// Upload the value
	gcs3.param_set(j, _value_temp, str_param_id);

	// Reset _state
	_state = 0;

	// kick the update function
	_updated = true;
}

uint8_t PagePlaneParametersTECS::_interact(uint8_t buttonid) {
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
			if (_stateFirstVal < _paramCount - LINECOUNT) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _paramCount) {
			if (_state == (_stateFirstVal + LINECOUNT)) {
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
			if (_avail[_stateFirstVal])
				_state = _stateFirstVal + 1;
		} else if (_state < 100) {
			// Allow editing if the value is available
			if (_avail[(_state - 1)]) {
				uint8_t j;
				j = _Types[_state - 1];
				//nvram.load_param(&j,&_value_temp);
				_value_temp = uav.param_plane_tecs[j];

				// Configure the encoder
				if (_scale[_state - 1] == 99 && 99 == _decPos[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder = (int) (_value_temp / (pow(10, _scale[_state - 1] - _decPos[_state - 1])));
					if (_state-1 == LIM_PITCH_MIN)
						rotary.configure(&_value_encoder, 1000, -1000, -4);
					else
						rotary.configure(&_value_encoder, 1000, 0, -4);
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
			_state += 100;
			_uploadConfirm();
			return 0; // Leave before we draw the marker again
		} else if (_state > 200) {
			// Save the value
			//_leave(OK);
			_uploadLocal();
			return 0; // Leave before we draw the marker again
		}
		break;
	case B_LEFT:
		// Navigation
		if (_state == 0)
			Pages::move(-1);
		// Editing
		else if (_state > 100 && _state < 200
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
		if (_state == 0)
			Pages::move(1);
		// Editing
		else if (_state > 100 && _state < 200
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

