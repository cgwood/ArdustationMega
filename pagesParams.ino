/*
 * pagesParams.ino
 *
 *  Created on: 7 Nov 2013
 *      Author: Colin
 */

#include "pagesParams.h"

PageParams::PageParams(const prog_char *title,
		   const prog_char *ParamNames,
		   const prog_char **paramTable,
		   const uint8_t *ParamScales,
		   const uint8_t *ParamDPs,
		   const uint8_t ParamCount,
		   float *params,
		   bool *avail) {
	_title = title;
	_ParamNames = ParamNames;
	_paramTable = paramTable;
	_ParamScales = ParamScales;
	_ParamDPs = ParamDPs;
	_paramCount = ParamCount;
	_params = params;
	_avail = avail;

	_linecount = LCD_ROWS-2;

	_paramLCD = gText(0, 16, LCD_COLUMNS, _linecount, SystemFont5x7);

	// Ensure we're at the top of the page not editing anything to start with
	_state = 0;
	_stateFirstVal = 0;
}
uint8_t PageParams::_enter() {
	// This function gets called when the user switches to this page
	uint8_t c;
	uint8_t i=0;

	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	for (;;) {
		c = pgm_read_byte_near(_title + i++);
		if (0 == c)
			break;
		if ('\n' == c)
			break;
		else {
			GLCD.write(c);
		}
	}
	GLCD.SelectFont(System5x7);


	_drawLocal();

	return 0;
}


void PageParams::_drawLocal(void) {
	uint8_t c;
	uint8_t i;
	uint8_t j;
	uint8_t k;
	uint8_t lineno;
	char decBuf[20 - PARAMNAMEFIELDWIDTH];
	uint32_t value;
	float value_local;

	_paramLCD.ClearArea();
	// Write the value names down the left hand side
	i = 0;
	for (j = 0; j < _stateFirstVal + _linecount; j++) { //Need to go from zero to read through the string
		// If there are less parameters than available lines, quit at end of list
		if (j >= _paramCount)
			break;

		if (j >= _stateFirstVal)
			_paramLCD.CursorTo(0, j - _stateFirstVal);

		k = 0;

		for (;;) {
			c = pgm_read_byte_near(_ParamNames + i++);
			if (0 == c)
				break;
			if ('\n' == c)
				break;
			else {
				if (j >= _stateFirstVal && k++ < PARAMNAMEFIELDWIDTH)
					_paramLCD.write(c);
			}
		}
	}

	// Write the values
	for (lineno = 0; lineno < _linecount; lineno++) {
		_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		i = _stateFirstVal + lineno;
		if (i >= _paramCount)
			break;

		if (_avail[i] == 1) {
			if (i == (_state - 101))
				value = _value_temp;
			else {
				value_local = _params[i];
				value = (uint32_t) floor(value_local + 0.5);
			}

			// Is it an On / Off value?
			if (99 == _ParamDPs[i] && 99 == _ParamScales[i]) {
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
				//_paramLCD.write(' ');
				while (j-- > 0)
					_paramLCD.write(decBuf[j]);
				_paramLCD.write(' ');
			} else {
				_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);
				_paramLCD.print(value_local * pow(10, -_ParamScales[i]), _ParamDPs[i]);
				_paramLCD.print("\n");
			}
		} else {
			// Data unavailable, display dashes
			for (j = 0; j < 16 - PARAMNAMEFIELDWIDTH; j++)
				decBuf[j] = '-';

			// Display the data
			_paramLCD.write(' ');
			while (j-- > 0)
				_paramLCD.write(decBuf[j]);
			_paramLCD.write(' ');
		}
	}
}



/// Draw the "choosing" marker
void PageParams::_paintMarker(void) {
	if (_state > 0 && _state < 100) {
		_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		_paramLCD.write('>');
	} else if (_state > 100 && _state < 200) {
		_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		_paramLCD.write('#');
	}
}

/// remove the "choosing" marker
void PageParams::_clearMarker(void) {
	if (_state > 0 && _state < 200) {
		if (_state > 100)
			_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 101 - _stateFirstVal);
		else
			_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH, _state - 1 - _stateFirstVal);
		_paramLCD.write(' ');
	}
}

void PageParams::_alterLocal(float alterMag) {
	// We don't do negative values here
	if (_value_temp + alterMag < 0)
		_value_temp = 0;
	else
		_value_temp += alterMag;

	// Keep the encoder value updated
	if (_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101]) {
		_value_encoder = (int) (_value_temp);
	} else {
		_value_encoder = (int) (_value_temp/(pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101])));
	}
}


