#include <Arduino.h>

/*  
  Feito pelo site MundoProjetado
  Código que toca um pedaço da música
  de abertura de Game of Thrones
*/
void setup(){
  pinMode(10,OUTPUT);
}

void loop(){
  /*
  Frequência das notas:
  Dó - 262 Hz
  Ré - 294 Hz
  Mi - 330 Hz
  Fá - 349 Hz
  Sol - 392 Hz
  Lá - 440 Hz
  Si - 494 Hz
  #Dó - 528 Hz
  */
  delay(1000);
  int tempo = 400;
  tone(10,440,tempo); //LA
  delay(tempo);
  tone(10,294,tempo); //RE
  delay(tempo);
  tone(10,349,tempo/2); //FA - O tempo/2 faz com que demore metade do valor estipulado anteriormente, pois essa parte é mais rápida
  delay(tempo/2);
  tone(10,392,tempo/2); //SOL
  delay(tempo/2);
  tone(10,440,tempo); //LA
  delay(tempo);
  tone(10,294,tempo); //RE
  delay(tempo);
  tone(10,349,tempo/2); //FA
  delay(tempo/2);
  tone(10,392,tempo/2); //SOL
  delay(tempo/2);
  tone(10,330,tempo); //MI
}