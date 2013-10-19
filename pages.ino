// Page class for ASM
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// ------ Declare each page ------ //
PageMain mainPage;
PageHardware hardwarePage;
PageUAVtest  UAVtestPage;
PageGLCDtest  GLCDPage;
PageSDtest  SDPage;
PagePID  PIDPage;
PagePlaneParameters  PlaneParametersPage;
PageCommands    CommandsPage;     ////< a page for sending commands to the APM

// Index of current page
uint8_t pageindex=0;

// Local pages - updated for different autopilot types
uint8_t _pageids[20];
uint8_t _pagecount;

Pages::Pages()
{
	// Hack. Would prefer to just call definePages from here
	//	definePages();

	// Default option - AP Unknown
	_pageids[0] = P_MAIN;
	_pageids[1] = P_COMMANDS;
	_pageids[2] = P_HARDWARE;
	_pageids[3] = P_UAVTEST;
	_pageids[4] = P_GLCD;
	_pageids[5] = P_SD;
	_pagecount = 6;

}

uint8_t
Pages::definePages()
{
	if (uav.type==MAV_TYPE_FIXED_WING) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_PARAMETERS; // APM parameters
		_pageids[3] = P_HARDWARE;
		_pageids[4] = P_UAVTEST;
		_pageids[5] = P_GLCD;
		_pageids[6] = P_SD;
		_pagecount = 7;
	}
	else if (uav.type==MAV_TYPE_QUADROTOR) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_PID;       // ACM PIDs (subset of)
		_pageids[3] = P_HARDWARE;
		_pageids[4] = P_UAVTEST;
		_pageids[5] = P_GLCD;
		_pageids[6] = P_SD;
		_pagecount = 7;
	}
	else if (uav.type==MAV_TYPE_GROUND_ROVER) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_PARAMETERS; // APM parameters
		_pageids[3] = P_HARDWARE;
		_pageids[4] = P_UAVTEST;
		_pageids[5] = P_GLCD;
		_pageids[6] = P_SD;
		_pagecount = 7;
	}
	else { // Default option - AP Unknown
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_HARDWARE;
		_pageids[3] = P_UAVTEST;
		_pageids[4] = P_GLCD;
		_pageids[5] = P_SD;
		_pagecount = 6;
	}
	pageindex = 0;

	for (uint8_t p=0;p<_pagecount;p++)
		_currPage(p)->_redefine();

	return 0;
}

Pages*
Pages::_currPage(uint8_t pageid)
{
  // ----- Assign each page declared above to the page ordering enumerated in pages.h ----- //
  switch(_pageids[pageid]) {
  case P_MAIN:
    return(&mainPage);
    break;
  case P_HARDWARE:
    return(&hardwarePage);
    break;
  case P_UAVTEST:
    return(&UAVtestPage);
    break;
  case P_COMMANDS:
    return(&CommandsPage);
    break;
  case P_GLCD:
    return(&GLCDPage);
    break;
  case P_SD:
    return(&SDPage);
    break;
  case P_PID:
    return(&PIDPage);
    break;
  case P_PARAMETERS:
    return(&PlaneParametersPage);
    break;
  }
}

uint8_t
Pages::move(int8_t dir)
{
  if (dir == 0) {
    pageindex = 0;
  }
  else if (dir > 0 || pageindex > 0) {
    pageindex += dir;
  }
  else
    return(0);

  if (pageindex >= _pagecount) {//P_COUNT) {
    pageindex --;
    return(0);
  }

  // Beep about it
  //	beep.play(BEEP_KEY);

  // Draw all of the new page now
  lcd.ClearArea();
  GLCD.ClearScreen();
  _currPage(pageindex)->_enter();
  _currPage(pageindex)->_refresh_slow();
  _currPage(pageindex)->_refresh_med();
  return(0);
}

uint8_t
Pages::enter() {
  lcd.ClearArea();
  GLCD.ClearScreen();
  _currPage(pageindex)->_enter();
  _currPage(pageindex)->_refresh_slow();
  _currPage(pageindex)->_refresh_med();
  
  return 0;
}

void
Pages::forceUpdate(uint8_t reason)
{
  _currPage(pageindex)->_forceUpdate(reason);
}

