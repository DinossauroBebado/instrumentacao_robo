#include <AFMotor.h>
#include <Arduino.h>

AF_DCMotor motor_left(3);//7,6,5
AF_DCMotor motor_right(4);

void setup() 
{
  //Seta a velocidade inicial do motor e o para
  motor_left.setSpeed(200);
  motor_left.run(RELEASE);

  motor_right.setSpeed(200);
  motor_right.run(RELEASE);
}

void loop() 
{
  uint8_t i;

  // Liga o motor

  
  // Acelera de zero à velocidade máxima
  for (i=0; i<255; i++) 
  {
    motor_left.setSpeed(i); 
    motor_right.setSpeed(i); 
    motor_left.run(FORWARD);
    motor_right.run(BACKWARD);
    delay(10);
  }
  
  // Desacelera da velocidade máxima para zero
  for (i=255; i!=0; i--) 
  {
    motor_left.setSpeed(i);  
    motor_right.setSpeed(i);  
    motor_left.run(BACKWARD);
    motor_right.run(BACKWARD);
    delay(10);
  }

  // Muda direção do motor
  
  
  // Acelera de zero à velocidade máxima
  for (i=0; i<255; i++) 
  {
    motor_left.setSpeed(i);  
    motor_right.setSpeed(i);  
    motor_left.run(BACKWARD);
    motor_right.run(BACKWARD);
    delay(10);
  }

  // Desacelera da velocidade máxima para zero
  for (i=255; i!=0; i--) 
  {
    motor_left.setSpeed(i); 
    motor_right.setSpeed(i); 
    motor_left.run(FORWARD);
    motor_right.run(FORWARD);
    delay(10);
  }

  // Desliga o motor
  motor_left.run(RELEASE);
  motor_right.run(RELEASE);
  delay(1000);
}