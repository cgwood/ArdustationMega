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
	576,    // serialSpeed
	65,// lowVoltage
	0,// useCompass
	4,// streamRate
	0,// Buttons muted
	1,// Alarm Sounds
	1,// mav number
	1,// GPS type
	0,// SD Card logging
	1,// TX Heartbeats
	5,// Keypad repeat rate
	5// Keypad repeat delay
};

void NVRAM::load(void) {
	// check for signature
	if (('c' != EEPROM.read(0)) || ('g' != EEPROM.read(1))) {
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

	EEPROM.write(0, 'c');
	EEPROM.write(1, 'g');
}

void NVRAM::load_param(uint8_t *param_id, float *param_value) {
	// load from NVRAM
//        _loadx(2 + sizeof(nv) + *param_id*sizeof(float), sizeof(float), param_value);
}

void NVRAM::load_setting(uint8_t *setting_id, float *param_value) {
	// load from NVRAM
	switch (*setting_id) {
	case SERIAL_SPEED:
		*param_value = nv.serialSpeed;
		break;
	case LOW_VOLTAGE:
		*param_value = nv.lowVoltage;
		break;
	case STREAM_RATE:
		*param_value = nv.streamRate;
		break;
	case MUTE_BUTTONS:
		*param_value = nv.buttonMute;
		break;
	case MAV_NUMBER:
		*param_value = nv.mavNumber;
		break;
	case GPS_TYPE:
		*param_value = nv.gpsType;
		break;
	case SD_LOGGING:
		*param_value = nv.sdLogging;
		break;
	case TX_HEARTBEAT:
		*param_value = nv.txHeartbeats;
		break;
	case REPEAT_RATE:
		*param_value = nv.keypadRepeatRate;
		break;
	case REPEAT_DELAY:
		*param_value = nv.keypadRepeatDelay;
		break;
	default:
		*param_value = 0;
		break;
	}
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
