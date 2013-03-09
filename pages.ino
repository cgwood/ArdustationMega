// Page class for ASM
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// ------ Declare each page ------ //
PageHardware hardwarePage;
PageUAVtest  UAVtestPage;
PageGLCDtest  GLCDPage;
PageSDtest  SDPage;
PagePID  PIDPage;
PageCommands    CommandsPage;     ////< a page for sending commands to the APM

// Index of current page
uint8_t pageindex=0;

Pages*
Pages::_currPage(uint8_t pageid)
{
  // ----- Assign each page declared above to the page ordering enumerated in pages.h ----- //
  switch(pageid) {
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
  }
}

uint8_t
Pages::move(int8_t dir)
{
  if (dir == 0)
    pageindex = 0;
  else if (dir > 0 || pageindex > 0)
    pageindex += dir;
  else
    return(0);

  if (pageindex >= P_COUNT) {
    pageindex --;
    return(0);
  }

  // Beep about it
  //	beep.play(BEEP_KEY);

  // Draw all of the new page now
  lcd.ClearArea();
  _currPage(pageindex)->_enter();
  _currPage(pageindex)->_refresh_slow();
  _currPage(pageindex)->_refresh_med();
  return(0);
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
PageHardware::_refresh_med()
{
  lcd.CursorTo(0, 0);
  lcd.print("Roll ");
  lcd.println(uav.roll);
  lcd.print("Encoder ");
  lcd.println(ASM.encoderval);
  lcd.CursorTo(0, 5);
  lcd.print("HB count ");
  lcd.println(hbcount);
}

uint8_t
PageHardware::_refresh_slow()
{
  lcd.CursorTo(0, 2);
  lcd.print("LiPo ");
  lcd.println(get_batt());
  lcd.print("Lat ");
  lcd.println(ASM.lat);
  lcd.print("Lon ");
  lcd.println(ASM.lon);
}

uint8_t
PageHardware::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_OK:
    ASM.lat = 1;
    ASM.lon = 1;
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
PageUAVtest::_refresh_med()
{
  lcd.CursorTo(0, 0);
  lcd.print("Roll  ");
  lcd.println(uav.roll);
  lcd.print("Pitch ");
  lcd.println(uav.pitch);
  lcd.print("Yaw   ");
  lcd.println(uav.yaw);
}

uint8_t
PageUAVtest::_refresh_slow()
{
  lcd.CursorTo(0, 3);
  lcd.print("Lat ");
  lcd.println(uav.lat);
  lcd.print("Lon ");
  lcd.println(uav.lon);
}


uint8_t
PageUAVtest::_interact(uint8_t buttonid)
{
  switch(buttonid) {
  case B_OK:
    ASM.lat = uav.lat;
    ASM.lon = uav.lon;
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
  }
}