void PageParams::_redrawLocal(void) {
	uint8_t i;
	uint8_t j;
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
		_paramLCD.CursorTo(PARAMNAMEFIELDWIDTH + 2, lineno);

		// Is it an On / Off value?
		if (99 == _ParamDPs[i] && 99 == _ParamScales[i]) {
			if (value_local > 0) {
				_paramLCD.print("ON ");
			} else {
				_paramLCD.print("OFF");
			}
		} else {
			_paramLCD.print(value_local * pow(10, -_ParamScales[i]), _ParamDPs[i]);
			_paramLCD.print("\n");
		}
	}
}

void PageParams::_voidLocal(void) {
	// Reset the variable and redraw
	int j = (_state % 100) - 1;
	_value_temp = _params[j];
	_redrawLocal();

	// Reset _state
	_state = 0;
}

void PageParams::_uploadConfirm(void) {
	uint8_t c;
	uint8_t row;
	const prog_char *str;
	str = confirmMessage;

	_paramLCD.ClearArea();
	row = 0;

	for (;;) {
		c = pgm_read_byte_near(str++);
		if (0 == c)
			break;
		if ('\n' == c) {
			_paramLCD.CursorTo(0, ++row);
			continue;
		}
		// emit
		_paramLCD.write(c);
	}
}

void PageParams::_uploadLocal(void) {
	uint8_t i,j;
	char str_param_id[16];

	// Send the value that we edited
	j = _state - 201;

	// First initialise the string to be empty
	for (i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
	strcpy_P(str_param_id, (char*)pgm_read_word(&(_paramTable[j])));

	// Upload the value
	gcs3.param_set(j, _value_temp, str_param_id);

	// Reset _state
	_state = 0;
}

uint8_t PageParams::_redefine(void) {
	return 0;
}

uint8_t PageParams::_refresh_med() {
	// This function gets called ten times a second
	return 0;
}

uint8_t PageParams::_refresh_slow() {
	// This function gets called every two seconds
	return 0;
}

uint8_t PageParams::_forceUpdate(uint8_t reason) {
	if (reason == R_PARAM) {
		// Exit edit mode
		_state = 0;

		// Redraw the entire page
		_drawLocal();
	}
	return 0;
}


uint8_t PageParams::_interact(uint8_t buttonid) {
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
			if (_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101]) {
				if (_value_temp == 0)
					_alterLocal(1);
			} else {
				_alterLocal(1 * pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101]));
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
			if (_stateFirstVal < _paramCount - _linecount) {
				_stateFirstVal++;
				_drawLocal();
			}
		} else if (_state > 0 && _state < _paramCount) {
			if (_state == (_stateFirstVal + _linecount)) {
				_stateFirstVal++;
				_drawLocal();
			}
			_state++;
		}
		// Editing
		else if (_state > 100 && _state < 200) {
			if (_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101]) {
				if (_value_temp == 1)
					_alterLocal(-1);
			} else {
				_alterLocal(-1 * pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101]));
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
				j = _state - 1;
				//nvram.load_param(&j,&_value_temp);
				_value_temp = _params[j];

				// Configure the encoder
				if (_ParamScales[_state - 1] == 99 && 99 == _ParamDPs[_state - 1]) {
					_value_encoder = (int) (_value_temp);
					rotary.configure(&_value_encoder, 1, 0, -4);
				} else {
					_value_encoder = (int) (_value_temp / (pow(10, _ParamScales[_state - 1] - _ParamDPs[_state - 1]))); // * 100;
					rotary.configure(&_value_encoder, 9999, 0, -4);
				}

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
		else if (_state > 100 && _state < 200 && !(_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101])) {
			_alterLocal(-10 * pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101]));
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
		else if (_state > 100 && _state < 200 && !(_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101])) {
			_alterLocal(10 * pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101]));
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
			if (_ParamScales[_state - 101] == 99 && 99 == _ParamDPs[_state - 101]) {
				_value_temp = _value_encoder;
			} else {
				_value_temp = _value_encoder * pow(10, _ParamScales[_state - 101] - _ParamDPs[_state - 101]);
			}
			_redrawLocal();
		}
		break;
	}
	_paintMarker();

	return 0;
}
