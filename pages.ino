// Page class for ASM
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// ------ Declare each page ------ //
PageMain mainPage;
PageStatus statusPage;
PageMeasure measurementPage;
PagePlot plottingPage;
PageSettings settingsPage;
PageTracker trackerPage;
PageHardware hardwarePage;
PageUAVtest UAVtestPage;
PageGLCDtest GLCDPage;
PageSDtest SDPage;
PagePID PIDPage;
PageParams PlaneParametersPage(ParamTitlePlane, ParamNamesPlane, paramTable_plane, ParamScales, ParamDPs, PARAM_COUNT_PLANE, uav.param_plane, uav.param_plane_avail);
PageParams PlaneParametersPageCTUN(ParamTitlePlaneCTUN, ParamNamesPlaneCTUN, paramTable_plane_ctun, ParamScalesCTUN, ParamDPsCTUN, PARAM_COUNT_PLANE_CTUN, uav.param_plane_ctun, uav.param_plane_ctun_avail);
PageParams PlaneParametersPageNTUN(ParamTitlePlaneNTUN, ParamNamesPlaneNTUN, paramTable_plane_ntun, ParamScalesNTUN, ParamDPsNTUN, PARAM_COUNT_PLANE_NTUN, uav.param_plane_ntun, uav.param_plane_ntun_avail);
PageParams PlaneParametersPageTECS(ParamTitlePlaneTECS, ParamNamesPlaneTECS, paramTable_plane_tecs, ParamScalesTECS, ParamDPsTECS, PARAM_COUNT_PLANE_TECS, uav.param_plane_tecs, uav.param_plane_tecs_avail);
PageParams RoverParametersPage(ParamTitleRover, ParamNamesRover, paramTable_rover, ParamScalesRover, ParamDPsRover, PARAM_COUNT_ROVER, uav.param_rover, uav.param_rover_avail);
PageParams RoverSonarPage(ParamTitleRoverSonar, ParamNamesRoverSonar, paramTable_rover_sonar, ParamScalesRoverSonar, ParamDPsRoverSonar, PARAM_COUNT_ROVER_SONAR, uav.param_rover_sonar, uav.param_rover_sonar_avail);
PageParams CopterParametersPage(ParamTitleCopter, ParamNamesCopter, paramTable_copter, ParamScalesCopter, ParamDPsCopter, PARAM_COUNT_COPTER, uav.param_copter, uav.param_copter_avail);
PageCommands CommandsPage;     ////< a page for sending commands to the APM

// Index of current page
uint8_t pageindex = 0;

// Local pages - updated for different autopilot types
uint8_t _pageids[20];
uint8_t _pagecount;

Pages::Pages() {
	// Hack. Would prefer to just call definePages from here
	//	definePages();

	// NOTE, THE FINAL TWO PAGES MUST BE P_STATUS AND P_SETTINGS

	// Default option - AP Unknown
	_pageids[0] = P_MAIN;
	_pageids[1] = P_TRACKER;
	_pageids[2] = P_HARDWARE;
	_pageids[3] = P_UAVTEST;
	_pageids[4] = P_SD;
	_pageids[5] = P_STATUS;
	_pageids[6] = P_SETTINGS;
	_pagecount = 7;

}

uint8_t Pages::definePages() {
	if (uav.type == MAV_TYPE_FIXED_WING) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_MEASURE;
		_pageids[2] = P_PLOT;
		_pageids[3] = P_COMMANDS;
		_pageids[4] = P_PARAMETERS; // APM parameters
		_pageids[5] = P_PARAMETERS_CTUN; // APM Control tuning parameters
		_pageids[6] = P_PARAMETERS_TECS; // APM TECS tuning parameters
		_pageids[7] = P_PARAMETERS_NTUN; // APM Navigation tuning parameters
		_pageids[8] = P_TRACKER;
		_pageids[9] = P_UAVTEST;
		_pageids[10] = P_SD;
		_pageids[11] = P_STATUS;
		_pageids[12] = P_SETTINGS;
		_pagecount = 13;
	} else if (uav.type == MAV_TYPE_HELICOPTER
			|| uav.type == MAV_TYPE_TRICOPTER
			|| uav.type == MAV_TYPE_QUADROTOR
			|| uav.type == MAV_TYPE_HEXAROTOR
			|| uav.type == MAV_TYPE_OCTOROTOR) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_PID;               // ACM Rate PIDs
		_pageids[3] = P_COPTER_PARAMETERS; // Copter parameters
		_pageids[4] = P_TRACKER;
		_pageids[5] = P_HARDWARE;
		_pageids[6] = P_UAVTEST;
		_pageids[7] = P_GLCD;
		_pageids[8] = P_SD;
		_pageids[9] = P_STATUS;
		_pageids[10] = P_SETTINGS;
		_pagecount = 11;
	} else if (uav.type == MAV_TYPE_GROUND_ROVER) {
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_ROVER_PARAMETERS; // Rover parameters
		_pageids[3] = P_ROVER_PARAMETERS_SONAR; // Rover parameters
		_pageids[4] = P_TRACKER;
		_pageids[5] = P_HARDWARE;
		_pageids[6] = P_UAVTEST;
		_pageids[7] = P_GLCD;
		_pageids[8] = P_SD;
		_pageids[9] = P_STATUS;
		_pageids[10] = P_SETTINGS;
		_pagecount = 11;
	} else { // Default option - AP Unknown
		_pageids[0] = P_MAIN;
		_pageids[1] = P_COMMANDS;
		_pageids[2] = P_HARDWARE;
		_pageids[3] = P_UAVTEST;
		_pageids[4] = P_GLCD;
		_pageids[5] = P_SD;
		_pageids[6] = P_STATUS;
		_pageids[7] = P_SETTINGS;
		_pagecount = 8;
	}
	pageindex = 0;

	for (uint8_t p = 0; p < _pagecount; p++)
		_currPage(p)->_redefine();

	return 0;
}

