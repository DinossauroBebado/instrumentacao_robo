
#include "mpu.h"
#include "config.h"
#include "driver.h"

Motor rightMotor = Motor(bitMotor3A, bitMotor3B, pinMotor3PWM);
Motor leftMotor = Motor(bitMotor4A, bitMotor4B, pinMotor4PWM);

#include "cinematic.h"

#include <dht.h> 
dht DHT; 


// #include <SoftwareSerial.h>
// SoftwareSerial mySerial(RX_BLU, TX_BLU); // RX, TX  
// String command = ""; // Stores response of bluetooth device  
//             // which simply allows n between each  
//             // response.  


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



void setup(){

  pinMode(pinBuzzer,OUTPUT);

  // digitalWrite(pinBuzzer,HIGH);
  
  // digitalWrite(pinBuzzer,LOW);
  

  Serial.begin(9600);
  // mySerial.begin(9600); 

  // check that the IMU initializes correctly
  _imu_connect = imu_setup();

  if(_imu_connect == 0) {
   
    digitalWrite(13, HIGH); 
  }

  
}


void loop(){




  float* imu_ypr = imu_get_ypr();  
  //retunr from + pi to -pi 
  int readData = DHT.read11(DHT_PIN);

	float t = DHT.temperature;        // Read temperature
	float h = DHT.humidity;           // Read humidity

  error_angular = SET_POINT - imu_ypr[0] ; 
  kp = error_angular*KP; 
  ki = ki + error_angular*KI;

  pid = kp ;

  linear = 8;
  angular = pid; 

  left_speed = cinematic_left(linear,angular); 
  right_speed = cinematic_right(linear,angular); 




  leftMotor.drive(left_speed);
  rightMotor.drive(right_speed);


  Serial.print("Orientation ");
  Serial.print(imu_ypr[0], 5);
  Serial.print(", ");
  Serial.print(imu_ypr[1],5);
  Serial.print(", ");
  Serial.print(imu_ypr[2],5);

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

  Serial.println(t);
  // String message = (String) ""+h+","+t+"";
  // String message = (String) ""+t+"";
  
  // mySerial.println(message);

  delay(100);
 
  
}   