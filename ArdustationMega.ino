// Ardustation Mega
// Created 2013 By Colin G http://www.diydrones.com/profile/ColinG
//
// Special thanks go to the ArduPilot and Mavlink dev teams and Michael Smith

// AVR runtime
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <math.h>

// Mavlink libraries - Path requires updating per install
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/mavlink/v1.0/ardupilotmega/mavlink.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/mavlink/v1.0/common/common.h"

// Arduino Libraries
#include <Servo.h>
#include <Wire.h>
#include <SD.h>
#include <EEPROM.h>

// GPS Includes
#include "AP_GPS.h"
//#include "AP_GPS_UBLOX.h"
//#include "AP_GPS_MTK.h"
//#include "AP_GPS_MTK19.h"
//#include "AP_GPS_Auto.h"

// LCD Includes
#include "glcd.h"
#include "fonts/allFonts.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/bitmaps/icon_altitude_small.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/bitmaps/sat.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/bitmaps/icn_conn.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/bitmaps/icn_batt.h"
#include "C:/Users/Colin/Documents/Arduino/ArduStation/ArdustationMega/bitmaps/icn_speed.h"

// Local modules
#include "GCS.h"                // Controls the ground station comms
#include "RotaryEncoder.h";     // Handles the rotary encoder events
#include "Tracker.h"            // Controls the antenna tracker
#include "Buttons.h"            // Routines for button presses
#include "Beep.h"               // Sounds the piezo buzzer
#include "nvram.h"              // For saving to EEPROM
// Variables and definitions
#include "hardware.h"           // Definitions for the ground station's hardware
#include "uav_params.h"         // Class containing the UAV parameters
#include "uav.h"                // Class containing the UAV variables
#include "asm.h"                // Class containing the ardustation mega's variables
#include "pages.h"	            // Contains the LCD pages
#include "pageSettings.h"	    // Contains the Settings LCD pages
#include "pagesPlane.h"	        // Contains the Plane LCD pages
#include "pagesRover.h"	        // Contains the Rover LCD pages
#include "pagesCopter.h"	    // Contains the Copter LCD pages


// GPS declarations
#define T3 1000
#define T6 1000000
#define T7 10000000
//AP_GPS_UBLOX g_gps(&Serial1);

// All GPS access should be through this pointer.
static GPS         *g_gps;

// GPS Selection
#if   GPS_PROTOCOL == GPS_PROTOCOL_AUTO
AP_GPS_Auto     g_gps_driver(&Serial1, &g_gps);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_NMEA
AP_GPS_NMEA     g_gps_driver(&Serial1);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_SIRF
AP_GPS_SIRF     g_gps_driver(&Serial1);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_UBLOX
AP_GPS_UBLOX    g_gps_driver(&Serial1);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_MTK
AP_GPS_MTK      g_gps_driver(&Serial1);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_MTK19
AP_GPS_MTK19    g_gps_driver(&Serial1);

 #elif GPS_PROTOCOL == GPS_PROTOCOL_NONE
AP_GPS_None     g_gps_driver(NULL);

 #else
  #error Unrecognised GPS_PROTOCOL setting.
 #endif // GPS PROTOCOL


//AP_GPS_UBLOX g_gps(&Serial1);
//static GPS         *g_gps;
//AP_GPS_Auto     g_gps_driver(&Serial1, &g_gps);


// EEPROM Declaration
NVRAM           nvram;                          ///< NVRAM driver

////////////////////////////////////////////////////////////////////////////////
// GCS selection
////////////////////////////////////////////////////////////////////////////////
//
GCS_MAVLINK gcs0(110);
GCS_MAVLINK gcs3(110);
//static uint8_t      apm_mav_system; 
static uint8_t apm_mav_component;

// Flag for passing mavlink through usb, for pc gcs
boolean gcs_passthrough = 0;

// Flag denoting that we're downloading the parameters
boolean downloading = 0;
unsigned long download_start_time;
int16_t download_index=0;

////////////////////////////////////////////////////////////////////////////////
// System Timers
////////////////////////////////////////////////////////////////////////////////
// Time in miliseconds of start of main control loop.  Milliseconds
static unsigned long fast_loopTimer;
static unsigned long med_loopTimer;
static unsigned long slow_loopTimer;
static unsigned long vslow_loopTimer;
unsigned long loopwait;
unsigned long maxloopwait = 0;

// Heartbeat counter
unsigned long hbcount = 0;

