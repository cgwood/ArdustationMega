/*
 * pagesPlane.h
 *
 *  Created on: 19 Oct 2013
 *      Author: Colin
 */

#ifndef PAGESPLANE_H_
#define PAGESPLANE_H_


// Parameters Page
class PagePlaneParameters :
public Pages {
public:
  PagePlaneParameters();

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason);

  /// One off function, executes on uav type change
  virtual uint8_t _redefine();

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
  uint8_t                 _state;

  /// Local editing temp value
  ///
  float _value_temp;
  int   _value_encoder;

  /// Availability of paramter values
  bool            _avail[PARAM_COUNT_PLANE];

  /// Position on the screen when scrolling
  /// Refers to first value out of four being displayed
  uint8_t	_stateFirstVal;

protected:
  /// flag indicating that the data the page should be redrawn
  bool            _updated;

  /// timestamp of the last page redraw, used to rate-limit redraw operations
  unsigned long   _lastRedraw;

  /// text to be displayed for APM settings, up to xxx characters
  const prog_char *_textHeader;

  /// Types to be displayed (in same order as _textHeader)
  uint8_t   _Types[PARAM_COUNT_PLANE];

  /// Scaling for values, e.g. / 1000 is -3
  uint8_t   *_scale;

  /// How many decimal places the value is given
  uint8_t   *_decPos;

  /// How many parameters there are
  uint8_t	_paramCount;
};


// Parameters Page
class PagePlaneParametersCTUN :
public Pages {
public:
  PagePlaneParametersCTUN();

protected:
  /// One off function, executes on page enter
  virtual uint8_t _enter();

  /// Force update the page
  virtual uint8_t _forceUpdate(uint8_t reason);

  /// One off function, executes on uav type change
  virtual uint8_t _redefine();

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
  uint8_t                 _state;

  /// Local editing temp value
  ///
  float _value_temp;
  int   _value_encoder;

  /// Availability of paramter values
  bool            _avail[PARAM_COUNT_PLANE_CTUN];

  /// Position on the screen when scrolling
  /// Refers to first value out of four being displayed
  uint8_t	_stateFirstVal;

protected:
  /// flag indicating that the data the page should be redrawn
  bool            _updated;

  /// timestamp of the last page redraw, used to rate-limit redraw operations
  unsigned long   _lastRedraw;

  /// text to be displayed for APM settings, up to xxx characters
  const prog_char *_textHeader;

  /// Types to be displayed (in same order as _textHeader)
  uint8_t   _Types[PARAM_COUNT_PLANE_CTUN];

  /// Scaling for values, e.g. / 1000 is -3
  uint8_t   *_scale;

  /// How many decimal places the value is given
  uint8_t   *_decPos;

  /// How many parameters there are
  uint8_t	_paramCount;
};


#endif /* PAGESPLANE_H_ */
