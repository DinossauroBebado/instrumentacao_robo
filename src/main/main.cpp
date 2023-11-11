
#include "config.h"
#include "driver.h"
#include "cinematic.h"
#include <dht.h> 
#include <SoftwareSerial.h>
#include "filter.h"

Motor rightMotor = Motor(bitMotor3A, bitMotor3B, pinMotor3PWM);
Motor leftMotor = Motor(bitMotor4A, bitMotor4B, pinMotor4PWM);

dht DHT; 



// #include "mpu.h"

SoftwareSerial mySerial(RX_BLU, TX_BLU); // RX, TX  
String command = ""; // Stores response of bluetooth device  
            // which simply allows n between each  
            // response.  


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


  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(LINE_PIN,INPUT);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.



  // check that the IMU initializes correctly
  // _imu_connect = imu_setup();


  
}


void loop(){




  // float* imu_ypr = imu_get_ypr();  
  // float theta = imu_ypr[0];
  // //retunr from + pi to -pi 
  int readData = DHT.read11(DHT_PIN);


  int ground = analogRead(LINE_PIN);
  mode = define_mode(ground);


	float t = DHT.temperature;        // Read temperature
	// float h = DHT.humidity;           // Read humidity

 

  linear = 8;
  angular = 0; 
  // angular = pid; 
  // true => auto / false => manual 
  if(mode){
    //auto

    // error_angular = SET_POINT - theta ; 
    // kp = error_angular*KP; 
    // ki = ki + error_angular*KI;

    // pid = kp ;
    linear = 8;
    angular = pid;

  }else{


    linear = 0;
    angular = 0; 

  }




  left_speed = cinematic_left(linear,angular); 
  right_speed = cinematic_right(linear,angular); 


  leftMotor.drive(left_speed);
  rightMotor.drive(right_speed);


  // Serial.print("Orientation ");
  // Serial.print(imu_ypr[0], 5);
  // Serial.print(", ");
  // Serial.print(imu_ypr[1],5);
  // Serial.print(", ");
  // Serial.print(imu_ypr[2],5);

  // Serial.print(" PID|error:  ");
  // Serial.print(error_angular);
  // Serial.print(" kp: ");
  // Serial.print(kp);
  // Serial.print(" pid: ");
  // Serial.print(pid);

  mySerial.print(" Speed_L: ");
  mySerial.print(left_speed);
  mySerial.print(" Speed_R: ");
  mySerial.print(right_speed);
 
  mySerial.print(" T: ");
  mySerial.print(t);
  mySerial.print(" L :");
  mySerial.print(ground);
  mySerial.print(" MODE: ");
  mySerial.print(mode ? "AUTO":"MANUAL");
  mySerial.println("");
  // String message = (String) ""+h+","+t+"";
  // String message = (String) ""+t+"";
  
  // mySerial.println(message);

  delay(100);
  
 
  
}   