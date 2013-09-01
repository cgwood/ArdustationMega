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

// GPS Includes
#include "AP_GPS_UBLOX.h"
#include "AP_GPS_MTK.h"
#include "AP_GPS_MTK19.h"

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

// Variables and definitions
#include "hardware.h"           // Definitions for the ground station's hardware
#include "uav_params.h"         // Class containing the UAV parameters
#include "uav.h"                // Class containing the UAV variables
#include "asm.h"                // Class containing the ardustation mega's variables
#include "pages.h"	        // Contains the LCD pages

// GPS declarations
#define T3 1000
#define T6 1000000
#define T7 10000000
AP_GPS_MTK19 gps(&Serial1);

////////////////////////////////////////////////////////////////////////////////
// GCS selection
////////////////////////////////////////////////////////////////////////////////
//
GCS_MAVLINK	gcs0(110);
GCS_MAVLINK	gcs3(110);
//static uint8_t      apm_mav_system; 
static uint8_t      apm_mav_component;

// Flag for passing mavlink through usb, for pc gcs
boolean gcs_passthrough = 0;

////////////////////////////////////////////////////////////////////////////////
// System Timers
////////////////////////////////////////////////////////////////////////////////
// Time in miliseconds of start of main control loop.  Milliseconds
static unsigned long 	fast_loopTimer;
static unsigned long 	med_loopTimer;
static unsigned long 	slow_loopTimer;
static unsigned long 	vslow_loopTimer;
unsigned long loopwait;
unsigned long maxloopwait=0;

// Heartbeat counter
unsigned long hbcount=0;

void setup()
{
  // Initialise the display driver object
  GLCD.Init(NON_INVERTED);

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

  // Initialise the serial ports
  Serial.begin(57600);   // USB comm port
  Serial1.begin(38400);  // GPS
  Serial3.begin(57600);  // Telemetry

  // Initialise the GCS
  gcs0.init(&Serial);
  gcs3.init(&Serial3);
  
  // Initialise the GPS  
  stderr = stdout;
  gps.print_errors = true;

//  Serial.println("GPS UBLOX library test");
//  gps.init(GPS::GPS_ENGINE_AIRBORNE_2G);       // GPS Initialization

  Serial.println("GPS MTK library test");
  stderr = stdout;
  gps.print_errors = true;
  gps.init();       // GPS Initialization

  // Write centre positions to servos
  Pan.attach(6,800,2200);// Ultimately make the end points as variables on some input screen
  Tilt.attach(7,800,2200);
  //  Pan.write(90);
  //  Tilt.write(90);
  
  // Start the first page
  Pages::enter();
}

void loop()
{
  uint8_t buttonid;

  // Update comms as fast as possible
  if (gcs3.initialised) {
    gcs3.update();
  }
  else {
    Serial.println("GCS not initialised");
  }
  
  // Update the GPS as fast as possible
  gps.update();
  
//  if (gps.new_data) {
//	  Serial.print("gps:");
//	  Serial.print(" Lat:");
//	  Serial.print((float)gps.latitude / T7, DEC);
//	  Serial.print(" Lon:");
//	  Serial.print((float)gps.longitude / T7, DEC);
//	  Serial.print(" Alt:");
//	  Serial.print((float)gps.altitude / 100.0, DEC);
//	  Serial.print(" GSP:");
//	  Serial.print(gps.ground_speed / 100.0);
//	  Serial.print(" COG:");
//	  Serial.print(gps.ground_course / 100.0, DEC);
//	  Serial.print(" SAT:");
//	  Serial.print(gps.num_sats, DEC);
//	  Serial.print(" FIX:");
//	  Serial.print(gps.fix, DEC);
//	  Serial.print(" TIM:");
//	  Serial.print(gps.time, DEC);
//	  Serial.println();
//	  gps.new_data = 0; // We have read the data
//}

  // This loop is to execute at 50Hz
  // -------------------------------------------
  loopwait = millis()-fast_loopTimer;
  if (loopwait  > 19) {
    maxloopwait = max(loopwait,maxloopwait);

    // Listen for button presses
    buttonid = keypad.pressed();
    switch(buttonid) {
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
    if (millis()-med_loopTimer > 99) {
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
    if (millis()-vslow_loopTimer > 1999) {
      Pages::refresh_slow();
      maxloopwait = 0;

      vslow_loopTimer = millis();
    }
  }
}

