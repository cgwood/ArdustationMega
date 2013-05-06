#include "GCS.h"

GCS_MAVLINK::GCS_MAVLINK(uint8_t key) :
packet_drops(0),

// parameters
// note, all values not explicitly initialised here are zeroed
waypoint_send_timeout(1000), // 1 second
waypoint_receive_timeout(1000) // 1 second
{

}

void
GCS_MAVLINK::init(Stream * port)
{
  GCS_Class::init(port);
  if (port == &Serial) {
    //    mavlink_comm_0_port = port;
    chan = MAVLINK_COMM_0;
  }
  else{
    //    mavlink_comm_1_port = port;
    chan = MAVLINK_COMM_1;
  }
  _queued_parameter = NULL;
}


void
GCS_MAVLINK::update(void)
{
  // receive new packets
  mavlink_message_t msg;
  mavlink_status_t status;
  status.packet_rx_drop_count = 0;

  // process received bytes
  while (_port->available())
  {
    uint8_t c = _port->read();

    // Try to get a new message
    if (mavlink_parse_char(chan, c, &msg, &status)) {
      //      mavlink_active = 1;
      handleMessage(&msg);
      //      Serial.println("A message");
    }
  }

  // Update packet drops counter
  packet_drops += status.packet_rx_drop_count;

  //  // send out queued params/ waypoints
  //  if (NULL != _queued_parameter) {
  //    send_message(MSG_NEXT_PARAM);
  //  }

  if (!waypoint_receiving && !waypoint_sending) {
    return;
  }

  uint32_t tnow = millis();

  // stop waypoint sending if timeout
  if (waypoint_sending && (millis() - waypoint_timelast_send) > waypoint_send_timeout){
    waypoint_sending = false;
  }

  // stop waypoint receiving if timeout
  if (waypoint_receiving && (millis() - waypoint_timelast_receive) > waypoint_receive_timeout){
    waypoint_receiving = false;
  }
}
void GCS_MAVLINK::handleMessage(mavlink_message_t* msg)
{
  static uint8_t mav_nav=255;							// For setting mode (some require receipt of 2 messages...)
  int32_t alt;

  switch (msg->msgid) {

  case MAVLINK_MSG_ID_HEARTBEAT:
    {
      //      Serial.println("Heartbeat");
      // decode some info
      mavlink_heartbeat_t packet;
      mavlink_msg_heartbeat_decode(msg, &packet);
      
      // Update mode
      uav.base_mode = packet.base_mode;
      
      if (uav.sysid != msg->sysid) {
        Serial.println("Connected to a new UAV");
        // Update system id, type and autopilot
        uav.sysid = msg->sysid;
        uav.type = packet.type;
        uav.autopilot = packet.autopilot;
        
        Serial.print("UAV Type: ");
        switch (uav.type) {
          case MAV_TYPE_GENERIC:
            Serial.println("Generic");
            break;
          case MAV_TYPE_FIXED_WING:
            Serial.println("Fixed wing");
            break;
          case MAV_TYPE_QUADROTOR:
            Serial.println("Quadrotor");
            break;
          case MAV_TYPE_HELICOPTER:
            Serial.println("Helicopter");
            break;
          case MAV_TYPE_TRICOPTER:
            Serial.println("Tricopter (Nice one!)");
            break;
          default:
            Serial.println("Unknown");
        }
        
        Serial.print("Autopilot Type: ");
        switch (uav.autopilot) {
          case MAV_AUTOPILOT_GENERIC:
            Serial.println("Generic");
            break;
          case MAV_AUTOPILOT_PIXHAWK:
            Serial.println("Pixhawk");
            break;
          case MAV_AUTOPILOT_ARDUPILOTMEGA:
            Serial.println("Ardupilot Mega");
            break;
          case MAV_AUTOPILOT_UDB:
            Serial.println("UAV Devboard");
            break;
          case MAV_AUTOPILOT_PX4:
            Serial.println("PX4");
            break;
          default:
            Serial.println("Unknown");
        }
            
        // Update the page due to the new info
        Pages::forceUpdate(0);
      }
      
      uav.sysid    = msg->sysid;
      apm_mav_component = msg->compid;
      hbcount++;
    }
    break;
  case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: 
    {
      // decode
      mavlink_global_position_int_t packet;
      mavlink_msg_global_position_int_decode(msg, &packet);
      
      // Save the position
      uav.lat = packet.lat/10000000.0;
      uav.lon = packet.lon/10000000.0;
      uav.alt = packet.alt/1000.0;
    }

  case MAVLINK_MSG_ID_ATTITUDE:
    {
      // decode
      mavlink_attitude_t packet;
      mavlink_msg_attitude_decode(msg, &packet);

      // save the attitude
      uav.roll = packet.roll;
      uav.pitch = packet.pitch;
      uav.yaw = packet.yaw;
      uav.rollspeed = packet.rollspeed,
      uav.pitchspeed = packet.pitchspeed;
      uav.yawspeed = packet.yawspeed;

      break;
    }
  case MAVLINK_MSG_ID_PARAM_VALUE: // for PIDs etc.
    {
      mavlink_param_value_t packet;
      mavlink_msg_param_value_decode(msg, &packet);
      char txt_id[15];
      
      // DEV: Display the parameters over serial
      Serial.print(packet.param_id);
      Serial.print(" = ");
      Serial.println(packet.param_value);

      for (uint8_t i=0;i<PARAM_COUNT;i++) {
        strcpy_P(txt_id, (char*)pgm_read_word(&(paramTable[i])));
        if (strcmp(txt_id,(const char*)packet.param_id) == 0) {
          uav.param[i] = packet.param_value;

          // Update the parameter pages
          Pages::forceUpdate(Pages::R_PARAM);
        }
      }

      //    if (strcmp("RATE_RLL_P", (const char*)packet.param_id) == 0) {
      //      uav.roll2srvP = packet.param_value;
      //      Serial.println();
      //      Serial.println(uav.roll2srvP);
      //      Serial.println();
      //    }
      //    else
      //      Serial.println(packet.param_id);

      break;
    }
  case MAVLINK_MSG_ID_SYS_STATUS: {
    mavlink_sys_status_t packet;
    mavlink_msg_sys_status_decode(msg, &packet);
    
//    Serial.print("Voltage: ");
//    Serial.println(packet.voltage_battery);
//    Serial.print("Drop rate: ");
//    Serial.println(packet.drop_rate_comm);
//    Serial.print("Load: ");
//    Serial.println(packet.load);
    uav.load = packet.load;
    uav.voltage_battery = packet.voltage_battery;
    break;
  }
  case MAVLINK_MSG_ID_RAW_IMU:
  case MAVLINK_MSG_ID_SCALED_PRESSURE:
  case MAV_COMP_ID_SERVO11: //?
  case MAV_COMP_ID_SERVO13: //?
  case MAVLINK_MSG_ID_MISSION_CURRENT:
  case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
  case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
  case MAVLINK_MSG_ID_HIGHRES_IMU:
  case MAVLINK_MSG_ID_ROLL_PITCH_YAW_THRUST_SETPOINT:
  case MAVLINK_MSG_ID_ROLL_PITCH_YAW_RATES_THRUST_SETPOINT:
  case MAVLINK_MSG_ID_MANUAL_CONTROL:
    break;
  case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT: {
//      mavlink_named_value_float_t packet;
//      mavlink_msg_named_value_float_decode(msg, &packet);
//
//      Serial.print("Debug: ");
//      Serial.print(packet.name);
//      Serial.print(" = ");
//      Serial.println(packet.value);
  
      break;
  }
  case MAVLINK_MSG_ID_STATUSTEXT: {
      mavlink_statustext_t packet;
      mavlink_msg_statustext_decode(msg, &packet);

      Serial.print("Severity: ");
      Serial.println(packet.severity);
      Serial.println(packet.text);
      
  
      break;
  }
  default:
    Serial.println(msg->msgid);
  }
}

