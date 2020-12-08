// Control de un servo
// por BARRAGAN <http://barraganstudio.com> 
// Modificado para Energia por <fughilli@gmail.com>
// Este ejemplo es de dominio publico.

#include <Servo.h> 
 
Servo myservo;  // crea un objeto servo 
                // un maximo de 8 servos pueden ser creados
 
int pos = 0;    // variable que almacena la posicion del servo 
 
void setup() 
{ 
  myservo.attach(3);  // Conecta el servo control al pin 3 
} 
 
 
void loop() 
{ 
  for(pos = 0; pos < 180; pos += 1)  // ves desde 0 a 180 grados
  {                                  // en pasos de 1 grado 
    myservo.write(pos);              
    delay(15);                       // espera 15ms para que alcanze la nueva posicion 
  } 
  for(pos = 180; pos>=1; pos-=1)     // ves desde 180 a 0 grados 
  {                                
    myservo.write(pos);              
    delay(15);                       // espera 15ms para que alcanze la nueva posicion 
  } 
} 
