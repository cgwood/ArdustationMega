/*
 * uav_params.h
 * Header file to contain the parameter types for different APs
 *
 *  Created on: 1 Sep 2013
 *      Author: Colin G http://www.diydrones.com/profile/ColinG
 */

#ifndef UAV_PARAMS_H_
#define UAV_PARAMS_H_

//  // Parameter page parameters
//  WP_LOITER_RAD,
//  WP_RADIUS,
//  XTRK_GAIN_SC,
//  XTRK_ANGLE_CD,
//  TRIM_ARSPD_CM,
//  ARSPD_FBW_MIN,
//  ARSPD_FBW_MAX,
//  KFF_PTCH2THR,
//  KFF_THR2PTCH,
//  LOG_BITMASK,
//  // Parameter count
//  PARAM_COUNT_PLANE

// ------------------------------ FIXED WING ------------------------------ //
enum PARAMS_PLANE {
	// Parameter page parameters
	TRIM_AUTO,
	ARSPD_RATIO,
	SCALING_SPEED,
	LOG_BITMASK,
	// Parameter count
	PARAM_COUNT_PLANE
};

// Parameter page - plane params
prog_char param_plane_0[] PROGMEM = "TRIM_AUTO";
prog_char param_plane_1[] PROGMEM = "ARSPD_RATIO";
prog_char param_plane_2[] PROGMEM = "SCALING_SPEED";
prog_char param_plane_3[] PROGMEM = "LOG_BITMASK";


//                                          "123456789012"
PROGMEM const prog_char ParamNamesPlane[] = "InFlightTrim\n"
											"Airsd Ratio \n"
											"ScalingSpeed\n"
											"Log Bitmask \n";

const uint8_t ParamScales[] = {99,    0,    0,    0}; // *10^(-x)
const uint8_t ParamDPs[] =    {99,    3,    1,    0}; // 99 in both denotes boolean

PROGMEM const char *paramTable_plane[] = { param_plane_0, param_plane_1, param_plane_2, param_plane_3};

enum PARAMS_PLANE_CTUN {
	// Parameter page parameters
	RLL2SRV_P,
	RLL2SRV_I,
	RLL2SRV_D,
	RLL2SRV_IMAX,
	RLL2SRV_RMAX,
	RLL2SRV_T_CONST,
	PTCH2SRV_P,
	PTCH2SRV_I,
	PTCH2SRV_D,
	PTCH2SRV_IMAX,
	PTCH2SRV_RMAX_UP,
	PTCH2SRV_RMAX_DN,
	PTCH2SRV_T_CONST,
	PTCH2SRV_RLL,
	YAW2SRV_SLIP,
	YAW2SRV_DAMP,
	YAW2SRV_RLL,
	YAW2SRV_INT,
	// Parameter count
	PARAM_COUNT_PLANE_CTUN
};

prog_char param_plane_ctun_0[] PROGMEM = "RLL2SRV_P";
prog_char param_plane_ctun_1[] PROGMEM = "RLL2SRV_I";
prog_char param_plane_ctun_2[] PROGMEM = "RLL2SRV_D";
prog_char param_plane_ctun_3[] PROGMEM = "RLL2SRV_IMAX";
prog_char param_plane_ctun_4[] PROGMEM = "RLL2SRV_RMAX";
prog_char param_plane_ctun_5[] PROGMEM = "RLL2SRV_TCONST";
prog_char param_plane_ctun_6[] PROGMEM = "PTCH2SRV_P";
prog_char param_plane_ctun_7[] PROGMEM = "PTCH2SRV_I";
prog_char param_plane_ctun_8[] PROGMEM = "PTCH2SRV_D";
prog_char param_plane_ctun_9[] PROGMEM = "PTCH2SRV_IMAX";
prog_char param_plane_ctun_10[] PROGMEM = "PTCH2SRV_RMAX_UP";
prog_char param_plane_ctun_11[] PROGMEM = "PTCH2SRV_RMAX_DN";
prog_char param_plane_ctun_12[] PROGMEM = "PTCH2SRV_TCONST";
prog_char param_plane_ctun_13[] PROGMEM = "PTCH2SRV_RLL";
prog_char param_plane_ctun_14[] PROGMEM = "YAW2SRV_SLIP";
prog_char param_plane_ctun_15[] PROGMEM = "YAW2SRV_DAMP";
prog_char param_plane_ctun_16[] PROGMEM = "YAW2SRV_RLL";
prog_char param_plane_ctun_17[] PROGMEM = "YAW2SRV_INT";

