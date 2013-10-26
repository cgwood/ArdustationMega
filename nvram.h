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

/// @file       nvram.h
/// @brief      non-volatile memory

//                                        "123456789012"
PROGMEM const prog_char SettingNames[]  = "Serial Speed\n"
		 "Low Voltage \n"
		 "Use Compass \n"
		 "Stream Rate \n"
		 "Mute Buttons\n"
		 "Alarm Sounds\n"
		 "MAV Number  \n"
		 "GPS Type    \n"
		 "SD Logging  \n"
		 "TX Heartbeat\n"
		 "Repeat Rate \n"
		 "Repeat Delay\n"
		 "Key Rotation\n";

enum SETTINGS_ORDER {
	// Settings page ordering
	SERIAL_SPEED=0,
	LOW_VOLTAGE,
	USE_COMPASS,
	STREAM_RATE,
	MUTE_BUTTONS,
	ALARM_SOUNDS,
	MAV_NUMBER,
	GPS_TYPE,
	SD_LOGGING,
	TX_HEARTBEAT,
	REPEAT_RATE,
	REPEAT_DELAY,
	KEYPAD_ORIENTATION,
	// Parameter count
	ASM_SETTINGS_COUNT
};

enum SERIAL_SPEEDS {
	// Available serial speeds
	SERIAL_9600=0,
	SERIAL_19200,
	SERIAL_38400,
	SERIAL_57600,
	SERIAL_115200,
	SERIAL_SPEED_COUNT
};

const uint8_t SettingScales[] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // *10^(-x)
const uint8_t SettingDPs[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0}; // 99 in both denotes boolean

/// Simple non-volatile memory support.
///
class NVRAM {
public:
        NVRAM() {};

        /// Load all variables from NVRAM
        ///
        void    load(void);

        /// Load parameters from NVRAM
        ///
        void    load_param(uint8_t *param_id, float *param_value);

        /// Load setting from NVRAM
        ///
        void    load_setting(uint8_t *setting_id, float *setting_value);
        void    load_setting(uint8_t *setting_id, uint16_t *setting_value);

        /// Write setting to NVRAM
        ///
        void    write_setting(uint8_t *setting_id, float *setting_value);
        void    write_setting(uint8_t *setting_id, uint16_t *setting_value);

        /// Load text format of setting from NVRAM
        ///
        void    load_setting_text(uint8_t *setting_id, uint16_t setting_value, char text_value[7]);

        /// Get limits for a setting's value
        ///
        void    get_setting_bounds(uint8_t *setting_id, uint8_t *lower_val, uint8_t *upper_val);

        /// Save all variables to NVRAM
        ///
        void    save(void);

        /// Save parameters to NVRAM
        ///
        void    save_param(uint8_t *param_id, float *param_value);

        /// Definition of the load/save area
        ///
        struct nv_data {
        	    uint8_t         serialSpeed;
                uint16_t        lowVoltage;
                uint8_t         useCompass;
                uint8_t         streamRate;
                uint8_t         buttonMute;
                uint8_t         alarmSounds;
                uint8_t         mavNumber;
                uint8_t         gpsType;
                uint8_t         sdLogging;
                uint8_t         txHeartbeats;
                uint8_t         keypadRepeatRate;
                uint8_t         keypadRepeatDelay;
                uint8_t         keypadRotation;
        };

        struct nv_data  nv;             ///< saved variables

private:
        /// Read bytes from NVRAM
        ///
        /// @param address      offset in NVRAM to read from
        /// @param size         count of bytes to read
        /// @param value        buffer to read into
        ///
        void            _loadx(uint8_t address, uint8_t size, void *value);

        /// Write bytes to NVRAM
        ///
        /// @param address      offset in NVRAM to write to
        /// @param size         count of bytes to write
        /// @param value        buffer to write from
        ///
        void            _savex(uint8_t address, uint8_t size, void *value);

        /// Reset downloaded parameters
        ///
        void            _reset_params(void);
};

