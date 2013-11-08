// -*- Mode: C++; c-basic-offset: 8; indent-tabs-mode: nil -*-
//-
// Copyright (c) 2010 Michael Smith. All rights reserved.
// Modified 2011 By Colin G http://www.diydrones.com/profile/ColinG
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

/// @file       nvram.pde
/// @brief      NVRAM functions

/// Default NVRAM contents
struct NVRAM::nv_data NVRAM_default PROGMEM = {
	SERIAL_57600, // serialSpeed
	65,// lowVoltage
	0, // useCompass
	4, // streamRate
	0, // Buttons muted
	1, // Alarm Sounds
	1, // mav number
	1, // GPS type
	0, // SD Card logging
	1, // TX Heartbeats
	150, // Keypad repeat delay
	500,  // Keypad repeat hold delay
	0  // Keypad repeat delay
};

void NVRAM::load(void) {
	// check for signature - second digit changes as nv_data def changes
	if (('V' != EEPROM.read(0)) || (NV_VERSION != EEPROM.read(1))) {
		// load defaults
		memcpy_P(&nv, &NVRAM_default, sizeof(nv));
		_reset_params();
	} else {
		// load from NVRAM
		_loadx(2, sizeof(nv), &nv);
	}
}

void NVRAM::save(void) {
	// save to NVRAM
	_savex(2, sizeof(nv), &nv);

	EEPROM.write(0, 'V');
	EEPROM.write(1, NV_VERSION);
}

void NVRAM::load_param(uint8_t *param_id, float *param_value) {
	// load from NVRAM
//        _loadx(2 + sizeof(nv) + *param_id*sizeof(float), sizeof(float), param_value);
}

void NVRAM::get_setting_bounds(uint8_t *setting_id, int16_t *lower_val, int16_t *upper_val) {
	*lower_val = 0;

	switch (*setting_id) {
	case SERIAL_SPEED:
		*upper_val = SERIAL_SPEED_COUNT-1;
		break;
	case KEYPAD_ORIENTATION:
		*upper_val = 3;
		break;
	case REPEAT_DELAY:
		*lower_val = 50;
		*upper_val = 1000;
		break;
	case HOLD_DELAY:
		*lower_val = 50;
		*upper_val = 3000;
		break;
	case USE_COMPASS:
	case MUTE_BUTTONS:
	case ALARM_SOUNDS:
	case SD_LOGGING:
	case TX_HEARTBEAT:
		*upper_val = 1;
		break;
	case GPS_TIMEZONE:
		*lower_val = -14;
		*upper_val = 14;
		break;
	default:
		*upper_val = 254;
		break;
	}
}

void NVRAM::load_setting_text(uint8_t *setting_id, int16_t setting_value, char text_value[7]) {
	// Convert numeric values to text values
	// setting_value = -32768 for loading stored value
	uint8_t strlen;

	// Find out the numerical value
	int16_t value;
	if (setting_value == -32768)
		load_setting(setting_id,&value);
	else
		value = setting_value;

	// Convert the numerical value into text
	switch (*setting_id) {
	case SERIAL_SPEED: {
		switch(value) {
		case SERIAL_9600:
			strcpy_P(text_value, PSTR("9600  "));
			break;
		case SERIAL_19200:
			strcpy_P(text_value, PSTR("19200 "));
			break;
		case SERIAL_38400:
			strcpy_P(text_value, PSTR("38400 "));
			break;
		case SERIAL_57600:
			strcpy_P(text_value, PSTR("57600 "));
			break;
		case SERIAL_115200:
			strcpy_P(text_value, PSTR("115200"));
			break;
		default:
			strcpy_P(text_value, PSTR("ERROR "));
			break;
		}
		break;
	}
	case LOW_VOLTAGE: {
		sprintf(text_value,"%.1fV",((float)value)/10);
		break;
	}
	case USE_COMPASS:
	case ALARM_SOUNDS:
	case SD_LOGGING:
	case TX_HEARTBEAT:
	case MUTE_BUTTONS:
		if (value == 0)
			strcpy_P(text_value, PSTR("OFF   "));
		else if (value == 1)
			strcpy_P(text_value, PSTR("ON    "));
		else
			strcpy_P(text_value, PSTR("ERROR "));
		break;
	case KEYPAD_ORIENTATION:
		switch (value) {
		case KP_CONN_RIGHT:
			strcpy_P(text_value, PSTR("RIGHT "));
			break;
		case KP_CONN_DOWN:
			strcpy_P(text_value, PSTR("DOWN  "));
			break;
		case KP_CONN_LEFT:
			strcpy_P(text_value, PSTR("LEFT  "));
			break;
		case KP_CONN_UP:
			strcpy_P(text_value, PSTR("UP    "));
			break;
		default:
			strcpy_P(text_value, PSTR("ERROR "));
			break;
		}
		break;
	case GPS_TIMEZONE:
		if (value>=0)
			strlen = sprintf(text_value,"+%d",value);
		else
			strlen = sprintf(text_value,"%d",value);

		// Fill the string with trailing blank spaces
		for (uint8_t i=strlen;i<6;i++)
			text_value[i] = 32;

		// End of string termination character
		text_value[6] = 0;
		break;
	default:
		strlen = sprintf(text_value,"%d",value);

		// Fill the string with trailing blank spaces
		for (uint8_t i=strlen;i<6;i++)
			text_value[i] = 32;

		// End of string termination character
		text_value[6] = 0;
		break;
	}
}