void
Pages::interact(uint8_t buttonid)
{
  _currPage(pageindex)->_interact(buttonid);
}

uint8_t
Pages::refresh_med()
{
  _currPage(pageindex)->_refresh_med();
  return(0);
}

uint8_t
Pages::refresh_slow()
{
  _currPage(pageindex)->_refresh_slow();
  return(0);
}

uint8_t
PageMain::_enter()
{
  // Reset last base mode to force the screen update
  _last_base_mode = 0;
    
//  GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX -3, GLCD.Bottom, 5);  // rounded rectangle around text area 
//  _textArea.DefineArea(GLCD.CenterX + 5, 3, GLCD.Right-2, GLCD.Bottom-4, SCROLL_UP); 
//  _textArea.SelectFont(System5x7, BLACK);
//  _textArea.CursorTo(0,0);
//  _textArea.print('A');

  // Name by autopilot type and system id (later could assign these to names, e.g. Twinstar)
  GLCD.CursorToXY(3,3);
  GLCD.SelectFont(Arial_bold_14);
  if (uav.sysid == 0) {
    GLCD.print("No Connection");
  }
  else {
    switch (uav.autopilot) {
      case MAV_AUTOPILOT_GENERIC:
        GLCD.Printf("MAV-%03i", uav.sysid);
        break;
      case MAV_AUTOPILOT_PIXHAWK:
        GLCD.Printf("PIX-%03i", uav.sysid);
        break;
      case MAV_AUTOPILOT_ARDUPILOTMEGA:
    	  if (uav.type==MAV_TYPE_QUADROTOR)
    		  GLCD.Printf("ACM-%03i", uav.sysid);
    	  else if (uav.type==MAV_TYPE_FIXED_WING)
    		  GLCD.Printf("APM-%03i", uav.sysid);
    	  else if (uav.type==MAV_TYPE_GROUND_ROVER)
    		  GLCD.Printf("Rover-%03i", uav.sysid);
    	  else
    		  GLCD.Printf("AxM-%03i", uav.sysid);
        break;
      case MAV_AUTOPILOT_UDB:
        GLCD.Printf("UDB-%03i", uav.sysid);
        break;
      case MAV_AUTOPILOT_PX4:
        GLCD.Printf("PX4-%03i", uav.sysid);
        break;
      default:
        GLCD.Printf("MAV-%03i", uav.sysid);
    }
  }
  GLCD.SelectFont(System5x7);
  
  // Remote Battery bar on right side of screen
  GLCD.DrawRect(GLCD.Right-8, 2, 6, GLCD.Bottom-12);

  // Bearing to UAV, circle
  if (uav.sysid != 0) {
    GLCD.DrawCircle(GLCD.Right-27, 16, 12);
    //GLCD.DrawLine(GLCD.Right-27, 16, GLCD.Right-27+5, 16-5);
  }
  
  // Local Battery bar on left side of screen
  GLCD.DrawRect(2, 20, 6, GLCD.Bottom-30);
  
  // Local RSSI bar on left side of screen
  GLCD.DrawRect(10, 20, 6, GLCD.Bottom-30);
  
  // Altitude bitmap
  GLCD.DrawBitmap(_alt_icon, 27, GLCD.Bottom - 26);
  
  // Speed bitmap
  GLCD.DrawBitmap(_speed_icon, 63, GLCD.Bottom - 26);
  
  // Sat bitmap
  GLCD.DrawBitmap(_sat_icon, GLCD.Right-26, GLCD.Bottom - 25);
  
  // Batt bitmaps
  GLCD.DrawBitmap(_batt_icon, 1, GLCD.Bottom - 8);
  GLCD.DrawBitmap(_batt_icon, GLCD.Right-9, GLCD.Bottom - 8);
  
  // Conn bitmap
  GLCD.DrawBitmap(_conn_icon, 10, GLCD.Bottom - 8);
}

