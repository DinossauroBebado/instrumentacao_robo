#include "config.h"
#include <Arduino.h>



float cinematic_right(float linear, float angular){

  float right_speed = (linear + angular*L)/R ; 

 

  return right_speed; 

}

float cinematic_left(float linear, float  angular){

  float left_speed = (linear - angular*L)/R ;

  return left_speed; 

}