void
GCS_MAVLINK::params_request(void)
{
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  Serial.println("Requesting params");

  mavlink_msg_param_request_list_pack(0xFF, 0xFA, &msg, uav.sysid, apm_mav_component);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  _port->write(buf, len);
}

void
GCS_MAVLINK::data_stream_request(void)
{
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  //    const int  maxStreams = 6;
  //    const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_RAW_SENSORS,
  //        MAV_DATA_STREAM_EXTENDED_STATUS,
  //        MAV_DATA_STREAM_RC_CHANNELS,
  //        MAV_DATA_STREAM_POSITION,
  //        MAV_DATA_STREAM_EXTRA1, 
  //        MAV_DATA_STREAM_EXTRA2};
  //    const uint16_t MAVRates[maxStreams] = {0x02, 0x02, 0x05, 0x02, 0x05, 0x02};

  const int  maxStreams = 4;
  const uint8_t MAVStreams[maxStreams] = {
    MAV_DATA_STREAM_EXTRA1, MAV_DATA_STREAM_EXTENDED_STATUS, MAV_DATA_STREAM_POSITION, MAV_DATA_STREAM_RAW_SENSORS };
  const uint16_t MAVRates[maxStreams] = {0x20, 0x01, 0x01, 0x01};

  for (int i=0; i < maxStreams; i++) {
    //        mavlink_msg_request_data_stream_send(chan,
    //            apm_mav_system, apm_mav_component,
    //            MAVStreams[i], MAVRates[i], 1);
    mavlink_msg_request_data_stream_pack(20, MAV_COMP_ID_IMU, &msg, uav.sysid, apm_mav_component, MAVStreams[i], MAVRates[i], 1);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    _port->write(buf, len);
    //Serial.println(MAVStreams[i]);
    //Serial.write(buf, len);
  }

  //	mavlink_msg_request_data_stream_send(chan, 1, 1, MAV_DATA_STREAM_EXTENDED_STATUS, 2, 1);
  //	mavlink_msg_request_data_stream_send(chan, 1, 1, MAV_DATA_STREAM_EXTRA1, 2, 1);  
  Serial.print("Requesting mavlink data with ");
  Serial.print(uav.sysid);
  Serial.print(", ");
  Serial.print(apm_mav_component);
  Serial.println();
}

