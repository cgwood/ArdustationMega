// ASM Keypad class
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

enum BUTTON_IDS {
	B_NONE = 0,
	B_UP,
	B_DOWN,
	B_LEFT,
	B_RIGHT,
	B_OK,
	B_CANCEL,
	B_ENCODER,
	// Extra enumerations for holding buttons
	B_OK_HOLD,
	B_CANCEL_HOLD
};
//#define B_UP            1
//#define B_DOWN          2
//#define B_LEFT          3
//#define B_RIGHT         4
//#define B_OK            5
//#define B_CANCEL        6
//#define B_ENCODER       7

enum KEYPAD_ORIENTATIONS {
	// Define which side the connector is on (jDrones v1.2 button board)
	KP_CONN_RIGHT = 0,
	KP_CONN_DOWN,
	KP_CONN_LEFT,
	KP_CONN_UP
};

/// Button timing behaviour
#define BUTTON_DEBOUNCE_TIMER 10 /// Debounce timer (milliseconds)
#define BUTTON_HOLD_TIMER 500    /// Button repeat/hold delay (milliseconds)
#define BUTTON_REPEAT_TIMER 150  /// Button repeat period (milliseconds)

class Buttons {
public:
	Buttons() {
		_scanStart = millis();
		_lastPress = millis();
		_currentButton = 0;
		_buttonHolding = 0;
		_scanCode = 0;
		_scanCodeLast = 0;
	};

	uint8_t pressed(void);

private:
	uint8_t _scan(void);            // Determine which button is being pressed
	uint8_t _scanDebounced(void);   // Perform debounce

	uint8_t _currentButton;
	long    _scanStart;
	uint8_t _scanCode;
	uint8_t _scanCodeLast;
	long    _lastPress;
	boolean _buttonHolding;

	/// Functions for talking to the i2c expander
	void expanderWrite(byte _data);
	byte expanderRead(void);
};

