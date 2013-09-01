// Created 2011 By Colin G http://www.diydrones.com/profile/ColinG
// Modified 2013 by Colin G for use with Ardustation Mega http://www.diydrones.com/profiles/blogs/ardustation-mega-with-graphic-lcd


////////////////////////////////////////////////////////////////////////////////
// PID Setup page
////////////////////////////////////////////////////////////////////////////////

#define PIDFIELDWIDTH 4
#define PIDDECPOS     2

/// @todo Could make this a generic parameter editing page?
PagePID::PagePID() //const prog_char *textHeader, const uint8_t *pidTypes, const uint8_t *pid_p, const uint8_t *pid_i, const uint8_t *pid_d)
{
  //	/// Copy the header and types to internal storage
  //	_textHeader = textHeader;
  //	_pidTypes = pidTypes;
  //	_pid_p = pid_p;
  //	_pid_i = pid_i;
  //	_pid_d = pid_d;
  //

  _pid_p[0] = RATE_RLL_P;
  _pid_p[1] = RATE_PIT_P;
  _pid_p[2] = RATE_YAW_P;
  _pid_i[0] = RATE_RLL_I;
  _pid_i[1] = RATE_PIT_I;
  _pid_i[2] = RATE_YAW_I;
  _pid_d[0] = RATE_RLL_D;
  _pid_d[1] = RATE_PIT_D;
  _pid_d[2] = RATE_YAW_D;

//  uav.param[RATE_RLL_P] = 0.15;

  /// XXX Initially, all values are available (will require attention)
  _avail[0] = 1;
  _avail[1] = 1;
  _avail[2] = 1;

  _textHeader = pidHeaderRPY;
  _state = 0;
  //        lcd.ClearArea();
  //        _refresh_med();
}


uint8_t
PagePID::_enter() {
//  gcs3.params_request();
  _drawLocal();

  return 0;
}

uint8_t
PagePID::_forceUpdate(uint8_t reason) {
  if (reason == R_PARAM) {
    // Exit edit mode
    _state = 0;
  
    // Redraw the entire page
    _drawLocal();
  }

  return 0;
}

//void
//PagePID::_enter(uint8_t fromPage)
//{
////        if (fromPage == P_PIDCONFIRM || fromPage == P_NAVPIDCONFIRM)
////          _uploadLocal();
//
//        uint8_t i;
//
//        for (i=0;i<3;i++) {
//          //if (!_avail[i])
//          //  comm.send_msg_pid_request(_pidTypes[i]);
//        }
//
//        _state = 0;
//        lcd.ClearArea();
//        _render();
//}
//
//void
//PagePID::notify(void *arg, mavlink_message_t *buf)
//{
////  // Put the message to the protected message function
////  ((PagePID *)arg)->_message(messageID, messageVersion, buf);
//	((PagePID *)arg)->_message();
//
//}
//
//void
//PagePID::_message(void)
//{
//
//	// Exit edit mode
//	_state = 0;
//
//	// Notify the update function
//	_updated = true;
//
//}
//
//void
//PagePID::_update(void)
//{
//        // don't waste time redrawing if we have no changes to announce
//        if (!_updated)
//                return;
//
//        // don't redraw too often
//        if ((millis() - _lastRedraw) < STATUS_UPDATE_INTERVAL)
//                return;
//
//        // redraw the page
//        _render();
//        _updated = false;
//        _lastRedraw = millis();
//}
//
//


uint8_t
PagePID::_refresh_med()
{
}

uint8_t
PagePID::_refresh_slow()
{

  return 0;
}

