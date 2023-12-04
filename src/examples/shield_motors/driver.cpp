#include "driver.h"
#include <Arduino.h>



#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  7  //Pino Latch  DIR_LATCH
#define pinDS     8  //Pino Data   DIR_SER
// #define pinEnable   //Pino Enable DIR_EN

#define qtdeCI   1

#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4
#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6 

int inA1_s = bitMotor1A; // input 1 of the stepper
int inA2_s = bitMotor1B; // input 2 of the stepper
int inB1_s = bitMotor2A; // input 3 of the stepper
int inB2_s = bitMotor2B; // input 4 of the stepper

int stepDelay = 25; // Delay between steps in milliseconds




void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  
  ci74HC595Write(pinST_CP, LOW); //Inicia a Transmissão
  
  ci74HC595Write(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  ci74HC595Write(pinSH_CP, LOW);

  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
  
          ci74HC595Write(pinSH_CP, LOW);  //Baixa o Clock      
          
          ci74HC595Write(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
          
          ci74HC595Write(pinSH_CP, HIGH); //Eleva o Clock
          ci74HC595Write(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  
  ci74HC595Write(pinST_CP, HIGH);  //Finaliza a Transmissão
}

void step1() {
  Serial.print("step1");
  ci74HC595Write(inA1_s, LOW);   
  ci74HC595Write(inA2_s, HIGH);   
  ci74HC595Write(inB1_s, HIGH);   
  ci74HC595Write(inB2_s, LOW);   
  delay(stepDelay);
  
}
void step2() {
   Serial.print("step2");
  ci74HC595Write(inA1_s, LOW);   
  ci74HC595Write(inA2_s, HIGH);   
  ci74HC595Write(inB1_s, LOW);   
  ci74HC595Write(inB2_s, HIGH);   
  delay(stepDelay);
}
void step3() {
  Serial.print("step3");
  ci74HC595Write(inA1_s, HIGH);   
  ci74HC595Write(inA2_s, LOW);   
  ci74HC595Write(inB1_s, LOW);   
  ci74HC595Write(inB2_s, HIGH);   
  delay(stepDelay);
}
void step4() {
  Serial.print("step4");
  ci74HC595Write(inA1_s, HIGH);   
  ci74HC595Write(inA2_s, LOW);   
  ci74HC595Write(inB1_s, HIGH);   
  ci74HC595Write(inB2_s, LOW);   
  delay(stepDelay);
}
void stopMotor() {
  Serial.print("stop");
  ci74HC595Write(inA1_s, LOW);   
  ci74HC595Write(inA2_s, LOW);   
  ci74HC595Write(inB1_s, LOW);   
  ci74HC595Write(inB2_s, LOW);   
}

Motor::Motor(int IN1pin, int IN2pin, int PWMpin)
{
  IN1 = IN1pin;
  IN2 = IN2pin;
  PWM = PWMpin;
 
  pinMode(PWM, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  // pinMode(pinEnable, OUTPUT);
  pinMode(pinDS, OUTPUT);

  // ci74HC595Write(pinEnable, LOW);


  
}



void Motor::drive(int speed) {

  if(speed > 1000)
      speed = 1000;

  if(speed < -1000)
      speed = -1000; 

  speed = map(speed,-1000,1000,-255,255);

  if(speed > 0) {
    ci74HC595Write(IN1, HIGH);
    ci74HC595Write(IN2, LOW);
  }
  else if(speed < 0) {
    ci74HC595Write(IN1, LOW);
    ci74HC595Write(IN2, HIGH);
  }
  else {
    ci74HC595Write(IN1, LOW);
    ci74HC595Write(IN2, LOW);
  }


 analogWrite(PWM,abs(speed));
}

void Motor::steper(){
  for (int i=0; i<=11; i++){ 
    step1(); 
    step2();
    step3();
    step4();
  }
  stopMotor();
  delay(2000);
  
  for (int i=0; i<=11; i++){
    step3();
    step2();
    step1();
    step4(); 
  }
  
  stopMotor();
  delay(2000);
}




                                   



