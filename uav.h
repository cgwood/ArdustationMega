// Header file to contain the parameters of the UAV
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

// Enumerate the parameters
enum PARAMS {  
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
  PARAM_COUNT
};

// Define the parameters of interest here - Update above enumeration to match any changes here
prog_char param_0[]  PROGMEM = "RATE_RLL_P";
prog_char param_1[]  PROGMEM = "RATE_RLL_I";
prog_char param_2[]  PROGMEM = "RATE_RLL_D";
prog_char param_3[]  PROGMEM = "RATE_RLL_IMAX";
prog_char param_4[]  PROGMEM = "RATE_PIT_P";
prog_char param_5[]  PROGMEM = "RATE_PIT_I";
prog_char param_6[]  PROGMEM = "RATE_PIT_D";
prog_char param_7[]  PROGMEM = "RATE_PIT_IMAX";
prog_char param_8[]  PROGMEM = "RATE_YAW_P";
prog_char param_9[]  PROGMEM = "RATE_YAW_I";
prog_char param_10[] PROGMEM = "RATE_YAW_D";
prog_char param_11[] PROGMEM = "RATE_YAW_IMAX";
prog_char param_12[] PROGMEM = "THR_RATE_P";
prog_char param_13[] PROGMEM = "THR_RATE_I";
prog_char param_14[] PROGMEM = "THR_RATE_D";
prog_char param_15[] PROGMEM = "THR_RATE_IMAX";

PROGMEM const char *paramTable[] = {
  param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9,
  param_10, param_11, param_12, param_13, param_14, param_15 }; //, param_16, param_17, param_18,
//		param_19, param_20, param_21, param_22, param_23, param_24, param_25, param_26, param_27,
//		param_28, param_29, param_30, param_31, param_32, param_33, param_34, param_35, param_36,
//		param_37, param_38, param_39, param_40, param_41 };

struct vehicle {
  float roll; ///< Roll angle (rad)
  float pitch; ///< Pitch angle (rad)
  float yaw; ///< Yaw angle (rad)
  float rollspeed; ///< Roll angular speed (rad/s)
  float pitchspeed; ///< Pitch angular speed (rad/s)
  float yawspeed; ///< Yaw angular speed (rad/s)
  float lat;
  float lon;
  float alt;
  int sat_count;
  float roll2srvP;
  float param[PARAM_COUNT];
};

// Create a vehicle instance called uav
vehicle uav;