Pages*
Pages::_currPage(uint8_t pageid) {
	// ----- Assign each page declared above to the page ordering enumerated in pages.h ----- //
	switch (_pageids[pageid]) {
	case P_MAIN:
		return (&mainPage);
		break;
	case P_STATUS:
		return (&statusPage);
		break;
	case P_SETTINGS:
		return (&settingsPage);
		break;
	case P_MEASURE:
		return (&measurementPage);
		break;
	case P_PLOT:
		return (&plottingPage);
		break;
	case P_TRACKER:
		return (&trackerPage);
		break;
	case P_HARDWARE:
		return (&hardwarePage);
		break;
	case P_UAVTEST:
		return (&UAVtestPage);
		break;
	case P_COMMANDS:
		return (&CommandsPage);
		break;
	case P_GLCD:
		return (&GLCDPage);
		break;
	case P_SD:
		return (&SDPage);
		break;
	case P_PID:
		return (&PIDPage);
		break;
	case P_PARAMETERS:
		return (&PlaneParametersPage);
		break;
	case P_PARAMETERS_CTUN:
		return (&PlaneParametersPageCTUN);
		break;
	case P_PARAMETERS_NTUN:
		return (&PlaneParametersPageNTUN);
		break;
	case P_PARAMETERS_TECS:
		return (&PlaneParametersPageTECS);
		break;
	case P_ROVER_PARAMETERS:
		return (&RoverParametersPage);
		break;
	case P_ROVER_PARAMETERS_SONAR:
		return (&RoverSonarPage);
		break;
	case P_COPTER_PARAMETERS:
		return (&CopterParametersPage);
		break;
	}
}

uint8_t Pages::move(int8_t dir) {
	if (dir == 0) {
		pageindex = 0;
	} else if (dir > 0 || pageindex > 0) {
		pageindex += dir;
	} else
		return (0);

	if (pageindex >= _pagecount-2) { //P_COUNT) {
		pageindex--;
		return (0);
	}

	// Draw all of the new page now
	lcd.ClearArea();
	GLCD.ClearScreen();
	_currPage(pageindex)->_enter();
	_currPage(pageindex)->_refresh_slow();
	_currPage(pageindex)->_refresh_med();
	return (0);
}

uint8_t Pages::moveTo(int8_t pageid) {
	uint8_t i;

	// Find the page index from pageid
	for (i=0;i<_pagecount;i++) {
		if (_pageids[i] == pageid) {
			pageindex = i;
			break;
		}
	}

	if (i==_pagecount) {
//		Serial.println("Invalid page id");
		return 0;
	}

	// Draw all of the new page now
	lcd.ClearArea();
	GLCD.ClearScreen();
	_currPage(pageindex)->_enter();
	_currPage(pageindex)->_refresh_slow();
	_currPage(pageindex)->_refresh_med();
	return (0);
}

uint8_t Pages::enter() {
	lcd.ClearArea();
	GLCD.ClearScreen();
	_currPage(pageindex)->_enter();
	_currPage(pageindex)->_refresh_slow();
	_currPage(pageindex)->_refresh_med();

	return 0;
}

void Pages::forceUpdate(uint8_t reason) {
	_currPage(pageindex)->_forceUpdate(reason);
}

void Pages::interact(uint8_t buttonid) {
	_currPage(pageindex)->_interact(buttonid);
}

uint8_t Pages::refresh_med() {
	_currPage(pageindex)->_refresh_med();
	return (0);
}

uint8_t Pages::refresh_slow() {
	_currPage(pageindex)->_refresh_slow();
	return (0);
}