//                                              "123456789012"
PROGMEM const prog_char ParamNamesPlaneCTUN[] = "Roll P      \n"
												"Roll I      \n"
												"Roll D      \n"
												"Roll Imax   \n"
												"Roll Rmax   \n"
												"Roll T Const\n"
												"Pitch P     \n"
												"Pitch I     \n"
												"Pitch D     \n"
												"Pitch Imax  \n"
												"Pitch RmaxUp\n"
												"Pitch RmaxDn\n"
												"Pitch TConst\n"
												"Pitch 2 Roll\n"
												"Yaw Slip    \n"
												"Yaw Damp    \n"
												"Yaw Roll    \n"
												"Yaw Int     \n";

const uint8_t ParamScalesCTUN[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // *10^(-x)
const uint8_t ParamDPsCTUN[] =    {2,2,2,0,2,2,2,2,2,0,2,2,2,2,2,2,2,2}; // 99 in both denotes boolean

PROGMEM const char *paramTable_plane_ctun[] = { param_plane_ctun_0, param_plane_ctun_1,
		param_plane_ctun_2, param_plane_ctun_3, param_plane_ctun_4, param_plane_ctun_5,
		param_plane_ctun_6, param_plane_ctun_7, param_plane_ctun_8, param_plane_ctun_9,
		param_plane_ctun_10, param_plane_ctun_11, param_plane_ctun_12, param_plane_ctun_13,
		param_plane_ctun_14, param_plane_ctun_15, param_plane_ctun_16, param_plane_ctun_17 };

enum PARAMS_PLANE_NTUN {
	// Parameter page parameters
	NAVL1_PERIOD,
	NAVL1_DAMPING,
	WP_RADIUS,
	LIM_ROLL_CD,
	// Parameter count
	PARAM_COUNT_PLANE_NTUN
};

prog_char param_plane_ntun_0[] PROGMEM = "NAVL1_PERIOD";
prog_char param_plane_ntun_1[] PROGMEM = "NAVL1_DAMPING";
prog_char param_plane_ntun_2[] PROGMEM = "WP_RADIUS";
prog_char param_plane_ntun_3[] PROGMEM = "LIM_ROLL_CD";

//                                              "123456789012"
PROGMEM const prog_char ParamNamesPlaneNTUN[] = "L1 Period   \n"
												"L1 Damping  \n"
												"WP Radius   \n"
												"Lim Roll CD \n";

const uint8_t ParamScalesNTUN[] = {0,0,0,2}; // *10^(-x)
const uint8_t ParamDPsNTUN[] =    {2,2,2,2}; // 99 in both denotes boolean

PROGMEM const char *paramTable_plane_ntun[] = { param_plane_ntun_0, param_plane_ntun_1,
		param_plane_ntun_2, param_plane_ntun_3 };

enum PARAMS_PLANE_TECS {
	// Parameter page parameters
	ARSPD_FBW_MIN,
	ARSPD_FBW_MAX,
	THR_MIN,
	TRIM_THROTTLE,
	THR_MAX,
	LIM_PITCH_MIN,
	LIM_PITCH_MAX,
	TECS_SINK_MIN,
	TECS_SINK_MAX,
	TECS_CLMB_MAX,
	TECS_TIME_CONST,
	TECS_PTCH_DAMP,
	// Parameter count
	PARAM_COUNT_PLANE_TECS
};

prog_char param_plane_tecs_0[] PROGMEM = "ARSPD_FBW_MIN";
prog_char param_plane_tecs_1[] PROGMEM = "ARSPD_FBW_MAX";
prog_char param_plane_tecs_2[] PROGMEM = "THR_MIN";
prog_char param_plane_tecs_3[] PROGMEM = "TRIM_THROTTLE";
prog_char param_plane_tecs_4[] PROGMEM = "THR_MAX";
prog_char param_plane_tecs_5[] PROGMEM = "LIM_PITCH_MIN";
prog_char param_plane_tecs_6[] PROGMEM = "LIM_PITCH_MAX";
prog_char param_plane_tecs_7[] PROGMEM = "TECS_SINK_MIN";
prog_char param_plane_tecs_8[] PROGMEM = "TECS_SINK_MAX";
prog_char param_plane_tecs_9[] PROGMEM = "TECS_CLMB_MAX";
prog_char param_plane_tecs_10[] PROGMEM = "TECS_TIME_CONST";
prog_char param_plane_tecs_11[] PROGMEM = "TECS_PTCH_DAMP";

//                                              "123456789012"
PROGMEM const prog_char ParamNamesPlaneTECS[] = "Airspeed Min\n"
												"Airspeed Max\n"
												"Throttle Min\n"
												"Thrtl Cruise\n"
												"Throttle Max\n"
												"Pitch Min   \n"
												"Pitch Max   \n"
												"TECS SinkMin\n"
												"TECS SinkMax\n"
												"TECS ClmbMax\n"
												"TECS T Const\n"
												"Tecs PchDamp\n";

const uint8_t ParamScalesTECS[] = {0,0,0,0,0,2,2,0,0,0,0,0}; // *10^(-x)
const uint8_t ParamDPsTECS[] =    {0,0,0,0,0,1,1,2,2,2,2,2}; // 99 in both denotes boolean

PROGMEM const char *paramTable_plane_tecs[] = { param_plane_tecs_0, param_plane_tecs_1,
		param_plane_tecs_2, param_plane_tecs_3, param_plane_tecs_4, param_plane_tecs_5,
		param_plane_tecs_6, param_plane_tecs_7, param_plane_tecs_8, param_plane_tecs_9,
		param_plane_tecs_10, param_plane_tecs_11};

// ------------------------------ ROVERS ------------------------------ //
enum PARAMS_ROVER {
	// Parameter page parameters
	ROVER_THR_MIN,
	ROVER_CRUISE_THROTTLE,
	ROVER_THR_MAX,
	ROVER_THR_SLEWRATE,
	STEER2SRV_P,
	STEER2SRV_I,
	STEER2SRV_D,
	STEER2SRV_IMAX,
	STEER2SRV_TCONST,
	TURN_MAX_G,
	ROVER_NAVL1_PERIOD,
	SPEED_TURN_GAIN,
	SPEED_TURN_DIST,
	SKID_STEER_OUT,
	SKID_STEER_IN,
	FS_ACTION,
	FS_TIMEOUT,
	FS_THR_ENABLE,
	FS_THR_VALUE,
	// Parameter count
	PARAM_COUNT_ROVER
};

const uint8_t ParamScalesRover[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,99,99,0,0,99,0}; // *10^(-x)
const uint8_t ParamDPsRover[] =    {0,0,0,0,0,0,0,0,2,2,2,0,2,99,99,0,0,99,0}; // 99 in both denotes boolean

// Parameter page - rover params
prog_char param_rover_0[] PROGMEM = "THR_MIN";
prog_char param_rover_1[] PROGMEM = "CRUISE_THROTTLE";
prog_char param_rover_2[] PROGMEM = "THR_MAX";
prog_char param_rover_3[] PROGMEM = "THR_SLEWRATE";
prog_char param_rover_4[] PROGMEM = "STEER2SRV_P";
prog_char param_rover_5[] PROGMEM = "STEER2SRV_I";
prog_char param_rover_6[] PROGMEM = "STEER2SRV_D";
prog_char param_rover_7[] PROGMEM = "STEER2SRV_IMAX";
prog_char param_rover_8[] PROGMEM = "STEER2SRV_TCONST";
prog_char param_rover_9[] PROGMEM = "TURN_MAX_G";
prog_char param_rover_10[] PROGMEM = "NAVL1_PERIOD";
prog_char param_rover_11[] PROGMEM = "SPEED_TURN_GAIN";
prog_char param_rover_12[] PROGMEM = "SPEED_TURN_DIST";
prog_char param_rover_13[] PROGMEM = "SKID_STEER_OUT";
prog_char param_rover_14[] PROGMEM = "SKID_STEER_IN";
prog_char param_rover_15[] PROGMEM = "FS_ACTION";
prog_char param_rover_16[] PROGMEM = "FS_TIMEOUT";
prog_char param_rover_17[] PROGMEM = "FS_THR_ENABLE";
prog_char param_rover_18[] PROGMEM = "FS_THR_VALUE";

//                                           "123456789012"
PROGMEM const prog_char ParamNamesRover[]  = "Throttle min\n"
		 "Cruise Thr  \n"
		 "Throttle max\n"
		 "Thr Slewrate\n"
		 "Steering P  \n"
		 "Steering I  \n"
		 "Steering D  \n"
		 "SteeringImax\n"
		 "SteeringTcon\n"
		 "Turn Max G  \n"
		 "Nav L1Period\n"
		 "Spd TurnGain\n"
		 "Spd TurnDist\n"
		 "Skid str in \n"
		 "Skid str out\n"
		 "FS Action   \n"
		 "FS Timeout  \n"
		 "FS Thr en   \n"
		 "FS Thr value\n";

PROGMEM const char *paramTable_rover[] = { param_rover_0, param_rover_1,
		param_rover_2, param_rover_3, param_rover_4, param_rover_5,
		param_rover_6, param_rover_7, param_rover_8, param_rover_9,
		param_rover_10, param_rover_11, param_rover_12, param_rover_13,
		param_rover_14, param_rover_15, param_rover_16, param_rover_17, param_rover_18 };

// ------------------------------ ROTARY WING ------------------------------ //
enum PARAMS_COPTER {
	// PID Parameters
	RATE_RLL_P,
	RATE_RLL_I,
	RATE_RLL_D,
	RATE_RLL_IMAX,
	RATE_PIT_P,
	RATE_PIT_I,
	RATE_PIT_D,
	RATE_PIT_IMAX,
	RATE_YAW_P,
	RATE_YAW_I,
	RATE_YAW_D,
	RATE_YAW_IMAX,
	THR_RATE_P,
	THR_RATE_I,
	THR_RATE_D,
	THR_RATE_IMAX,
	// Parameter count
	PARAM_COUNT_COPTER
};

// PIDs for copters
prog_char param_copter_0[] PROGMEM = "RATE_RLL_P";
prog_char param_copter_1[] PROGMEM = "RATE_RLL_I";
prog_char param_copter_2[] PROGMEM = "RATE_RLL_D";
prog_char param_copter_3[] PROGMEM = "RATE_RLL_IMAX";
prog_char param_copter_4[] PROGMEM = "RATE_PIT_P";
prog_char param_copter_5[] PROGMEM = "RATE_PIT_I";
prog_char param_copter_6[] PROGMEM = "RATE_PIT_D";
prog_char param_copter_7[] PROGMEM = "RATE_PIT_IMAX";
prog_char param_copter_8[] PROGMEM = "RATE_YAW_P";
prog_char param_copter_9[] PROGMEM = "RATE_YAW_I";
prog_char param_copter_10[] PROGMEM = "RATE_YAW_D";
prog_char param_copter_11[] PROGMEM = "RATE_YAW_IMAX";
prog_char param_copter_12[] PROGMEM = "THR_RATE_P";
prog_char param_copter_13[] PROGMEM = "THR_RATE_I";
prog_char param_copter_14[] PROGMEM = "THR_RATE_D";
prog_char param_copter_15[] PROGMEM = "THR_RATE_IMAX";

PROGMEM const char *paramTable_copter[] = { param_copter_0, param_copter_1,
		param_copter_2, param_copter_3, param_copter_4, param_copter_5,
		param_copter_6, param_copter_7, param_copter_8, param_copter_9,
		param_copter_10, param_copter_11, param_copter_12, param_copter_13,
		param_copter_14, param_copter_15 };

#endif /* UAV_PARAMS_H_ */