uint8_t
PageMain::_refresh_med()
{
  
//  GLCD.FillRect(GLCD.Right-7, 3, 4, GLCD.Bottom-6, WHITE);
  if (gps.num_sats > 9)
	  GLCD.CursorToXY(GLCD.Right-20, GLCD.Bottom - 8);
  else
	  GLCD.CursorToXY(GLCD.Right-18, GLCD.Bottom - 8);
  GLCD.SelectFont(System5x7);
  GLCD.print(gps.num_sats);
  
  // UAV Mode
  if (uav.sysid != 0 && _last_base_mode != uav.base_mode) {
    _last_base_mode = uav.base_mode; // Only update when mode changes
    
    GLCD.CursorToXY(20,20);
    GLCD.print("           ");
    GLCD.CursorToXY(20,20);
    GLCD.SelectFont(System5x7);
    if (uav.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) {
      GLCD.print("Armed");
    }
    else {
      GLCD.print("Safe");
    }
    
    if (uav.base_mode & MAV_MODE_FLAG_STABILIZE_ENABLED) {
      GLCD.print(", Stab");
    }
    else {
      GLCD.print(", Acro");
    }
  }
}

uint8_t
PageMain::_refresh_slow()
{
  uint8_t batt_height;
  uint8_t rssi_height;
  float batt_ratio, rssi_ratio;
  
  int rssi;
  
  // Local battery level
  batt_height = GLCD.Bottom-32;
  GLCD.FillRect(3, 21, 4, batt_height, WHITE); // Clear area
  batt_ratio = constrain((get_batt()-3.2),0.0,1.0); // Allows range between 3.2 and 4.2  
  batt_height = batt_height * batt_ratio;
  GLCD.FillRect(3, 21+GLCD.Bottom-32-batt_height, 4, batt_height, BLACK); // Fill area
  
  // RSSI level
  rssi = get_rssi();
  rssi_height = GLCD.Bottom-32;
  GLCD.FillRect(11, 21, 4, rssi_height, WHITE); // Clear area
  rssi_ratio = rssi/1023.0;
  rssi_height = rssi_height * rssi_ratio;
  GLCD.FillRect(11, 21+GLCD.Bottom-32-rssi_height, 4, rssi_height, BLACK); // Fill area
  
  // Remote battery level
  batt_height = GLCD.Bottom-14;
  GLCD.FillRect(GLCD.Right-7, 3, 4, batt_height, WHITE);
  batt_height = batt_height * uav.load/1000.0;
  GLCD.FillRect(GLCD.Right-7, 3+GLCD.Bottom-14-batt_height, 4, batt_height, BLACK);

  // Clear bottom line
  GLCD.FillRect(23,GLCD.Bottom-8,GLCD.Right-43,7,WHITE);
  
  // Print Altitude
  GLCD.SelectFont(System5x7);
  GLCD.CursorToXY(23,GLCD.Bottom-8);
//  GLCD.print(constrain(uav.alt,-99.9,999.9),1);
  if (gps.altitude > 10000)
    GLCD.print(constrain((int)(gps.altitude/100.0),0,9999));
  else if (gps.altitude > 0)
    GLCD.print(constrain(gps.altitude/100.0,0,99.9),1);
  else
    GLCD.print(constrain(gps.altitude/100.0,-99,0),1);
  GLCD.print('m');
  
  // Print Velocity
  GLCD.SelectFont(System5x7);
  GLCD.CursorToXY(59,GLCD.Bottom-8);
  if (gps.speed_3d > 10000)
    GLCD.print(constrain((int)(gps.speed_3d/100.0),0,999));
  else
    GLCD.print(constrain(gps.speed_3d/100.0,-9.9,99.9),1);
  GLCD.print("m/s");
  
  
//  // Find difference in latitude and longitude between GCS and UAV
//  float lat1 = ((float)gps.latitude / T7 * 3.14159/180.0);
//  float lat2 = uav.lat * 3.14159/180.0;
//  float lon1 = ((float)gps.longitude / T7 * 3.14159/180.0);
//  float lon2 = uav.lon * 3.14159/180.0;
//  float dlat = lat2 - lat1;
//  float dlong = lon2 - lon1;
//
//  // Calculate distance from Home
//  float a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos((float)gps.latitude / T7 * 3.14159/180.0) * cos(uav.lat * 3.14159/180.0) * sin(dlong / 2.0) * sin(dlong / 2.0);
//  float dist = 6371000.0 * 2.0 * atan2(sqrt(a), sqrt(1 - a));
//
//  // Calculate bearing from Home
//  float y = sin(dlong)*cos(lat2);
//  float x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dlong);
//  float bear = atan2(y,x) * 180.0 / 3.14159;
  
  // Print UAV Distance from Home 
  if (uav.sysid != 0) {
    GLCD.SelectFont(System5x7);
    GLCD.CursorToXY(GLCD.Right-40, 30);
    GLCD.print((int)constrain(tracker.get_dist(),-999,9999));
    GLCD.print('m');
  }

  // Draw line showing bearing to UAV
  if (uav.sysid != 0) {
	x0 = GLCD.Right-27;
	y0 = 16;

	GLCD.DrawLine( x0, y0, x1, y1, WHITE ) ;
	this->calcangle( &x1, &y1 ) ;
	GLCD.DrawLine( x0, y0, x1, y1, BLACK ) ;
  }
}