uint8_t PageMain::_enter() {
	// Reset last base mode to force the screen update
	_last_base_mode = 0;

//  GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX -3, GLCD.Bottom, 5);  // rounded rectangle around text area 
//  _textArea.DefineArea(GLCD.CenterX + 5, 3, GLCD.Right-2, GLCD.Bottom-4, SCROLL_UP); 
//  _textArea.SelectFont(System5x7, BLACK);
//  _textArea.CursorTo(0,0);
//  _textArea.print('A');

	// Name by autopilot type and system id (later could assign these to names, e.g. Twinstar)
	GLCD.CursorToXY(3, 3);
	GLCD.SelectFont(Arial_bold_14);
	if (uav.sysid == 0) {
		GLCD.print("No Connection");
	} else {
		switch (uav.autopilot) {
		case MAV_AUTOPILOT_GENERIC:
			GLCD.Printf("MAV-%03i", uav.sysid);
			break;
		case MAV_AUTOPILOT_PIXHAWK:
			GLCD.Printf("PIX-%03i", uav.sysid);
			break;
		case MAV_AUTOPILOT_ARDUPILOTMEGA:
			switch (uav.type) {
			case MAV_TYPE_HELICOPTER:
				GLCD.Printf("Heli-%03i", uav.sysid);
				break;
			case MAV_TYPE_TRICOPTER:
				GLCD.Printf("Tri-%03i", uav.sysid);
				break;
			case MAV_TYPE_QUADROTOR:
				GLCD.Printf("Quad-%03i", uav.sysid);
				break;
			case MAV_TYPE_HEXAROTOR:
				GLCD.Printf("Hex-%03i", uav.sysid);
				break;
			case MAV_TYPE_OCTOROTOR:
				GLCD.Printf("Oct-%03i", uav.sysid);
				break;
			case MAV_TYPE_FIXED_WING:
				GLCD.Printf("Plane-%03i", uav.sysid);
				break;
			case MAV_TYPE_GROUND_ROVER:
				GLCD.Printf("Rover-%03i", uav.sysid);
				break;
			default:
				GLCD.Printf("APM-%03i", uav.sysid);
				break;
			}
//			if (uav.type == MAV_TYPE_QUADROTOR)
//				GLCD.Printf("Quad-%03i", uav.sysid);
//			else if (uav.type == MAV_TYPE_FIXED_WING)
//				GLCD.Printf("APM-%03i", uav.sysid);
//			else if (uav.type == MAV_TYPE_GROUND_ROVER)
//				GLCD.Printf("Rover-%03i", uav.sysid);
//			else
//				GLCD.Printf("AxM-%03i", uav.sysid);
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
	GLCD.DrawRect(GLCD.Right - 8, 2, 6, GLCD.Bottom - 12);

	// Bearing to UAV, circle
	if (uav.sysid != 0) {
		x0 = GLCD.Right - 27;
		y0 = 16;
		x1 = x0;
		y1 = y0;
		GLCD.DrawCircle(GLCD.Right - 27, 16, 12);
		//GLCD.DrawLine(GLCD.Right-27, 16, GLCD.Right-27+5, 16-5);
	}

	// Local Battery bar on left side of screen
	GLCD.DrawRect(2, 20, 6, GLCD.Bottom - 30);

	// Local RSSI bar on left side of screen
	GLCD.DrawRect(10, 20, 6, GLCD.Bottom - 30);

	// Altitude bitmap
	if (uav.type != MAV_TYPE_GROUND_ROVER)
		GLCD.DrawBitmap(_alt_icon, 27, GLCD.Bottom - 26);

	// Speed bitmap
	GLCD.DrawBitmap(_speed_icon, 63, GLCD.Bottom - 26);

	// Sat bitmap
	GLCD.DrawBitmap(_sat_icon, GLCD.Right - 26, GLCD.Bottom - 25);

	// Batt bitmaps
	GLCD.DrawBitmap(_batt_icon, 1, GLCD.Bottom - 8);
	GLCD.DrawBitmap(_batt_icon, GLCD.Right - 9, GLCD.Bottom - 8);

	// Conn bitmap
	GLCD.DrawBitmap(_conn_icon, 10, GLCD.Bottom - 8);
}

uint8_t PageMain::_refresh_med() {

	GLCD.SelectFont(System5x7);
//  GLCD.FillRect(GLCD.Right-7, 3, 4, GLCD.Bottom-6, WHITE);
//	if (uav.sat_count > 9)
//		GLCD.CursorToXY(GLCD.Right - 20, GLCD.Bottom - 8);
//	else
//		GLCD.CursorToXY(GLCD.Right - 18, GLCD.Bottom - 8);

	GLCD.CursorToXY(GLCD.Right - 24, GLCD.Bottom - 8);
	GLCD.Printf("%2d",uav.sat_count);

	// UAV Mode
	if (uav.sysid != 0 && _last_base_mode != uav.base_mode) {
		_last_base_mode = uav.base_mode; // Only update when mode changes

//		GLCD.CursorToXY(20, 20);
//		GLCD.print("           ");
		GLCD.CursorToXY(20, 20);
		GLCD.SelectFont(System5x7);
//		if (uav.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) {
//			GLCD.print("Armed");
//		} else {
//			GLCD.print("Safe");
//		}
//
//		if (uav.base_mode & MAV_MODE_FLAG_STABILIZE_ENABLED) {
//			GLCD.print(", Stab");
//		} else {
//			GLCD.print(", Acro");
//		}
		switch (uav.custom_mode) {
		case MANUAL:
			GLCD.Printf_P(PSTR("Manual      "));
			break;
		case CIRCLE:
			GLCD.Printf_P(PSTR("Circle      "));
			break;
		case STABILIZE:
			GLCD.Printf_P(PSTR("Stabilise  "));
			break;
		case TRAINING:
			GLCD.Printf_P(PSTR("Training   "));
			break;
		case ACRO:
			GLCD.Printf_P(PSTR("Acro       "));
			break;
		case FLY_BY_WIRE_A:
			GLCD.Printf_P(PSTR("FBW-A      "));
			break;
		case FLY_BY_WIRE_B:
			GLCD.Printf_P(PSTR("FBW-B      "));
			break;
		case CRUISE:
			GLCD.Printf_P(PSTR("Cruise     "));
			break;
		case AUTO:
			GLCD.Printf_P(PSTR("Auto       "));
			break;
		case RTL:
			GLCD.Printf_P(PSTR("RTL        "));
			break;
		case LOITER:
			GLCD.Printf_P(PSTR("Loiter     "));
			break;
		case GUIDED:
			GLCD.Printf_P(PSTR("Guided     "));
			break;
		case INITIALISING:
			GLCD.Printf_P(PSTR("Startup    "));
			break;
		default:
			GLCD.Printf_P(PSTR("\n"));
			break;
		}
	}
}

uint8_t PageMain::_refresh_slow() {
	uint8_t batt_height;
	uint8_t rssi_height;
	float batt_ratio, rssi_ratio;

	int rssi;

	// Local battery level
	batt_height = GLCD.Bottom - 32;
	GLCD.FillRect(3, 21, 4, batt_height, WHITE); // Clear area
//	batt_ratio = constrain((get_batt() - 3.2), 0.0, 1.0); // Allows range between 3.2 and 4.2
	batt_ratio = constrain((get_batt() - ASM.batt_min), 0.0, (ASM.batt_max-ASM.batt_min))/(ASM.batt_max-ASM.batt_min);
	batt_height = batt_height * batt_ratio;
	GLCD.FillRect(3, 21 + GLCD.Bottom - 32 - batt_height, 4, batt_height,
			BLACK); // Fill area

	// RSSI level
	rssi = get_rssi();
	rssi_height = GLCD.Bottom - 32;
	GLCD.FillRect(11, 21, 4, rssi_height, WHITE); // Clear area
	rssi_ratio = rssi / 1023.0;
	rssi_height = rssi_height * rssi_ratio;
	GLCD.FillRect(11, 21 + GLCD.Bottom - 32 - rssi_height, 4, rssi_height,
			BLACK); // Fill area

	// Remote battery level
	batt_height = GLCD.Bottom - 14;
	GLCD.FillRect(GLCD.Right - 7, 3, 4, batt_height, WHITE);
	batt_height = batt_height * uav.load / 1000.0;
	GLCD.FillRect(GLCD.Right - 7, 3 + GLCD.Bottom - 14 - batt_height, 4,
			batt_height, BLACK);

	// Clear bottom line
	GLCD.FillRect(23, GLCD.Bottom - 8, GLCD.Right - 43, 7, WHITE);

	// Print Altitude
	if (uav.type != MAV_TYPE_GROUND_ROVER) {
		GLCD.SelectFont(System5x7);
		GLCD.CursorToXY(23, GLCD.Bottom - 8);
	//  GLCD.print(constrain(uav.alt,-99.9,999.9),1);
		if (uav.alt > 100)
			GLCD.print(constrain((int) (uav.alt), 0, 9999));
		else if (uav.alt > 0)
			GLCD.print(constrain(uav.alt, 0, 99.9), 1);
		else
			GLCD.print(constrain(uav.alt, -99, 0), 1);
		GLCD.print('m');
	}
	else {
		uint8_t seconds = (ASM.time/1000) % 60;
		uint8_t minutes = (ASM.time/1000)/60 % 60;
		uint8_t hours = (ASM.time/1000)/60/60 % 24;

		GLCD.CursorToXY(20, GLCD.Bottom - 16);
		GLCD.Printf("%02d:%02d", hours, minutes);
	}

	// Print Velocity
	GLCD.SelectFont(System5x7);
	GLCD.CursorToXY(59, GLCD.Bottom - 8);
	if (uav.type == MAV_TYPE_FIXED_WING) {
		if (uav.airspeed > 100)
			GLCD.print(constrain((int) (uav.airspeed), 0, 999));
		else
			GLCD.print(constrain(uav.airspeed, -9.9, 99.9), 1);
	}
	else {
		if (uav.vel > 10000)
			GLCD.print(constrain((int) (uav.vel / 100.0), 0, 999));
		else
			GLCD.print(constrain(uav.vel / 100.0, -9.9, 99.9), 1);
	}
	GLCD.print("m/s");

	// Print UAV Distance from Home
	if (uav.sysid != 0) {
		GLCD.SelectFont(System5x7);
		GLCD.CursorToXY(GLCD.Right - 40, 30);
		GLCD.Printf("%4dm",(int) constrain(tracker.get_dist(), -999, 9999));
//		GLCD.print((int) constrain(tracker.get_dist(), -999, 9999));
//		GLCD.print('m');
	}

	// Draw line showing bearing to UAV
	if (uav.sysid != 0) {
		GLCD.DrawLine(x0, y0, x1, y1, WHITE);
		this->calcangle(&x1, &y1);
		GLCD.DrawLine(x0, y0, x1, y1, BLACK);
	}
}

// Displaying roll angle of uav using demo from glcd clock example
void PageMain::calcangle(byte *x, byte *y)
/* angle is location of hand on dial (0-60)          */
/* radius is length of hand                           */
/* *x   return x-coordinate of hand on dial face */
/* *y   return y-coordinate of hand on dial face */
{
	byte angle = tracker.get_bearing() * 9.5492965855137201461330258023509;
	byte radius = 10;
	short quadrant, x_flip, y_flip;

	/* calculate which quadrant the hand lies in */
	quadrant = angle / 15;

	/* setup for reflection or rotation */
	switch (quadrant) {
	case 0:
		x_flip = 1;
		y_flip = -1;
		break;
	case 1:
		angle = abs(angle - 30);
		x_flip = y_flip = 1;
		break;
	case 2:
		angle = angle - 30;
		x_flip = -1;
		y_flip = 1;
		break;
	case 3:
		angle = abs(angle - 60);
		x_flip = y_flip = -1;
		break;
	default:
		x_flip = y_flip = 1; // this should not happen
	}
	*x = x0;
	*y = y0;
	*x += (x_flip * ((byteSine[angle] * radius) >> 8));
	*y += (y_flip * ((byteSine[15 - angle] * radius) >> 8));
}

uint8_t PageMain::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_OK:
//		gcs_passthrough = !gcs_passthrough; // quick hack to enable / disable gcs passthrough
		break;
	case B_OK_HOLD:
		Pages::moveTo(P_SETTINGS);
		break;
	case B_RIGHT:
		Pages::move(1);
		break;
	case B_LEFT:
		Pages::moveTo(P_STATUS);
		break;
	case B_CANCEL:
		Pages::move(0);
		break;
	}
}

