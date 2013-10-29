// Antenna tracking class
// Created 2011 By Colin G http://www.diydrones.com/profile/ColinG

/// @file       tracker.h
/// @brief      antenna tracking

/// @class      Tracker
/// @brief      Points the antenna at the aircraft

// Macros for angle conversions
#define toRad(x) (x*PI)/180.0
#define toDeg(x) (x*180.0)/PI

// Limits for the antenna elevation
#define tilt_pos_upper_limit 140 // Upper tilt limit (antenna points to the sky)
#define tilt_pos_lower_limit 60 //  Lower tilt limit (anntenna points straight ahead)

// Servo update rate, period in ms
#define TRACKERPERIOD 200

class Tracker {
public:
  Tracker();

  /// Notify the tracker of a new home position
  ///
  //static void 	notify(void *arg, mavlink_message_t *messageData);

  /// Update the position of the servos for the antenna
  ///
  void			update(void);
  float         get_bearing(void);
  float         get_elevation(void);
  void          set_bearing(float fake_bearing);
  void          set_elevation(float fake_elevation);
  float         get_dist(void);
  float         get_servo_pan(void);
  float         get_servo_tilt(void);

private:
  float   _pan;		///< Pan  servo position
  float   _tilt;	///< Tilt servo position
  float _uavDist; ///< Distance to UAV
  float _uavBear; ///< Bearing to UAV
  float _uavElev; ///< Elevation to UAV
  float _fakeBear; ///< Bearing to UAV
  float _fakeElev; ///< Elevation to UAV
  float _offset;  ///< An offset for rotating servo limit point, currently not used
  //        Servo Pan;		///< Pan servo
  //        Servo Tilt;		///< Tilt servo
  long _last_update; ///< Timestamp of last servo position update

    /// Update the position of the servos for the antenna
  ///
  void			_update(void);

  /// Calculate the distance and bearing to the aircraft
  ///
  void			_calcs(float lat1, float lat2, float lon1, float lon2);
};