void NVRAM::load_setting(uint8_t *setting_id, float *param_value) {
	int16_t param_value_int;
	load_setting(setting_id, &param_value_int);
	*param_value = (float)param_value_int;
}

void NVRAM::load_setting(uint8_t *setting_id, int16_t *param_value) {
	// load from NVRAM
	switch (*setting_id) {
	case SERIAL_SPEED:
		*param_value = nv.serialSpeed;
		break;
	case LOW_VOLTAGE:
		*param_value = nv.lowVoltage;
		break;
	case USE_COMPASS:
		*param_value = nv.useCompass;
		break;
	case STREAM_RATE:
		*param_value = nv.streamRate;
		break;
	case MUTE_BUTTONS:
		*param_value = nv.buttonMute;
		break;
	case ALARM_SOUNDS:
		*param_value = nv.alarmSounds;
		break;
	case MAV_NUMBER:
		*param_value = nv.mavNumber;
		break;
	case GPS_TIMEZONE:
		*param_value = nv.gpsTimezone;
		break;
	case SD_LOGGING:
		*param_value = nv.sdLogging;
		break;
	case TX_HEARTBEAT:
		*param_value = nv.txHeartbeats;
		break;
	case REPEAT_DELAY:
		*param_value = nv.keypadRepeatDelay;
		break;
	case HOLD_DELAY:
		*param_value = nv.keypadHoldDelay;
		break;
	case KEYPAD_ORIENTATION:
		*param_value = nv.keypadRotation;
		break;
	default:
		*param_value = 0;
		break;
	}
}

void NVRAM::write_setting(uint8_t *setting_id, float *param_value) {
	int16_t param_value_int;
	param_value_int = (float)*param_value;
	write_setting(setting_id, &param_value_int);
}

void NVRAM::write_setting(uint8_t *setting_id, int16_t *param_value) {
	uint8_t updated = 1;

	// Update the value
	switch (*setting_id) {
	case SERIAL_SPEED:
		nv.serialSpeed = *param_value;
		break;
	case LOW_VOLTAGE:
		nv.lowVoltage = *param_value;
		break;
	case USE_COMPASS:
		nv.useCompass = *param_value;
		break;
	case STREAM_RATE:
		nv.streamRate = *param_value;
		break;
	case MUTE_BUTTONS:
		nv.buttonMute = *param_value;
		break;
	case ALARM_SOUNDS:
		nv.alarmSounds = *param_value;
		break;
	case MAV_NUMBER:
		nv.mavNumber = *param_value;
		break;
	case GPS_TIMEZONE:
		nv.gpsTimezone = *param_value;
		break;
	case SD_LOGGING:
		nv.sdLogging = *param_value;
		break;
	case TX_HEARTBEAT:
		nv.txHeartbeats = *param_value;
		break;
	case REPEAT_DELAY:
		nv.keypadRepeatDelay = *param_value;
		break;
	case HOLD_DELAY:
		nv.keypadHoldDelay = *param_value;
		break;
	case KEYPAD_ORIENTATION:
		nv.keypadRotation = *param_value;
		break;
	default:
		updated = 0;
		break;
	}

	if (updated)
		save();
}

void NVRAM::save_param(uint8_t *param_id, float *param_value) {
	// save to NVRAM
//        _savex(2 + sizeof(nv) + *param_id*sizeof(float), sizeof(float), param_value);
}

void NVRAM::_reset_params(void) {
	uint8_t i;

	// Save a zero to NVRAM
//	for (i=0;i<Parameters::COUNT;i++) {
//		_savex(2 + sizeof(nv) + i*sizeof(float), sizeof(float), 0);
//	}
}

void NVRAM::_loadx(uint8_t address, uint8_t size, void *value) {
	uint8_t *rp;

	rp = (uint8_t *) value;
	while (size--)
		*rp++ = EEPROM.read(address++);
}

void NVRAM::_savex(uint8_t address, uint8_t size, void *value) {
	uint8_t *rp;

	rp = (uint8_t *) value;
	while (size--)
		EEPROM.write(address++, *rp++);
}
