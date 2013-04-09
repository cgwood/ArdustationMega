// Page class for ASM
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

#ifndef PAGES_H_
#define PAGES_H_


/* Look-up sine table for integer math */
byte byteSine[16] = {
  0,  27,  54,  79, 104, 128, 150, 171, 190, 201, 221, 233, 243, 250, 254, 255} 
;

class Pages {
public:
  // ----- Declare page order here ----- //
  enum PAGEIDS {
    P_MAIN = 0,
    P_HARDWARE,
    P_UAVTEST,
    P_COMMANDS,
    P_GLCD,
    P_SD,
    P_PID,
    P_COUNT
  };
  
public:
  /// Constructor
  Pages() {
  };

  /// Move page
  static uint8_t move(int8_t dir);

  /// Force update the page
  static void forceUpdate(uint8_t reason);

  /// Interact with the page
  static void interact(uint8_t buttonid);
  
  /// Force the page enter function (used for startup)
  static uint8_t enter();

  /// refresh page - medium items (10Hz)
  static uint8_t refresh_med();

  /// refresh page - slow items (0.5 Hz)
  static uint8_t refresh_slow();

  /// Directions
  enum DIRECTIONS {
    D_UP = 1,
    D_DOWN,
    D_LEFT,
    D_RIGHT
  };

  /// Update reasons
  enum REASONS {
    R_PARAM = 0,
    R_COUNT
  };

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason);

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
  
  /// One off function, executes on leaving a page
  //virtual uint8_t _leave();

private:
  /// Access to the pages
  static Pages		*_currPage(uint8_t pageid);
};


// Main page
class PageMain : 
public Pages {
public:
  PageMain() {
    _alt_icon=icon_altitude_small;
    _sat_icon=sat;
    _batt_icon=icn_batt;
    _conn_icon=icn_conn;
    _speed_icon=icn_speed;
  };

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {lcd.ClearArea();GLCD.ClearScreen();_enter();};

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
  
private:
  gText _textArea;
  Image_t _alt_icon;
  Image_t _sat_icon;
  Image_t _batt_icon;
  Image_t _conn_icon;
  Image_t _speed_icon;
  
private:
  uint8_t _last_base_mode;
};

// Hardware display page
class PageHardware : 
public Pages {
public:
  PageHardware() {
  };

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter() {
    _refresh_med();
    _refresh_slow();
  };

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {
  };

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
};

// Test display page
class PageUAVtest : 
public Pages {
public:
  PageUAVtest() {
  };

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter() {
  };

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {
  };

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
};

// Test display page
class PageGLCDtest : 
public Pages {
public:
  PageGLCDtest() {
  };

private:
  byte x0,y0,x1,y1;
  void calcangle(byte *x, byte *y);

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter() {
  };

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {
  };

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
};

// Test display page
class PageSDtest : 
public Pages {
public:
  PageSDtest() {
  };

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {};

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
};

// Commands page
class PageCommands : 
public Pages {
public:
  PageCommands();

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason) {};

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);
  
protected:
  void            _clearMarker(void);
  void            _paintMarker(void);
  void            _drawLocal();
  void  _commandConfirm();
  void  _commandConfirmMessage(const prog_char *str);
  void  _commandSend();
//  void            _alterLocal(float alterMag);
//  void            _redrawLocal();
//  void            _voidLocal(void);
//  void            _uploadConfirm(void);
//  void            _uploadLocal(void);

private:
  /// text to be displayed for APM settings, up to xxx characters
  const prog_char *_textCommands;
  
  /// current state of the internal navigation state machine
  uint8_t                     _state;
  
  /// Position on the screen when scrolling
  /// Refers to first value out of four being displayed
  uint8_t                     _stateFirstVal;
  /// flag indicating that the data the page should be redrawn
  bool            _updated;
};

// PID Page
class PagePID : 
public Pages {
public:
  PagePID();

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason);

  /// refresh page - medium items (10Hz)
  virtual uint8_t _refresh_med();

  /// refresh page - slow items (0.5 Hz)
  virtual uint8_t _refresh_slow();

  /// Interact with the page
  virtual uint8_t _interact(uint8_t buttonid);

protected:
  void            _clearMarker(void);
  void            _paintMarker(void);
  void            _drawLocal();
  void            _alterLocal(float alterMag);
  void            _redrawLocal();
  void            _voidLocal(void);
  void            _uploadConfirm(void);
  void            _uploadLocal(void);
private:
  /// current state of the internal navigation state machine
  /// 0         = Viewing
  /// 1 - 9     = Navigating
  /// 101 - 109 = Editing
  /// 201 - 209 = Uploading
  uint8_t                 _state;
  /// Values onboard the aircraft
  //struct msg_pid          _pidlive[3];
  /// Local temp value
  //struct msg_pid          _pidtemp;

  /// Local editing temp value
  ///
  float _value_temp;
  int   _value_encoder;

  /// Availability of pid values
  bool                             _avail[3];

protected:
  /// flag indicating that the data the page should be redrawn
  bool            _updated;

  /// timestamp of the last page redraw, used to rate-limit redraw operations
  unsigned long   _lastRedraw;

  /// text to be displayed for PID headings
  const prog_char *_textHeader;

  /// PID Types to be displayed (in same order as _textHeader)
  const uint8_t   *_pidTypes;

  /// PID indices (in same order as _textHeader)
  uint8_t   _pid_p[3];
  uint8_t   _pid_i[3];
  uint8_t   _pid_d[3];
};


#endif /* PAGES_H_ */

