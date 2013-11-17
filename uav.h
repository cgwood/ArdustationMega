// Header file to contain the parameters of the UAV
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

struct vehicle {
  float roll; ///< Roll angle (rad)
  float pitch; ///< Pitch angle (rad)
  float yaw; ///< Yaw angle (rad)
  float rollspeed; ///< Roll angular speed (rad/s)
  float pitchspeed; ///< Pitch angular speed (rad/s)
  float yawspeed; ///< Yaw angular speed (rad/s)
  float airspeed; ///< Current airspeed in m/s
  float groundspeed; ///< Current ground speed in m/s
  float climb; ///< Current climb rate in meters/second
  int16_t heading; ///< Current heading in degrees, in compass units (0..360, 0=north)
  uint16_t throttle; ///< Current throttle setting in integer percent, 0 to 100
  uint16_t load; ///< Maximum usage in percent of the mainloop time, (0%: 0, 100%: 1000) should be always below 1000
  uint16_t voltage_battery; ///< Battery voltage, in millivolts (1 = 1 millivolt)
  uint8_t type; ///< Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)
  uint8_t autopilot; ///< Autopilot type / class. defined in MAV_AUTOPILOT ENUM
  uint8_t sysid; ///< MAV System ID
  uint32_t custom_mode; ///< A bitfield for use for autopilot-specific flags.
  uint8_t base_mode; ///< System mode bitfield, see MAV_MODE_FLAGS ENUM in mavlink/include/mavlink_types.h
  uint8_t system_status; ///< System status flag, see MAV_STATE ENUM
  float lat;
  float lon;
  float alt;
  float vx,vy,vz,vel;
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
  bool param_copter_avail[PARAM_COUNT_COPTER];
  float rate_pid_copter[RATE_PID_COUNT_COPTER];
  bool rate_pid_copter_avail[RATE_PID_COUNT_COPTER];
  // Rover parameters
  float param_rover[PARAM_COUNT_ROVER];
  bool param_rover_avail[PARAM_COUNT_ROVER];
  float param_rover_sonar[PARAM_COUNT_ROVER_SONAR];
  bool param_rover_sonar_avail[PARAM_COUNT_ROVER_SONAR];
  // How many parameters are stored on the UAV
  int16_t onboard_param_count;
  // Have we ever connected
  bool connected;
  // When did we connect
  unsigned long connTime;
  // have we ever requested the parameters
  bool bln_requested_params;
};

// Create a vehicle instance called uav
vehicle uav;

