#include "driver.h"
#include <Arduino.h>



#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  7  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER


#define qtdeCI   1

#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4
#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6 

void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
  
          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
          
          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
          
          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
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
  pinMode(pinDS, OUTPUT);



  
}



void Motor::drive(int speed) {

  if(speed > 255)
      speed = 255;

  if(speed < -255)
      speed = -255; 

  speed = map(speed,-255,255,-255,255);

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