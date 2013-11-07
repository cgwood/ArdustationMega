/*
 * pagesParams.h
 *
 *  Created on: 7 Nov 2013
 *      Author: Colin
 */

#ifndef PAGESPARAMS_H_
#define PAGESPARAMS_H_

class PageParams :
public Pages {
public:
	PageParams(const prog_char *title,
			   const prog_char *ParamNames,
			   const prog_char **paramTable,
			   const uint8_t *ParamScales,
			   const uint8_t *ParamDPs,
			   const uint8_t ParamCount,
			   float *params,
			   bool *avail);

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
	// Parameter specific functions
	  void            _clearMarker(void);
	  void            _paintMarker(void);
	  void            _drawLocal();
	  void            _alterLocal(float alterMag);
	  void            _redrawLocal();
	  void            _voidLocal(void);
	  void            _uploadConfirm(void);
	  void            _uploadLocal(void);

private:
	uint8_t _linecount;
	gText _paramLCD;

private:
	/// current state of the internal navigation state machine
	uint8_t                 _state;

	/// Local editing temp value
	///
	float _value_temp;
	int   _value_encoder;

	/// Pointer to availability of parameter values
	bool *_avail;

	/// Pointer to parameter values
	float *_params;

	/// Position on the screen when scrolling
	/// Refers to first value out of four being displayed
	uint8_t	_stateFirstVal;

private:
	/// Title of the particular parameter page
	const prog_char *_title;

	/// text to be displayed for APM settings, up to xxx characters
	const prog_char *_ParamNames;

	/// MAVLink names of parameters
	const prog_char **_paramTable;

	/// Scaling for values, e.g. / 1000 is -3
	const uint8_t   *_ParamScales;

	/// How many decimal places the value is given
	const uint8_t   *_ParamDPs;

	/// How many parameters there are
	uint8_t	_paramCount;
};



#endif /* PAGESPARAMS_H_ */
