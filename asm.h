// ASM Parameters
// Created 2012 By Colin G http://www.diydrones.com/profile/ColinG

static float 			load;

#define BATTSAMPLES 30 // Too high and the averaging code has to change

struct ArduStation {
  float lat;
  float lon;
  float alt;
  int encoderval;
  int batt_sum;
  int batt_sample[BATTSAMPLES]; // Average over last BATTSAMPLES readings
  uint8_t batt_i; // Which index of batt did we last write to
};

ArduStation ASM;

// Initialize default values (this will later be in the construction section of the class)
void
init_batt() {
  ASM.batt_i=0;
  for (uint8_t i=0;i<BATTSAMPLES;i++)
    ASM.batt_sample[i] = 0;
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

  v /= BATTSAMPLES;
  v *= 0.0048828125;
  return v;
}

