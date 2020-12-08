/*
 Aumento y disminucion de luz
 Este ejemplo muestra como modificar la luminosidad de un led
 
 El circuito:
 * LED al pin 03 y negativo (ground).
 
 Creado 1 Nov 2008
 por David A. Mellis
 modificado 30 Ago 2011
 por Tom Igoe
 
 Este ejemplo es de dominio publico.
 
 */


int ledPin = 3;    // LED conectado al pin 3

void setup()  { 
  // nada que hacer aqui
} 

void loop()  { 
  // variamos el fadevalue de 5 en 5 puntos:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // escribe el valor (rango desde 0-255):
    analogWrite(ledPin, fadeValue);         
    // espera 30 milisegundos para ver el efecto variable    
    delay(30);                            
  } 

  // Cuando el fadevalue llegue al maximo o minimo:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // escribe el valor (rango desde 0-255):
    analogWrite(ledPin, fadeValue);         
    // espera 30 milisegundos para ver el efecto de luminosidad    
    delay(30);                            
  } 
}