// Displaying roll angle of uav using demo from glcd clock example
void PageMain::calcangle( byte *x, byte *y )
/* angle is location of hand on dial (0-60)          */
/* radius is length of hand                           */
/* *x   return x-coordinate of hand on dial face */
/* *y   return y-coordinate of hand on dial face */
{
  byte angle = tracker.get_bearing()*9.5492965855137201461330258023509;
  byte radius = 10;
  short quadrant, x_flip, y_flip ;

  /* calculate which quadrant the hand lies in */
  quadrant = angle/15 ;

  /* setup for reflection or rotation */
  switch ( quadrant ) {
  case 0 :
    x_flip = 1 ;
    y_flip = -1 ;
    break ;
  case 1 :
    angle = abs(angle-30) ;
    x_flip = y_flip = 1 ;
    break ;
  case 2 :
    angle = angle-30 ;
    x_flip = -1 ;
    y_flip = 1 ;
    break ;
  case 3 :
    angle = abs(angle-60) ;
    x_flip = y_flip = -1 ;
    break ;
  default:
    x_flip = y_flip =1; // this should not happen
  }
  *x = x0 ;
  *y = y0 ;
  *x += ( x_flip*(( byteSine[angle]*radius ) >> 8 )) ;
  *y += ( y_flip*(( byteSine[15-angle]*radius ) >> 8 )) ;
}

uint8_t
PageMain::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_OK:
    gcs_passthrough = !gcs_passthrough; // quick hack to enable / disable gcs passthrough
    break;
  case B_RIGHT:
    Pages::move(1);
    break;
  case B_LEFT:
    Pages::move(-1);
    break;
  case B_CANCEL:
    Pages::move(0);
    break;
  }
}

uint8_t
PageHardware::_refresh_med()
{
  lcd.CursorTo(0, 0);
  lcd.print("Roll ");
  lcd.println(uav.roll);
  lcd.print("Encoder ");
  lcd.println(ASM.encoderval);
  lcd.CursorTo(0, 6);
  lcd.print("HB count ");
  lcd.println(hbcount);
}

uint8_t
PageHardware::_refresh_slow()
{
  lcd.CursorTo(0, 2);
  lcd.print("LiPo ");
  lcd.println(get_batt());
  lcd.print("Sat count ");
  lcd.println(gps.num_sats);
  lcd.print("3D fix ");
  lcd.println(gps.fix);
  lcd.print("GPS Time ");
  lcd.println(gps.time);
}

uint8_t
PageHardware::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_OK:
    break;
  case B_RIGHT:
    Pages::move(1);
    break;
  case B_LEFT:
    Pages::move(-1);
    break;
  case B_CANCEL:
    Pages::move(0);
    break;
  }
}

uint8_t
PageUAVtest::_refresh_med()
{
//  lcd.CursorTo(0, 0);
//  lcd.print("Roll  ");
//  lcd.println(uav.roll);
//  lcd.print("Pitch ");
//  lcd.println(uav.pitch);
//  lcd.print("Yaw   ");
//  lcd.println(uav.yaw);
}

