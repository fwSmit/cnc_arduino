#include <AccelStepper.h> // Stepper driver library  
#include <MultiStepper.h> // Multiple stepper driver library
#include <math.h> // Wiskundige functies 
// Dit zijn de pinnen die de Arduino gebruikt om de stappenmotoren aan te sturen
#define XSTEP 2
#define YSTEP 3
#define ZSTEP 4
#define XDIR 5
#define YDIR 6
#define ZDIR 7
#define ENABLEPIN 8
#define LEDPIN 13 // Dit is de onboard led  
#define STEPS (float)200
#define DEGREES (float)360

#define N_MICROSTEPS 16
AccelStepper stepperX(1, XSTEP, XDIR); // initialiseren van de stapper op poort x
AccelStepper stepperY(1, YSTEP, YDIR); // initialiseren van de stapper op poort y
AccelStepper stepperZ(1, ZSTEP, ZDIR); // initialiseren van de stapper op poort z

MultiStepper steppers;

bool acceleration = true;

void setup() // Deze routine wordt 1 keer gerund aan het begin van het programma
{
  // Met pinMode zet je een pin op input/output
  pinMode(LEDPIN, OUTPUT);  //Nu zetten we de LEDPIN(13)
  pinMode(ENABLEPIN, OUTPUT); // en ENABLEPIN(8) op output.
  // Met Serial kan je communiceren met de Serial monitor in Arduino IDE op je pc.
  Serial.begin(9600);  // Gebruik dit commando eenmalig om de verbinding te maken.
  Serial.println("hoi"); // Nu kan je Serial.print en Serial.println gebruiken.
  // Met digitalWrite kan je pinnen HIGH en LOW maken. Dit gebruiken we nu om de

  digitalWrite(ENABLEPIN, LOW); // LOW= de motoren zijn actief. HIGH= de motoren

  // Tot slot laten we de led kort aangaan om te zien dat void setup() klaar is.
  digitalWrite(13, HIGH);
  delay(1000); // Met delay kan je de Arduino X aantal miliseconden laten wachten.
  digitalWrite(13, LOW);
  // Hier stellen we een arbitraire snelheid en versnelling in als voorbeeld
  //  stepperX.setSpeed(400);
  //  stepperY.setSpeed(400);
  //  stepperZ.setSpeed(400);
  //stepperX.setMaxSpeed(5000);
  stepperX.setSpeed(5000);
  stepperY.setMaxSpeed(5000);
  stepperZ.setMaxSpeed(5000);

  stepperX.setAcceleration(1000);
  stepperY.setAcceleration(1000);
  stepperZ.setAcceleration(1000);

  // Add all steppers to multistepper
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperZ);

}

void loop()
{
 
  stepperX.move(10000);
  stepperY.move(10000);
  stepperZ.move(3000);
  while(stepperX.run()||stepperY.run()||stepperZ.run()){}
  delay(1000);
  stepperX.move(-10000);  
  stepperY.move(-10000);  
  stepperZ.move(-3000);  
  while(stepperX.run()||stepperY.run()||stepperZ.run()){}

  Serial.println("FINISHED");
  delay(10000);
}
