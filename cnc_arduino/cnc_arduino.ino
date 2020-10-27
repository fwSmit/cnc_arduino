#include <AccelStepper.h> // Stepper driver library
#include <math.h> // Wiskundige functies 
// Dit zijn de pinnen die de Arduino gebruikt om de stappenmotoren aan te sturen
#define XSTEP 2
#define YSTEP 3
#define ZSTEP 4
#define XDIR 5
#define YDIR 6
#define ZDIR 7
#define ENABLEPIN 8
#define SPN_DIR 13 // Dit is de onboard led en de spin dir pin op de shield
#define STEPS_FULL (float)200
#define N_MICROSTEPS (float)8
#define N_AXES 3

AccelStepper stepperX(1, XSTEP, XDIR); // initialiseren van de stapper op poort x
AccelStepper stepperY(1, YSTEP, YDIR); // initialiseren van de stapper op poort y
AccelStepper stepperZ(1, ZSTEP, ZDIR); // initialiseren van de stapper op poort z


float steps_per_rotation = STEPS_FULL * N_MICROSTEPS;
float spoed = 8.; // [mm]
float distance_per_rotation[N_AXES] = {spoed, spoed, spoed * 16. / 24. }; // [mm]
Vec3 speling = {0.5, 0.5, 0.5}
int stepper_speed = 10000;
int stepper_acceleration = 1000;

struct Vec3 {
  float val[N_AXES];
};

Vec3 boundary_negative = {0, 0, 0}; // [mm]
Vec3 boundary_positive = {210, 148, 100}; // [mm]

void setup()
{
  pinMode(SPN_DIR, OUTPUT);
  pinMode(ENABLEPIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Start Serial");

  digitalWrite(ENABLEPIN, LOW); // LOW= de motoren zijn actief. HIGH= de motoren

  stepperX.setSpeed(stepper_speed);
  stepperY.setSpeed(stepper_speed);
  stepperZ.setSpeed(stepper_speed);

  stepperX.setAcceleration(stepper_acceleration);
  stepperY.setAcceleration(stepper_acceleration);
  stepperZ.setAcceleration(stepper_acceleration);
}

Vec3 pos_to_steps(Vec3 positions) {
  Vec3 result;
  for (int i = 0; i < N_AXES; i++) {
    result.val[i] = positions.val[i] / distance_per_rotation[i] * steps_per_rotation;
  }
  return result;
}

bool moveXY(Vec3 pos_mm) {
  // check if in bounds
  if (pos_mm.val[0] > boundary_positive.val[0] ||
      pos_mm.val[0] < boundary_negative.val[0] ||
      pos_mm.val[1] > boundary_positive.val[1] ||
      pos_mm.val[1] < boundary_negative.val[1])
  {
    return false;
  }
  Vec3 steps = pos_to_steps(pos_mm);
  stepperX.moveTo(steps.val[0]);
  stepperY.moveTo(steps.val[1]);
  while (stepperX.isRunning() || stepperY.isRunning()) {
    stepperX.run();
    stepperY.run();
  }
  return true;
}
bool moveZ(Vec3 pos_mm) {
  return true;
  // check if in bounds
  if (pos_mm.val[2] > boundary_positive.val[2] ||
      pos_mm.val[2] < boundary_negative.val[2])
  {
    return false;
  }
  Vec3 steps = pos_to_steps(pos_mm);
  stepperZ.moveTo(steps.val[2]);
  while (stepperZ.isRunning()) {
    stepperZ.run();
  }
  return true;
}

void loop()
{
  const int z_rest_height = 0;
  const int z_cut_height = 20;
  const int n_positions = 3;
  const int move_rest = 1000;
  const int cut_down_rest = 100;
  const int cut_up_rest = 0;
  const int small_rest = 100;
  bool res;
  Vec3 start_pos = {0, 0, z_rest_height+speling.val[2]};
  Vec3 drill_down = {0, 0, z_cut_height+speling.val[2]};
  Vec3 drill_up = {0, 0, z_rest_height+speling.val[2]};
  Vec3 positions[n_positions] = {
    {150.+speling.val[0], 20.+speling.val[1], 0.},
    {150.+speling.val[0], 120.+speling.val[1], 0.},
    {50., 120.+speling.val[1], 0.}
  };
  res = moveZ(start_pos);
  if (!res) {
    Serial.println("ABORT!!!!");
    return;
  }

  res = moveXY(start_pos);
  if (!res) {
    Serial.println("ABORT!!!!");
    return;
  }
  digitalWrite(SPN_DIR, HIGH); // Turn on the drill
  delay(1000);
  for (int i = 0; i < n_positions; i++) {
    res = moveXY(positions[i]);
    if (!res) {
      Serial.println("ABORT!!!!");
      return;
    }
    delay(1000);
    res = moveZ(drill_down);
    if (!res) {
      Serial.println("ABORT!!!!");
      return;
    }
    delay(1000);
    res = moveZ(drill_up);
    if (!res) {
      Serial.println("ABORT!!!!");
      return;
    }
    delay(1000);

  }

  moveXY(start_pos);

  Serial.println("FINISHED");
  delay(10000);
}
