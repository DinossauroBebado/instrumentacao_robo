
#include "config.h"
// #include "driver.h"
#include "cinematic.h"
#include <dht.h> 
#include <SoftwareSerial.h>
#include "filter.h"

#include "mpu.h"

#include <SPI.h> 
#include <MFRC522.h> 

MFRC522 rfid(SS_PIN, RST_PIN); //PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS


dht DHT; 

#include "AFMotor.h"
// to motor port #2 (M3 and M4)
AF_Stepper motor(48, 2);
AF_DCMotor motor_right(2);
AF_DCMotor motor_left(1);

#include "Ultrasonic.h" //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO
Ultrasonic ultrasonic(TRIG_PIN,ECHO_PIN); //INICIALIZANDO OS PINOS DO ARDUINO
int distancia = -1 ; //VARIÁVEL DO TIPO INTEIRO
//MÉTODO RESPONSÁVEL POR CALCULAR A DISTÂNCIA

void Write_speed(int speed,AF_DCMotor motor){
  if(speed>0){
    motor.run(FORWARD);
  }
// void Write_speed(int speed,AF_DCMotor motor){
//   if(speed>0){
//     motor.run(FORWARD);
//   }

  if(speed<0){
    motor.run(BACKWARD);
  }
//   if(speed<0){
//     motor.run(BACKWARD);
//   }

  if(abs(speed)>MAX_VALUE){
    speed = MAX_VALUE;
  }
//   if(abs(speed)>MAX_VALUE){
//     speed = MAX_VALUE;
//   }

  motor.setSpeed(speed);
//   motor.setSpeed(speed);

}
// }

void hcsr04(){
    digitalWrite(TRIG_PIN, LOW); //SETA O PINO 6 COM UM PULSO BAIXO "LOW"
    delayMicroseconds(2); //INTERVALO DE 2 MICROSSEGUNDOS
    digitalWrite(TRIG_PIN, HIGH); //SETA O PINO 6 COM PULSO ALTO "HIGH"
    delayMicroseconds(10); //INTERVALO DE 10 MICROSSEGUNDOS
    digitalWrite(TRIG_PIN, LOW); //SETA O PINO 6 COM PULSO BAIXO "LOW" NOVAMENTE
    //FUNÇÃO RANGING, FAZ A CONVERSÃO DO TEMPO DE
    //RESPOSTA DO ECHO EM CENTIMETROS, E ARMAZENA
    //NA VARIAVEL "distancia"
    distancia = (ultrasonic.Ranging(CM)); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA MEDIDA
    delay(5); //INTERVALO DE 5 MILISSEGUNDOS
 }

// SoftwareSerial blu(RX_BLU, TX_BLU); // RX, TX  
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
bool card_read_flag = false;

String message = String(10);



// ----------- ultrasonic sensor - distance




void setup(){


  Serial.begin(9600);
  // blu.begin(9600);

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
  motor.setSpeed(500);  // 500 rpm  
  motor_left.setSpeed(100);
  motor_right.setSpeed(100); 
  _connect = imu_setup();

  
  pinMode(LINE_PIN,INPUT);

  Serial.println("");
  delay(100);
}


void loop(){


  float* imu_ypr = imu_get_ypr();
  float theta = imu_ypr[0];

  // //retunr from + pi to -pi 
  int readData = DHT.read11(DHT_PIN);

   
    
    

  card_read_flag = rfid.PICC_IsNewCardPresent();

  int ground = analogRead(LINE_PIN);
  mode = define_mode(ground);


	float t = DHT.temperature;        // Read temperature
	// float h = DHT.humidity;           // Read humidity

 

  // angular = pid; 
  // true => auto / false => manual 
  if(mode){
    //auto
    hcsr04(); //atualiza a distancia 
    motor.step(300, FORWARD, SINGLE); //movimento horario do sensor de distancia 
    motor.step(300, BACKWARD, SINGLE); //movimento ant-horario do sensor de distancia 
    
    if(card_read_flag){
      //busca finalizada 
      //executar meia volta e ir para o proximo objetivo 
    }

    error_angular = SET_POINT - theta ; 
    kp = error_angular*KP; 
    ki = ki + error_angular*KI;

    // pid = kp ;
    linear = 4;
    // angular = pid;
    angular = 0;

  }else{
    //manual 
    // if(blu.available()){
    //     // x2.71x7.61x
    //     message = blu.readString();
    //     Serial.print(message);
    //     // Serial.println("  ");
    //     message.trim();
    //     linear = message.substring(1, 4).toFloat();
    //     angular = message.substring(5, 8).toFloat();
    
    // }
     linear = 0;
    // angular = pid;
    angular = 0;


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
  Serial.print(card_read_flag ? "card":"no card");
  Serial.print(" Dist: ");
  Serial.print(distancia);
  Serial.print(" ");
  Serial.print( "theta");
  Serial.print(theta);
  Serial.print(" PID ");
  Serial.print(pid);
  Serial.println("");



  // blu.print(" Speed_L: ");
  // blu.print(left_speed);
  // blu.print(" Speed_R: ");
  // blu.print(right_speed);
 
  // blu.print(" T: ");
  // blu.print(t);
  // blu.print(" L :");
  // blu.print(ground);
  // blu.print(" MODE: ");
  // blu.print(mode ? "AUTO":"MANUAL");
  // blu.println("");
  // String message = (String) ""+h+","+t+"";
  // String message = (String) ""+t+"";
  
  // blu.println(message);

  delay(100);
  
 
  
}   