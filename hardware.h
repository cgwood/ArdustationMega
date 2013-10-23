// ASM hardware definitions
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// GPS type codes - use the names, not the numbers
#define GPS_PROTOCOL_NONE       -1
#define GPS_PROTOCOL_NMEA       0
#define GPS_PROTOCOL_SIRF       1
#define GPS_PROTOCOL_UBLOX      2
#define GPS_PROTOCOL_IMU        3
#define GPS_PROTOCOL_MTK        4
#define GPS_PROTOCOL_HIL        5
#define GPS_PROTOCOL_MTK19      6
#define GPS_PROTOCOL_AUTO       7

#define GPS_PROTOCOL GPS_PROTOCOL_UBLOX

// Button definitions
#define expander B0100000  // Address with three address pins grounded.

/// Buzzer pin
#define TONE_PIN        A1

// ------------------------------------- LCD Definitions ------------------------------------- //
// ------------------------------------------------------------------------------------------- //
#define LCD_COLUMNS    20                       ///< standard display for ArduStation
#define LCD_ROWS        8                       ///< standard display for ArduStation
gText lcd = gText(0, 0, LCD_COLUMNS, LCD_ROWS, SystemFont5x7);

#define LCD_CHAR_LINK           0
#define LCD_CHAR_ROLL_LEFT      1
#define LCD_CHAR_ROLL_RIGHT     2
#define LCD_CHAR_UP_ARROW       3
#define LCD_CHAR_DOWN_ARROW     4
#define LCD_CHAR_MINUS_ONE      5
#define LCD_CHAR_BATTERY        6
#define LCD_CHAR_MODIFY         7

static uint8_t lcdCharRollLeft[] 	PROGMEM = {5, 8, 0x5C, 0x62, 0x72, 0x02, 0x0C};
static uint8_t lcdCharRollRight[] 	PROGMEM = {5, 8, 0x0C, 0x02, 0x72, 0x62, 0x5C};
static uint8_t lcdCharUpArrow[] 	PROGMEM = {5, 8, 0x04, 0x02, 0x3F, 0x02, 0x04};
static uint8_t lcdCharDownArrow[] 	PROGMEM = {5, 8, 0x10, 0x20, 0x7E, 0x20, 0x10};
static uint8_t lcdCharMinusOne[] 	PROGMEM = {5, 8, 0x02, 0x02, 0x09, 0x0F, 0x08};
static uint8_t lcdCharBattery[] 	PROGMEM = {5, 8, 0x7E, 0x53, 0x5B, 0x4B, 0x7E};
static uint8_t lcdCharModify[] 		PROGMEM = {5, 8, 0x00, 0x7F, 0x3E, 0x1C, 0x08};
static uint8_t lcdCharLinkWait[] 	PROGMEM = {5, 8, 0xFF, 0xFB, 0xAD, 0xF3, 0xFF};
static uint8_t lcdCharLinkOK[] 		PROGMEM = {5, 8, 0x24, 0x74, 0xBD, 0x2E, 0x24};
static uint8_t lcdCharLinkLost[] 	PROGMEM = {5, 8, 0xDF, 0xDF, 0x00, 0xDF, 0xDF};

// ------------------------------------- PID Definitions ------------------------------------- //
// ------------------------------------------------------------------------------------------- //

/// The PID Setup page for Roll Pitch and Yaw
// Header format                            11111 22222 33333
PROGMEM const prog_char pidHeaderRPY[] = "   Roll Pitch  Yaw";

/// the PID / APM setup page confirmation message
PROGMEM const prog_char confirmMessage[] =
//01234567890123456789
"This will apply the\n"
"changes made to the\n"
" settings, press\n"
"  OK to Upload";

// ----------------------------------- Tracker Definitions ----------------------------------- //
// ------------------------------------------------------------------------------------------- //

Servo Pan;		///< Pan servo
Servo Tilt;		///< Tilt servo
Tracker			tracker;						///< Antenna tracker

// ----------------------------------- Encoder Definitions ----------------------------------- //
// ------------------------------------------------------------------------------------------- //

RotaryEncoder   rotary(2, 3);                   ///< rotary encoder on digital 2 and 3, gnd on gnd // old : on A1/A2 gnd on A3
Buttons keypad;
Beep            beep(TONE_PIN);                 ///< tune machine

void doEncoder() {
  rotary.update();
}

// ------------------------------------- SD Definitions ------------------------------------- //
// ------------------------------------------------------------------------------------------ //

// set up variables using the SD utility library functions:
File root;
const int chipSelect = 53;

void
init_sdcard() {

  lcd.CursorTo(0, 6);
  pinMode(chipSelect, OUTPUT);

  if (!SD.begin(chipSelect)) {
    lcd.println("SD init fail!");
    return;
  }
  lcd.println("SD init ok");
}

void printDirectory(File dir, int numTabs) {
  uint8_t ff=0;
  while (ff<=8) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //       lcd.println("**nomorefiles**");
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      lcd.print('\t');
    }
    if (entry.isDirectory()) {
      //      lcd.println("/");
      //       printDirectory(entry, numTabs+1);
    } 
    else {
      lcd.println(entry.name());
      ff++;
      // files have sizes, directories do not
      //       lcd.print(" ");
      //       lcd.println(entry.size(), DEC);
    }
  }
}