uint8_t PageStatus::_enter() {
	uint8_t seconds = (ASM.time/1000) % 60;
	uint8_t minutes = (ASM.time/1000)/60 % 60;
	uint8_t hours = (ASM.time/1000)/60/60 % 24;

	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	GLCD.Printf("ASM Status  %02d:%02d", hours, minutes);
	GLCD.SelectFont(System5x7);
	GLCD.CursorTo(0, 2);
	GLCD.Printf("Parameters: %d/%d", download_index, uav.onboard_param_count);
	GLCD.CursorTo(0, 3);
	GLCD.print("GPS Status: ");
	switch (ASM.gps_status) {
	default:
	case 0:
		GLCD.println("No GPS");
		break;
	case 1:
		GLCD.println("No Fix");
		break;
	case 2:
		GLCD.println("Fix");
		break;
	}
	GLCD.print("Sat count: ");
	GLCD.println(ASM.num_sats);
	GLCD.Printf("HB Period: %dms\n", ASM.hb_period);
	if (ASM.SDok)
		GLCD.println("SD Card found");
	else
		GLCD.println("SD Card not found");

	return 0;
}

uint8_t PageStatus::_refresh_med() {
	uint8_t seconds = (ASM.time/1000) % 60;
	uint8_t minutes = (ASM.time/1000)/60 % 60;
	uint8_t hours = (ASM.time/1000)/60/60 % 24;

	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	GLCD.Printf("ASM Status  %02d:%02d\n", hours, minutes);
	GLCD.SelectFont(System5x7);

//	GLCD.CursorTo(0, 2);
//	GLCD.Printf("Time: %02d:%02d", hours, minutes);
	GLCD.CursorTo(0, 2);
	GLCD.Printf("Parameters: %d/%d", download_index, uav.onboard_param_count);
	GLCD.CursorTo(0, 3);
	GLCD.print("GPS Status: ");
	switch (ASM.gps_status) {
	default:
	case 0:
		GLCD.println("No GPS");
		break;
	case 1:
		GLCD.println("No Fix");
		break;
	case 2:
		GLCD.println("Fix");
		break;
	}
	GLCD.print("Sat count: ");
	GLCD.println(ASM.num_sats);
	GLCD.Printf("HB Period: %dms\n", ASM.hb_period);
	if (ASM.SDok)
		GLCD.println("SD Card found");
	else
		GLCD.println("SD Card not found");

	return 0;
}

