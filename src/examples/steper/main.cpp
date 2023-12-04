// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!
#include <Arduino.h>
#include "AFMotor.h"

// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #2 (M3 and M4)
AF_Stepper motor_steper(48, 2);

AF_DCMotor motor(2);
AF_DCMotor motor_one(1);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  motor_steper.setSpeed(500);  // 10 rpm   
  motor.setSpeed(200);
  motor_one.setSpeed(200);
 
  motor.run(RELEASE);
  motor_one.run(RELEASE);
}

void loop() {
  Serial.println("Single coil steps");
  motor_steper.step(300, FORWARD, SINGLE); 
  motor_steper.step(300, BACKWARD, SINGLE); 

//   Serial.println("Double coil steps");
//   motor.step(100, FORWARD, DOUBLE); 
//   motor.step(100, BACKWARD, DOUBLE);

//   Serial.println("Interleave coil steps");
//   motor.step(100, FORWARD, INTERLEAVE); 
//   motor.step(100, BACKWARD, INTERLEAVE); 

//   Serial.println("Micrsostep steps");
//   motor.step(100, FORWARD, MICROSTEP); 
//   motor.step(100, BACKWARD, MICROSTEP); 

uint8_t i;
  
  Serial.print("tick");
  
  motor.run(FORWARD);
  motor_one.run(FORWARD);
 

}


 
 