// Antenna tracking class
// Created 2011 By Colin G http://www.diydrones.com/profile/ColinG

Tracker::Tracker()
{
  // Set some servo variables
  _pan=90; 
  _tilt=90; 
  _offset=0;

  // Start the pan and tilt servos
  //	Pan.attach(6);// Ultimately make the end points as variables on some input screen
  //	Tilt.attach(7);
  Pan.write(90);
  Tilt.write(90);

  // Time stamp of last update
  _last_update = millis();
}

//void
//Tracker::notify(void *arg, mavlink_message_t *messageData)
//{
//
//}

void
Tracker::update()
{
  //  if (millis() - _last_update > TRACKERPERIOD) {
  //    _last_update = millis();
  _update();
  //  }
}

float
Tracker::get_dist(void)
{
	return _uavDist;
}

float
Tracker::get_bearing(void)
{
	return _uavBear;
}

float
Tracker::get_elevation(void)
{
	return _uavElev;
}

float
Tracker::get_servo_pan(void)
{
	return _pan;
}

float
Tracker::get_servo_tilt(void)
{
	return _tilt;
}

void
Tracker::set_bearing(float fake_bearing) {
	_fakeBear = fake_bearing;
}

void
Tracker::set_elevation(float fake_elevation) {
	_fakeElev = fake_elevation;
}

void
Tracker::_update()
{
	// Calculate the distance and bearing to the UAV
  _calcs(ASM.lat, uav.lat, ASM.lon, uav.lon);

  // Turn bearing into a servo pan command
  _pan = _uavBear;
  while (_pan <= 0){
	  _pan += 360;
  }
  _pan = constrain(_pan,0,360);
  _pan = map(_pan,0,360,0,180);

  Pan.write(_pan);

  // Calculate the elevation to the UAV
  if (_fakeElev == -1) {
	  if (_uavDist == 0)
		  _uavElev = 0;
	  else
		  _uavElev = toDeg(atan((uav.alt-ASM.alt)/_uavDist));
  }
  else
	  _uavElev = _fakeElev;

  // Turn elevation into servo tilt command
  _tilt = constrain(_uavElev,0,90);
  _tilt = map(_tilt,0,90,0,180);
  Tilt.write(_tilt);

//  _tilt = 90-_tilt;
//  _tilt = _tilt + 50;
//  _tilt = constrain(_tilt,tilt_pos_lower_limit,tilt_pos_upper_limit);

//  Tilt.write(_tilt);
//  Tilt.write((uav.roll+3.14159)*90.0/3.14159);
}

void
Tracker::_calcs(float lat1, float lat2, float lon1, float lon2)
{
  float bearing;

  float x = 69.1 * (lat2 - lat1);
  float y = 69.1 * (lon2 - lon1) * cos(lat1/57.3);

  if (_fakeBear == -1)
	  _uavBear = toDeg(atan2(y,x));
  else
	  _uavBear = _fakeBear;

  _uavDist = (float)sqrt((float)(x*x) + (float)(y*y))*1609.344;

}

