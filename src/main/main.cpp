
#include "mpu.h"
#include "config.h"
#include <AFMotor.h>
#include "cinematic.h"

AF_DCMotor motor_left(3);//7,6,5
AF_DCMotor motor_right(4);


const float SET_POINT = 0 ;
const float KP = 30;
const float KI = 0.1 ;

bool _imu_connect; 
bool _connect = false;
int left_speed, right_speed; 
float error_angular;
float kp,ki,kd,pid; 
float linear, angular;



// ----------- ultrasonic sensor - distance


void Write_speed(int speed,AF_DCMotor motor){
  if(speed>0){
    motor.run(FORWARD);
  }

  if(speed<0){
    motor.run(BACKWARD);
  }

  if(abs(speed)>MAX_VALUE){
    speed = MAX_VALUE;
  }

  motor.setSpeed(speed);

}

void setup(){

  Serial.begin(9600);

  // check that the IMU initializes correctly
  _imu_connect = imu_setup();

  if(_imu_connect == 0) {
   
    digitalWrite(13, HIGH); 
  }

 motor_left.setSpeed(200);
  motor_left.run(RELEASE);

  motor_right.setSpeed(200);
  motor_right.run(RELEASE);
}


void loop(){




  float* imu_ypr = imu_get_ypr();  
  //retunr from + pi to -pi 

  error_angular = SET_POINT - imu_ypr[0] ; 
  kp = error_angular*KP; 
  // ki = ki + error_angular*KI;

  pid = kp ;

  linear = 8;
  angular = pid; 

  left_speed = cinematic_left(linear,angular); 
  right_speed = cinematic_right(linear,angular); 




  Write_speed(left_speed,motor_left);
  Write_speed(right_speed,motor_right);

  Serial.print("Orientation ");
  Serial.print(imu_ypr[0], 5);
  // Serial.print(", ");
  // Serial.print(imu_ypr[1],5);
  // Serial.print(", ");
  // Serial.print(imu_ypr[2],5);

  Serial.print(" PID|error:  ");
  Serial.print(error_angular);
  Serial.print(" kp: ");
  Serial.print(kp);
  Serial.print(" pid: ");
  Serial.print(pid);

  Serial.print(" Speed_L: ");
  Serial.print(left_speed);
  Serial.print(" Speed_R: ");
  Serial.print(right_speed);
 
  Serial.println("");
 
  
}   