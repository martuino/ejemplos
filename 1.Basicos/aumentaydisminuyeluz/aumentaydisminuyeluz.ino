/*
  Aumenta y disminuye lumisonidad
 
  Este ejemplo muestra como varia la luminosidad de un LED
  usando la funci�n analogWrite().
 
  Hardware requerido:
  * Martuino
  
  Este c�digo es de dominio p�blico.
 
*/

int brightness = 0;    // valor del brillo
int fadeAmount = 5;    // cantidad de saltos
int RED_LED = 3;

void setup()  { 
  // declaraci�n del PIN 3 como salida:
  pinMode(RED_LED, OUTPUT);
} 

void loop()  { 
  // pon el valor del brillo al pin 3:
  analogWrite(RED_LED, brightness);    

  // cambia el brillo sumandole el salto:
  brightness = brightness + fadeAmount;

  // cambia de direcci�n (ascendente o descendente): 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ; 
  }     
  // espera 30 milisegundos para ver el efecto    
  delay(30);                            
}