void setup() {
	// Initialise the display driver object
	GLCD.Init(NON_INVERTED);

    // load the NVRAM
    nvram.load();

	// Print the welcome message
	//lcd.print("Starting up");

	// Initialize the keypad
	Wire.begin();

	// SD Card
	pinMode(chipSelect, OUTPUT);
	init_sdcard();

	// Attach the rotary encoder
	attachInterrupt(0, doEncoder, CHANGE);
	attachInterrupt(1, doEncoder, CHANGE);
	rotary.configure(&ASM.encoderval, 500, 0, -4);

	// Initialize stuff that needs to go in a class
	init_batt();
	uav.sysid = 0;
	uav.onboard_param_count = 0;
	uav.bln_requested_params = 0;
	ASM.num_sats = 0;
	uint8_t i;
	for (i=0;i<PARAM_COUNT_PLANE;i++) {
		uav.param_plane_avail[i] = 0;
	}
	for (i=0;i<PARAM_COUNT_PLANE_CTUN;i++) {
		uav.param_plane_ctun_avail[i] = 0;
	}
	for (i=0;i<PARAM_COUNT_PLANE_NTUN;i++) {
		uav.param_plane_ntun_avail[i] = 0;
	}
	for (i=0;i<PARAM_COUNT_PLANE_TECS;i++) {
		uav.param_plane_tecs_avail[i] = 0;
	}
	for (i=0;i<PARAM_COUNT_ROVER;i++) {
		uav.param_rover_avail[i] = 0;
	}
	for (i=0;i<PARAM_COUNT_COPTER;i++) {
		uav.param_copter_avail[i] = 0;
	}

	// Initialise the serial ports
	Serial.begin(57600);   // USB comm port
	Serial1.begin(38400);  // GPS
	Serial3.begin(57600);  // Telemetry

	// Initialise the GCS
	gcs0.init(&Serial);
	gcs3.init(&Serial3);

	// Initialise the GPS
//	stderr = stdout;
//	g_gps.print_errors = true;
//	g_gps.init()

    // Do GPS init
    g_gps = &g_gps_driver;
    // GPS Initialization
    g_gps->init(GPS::GPS_ENGINE_STATIONARY);

	// Write centre positions to servos
	Pan.attach(6, 800, 2200); // Ultimately make the end points as variables on some input screen
	Tilt.attach(7, 800, 2200);
	//  Pan.write(90);
	//  Tilt.write(90);

	// Start the first page
	Pages::enter();
}

void loop() {
	uint8_t buttonid;

	// Update comms as fast as possible
	if (gcs3.initialised) {
		gcs3.update();
	} else {
		Serial.println("GCS not initialised");
	}

	// Update the GPS as fast as possible
	g_gps->update();

	// This loop is to execute at 50Hz
	// -------------------------------------------
	loopwait = millis() - fast_loopTimer;
	if (loopwait > 19) {
		maxloopwait = max(loopwait, maxloopwait);

		// Listen for button presses
		buttonid = keypad.pressed();
		switch (buttonid) {
//		case B_OK_HOLD:
//			Serial.println("Held OK");
//			break;
//		case B_CANCEL_HOLD:
//			Serial.println("Held Cancel");
//			break;
		// By default all keypad presses are sent to the pages
		default:
			Pages::interact(buttonid);
			break;
		}

		// Listen for encoder updates, notify the pages
		if (rotary.haschanged())
			Pages::interact(B_ENCODER);

		// update the currently-playing tune
		beep.update();

		// Update the antenna tracker
		tracker.update();

		// Update the fast loop timer
		fast_loopTimer = millis();

		// This loop is to execute at 10Hz
		// -------------------------------------------
		if (millis() - med_loopTimer > 99) {
			// Sample battery sensor
			sample_batt();

			// Update the pages
			Pages::refresh_med();
			if (ASM.encoderval == 20) {
				beep.play(BEEP_LAND);
				ASM.encoderval = 0;
			}

			// Update the medium loop timer
			med_loopTimer = millis();
		}

		//    // This loop is to execute at 5Hz
		//    // -------------------------------------------
		//    if (millis()-slow_loopTimer > 199) {
		//      slow_loopTimer = millis();
		//    }

		// This loop is to execute at 0.5Hz
		// -------------------------------------------
		if (millis() - vslow_loopTimer > 1999) {
			Pages::refresh_slow();
			maxloopwait = 0;

			// If we're downloading parameters, check the progress
			if (downloading) {
				if (millis() - download_start_time >= 1000) {
					downloading = 0;
					Serial.println("Download timed out, retrying");
					download_start_time = millis();
					gcs3.param_request(download_index);
					downloading = 1;
				}
			}

			// Automatically download parameters if we haven't already
			if (uav.connected && !uav.bln_requested_params) {
				// Don't do it straight away
				if (millis() - uav.connTime > 1000) {
					// Set the flag to say we've requested parameters
					uav.bln_requested_params = 1;

					// First tell the MAV to stop streaming data
					// So we can maximise available bandwidth
					gcs3.data_stream_stop();
					delay(50);

					// Request the parameters
					gcs3.param_request(0);
				}
			}

			vslow_loopTimer = millis();
		}
	}
}