void
PagePID::_drawLocal()
{
  uint8_t         c;
  uint8_t         i;
  uint8_t         j;
  uint8_t         lineno;
  char            decBuf[PIDFIELDWIDTH];
  uint32_t        value;
  float			value_local;

  lcd.CursorTo(0, 0);
  for (i=0;i<19;i++) {
    // Write the header, e.g. roll pitch yaw
    c = pgm_read_byte_near(_textHeader + i);
    if (0 == c)
      break;
    else
      lcd.write(c);
  }
  //        lcd.print(" roll pitch yaw");
  for (lineno=1;lineno<4;lineno++) {
    lcd.CursorTo(0, lineno);
    // Write the left hand labels for PID
    if (lineno==1)
      lcd.print("P:");
    if (lineno==2)
      lcd.print("I:");
    if (lineno==3)
      lcd.print("D:");

    // Write the values
    for (i=0;i<3;i++) {
      value_local = 0;
      if (_avail[i] == 1) {
        // Load the PID into value, either editing or live val
        if (lineno==1) {
          //					if (i == (_state-101)%3)
          if (_state == 100 + (lineno-1)*3 + i + 1)
            value_local = _value_temp; //_pidtemp.p;
          else {
            j = _pid_p[i];
            //						nvram.load_param(&j,&value_local);
            value_local = uav.param[j];
          }
        }
        if (lineno==2)   {
          //					if (i == (_state-101)%3)
          if (_state == 100 + (lineno-1)*3 + i + 1)
            value_local = _value_temp; //_pidtemp.i;
          else {
            j = _pid_i[i];
            //						nvram.load_param(&j,&value_local);
            value_local = uav.param[j];
          }
        }
        if (lineno==3) {
          //					if (i == (_state-101)%3)
          if (_state == 100 + (lineno-1)*3 + i + 1)
            value_local = _value_temp; //_pidtemp.d;
          else {
            j = _pid_d[i];
            //						nvram.load_param(&j,&value_local);
            value_local = uav.param[j];
          }
        }

        // Scale the PID value and fix for the number of decimal places
        value_local *= pow(10,PIDDECPOS);
        value = (uint32_t)(floor(value_local+0.5));
        decBuf[0] = '0' + (value % 10);
        value /= 10;
        for (j=1;j<PIDFIELDWIDTH;j++) {
          if (j == PIDDECPOS) {
            decBuf[j] = '.';
          }
          else if ((0 == value) && (j > (PIDDECPOS + 1))) {
            decBuf[j] = ' ';
          }
          else {
            decBuf[j] = '0' + (value % 10);
            value /= 10;
          }
        }
      }
      else {
        // Data unavailable, display dashes
        for (j=0;j<PIDFIELDWIDTH;j++)
          decBuf[j] = '-';
      }
      // Display the data
      lcd.write(' ');
      while (j-- > 0)
        lcd.write(decBuf[j]);
      lcd.write(' ');
    }
  }

  // redraw the "choosing" marker
  _paintMarker();
}

/// Draw the "choosing" marker
void
PagePID::_paintMarker(void)
{
  if (_state > 0 && _state < 100) {
    lcd.CursorTo(((_state-1)%3+1)*6-4,(_state-1)/3+1);
    lcd.write('>');
  }
  else if (_state > 100 && _state < 200)
  {
    lcd.CursorTo(((_state-101)%3+1)*6-4,(_state-101)/3+1);
    lcd.write('#');//LCD_CHAR_MODIFY);
  }
}

/// remove the "choosing" marker
void
PagePID::_clearMarker(void)
{
  if (_state > 0 && _state < 200) {
    if (_state > 100)
      lcd.CursorTo(((_state-101)%3+1)*6-4,(_state-101)/3+1);
    else
      lcd.CursorTo(((_state-1)%3+1)*6-4,(_state-1)/3+1);
    lcd.write(' ');
  }
}

void
PagePID::_alterLocal(float alterMag)
{
  //  uint8_t i,j;
  //  i = (_state-101)%3; // Column, i.e. Roll/pitch/yaw
  //  j = (_state-101)/3; // Row, i.e. PID

  _value_temp = constrain(_value_temp + alterMag, 0, 3);
  _value_encoder = _value_temp*100;
  //  //@bug sometimes this makes the value wrap around
  //  //@{
  //  switch ((_state-101)/3) {
  //    case 0:
  //      _pidtemp.p = constrain(_pidtemp.p + alterMag, 0, 3000000);
  //      break;
  //    case 1:
  //      _pidtemp.i = constrain(_pidtemp.i + alterMag, 0, 3000000);
  //      break;
  //    case 2:
  //      _pidtemp.d = constrain(_pidtemp.d + alterMag, 0, 3000000);
  //      break;
  //  }
  //  //@}

  // kick the update function
  _updated = true;
}