uint8_t PageStatus::_refresh_slow() {
//	GLCD.CursorTo(0, 0);
//	GLCD.Printf("Parameters: %d/%d", download_index, uav.onboard_param_count);
	return 0;
}

uint8_t PageStatus::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_OK:
		break;
	case B_RIGHT:
		Pages::move(0);
		break;
//	case B_LEFT:
//		Pages::move(-1);
//		break;
	case B_CANCEL:
		Pages::move(0);
		break;
	}
	return 0;
}

uint8_t PageStatus::_forceUpdate(uint8_t reason) {
//	if (reason == R_PARAM) {
//		GLCD.CursorTo(0, 12);
//		GLCD.Printf("%d/%d", download_index, uav.onboard_param_count); //Parameters:
//	}
	return 0;
}

uint8_t PageMeasure::_enter() {
	// This function gets called when the user switches to this page
	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	GLCD.print("Measurements");
	GLCD.SelectFont(System5x7, BLACK);

	_textArea1.DefineArea(GLCD.CenterX + 8, 20, GLCD.Right-2, 39, SCROLL_UP);
	_textArea1.SelectFont(fixednums8x16, BLACK);
	_textArea2.DefineArea(GLCD.CenterX + 8, 44, GLCD.Right-2, 63, SCROLL_UP);
	_textArea2.SelectFont(fixednums8x16, BLACK);
	return 0;
}

uint8_t PageMeasure::_refresh_med() {
	// This function gets called ten times a second

	if (_state == 1)
		GLCD.SelectFont(System5x7, WHITE);
	GLCD.CursorTo(0, 3);
	_printName(_measurementids[0]);
	GLCD.SelectFont(System5x7, BLACK);

	if (_state == 2)
		GLCD.SelectFont(System5x7, WHITE);
	GLCD.CursorTo(0, 6);
	_printName(_measurementids[1]);
	GLCD.SelectFont(System5x7, BLACK);

//	GLCD.CursorTo(12, 3);
//	GLCD.SelectFont(fixednums8x16);
	_printValue(_measurementids[0], &_textArea1);
//	GLCD.SelectFont(System5x7, BLACK);
//	GLCD.CursorTo(12, 6);
//	GLCD.SelectFont(fixednums8x16);
	_printValue(_measurementids[1], &_textArea2);
//	GLCD.SelectFont(System5x7, BLACK);


	return 0;
}

void PageMeasure::_printName(uint8_t measurementid)
{
	switch (measurementid) {
	default:
	case M_NONE:
		GLCD.Printf_P(PSTR("Nothing    "));
		break;
	case M_ROLL:
		GLCD.Printf_P(PSTR("      Roll:"));
		break;
	case M_PITCH:
		GLCD.Printf_P(PSTR("     Pitch:"));
		break;
	case M_AIRSPEED:
		GLCD.Printf_P(PSTR("  Airspeed:"));
		break;
	case M_GROUNDSPEED:
		GLCD.Printf_P(PSTR("Ground spd:"));
		break;
	case M_THROTTLE:
		GLCD.Printf_P(PSTR("  Throttle:"));
		break;
	case M_CLIMBRATE:
		GLCD.Printf_P(PSTR("Climb Rate:"));
		break;
	}
}