uint8_t
PageUAVtest::_refresh_slow()
{
  // Find difference
  float lat1 = ((float)gps.latitude / T7 * 3.14159/180.0);
  float lat2 = uav.lat * 3.14159/180.0;
  float lon1 = ((float)gps.longitude / T7 * 3.14159/180.0);
  float lon2 = uav.lon * 3.14159/180.0;
  float dlat = lat2 - lat1;
  float dlong = lon2 - lon1;
  
  // Calculate distance
  float a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos((float)gps.latitude / T7 * 3.14159/180.0) * cos(uav.lat * 3.14159/180.0) * sin(dlong / 2.0) * sin(dlong / 2.0);
  float dist = 6371000.0 * 2.0 * atan2(sqrt(a), sqrt(1 - a));
  
  // Calculate bearing
  float y = sin(dlong)*cos(lat2);
  float x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dlong);
  float bear = atan2(y,x) * 180.0 / 3.14159;
                  
  lcd.CursorTo(0, 0);
  lcd.print("UAV Lat ");
  lcd.println(uav.lat, 5);
  lcd.print("UAV Lon ");
  lcd.println(uav.lon, 5);
  lcd.print("UAV alt ");
  lcd.println(uav.alt, 2);
  lcd.print("Dist ");
  lcd.println(tracker.get_dist());
  lcd.print("Bearing ");
  lcd.println(tracker.get_bearing());
  lcd.print("GCS Lat ");
  lcd.println((float)gps.latitude / T7, 5);
  lcd.print("GCS Lon ");
  lcd.println((float)gps.longitude / T7, 5);
  lcd.print("GCS Alt ");
  lcd.println((float)gps.altitude / 100.0, 2);
}


uint8_t
PageUAVtest::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_OK:
    break;
  case B_UP:
    break;
  case B_DOWN:
    break;
  case B_CANCEL:
    gcs3.data_stream_request();
    beep.play(BEEP_KEY);
    break;
  case B_RIGHT:
    Pages::move(1);
    break;
  case B_LEFT:
    Pages::move(-1);
    break;
  }
}

uint8_t
PageSDtest::_enter() {
  // Run through some tests

  lcd.CursorTo(0, 0);
  root = SD.open("/");
  printDirectory(root, 0);  

  return 0;
}

uint8_t
PageSDtest::_refresh_med()
{
}

uint8_t
PageSDtest::_refresh_slow()
{
}


uint8_t
PageSDtest::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_RIGHT:
    Pages::move(1);
    break;
  case B_LEFT:
    Pages::move(-1);
    break;
  case B_CANCEL:
    Pages::move(0);
    break;
  }
}

// Displaying roll angle of uav using demo from glcd clock example
void PageGLCDtest::calcangle( byte *x, byte *y )
/* angle is location of hand on dial (0-60)          */
/* radius is length of hand                           */
/* *x   return x-coordinate of hand on dial face */
/* *y   return y-coordinate of hand on dial face */
{
  byte angle = uav.roll*9.5492965855137201461330258023509+30;
  byte radius = GLCD.CenterY-4;
  short quadrant, x_flip, y_flip ;

  /* calculate which quadrant the hand lies in */
  quadrant = angle/15 ;

  /* setup for reflection or rotation */
  switch ( quadrant ) {
  case 0 : 
    x_flip = 1 ; 
    y_flip = -1 ; 
    break ;
  case 1 : 
    angle = abs(angle-30) ; 
    x_flip = y_flip = 1 ; 
    break ;
  case 2 : 
    angle = angle-30 ; 
    x_flip = -1 ; 
    y_flip = 1 ; 
    break ;
  case 3 : 
    angle = abs(angle-60) ; 
    x_flip = y_flip = -1 ; 
    break ; 
  default:  
    x_flip = y_flip =1; // this should not happen
  }
  *x = x0 ;
  *y = y0 ;
  *x += ( x_flip*(( byteSine[angle]*radius ) >> 8 )) ;
  *y += ( y_flip*(( byteSine[15-angle]*radius ) >> 8 )) ;
}

uint8_t
PageGLCDtest::_refresh_med()
{
  x0 = GLCD.CenterX;
  y0 = GLCD.CenterY;

  GLCD.DrawLine( x0, y0, x1, y1, WHITE ) ;
  this->calcangle( &x1, &y1 ) ;
  GLCD.DrawLine( x0, y0, x1, y1, BLACK ) ;
}

uint8_t
PageGLCDtest::_refresh_slow()
{
}


uint8_t
PageGLCDtest::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_RIGHT:
    Pages::move(1);
    break;
  case B_LEFT:
    Pages::move(-1);
    break;
  case B_CANCEL:
    Pages::move(0);
    break;
  }
}