void
PagePID::_redrawLocal(void)
{
  //	uint8_t         c;
  uint8_t         i;
  uint8_t         j;
  //	uint8_t         lineno;
  char            decBuf[PIDFIELDWIDTH];
  uint32_t        value;
  float			value_local;

  // Check that the state is not in confirmation mode and a value is being pointed at
  if (_state < 200 && _state > 0) {
    if (_state > 100) {
      value_local = _value_temp;
      lcd.CursorTo(((_state-101)%3+1)*6-3,(_state-101)/3+1);
    }
    else {
      lcd.CursorTo(((_state-1)%3+1)*6-3,(_state-1)/3+1);
      Serial.println(_state);
      Serial.println(((_state-1)%3+1)*6-3);
      Serial.println((_state-1)/3+1);
    }

    // Scale the PID value and fix for the number of decimal places
    value_local *= pow(10,PIDDECPOS);
    value = (uint32_t)(floor(value_local+0.5));
    decBuf[0] = '0' + (value % 10);
    value /= 10;
    for (j=1;j<PIDFIELDWIDTH;j++) {
      if (j == PIDDECPOS) {
        decBuf[j] = '.';
      }
      else if ((0 == value) && (j > (PIDDECPOS + 1))) {
        decBuf[j] = ' ';
      }
      else {
        decBuf[j] = '0' + (value % 10);
        value /= 10;
      }
    }
    //j = PIDFIELDWIDTH;

    // Display the data
    while (j-- > 0)
      lcd.write(decBuf[j]);
  }
}

void
PagePID::_voidLocal(void)
{  
  uint8_t j;

  // Overwrite value
  switch((_state-101)/3) {
  case 0:
    j = _pid_p[(_state-101)%3];
    break;
  case 1:
    j = _pid_i[(_state-101)%3];
    break;
  case 2:
    j = _pid_d[(_state-101)%3];
    break;
  default:
    return;
  }
  _value_temp = uav.param[j];
  _redrawLocal();

  // Reset _state
  _state = 0;

  // kick the update function
  _updated = true;
}

void
PagePID::_uploadConfirm(void)
{
  uint8_t         c;
  uint8_t         row;
  const prog_char *str;
  str = confirmMessage;

  //  lcd.CursorTo(0, 0);
  lcd.ClearArea();
  row = 0;

  for (;;) {
    c = pgm_read_byte_near(str++);
    if (0 == c)
      break;
    if ('\n' == c) {
      lcd.CursorTo(0, ++row);
      continue;
    }
    // emit
    lcd.write(c);
  }
}

void
PagePID::_uploadLocal(void)
{
  //  uint8_t i;
  uint8_t j;
  //  i = (_state-101)%3; // Column, i.e. Roll/pitch/yaw

  switch((_state-201)/3) {
  case 0:
    j = _pid_p[(_state-201)%3];
    break;
  case 1:
    j = _pid_i[(_state-201)%3];
    break;
  case 2:
    j = _pid_d[(_state-201)%3];
    break;
  default:
    return;
  }
  
  Serial.println("Uploading value");
  
  
    char str_param_id[15];
    
	// First initialise the string to be empty
	for (uint8_t i=0;i<15;i++)
		str_param_id[i] = 0;

	// Copy the relevant one into memory
    strcpy_P(str_param_id, (char*)pgm_read_word(&(paramTable_copter[j])));
    
    // Construct the packet
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
//static inline uint16_t mavlink_msg_param_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
//						       uint8_t target_system, uint8_t target_component, const char *param_id, float param_value, uint8_t param_type)
    mavlink_msg_param_set_pack(0xFF, 0x00, &msg, uav.sysid, apm_mav_component, (const char*)&str_param_id, _value_temp, MAV_PARAM_TYPE_REAL32);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    Serial3.write(buf, len);

  //  params.set_param(j, _value_temp); // want this one, but need params


  //  params.set_param(_pid_p[i], _value_temp);


  // Send the value that we edited
  //comm.send_msg_pid_set(_pidTypes[(_state-201)%3], _pidtemp.p, _pidtemp.i, _pidtemp.d, _pidtemp.integratorMax);


  // Reset _state
  _state = 0;

  // kick the update function
  _updated = true;
}