void PageMeasure::_printValue(uint8_t measurementid, gText *area)
{
	float val;
	uint16_t val1;
	uint8_t val2;

	switch (measurementid) {
	case M_ROLL:
		val = uav.roll*57.2957795;
		break;
	case M_PITCH:
		val = uav.pitch*57.2957795;
		break;
	case M_AIRSPEED:
		val = uav.airspeed;
		break;
	case M_GROUNDSPEED:
		val = uav.groundspeed;
		break;
	case M_THROTTLE:
		val = uav.throttle;
		break;
	case M_CLIMBRATE:
		val = uav.climb;
		break;
	default:
		val = 0;
		break;
	}

	val1 = floor(val);
	val2 = floor((val-val1)*10 + 0.5);

	if (measurementid == M_NONE) {
		area->ClearArea();
//		area->print("None  ");
	}
	else {
		area->ClearArea();
		area->print(constrain(val,-99.9,999.9), 1);
	}
}

uint8_t PageMeasure::_refresh_slow() {
	// This function gets called every two seconds
	return 0;
}

// Note B_RIGHT and B_LEFT code always required for moving between pages:
uint8_t PageMeasure::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_OK:
		if (_state == 0) {
			_state++;
			rotary.configure(&_measurementids[0], M_COUNT-1, 0, -4);
		}
		break;
	case B_UP:
		_state = constrain(_state-1,0,2);
		if (_state)
			rotary.configure(&_measurementids[_state-1], M_COUNT-1, 0, -4);
		break;
	case B_DOWN:
		_state = constrain(_state+1,0,2);
		if (_state)
			rotary.configure(&_measurementids[_state-1], M_COUNT-1, 0, -4);
		break;
	case B_RIGHT:
		Pages::move(1);
		break;
	case B_LEFT:
		Pages::move(-1);
		break;
	case B_CANCEL:
		if (_state > 0)
			_state = 0;
		else
			Pages::move(0);
		break;
	}

	if (_state == 0)
		rotary.configure(NULL, M_COUNT-1, 0, -4);

	return 0;
}

uint8_t PageMeasure::_forceUpdate(uint8_t reason) {
	return 0;
}

uint8_t PagePlot::_enter() {
	// This function gets called when the user switches to this page
	GLCD.CursorTo(0, 0);
	GLCD.SelectFont(Arial_bold_14);
	GLCD.print("Plotting");
	GLCD.SelectFont(System5x7, BLACK);

	// Plot the axis markers on RHS (Essentially marks for 0,25,50,75,100 percent)
	GLCD.DrawLine(GLCD.Right, GLCD.Bottom, GLCD.Right, GLCD.Bottom-32);
	GLCD.DrawLine(PLOTW+1, GLCD.Bottom, GLCD.Right, GLCD.Bottom); // 0
	GLCD.DrawLine(PLOTW+3, GLCD.Bottom-8, GLCD.Right, GLCD.Bottom-8); // 25
	GLCD.DrawLine(PLOTW+3, GLCD.Bottom-16, GLCD.Right, GLCD.Bottom-16); // 50
	GLCD.DrawLine(PLOTW+3, GLCD.Bottom-24, GLCD.Right, GLCD.Bottom-24); // 75
	GLCD.DrawLine(PLOTW+1, GLCD.Bottom-32, GLCD.Right, GLCD.Bottom-32); // 100
	return 0;
}

uint8_t PagePlot::_refresh_med() {
	// This function gets called ten times a second
	uint8_t value;
	float value_float;

	if (_state == 1)
		GLCD.SelectFont(System5x7, WHITE);
	GLCD.CursorTo(0, 2);
	_printName(_measurementid);
	GLCD.SelectFont(System5x7, BLACK);

	if (_measurementid != M_NONE) {
		value_float = _getValue(_measurementid);
		GLCD.println(value_float);
		value = _getValueInt(_measurementid);
		_clearPlot();
		_addValue(value);
		_plot();
	}


	return 0;
}

uint8_t PagePlot::_getValueInt(uint8_t measurementid)
{
	uint8_t val;

	switch (measurementid) {
	case M_ROLL:
		val = constrain(uav.roll*10,-16,16)+16; // Only give +- 90 degs
		break;
	case M_PITCH:
		val = constrain(uav.pitch*10,-16,16)+16; // Only give +- 90 degs
		break;
	case M_AIRSPEED:
		val = constrain(uav.airspeed,0,32);
		break;
	case M_GROUNDSPEED:
		val = constrain(uav.groundspeed,0,32);
		break;
	case M_THROTTLE:
		val = map(uav.throttle,0,100,0,32);
		break;
	case M_CLIMBRATE:
		val = constrain(uav.climb+16,-16,16);
		break;
	default:
		val = 0;
		break;
	}

	return val;
}

float PagePlot::_getValue(uint8_t measurementid)
{
	float val;

	switch (measurementid) {
	case M_ROLL:
		val = uav.roll*57.2957795;
		break;
	case M_PITCH:
		val = uav.pitch*57.2957795;
		break;
	case M_AIRSPEED:
		val = uav.airspeed;
		break;
	case M_GROUNDSPEED:
		val = uav.groundspeed;
		break;
	case M_THROTTLE:
		val = uav.throttle;
		break;
	case M_CLIMBRATE:
		val = uav.climb;
		break;
	default:
		val = 0;
		break;
	}

	return val;
}

