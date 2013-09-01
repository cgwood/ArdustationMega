/*
 * uav_params.h
 * Header file to contain the parameter types for different APs
 *
 *  Created on: 1 Sep 2013
 *      Author: Colin G http://www.diydrones.com/profile/ColinG
 */

#ifndef UAV_PARAMS_H_
#define UAV_PARAMS_H_

#define MAX_PARAM_COUNT 16

// Enumerate the parameters
enum PARAMS_PLANE {
  // Parameter page parameters
  WP_LOITER_RAD,
  WP_RADIUS,
  XTRK_GAIN_SC,
  XTRK_ANGLE_CD,
  TRIM_ARSPD_CM,
  ARSPD_FBW_MIN,
  ARSPD_FBW_MAX,
  KFF_PTCH2THR,
  KFF_THR2PTCH,
  LOG_BITMASK,
  // Parameter count
  PARAM_COUNT_PLANE
};

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

// Define the parameters of interest here - Update above enumeration to match any changes here

// Parameter page - plane params
prog_char param_plane_0[] PROGMEM = "WP_LOITER_RAD";
prog_char param_plane_1[] PROGMEM = "WP_RADIUS";
prog_char param_plane_2[] PROGMEM = "XTRK_GAIN_SC";
prog_char param_plane_3[] PROGMEM = "XTRK_ANGLE_CD";
prog_char param_plane_4[] PROGMEM = "TRIM_ARSPD_CM";
prog_char param_plane_5[] PROGMEM = "ARSPD_FBW_MIN";
prog_char param_plane_6[] PROGMEM = "ARSPD_FBW_MAX";
prog_char param_plane_7[] PROGMEM = "KFF_PTCH2THR";
prog_char param_plane_8[] PROGMEM = "KFF_THR2PTCH";
prog_char param_plane_9[] PROGMEM = "LOG_BITMASK";

// PIDs for copters
prog_char param_copter_0[]  PROGMEM = "RATE_RLL_P";
prog_char param_copter_1[]  PROGMEM = "RATE_RLL_I";
prog_char param_copter_2[]  PROGMEM = "RATE_RLL_D";
prog_char param_copter_3[]  PROGMEM = "RATE_RLL_IMAX";
prog_char param_copter_4[]  PROGMEM = "RATE_PIT_P";
prog_char param_copter_5[]  PROGMEM = "RATE_PIT_I";
prog_char param_copter_6[]  PROGMEM = "RATE_PIT_D";
prog_char param_copter_7[]  PROGMEM = "RATE_PIT_IMAX";
prog_char param_copter_8[]  PROGMEM = "RATE_YAW_P";
prog_char param_copter_9[]  PROGMEM = "RATE_YAW_I";
prog_char param_copter_10[] PROGMEM = "RATE_YAW_D";
prog_char param_copter_11[] PROGMEM = "RATE_YAW_IMAX";
prog_char param_copter_12[] PROGMEM = "THR_RATE_P";
prog_char param_copter_13[] PROGMEM = "THR_RATE_I";
prog_char param_copter_14[] PROGMEM = "THR_RATE_D";
prog_char param_copter_15[] PROGMEM = "THR_RATE_IMAX";

PROGMEM const char *paramTable_plane[] = {
  param_plane_0, param_plane_1, param_plane_2, param_plane_3, param_plane_4, param_plane_5, param_plane_6, param_plane_7, param_plane_8, param_plane_9};

PROGMEM const char *paramTable_copter[] = {
  param_copter_0, param_copter_1, param_copter_2, param_copter_3, param_copter_4, param_copter_5, param_copter_6, param_copter_7, param_copter_8, param_copter_9,
  param_copter_10, param_copter_11, param_copter_12, param_copter_13, param_copter_14, param_copter_15};



#endif /* UAV_PARAMS_H_ */
