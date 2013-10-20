// Header file to contain the parameters of the UAV
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

//// Enumerate the parameters
//enum PARAMS {
//  // PID Paramters
//  RATE_RLL_P,
//  RATE_RLL_I,
//  RATE_RLL_D,
//  RATE_RLL_IMAX,
//  RATE_PIT_P,
//  RATE_PIT_I,
//  RATE_PIT_D,
//  RATE_PIT_IMAX,
//  RATE_YAW_P,
//  RATE_YAW_I,
//  RATE_YAW_D,
//  RATE_YAW_IMAX,
//  THR_RATE_P,
//  THR_RATE_I,
//  THR_RATE_D,
//  THR_RATE_IMAX,
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
//  PARAM_COUNT
//};
//
//// Define the parameters of interest here - Update above enumeration to match any changes here
//prog_char param_0[]  PROGMEM = "RATE_RLL_P";
//prog_char param_1[]  PROGMEM = "RATE_RLL_I";
//prog_char param_2[]  PROGMEM = "RATE_RLL_D";
//prog_char param_3[]  PROGMEM = "RATE_RLL_IMAX";
//prog_char param_4[]  PROGMEM = "RATE_PIT_P";
//prog_char param_5[]  PROGMEM = "RATE_PIT_I";
//prog_char param_6[]  PROGMEM = "RATE_PIT_D";
//prog_char param_7[]  PROGMEM = "RATE_PIT_IMAX";
//prog_char param_8[]  PROGMEM = "RATE_YAW_P";
//prog_char param_9[]  PROGMEM = "RATE_YAW_I";
//prog_char param_10[] PROGMEM = "RATE_YAW_D";
//prog_char param_11[] PROGMEM = "RATE_YAW_IMAX";
//prog_char param_12[] PROGMEM = "THR_RATE_P";
//prog_char param_13[] PROGMEM = "THR_RATE_I";
//prog_char param_14[] PROGMEM = "THR_RATE_D";
//prog_char param_15[] PROGMEM = "THR_RATE_IMAX";
//// Parameter page params
//prog_char param_16[]  PROGMEM = "WP_LOITER_RAD";
//prog_char param_17[]  PROGMEM = "WP_RADIUS";
//prog_char param_18[]  PROGMEM = "XTRK_GAIN_SC";
//prog_char param_19[]  PROGMEM = "XTRK_ANGLE_CD";
//prog_char param_20[] PROGMEM = "TRIM_ARSPD_CM";
//prog_char param_21[] PROGMEM = "ARSPD_FBW_MIN";
//prog_char param_22[] PROGMEM = "ARSPD_FBW_MAX";
//prog_char param_23[] PROGMEM = "KFF_PTCH2THR";
//prog_char param_24[] PROGMEM = "KFF_THR2PTCH";
//prog_char param_25[] PROGMEM = "LOG_BITMASK";
//
//PROGMEM const char *paramTable[] = {
//  param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9,
//  param_10, param_11, param_12, param_13, param_14, param_15,  param_16, param_17, param_18,
//  param_19, param_20, param_21, param_22, param_23, param_24, param_25}; //, param_26, param_27,
////		param_28, param_29, param_30, param_31, param_32, param_33, param_34, param_35, param_36,
////		param_37, param_38, param_39, param_40, param_41 };

struct vehicle {
  float roll; ///< Roll angle (rad)
  float pitch; ///< Pitch angle (rad)
  float yaw; ///< Yaw angle (rad)
  float rollspeed; ///< Roll angular speed (rad/s)
  float pitchspeed; ///< Pitch angular speed (rad/s)
  float yawspeed; ///< Yaw angular speed (rad/s)
  uint16_t load; ///< Maximum usage in percent of the mainloop time, (0%: 0, 100%: 1000) should be always below 1000
  uint16_t voltage_battery; ///< Battery voltage, in millivolts (1 = 1 millivolt)
  uint8_t type; ///< Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)
  uint8_t autopilot; ///< Autopilot type / class. defined in MAV_AUTOPILOT ENUM
  uint8_t sysid; ///< MAV System ID
  uint8_t base_mode; ///< System mode bitfield, see MAV_MODE_FLAGS ENUM in mavlink/include/mavlink_types.h
  float lat;
  float lon;
  float alt;
  int sat_count;
  float dist; // Distance from gcs
  float roll2srvP;
  // Plane parameters
  float param_plane[PARAM_COUNT_PLANE];
  float param_plane_ctun[PARAM_COUNT_PLANE_CTUN];
  float param_plane_ntun[PARAM_COUNT_PLANE_NTUN];
  float param_plane_tecs[PARAM_COUNT_PLANE_TECS];
  bool param_plane_avail[PARAM_COUNT_PLANE];
  bool param_plane_ctun_avail[PARAM_COUNT_PLANE_CTUN];
  bool param_plane_ntun_avail[PARAM_COUNT_PLANE_NTUN];
  bool param_plane_tecs_avail[PARAM_COUNT_PLANE_TECS];
  // Copter parameters
  float param_copter[PARAM_COUNT_COPTER];
  float param_copter_avail[PARAM_COUNT_COPTER];
  // Rover parameters
  float param_rover[PARAM_COUNT_ROVER];
  float param_rover_avail[PARAM_COUNT_ROVER];
  // How many parameters are stored on the UAV
  int16_t onboard_param_count;
};

// Create a vehicle instance called uav
vehicle uav;