void PagePlot::_addValue(uint8_t value)
{
	uint8_t i;
	for (i=0;i<(PLOTW-1);i++) {
		_values[i] = _values[i+1];
	}
	_values[PLOTW-1] = value;
}

void PagePlot::_printName(uint8_t measurementid)
{
	switch (measurementid) {
	default:
	case M_NONE:
		GLCD.Printf_P(PSTR("Nothing    "));
		break;
	case M_ROLL:
		GLCD.Printf_P(PSTR("      Roll:"));
		break;
	case M_PITCH:
		GLCD.Printf_P(PSTR("     Pitch:"));
		break;
	case M_AIRSPEED:
		GLCD.Printf_P(PSTR("  Airspeed:"));
		break;
	case M_GROUNDSPEED:
		GLCD.Printf_P(PSTR("Ground spd:"));
		break;
	case M_THROTTLE:
		GLCD.Printf_P(PSTR("  Throttle:"));
		break;
	case M_CLIMBRATE:
		GLCD.Printf_P(PSTR("Climb Rate:"));
		break;
	}
}

void PagePlot::_plot()
{
	uint8_t i;
	for (i=0;i<PLOTW;i++) {
		GLCD.SetDot(i,GLCD.Bottom-_values[i], BLACK);  // draws a BLACK pixel at x,y
	}
}

void PagePlot::_clearPlot()
{
	uint8_t i;
	for (i=0;i<PLOTW;i++) {
		GLCD.SetDot(i,GLCD.Bottom-_values[i], WHITE);  // erases the pixel at x,y
	}
}

uint8_t PagePlot::_refresh_slow() {
	// This function gets called every two seconds
//	uint8_t i,vmin,vmax;
//	vmin = 255;
//	vmax = 0;
//	for (i=0;i<128;i++) {
//		vmin = min(vmin,_values[i]);
//		vmax = max(vmax,_values[i]);
//	}
//	Serial.print(vmin);
//	Serial.print(", ");
//	Serial.println(vmax);
	return 0;
}

// Note B_RIGHT and B_LEFT code always required for moving between pages:
uint8_t PagePlot::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_OK:
		_state = 1;
		rotary.configure(&_measurementid, M_COUNT-1, 0, -4);
		break;
	case B_UP:
		break;
	case B_DOWN:
		break;
	case B_RIGHT:
		Pages::move(1);
		break;
	case B_LEFT:
		Pages::move(-1);
		break;
	case B_CANCEL:
		if (_state > 0)
			_state = 0;
		else
			Pages::move(0);
		break;
	}

	if (_state == 0)
		rotary.configure(NULL, M_COUNT-1, 0, -4);

	return 0;
}

uint8_t PagePlot::_forceUpdate(uint8_t reason) {
	return 0;
}

uint8_t PageTracker::_enter() {
	// This function gets called when the user switches to this page
	GLCD.CursorTo(0, 0);
	GLCD.println("Tracker Dev. Page");
	GLCD.CursorTo(0, 7);
	GLCD.print("Encoder: ");
	return 0;
}

uint8_t PageTracker::_refresh_med() {
	// This function gets called ten times a second
	GLCD.CursorTo(9, 7);
	switch (_state) {
	default:
	case 0:
		GLCD.print("Nothing\n");
		tracker.set_bearing(-1);
		tracker.set_elevation(-1);
		break;
	case 1:
		GLCD.print("Bearing\n");
		_value_encoder = (int)tracker.get_bearing();
		rotary.configure(&_value_encoder, 360, 0, -4);
		tracker.set_bearing(_value_encoder);
		tracker.set_elevation(-1);
		break;
	case 2:
		GLCD.print("Elevation\n");
		_value_encoder = (int)tracker.get_elevation();
		rotary.configure(&_value_encoder, 90, 0, -4);
		tracker.set_bearing(-1);
		tracker.set_elevation(_value_encoder);
		break;
	}

	float val;
	uint16_t val1,val2;
	GLCD.CursorTo(0, 2);
	val = tracker.get_bearing();
	val1 = floor(val); val2 = floor((val-val1)*10+0.5);
	GLCD.Printf("Bearing:   %3d.%ddegs\n",val1,val2);
	val = tracker.get_elevation();
	val1 = floor(val); val2 = floor((val-val1)*10+0.5);
	GLCD.Printf("Elevation: %3d.%ddegs\n",val1,val2);
	val = tracker.get_servo_pan()/1.80;
	val1 = floor(val); val2 = floor((val-val1)*10+0.5);
	GLCD.Printf("Pan:       %3d.%d%%\n",val1,val2);
	val = tracker.get_servo_tilt()/1.80;
	val1 = floor(val); val2 = floor((val-val1)*10+0.5);
	GLCD.Printf("Tilt:      %3d.%d%%\n",val1,val2);

	return 0;
}

uint8_t PageTracker::_refresh_slow() {
	// This function gets called every two seconds

	return 0;
}

// Note B_RIGHT and B_LEFT code always required for moving between pages:
uint8_t PageTracker::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_UP:
		_state = constrain(_state-1,0,2);
		break;
	case B_DOWN:
		_state = constrain(_state+1,0,2);
		break;
	case B_ENCODER:
		if (_state == 1)
			tracker.set_bearing(_value_encoder);
		else if (_state == 2)
			tracker.set_elevation(_value_encoder);
		break;
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
	return 0;
}

