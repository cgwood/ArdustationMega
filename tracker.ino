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

void
Tracker::_update()
{
  // Find out how far away the UAV is
  _uavDist = _distance(uav.lat, uav.lon, ASM.lat, ASM.lon);

  // Calculate the bearing to the UAV
  _uavBear = _bearing(uav.lat, uav.lon, ASM.lat, ASM.lon);
  _uavBear = 180-(_uavBear/2.0) + _offset;

  if (_uavBear > 180.0)
    _uavBear -= 180.0;
  else if (_uavBear < 0.0)
    _uavBear += 180.0;

  // Point the antenna bearing servo
  _uavBear = constrain(_uavBear,0,180);
  Pan.write(_uavBear);

  // Calculate the elevation to the UAV
  _uavElev=toDeg(atan((uav.alt-ASM.alt)/_uavDist));
  _uavElev = constrain(_uavElev,0,90);
  _uavElev = 90-_uavElev;

  int tilt_position;
  tilt_position = _uavElev + 50;

  //	if (tilt_position > tilt_pos_upper_limit) tilt_position = tilt_pos_upper_limit;
  //
  //	if (tilt_position < tilt_pos_lower_limit) tilt_position = tilt_pos_lower_limit;
  tilt_position = constrain(tilt_position,tilt_pos_lower_limit,tilt_pos_upper_limit);

//  Tilt.write(tilt_position);
  Tilt.write((uav.roll+3.14159)*90.0/3.14159);
  //        Pan.write(90);
  //        Tilt.write(90);
}

float
Tracker::_bearing(float lat1, float lat2, float lon1, float lon2)
{
  float bearing;

  float x = 69.1 * (lat2 - lat1);
  float y = 69.1 * (lon2 - lon1) * cos(lat1/57.3);

  bearing = toDeg(atan2(y,x));

  if(bearing <= 0){
    bearing += 360;
  }

  return bearing;
}

float
Tracker::_distance(float lat1, float lat2, float lon1, float lon2)
{
  float x = 69.1 * (lat2 - lat1);
  float y = 69.1 * (lon2 - lon1) * cos(lat1/57.3);

  return (float)sqrt((float)(x*x) + (float)(y*y))*1609.344;
}

