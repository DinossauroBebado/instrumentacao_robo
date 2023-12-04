#include <Arduino.h>
#include "driver.h"

#define pinMotor3PWM 5
#define pinMotor4PWM 6
#define pinMotor2PWM 3 
#define pinMotor1PWM 9


#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4

#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6




Motor rightMotor = Motor(bitMotor3A, bitMotor3B, pinMotor3PWM);
Motor leftMotor = Motor(bitMotor4A, bitMotor4B, pinMotor4PWM);

void setup() {

  Serial.begin(9600);

}

void loop() {

  // //PWM for the right motor
  // for(int i=0; i< 1000; i++) {
  //   rightMotor.drive(i);
  //   leftMotor.drive(i);
  //   Serial.println(i);
  //   delay(10);
  // }

  // for(int j = 1000; j > - 1000; j--) {
  //   rightMotor.drive(j);
  //   leftMotor.drive(j);
  //   Serial.println(j);
  //   delay(10);
  // }

  // for(int k = -1000; k < 0; k++) {
  //   rightMotor.drive(k);
  //   leftMotor.drive(k);
  //   Serial.println(k);
  //   delay(10);
  // }

  rightMotor.steper();



}