uint8_t 
PagePID::_interact(uint8_t buttonid)
{
  /// State positions are:
  /// 1   2   3
  /// 4   5   6
  /// 7   8   9

  _clearMarker();

  switch(buttonid) {
  case B_UP:
    // Navigation
    if (_state > 3 && _state < 10)
      _state -= 3;
    // Editing
    else if (_state > 100 && _state < 200)
      _alterLocal(0.01);
    // Confirming
    else if (_state > 200)
      return 0;
    break;
  case B_DOWN:
    // Navigation
    if (_state > 0 && _state < 7)
      _state += 3;
    // Editing
    else if (_state > 100 && _state < 200)
      _alterLocal(-0.01);
    // Confirming
    else if (_state > 200)
      return 0;
    break;
  case B_OK:
    if (_state == 0) {
      // Allow selection if the value is available
      //                  if (_avail[(_state-1)%3])
      if (_avail[0])
        _state = 1;
    }
    else if(_state < 100) {
      // Allow editing if the value is available
      if (_avail[(_state-1)%3]) {
        // Copy the PID values to temp for editing
        uint8_t j;
        switch((_state-1)/3) {
        case 0:
          j = _pid_p[(_state-1)%3];
          break;
        case 1:
          j = _pid_i[(_state-1)%3];
          break;
        case 2:
          j = _pid_d[(_state-1)%3];
          break;
        default:
          return 0;
        }

        //                	  nvram.load_param(&j,&_value_temp);
        _value_temp = uav.param[j];
        _value_encoder = _value_temp * 100;
        rotary.configure(&_value_encoder, 300, 0, -4);
        //rotary.configure(&encoderval, &_value_temp, 0, 10, -4);

        //                    _pidtemp.pidSet = _pidlive[(_state-1)%3].pidSet;
        //                    _pidtemp.p = _pidlive[(_state-1)%3].p;
        //                    _pidtemp.i = _pidlive[(_state-1)%3].i;
        //                    _pidtemp.d = _pidlive[(_state-1)%3].d;
        //                    _pidtemp.integratorMax = _pidlive[(_state-1)%3].integratorMax;
        // Update the state
        _state += 100;
      }
    }
    else if(_state > 100 && _state < 200) {
      // Save the value
      //_leave(OK);
      _state += 100;
      _uploadConfirm();
      return 0; // Leave before we draw the marker again
    }
    else if(_state > 200) {
      // Save the value
      //_leave(OK);
      _uploadLocal();
      _drawLocal();
      return 0; // Leave before we draw the marker again
    }
    break;
  case B_ENCODER:
    if (_state > 100 && _state < 200) {
      _value_temp = _value_encoder / 100.0;
      _redrawLocal();
    }
    break;
  case B_CANCEL:
    if (_state == 0) {
      Pages::move(0);
      return 0;         // avoid drawing the cursor
    }
    else {
      if (_state > 100) {
        if (_state > 200) {
          _drawLocal();
        }

        // Don't save the changes to local variable
        _voidLocal();

        // Ensure state has returned to nothing selected
        _state = 0;
      }
      else
        _state = 0;
    }
    break;
  case B_LEFT:
    // Navigation
    if (_state == 0)
      Pages::move(-1);
    else if (_state < 100 && _state % 3 != 1)
      _state -= 1;
    // Editing
    else if (_state > 100 && _state < 200)
      _alterLocal(-0.1);
    // Confirming
    else if (_state > 200)
      return 0;
    break;
  case B_RIGHT:
    // Navigation
    if (_state == 0)
      Pages::move(1);
    else if (_state < 100 && _state % 3 != 0)
      _state += 1;
    // Editing
    else if (_state > 100 && _state < 200)
      _alterLocal(0.1);
    // Confirming
    else if (_state > 200)
      return 0;
    break;
    //  case B_LEFT:
    //    Pages::move(-1);
    //    break;
    //  case B_RIGHT:
    //    Pages::move(1);
    //    break;
  }

  _paintMarker();
}


