// ASM Parameters
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

static float 			load;

#define BATTSAMPLES 30 // Too high and the averaging code has to change

struct ArduStation {
  // Properties
  uint32_t time;                      ///< GPS time (milliseconds from epoch)
  uint32_t date;                      ///< GPS date (FORMAT TBD)
  int32_t altitude;                   ///< altitude in cm
  int16_t hdop;                       ///< horizontal dilution of precision in cm
  uint8_t num_sats;                   ///< Number of visible satelites
  uint8_t gps_status;                 ///< GPS fix status
  float lat;
  float lon;
  float alt;
  int encoderval;
  int batt_sum;
  int batt_sample[BATTSAMPLES]; // Average over last BATTSAMPLES readings
  uint8_t batt_i; // Which index of batt did we last write to
  
  int rssi_sum;
  int rssi_sample[BATTSAMPLES]; // Average over last BATTSAMPLES readings
  uint8_t rssi_i; // Which index of rssi did we last write to
};

ArduStation ASM;

// Initialize default values (this will later be in the construction section of the class)
void
init_batt() {
  int initval = analogRead(A8);
  ASM.batt_i=0;
  for (uint8_t i=0;i<BATTSAMPLES;i++)
    ASM.batt_sample[i] = initval;
    
  ASM.batt_sum = initval*BATTSAMPLES;
}


void
sample_batt() {
  // Get the new reading for the moving average
  ASM.batt_sum -= ASM.batt_sample[ASM.batt_i];
  ASM.batt_sample[ASM.batt_i] = analogRead(A8);
  ASM.batt_sum += ASM.batt_sample[ASM.batt_i];

  // Move the index along
  ASM.batt_i++;
  if (ASM.batt_i > BATTSAMPLES-1)
    ASM.batt_i = 0;
}

float
get_batt() {
  float v = ASM.batt_sum;

  v /= 30.0;//BATTSAMPLES;
  v *= 0.0048828125;
  return v+0.1;
}

int get_rssi() {
  int rssi;
  
  rssi = analogRead(A0);
  
  // Scale as working on 3.3v logic
  rssi*=1.5;
  
  return rssi;
}

