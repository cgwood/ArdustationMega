// ASM Keypad class
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// Todo, enum these
#define B_UP            1
#define B_DOWN          2
#define B_LEFT          3
#define B_RIGHT         4
#define B_OK            5
#define B_CANCEL        6
#define B_ENCODER       7

/// Debounce timer (milliseconds)
#define BUTTON_DEBOUNCE_TIMER   10

/// Button repeat period (milliseconds)
#define BUTTON_REPEAT_TIMER 250


class Buttons {
public:
  Buttons() {
    _lastPress = millis();
  };
  
  uint8_t  pressed(void);

private:
  uint8_t  _scan(void);            // Determine which button is being pressed
  uint8_t  _scanDebounced(void);   // Perform debounce
  
  uint8_t  _currentButton;
  long     _scanStart;
  uint8_t  _scanCode;
  long	   _lastPress;

  /// Functions for talking to the i2c expander
  void expanderWrite(byte _data);
  byte expanderRead(void);
};


