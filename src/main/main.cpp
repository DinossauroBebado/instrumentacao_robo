
#include "config.h"
// #include "driver.h"
#include "cinematic.h"
#include <dht.h> 
// #include <SoftwareSerial.h>
#include "filter.h"


#include <SPI.h> 
#include <MFRC522.h> 

MFRC522 rfid(SS_PIN, RST_PIN); //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS

#include <Wire.h>
#include "MPU6050.h"
MPU6050 mpu;

dht DHT; 

#include "AFMotor.h"
// to motor port #2 (M3 and M4)
AF_Stepper motor(48, 2);
AF_DCMotor motor_right(2);
AF_DCMotor motor_left(1);

int step = 1;
int last_step = 0;
int step_orientation = 1;
int step_converted = 0 ;


#include <Wire.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_GFX.h> //INCLUSÃO DE BIBLIOTECA
#include <Adafruit_SSD1306.h> //INCLUSÃO DE BIBLIOTECA
 
Adafruit_SSD1306 display = Adafruit_SSD1306(); //OBJETO DO TIPO Adafruit_SSD1306

#include "NewPing.h"

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

float distancia = -1 ; //VARIÁVEL DO TIPO INTEIRO
//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA

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



// SoftwareSerial blu(RX_BLU, TX_BLU); // RX, TX  
String command = ""; // Stores response of bluetooth device  
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
bool card_read_flag = false;

String message = String(10);



// ----------- ultrasonic sensor - distance




void setup(){


  Serial.begin(9600);
  // blu.begin(57600.);

   while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
  motor.setSpeed(500);  // 500 rpm  
  motor_left.setSpeed(100);
  motor_right.setSpeed(100); 

   // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  // mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  // mpu.setThreshold(3);
  

  
  pinMode(LINE_PIN,INPUT);
  pinMode(pinBuzzer,OUTPUT);
  digitalWrite(pinBuzzer,HIGH);

  Serial.println("");
  delay(100);

  Wire.begin(); //INICIALIZA A BIBLIOTECA WIRE
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //INICIALIZA O DISPLAY COM ENDEREÇO I2C 0x3C
  display.setTextColor(WHITE); //DEFINE A COR DO TEXTO
  display.setTextSize(2); //DEFINE O TAMANHO DA FONTE DO TEXTO
  display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
}


void loop(){

Vector rawGyro = mpu.readRawGyro();

  float theta = rawGyro.ZAxis;
  //retunr from + pi to -pi 


  if(Serial.available()){
        // x2.71x7.61x
        message = Serial.readString();
        // Serial.print(message);
        // Serial.println("  ");
        message.trim();
        Serial.flush();
  }


  DHT.read11(DHT_PIN);

   
    
    

  card_read_flag = rfid.PICC_IsNewCardPresent();

  int ground = analogRead(LINE_PIN);
  mode = define_mode(ground);


	float t =23.00;        // Read temperature


  distancia = sonar.ping_cm() ; 
 



  // angular = pid; 
  // true => auto / false => manual 
  if(mode){
     
      

  
      // step_converted = map(step,-5,5,0,40);


      // if(distancia>70){


      //   if(step > 0){
      //     motor.step(300,FORWARD, SINGLE) ; 
      //   }
      //   if(step < 0){
      //     motor.step(300,BACKWARD, SINGLE) ; 
      //   }


      //  step = -1*step;

      //  linear = 6;

      //  if(step>0){
      //   angular = 1 ;
      //  }
      
      // if(step < 0){
      //   angular = -1;
      // }


      // }else if (distancia >30 && distancia < 70)
      // {
      //   linear = 4;
      // }else if( distancia <30){

      //   linear = (distancia/9);
      // }
      linear = 2 ;
    
    // motor.step(300,FORWARD, SINGLE);

    
    if(card_read_flag==true){
      //busca finalizada 
      //executar meia volta e ir para o proximo objetivo 

      digitalWrite(pinBuzzer,LOW);
      Serial.println("---------------------------------------");
      angular = theta + 180;
      error_angular = SET_POINT - theta ; 
      kp = error_angular*KP; 
      angular = kp;
      linear = 4 ;

      left_speed = cinematic_left(linear,angular); 
      right_speed = cinematic_right(linear,angular); 
      Write_speed(left_speed,motor_left);
      Write_speed(right_speed,motor_right);
    }
      delay(200);
      digitalWrite(pinBuzzer,HIGH);

      left_speed = cinematic_left(linear,angular); 
      right_speed = cinematic_right(linear,angular); 

      Write_speed(left_speed,motor_left);
      Write_speed(right_speed,motor_right);


  }else{
    // manual 
    
        linear = 0;
        angular = 0 ;
        linear = message.substring(0, 4).toFloat();
        angular = message.substring(5, 8).toFloat();
  
  }




  left_speed = cinematic_left(linear,angular); 
  right_speed = cinematic_right(linear,angular); 


  Write_speed(left_speed,motor_left);
  Write_speed(right_speed,motor_right);

  //---PID------

  // Serial.print(" PID|error:  ");
  // Serial.print(error_angular);
  // Serial.print(" kp: ");
  // Serial.print(kp);
  // Serial.print(" pid: ");
  // Serial.print(pid);

  //---Motor---

  Serial.print(" Linear: ");
  Serial.print(linear);
  Serial.print(" Angular: ");
  Serial.print(angular);

  // Serial.print(" Speed_L: ");
  // Serial.print(left_speed);
  // Serial.print(" Speed_R: ");
  // Serial.print(right_speed);
  Serial.print(" ");
  Serial.print(ground);
  Serial.print(" MODE: ");
  Serial.print(mode ? "AUTO":"MANUAL");
  Serial.print(" card: ");
  Serial.print(card_read_flag );
  Serial.print(" Dist: ");
  Serial.print(distancia);
  Serial.print(" ");
  Serial.print( "theta");
  Serial.print(theta);
  Serial.print(" PID ");
  Serial.print(pid);
  Serial.print(" Temp: ");
  Serial.print(t);
  Serial.print(" ");
  Serial.print(message);

  // Serial.print(step);

  Serial.println("");

  // blu.print(" Speed_L: ");
  // blu.print(left_speed);

  // blu.print(" Speed_R: ");
  // blu.print(right_speed);
 
  // blu.print(" T: ");
  // blu.print(t);
  // blu.print(" L :");
  // blu.print(ground);
  // blu.println(" MODE: ");
  // blu.print(mode ? "AUTO":"MANUAL");
  // blu.println("");
  // String message = (String) ""+h+","+t+"";
  // String message = (String) ""+t+"";
  
  // blu.println(message);

  // delay(100);

  display.setCursor(0,0); //POSIÇÃO EM QUE O CURSOR IRÁ FAZER A ESCRITA
  display.print(distancia);
  display.println(" ");
  display.print(ground);
  display.print(" "); 
  display.print(t);
  display.display(); //EFETIVA A ESCRITA NO DISPLAY
  display.clearDisplay(); //LIMPA AS INFORMAÇÕES DO DISPLAY
   
}   