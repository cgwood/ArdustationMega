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
      apm_mav_system    = msg->sysid;
      apm_mav_component = msg->compid;
      hbcount++;
    }
    break;
  case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: 
    {
      alt = mavlink_msg_command_long_get_param4(msg);
      //Serial.println(alt);
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

      for (uint8_t i=0;i<PARAM_COUNT;i++) {
        strcpy_P(txt_id, (char*)pgm_read_word(&(paramTable[i])));
        if (strcmp(txt_id,(const char*)packet.param_id) == 0) {
          uav.param[i] = packet.param_value;
          Serial.print(packet.param_id);
          Serial.print(" = ");
          Serial.println(uav.param[i]);

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
  case MAVLINK_MSG_ID_RAW_IMU:
  case MAVLINK_MSG_ID_SCALED_PRESSURE:
  case MAV_COMP_ID_SERVO11: //?
  case MAV_COMP_ID_SERVO13: //?
  case MAVLINK_MSG_ID_SYS_STATUS:
  case MAVLINK_MSG_ID_MISSION_CURRENT:
  case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
  case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
    break;

    break;
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

  mavlink_msg_param_request_list_pack(0xFF, 0xFA, &msg, apm_mav_system, apm_mav_component);
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

  const int  maxStreams = 2;
  const uint8_t MAVStreams[maxStreams] = {
    MAV_DATA_STREAM_EXTRA1, MAV_DATA_STREAM_EXTENDED_STATUS  };
  const uint16_t MAVRates[maxStreams] = {0x20, 0x01};

  for (int i=0; i < maxStreams; i++) {
    //        mavlink_msg_request_data_stream_send(chan,
    //            apm_mav_system, apm_mav_component,
    //            MAVStreams[i], MAVRates[i], 1);
    mavlink_msg_request_data_stream_pack(20, MAV_COMP_ID_IMU, &msg, apm_mav_system, apm_mav_component, MAVStreams[i], MAVRates[i], 1);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    _port->write(buf, len);
    //Serial.println(MAVStreams[i]);
    //Serial.write(buf, len);
  }

  //	mavlink_msg_request_data_stream_send(chan, 1, 1, MAV_DATA_STREAM_EXTENDED_STATUS, 2, 1);
  //	mavlink_msg_request_data_stream_send(chan, 1, 1, MAV_DATA_STREAM_EXTRA1, 2, 1);  
  Serial.print("Requesting mavlink data with ");
  Serial.print(apm_mav_system);
  Serial.print(", ");
  Serial.print(apm_mav_component);
  Serial.println();
}