uint8_t PageTracker::_forceUpdate(uint8_t reason) {
	return 0;
}

uint8_t PageHardware::_refresh_med() {
	lcd.CursorTo(0, 0);
	lcd.print("Roll: ");
	lcd.println(uav.roll);
	lcd.print("Encoder: ");
	lcd.println(ASM.encoderval);
	lcd.CursorTo(0, 6);
	lcd.print("HB count: ");
	lcd.println(hbcount);
}

uint8_t PageHardware::_refresh_slow() {
	lcd.CursorTo(0, 2);
	lcd.print("LiPo: ");
	lcd.println(get_batt());
	lcd.print("Sat count: ");
	lcd.println(ASM.num_sats);
	lcd.print("GPS Status: ");
	switch (ASM.gps_status) {
	default:
	case 0:
		lcd.println("No GPS");
		break;
	case 1:
		lcd.println("No Fix");
		break;
	case 2:
		lcd.println("Fix");
		break;
	}

	uint8_t seconds = (ASM.time/1000) % 60;
	uint8_t minutes = (ASM.time/1000)/60 % 60;
	uint8_t hours = (ASM.time/1000)/60/60 % 24;

	lcd.print("GPS Time: ");
	lcd.Printf("%02d:%02d", hours, minutes);
}

uint8_t PageHardware::_interact(uint8_t buttonid) {
	switch (buttonid) {
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

uint8_t PageUAVtest::_refresh_med() {
//  lcd.CursorTo(0, 0);
//  lcd.print("Roll  ");
//  lcd.println(uav.roll);
//  lcd.print("Pitch ");
//  lcd.println(uav.pitch);
//  lcd.print("Yaw   ");
//  lcd.println(uav.yaw);
}

uint8_t PageUAVtest::_refresh_slow() {
	// Find difference
	float lat1 = (ASM.lat * 3.14159 / 180.0);
	float lat2 = uav.lat * 3.14159 / 180.0;
	float lon1 = (ASM.lon * 3.14159 / 180.0);
	float lon2 = uav.lon * 3.14159 / 180.0;
	float dlat = lat2 - lat1;
	float dlong = lon2 - lon1;

	// Calculate distance
	float a = sin(dlat / 2.0) * sin(dlat / 2.0)
			+ cos(ASM.lat * 3.14159 / 180.0) * cos(uav.lat * 3.14159 / 180.0)
					* sin(dlong / 2.0) * sin(dlong / 2.0);
	float dist = 6371000.0 * 2.0 * atan2(sqrt(a), sqrt(1 - a));

	// Calculate bearing
	float y = sin(dlong) * cos(lat2);
	float x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dlong);
	float bear = atan2(y, x) * 180.0 / 3.14159;

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
	lcd.println(ASM.lat, 5);
	lcd.print("GCS Lon ");
	lcd.println(ASM.lon, 5);
	lcd.print("GCS Alt ");
	lcd.println((float) ASM.altitude / 100.0, 2);
}

uint8_t PageUAVtest::_interact(uint8_t buttonid) {
	switch (buttonid) {
	case B_OK:
		break;
	case B_UP:
		break;
	case B_DOWN:
		break;
	case B_CANCEL:
		Pages::move(0);
		break;
	case B_RIGHT:
		Pages::move(1);
		break;
	case B_LEFT:
		Pages::move(-1);
		break;
	}
}

uint8_t PageSDtest::_enter() {
	// Run through some tests

	lcd.CursorTo(0, 0);
	root = SD.open("/");
	printDirectory(root, 0);

	return 0;
}

uint8_t PageSDtest::_refresh_med() {
}

uint8_t PageSDtest::_refresh_slow() {
}

uint8_t PageSDtest::_interact(uint8_t buttonid) {
	switch (buttonid) {
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
void PageGLCDtest::calcangle(byte *x, byte *y)
/* angle is location of hand on dial (0-60)          */
/* radius is length of hand                           */
/* *x   return x-coordinate of hand on dial face */
/* *y   return y-coordinate of hand on dial face */
{
	byte angle = uav.roll * 9.5492965855137201461330258023509 + 30;
	byte radius = GLCD.CenterY - 4;
	short quadrant, x_flip, y_flip;

	/* calculate which quadrant the hand lies in */
	quadrant = angle / 15;

	/* setup for reflection or rotation */
	switch (quadrant) {
	case 0:
		x_flip = 1;
		y_flip = -1;
		break;
	case 1:
		angle = abs(angle - 30);
		x_flip = y_flip = 1;
		break;
	case 2:
		angle = angle - 30;
		x_flip = -1;
		y_flip = 1;
		break;
	case 3:
		angle = abs(angle - 60);
		x_flip = y_flip = -1;
		break;
	default:
		x_flip = y_flip = 1; // this should not happen
	}
	*x = x0;
	*y = y0;
	*x += (x_flip * ((byteSine[angle] * radius) >> 8));
	*y += (y_flip * ((byteSine[15 - angle] * radius) >> 8));
}

uint8_t PageGLCDtest::_refresh_med() {
	x0 = GLCD.CenterX;
	y0 = GLCD.CenterY;

	GLCD.DrawLine(x0, y0, x1, y1, WHITE);
	this->calcangle(&x1, &y1);
	GLCD.DrawLine(x0, y0, x1, y1, BLACK);
}

uint8_t PageGLCDtest::_refresh_slow() {
}

uint8_t PageGLCDtest::_interact(uint8_t buttonid) {
	switch (buttonid) {
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

