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
#define DEGREES (float)360
#define N_AXES 3

#define N_MICROSTEPS 8
AccelStepper stepperX(1, XSTEP, XDIR); // initialiseren van de stapper op poort x
AccelStepper stepperY(1, YSTEP, YDIR); // initialiseren van de stapper op poort y
AccelStepper stepperZ(1, ZSTEP, ZDIR); // initialiseren van de stapper op poort z

//MultiStepper steppers;

float steps_per_rotation = STEPS_FULL * N_MICROSTEPS;
float spoed = 8.; // [mm]
float distance_per_rotation[N_AXES] = {spoed, spoed, spoed * 16. / 24. }; // [mm]
int stepper_speed = 10000;
int stepper_acceleration = 1000;

struct Vec3 {
  float val[N_AXES];
};

Vec3 boundary_negative = {0, 0, 0};
Vec3 boundary_positive = {210, 148, 100};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ENABLEPIN, OUTPUT); // en ENABLEPIN(8) op output.

  Serial.begin(9600);
  Serial.println("Start Serial");

  digitalWrite(ENABLEPIN, LOW); // LOW= de motoren zijn actief. HIGH= de motoren

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

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
    //return false;
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

void loop()
{
  Vec3 pos = {0, 45, 0};

  moveXY(pos);
  

  Serial.println("FINISHED");
  delay(10000);
}
