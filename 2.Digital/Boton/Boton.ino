/*
  Boton
 
 Enciende un LED cuando pulsas sobre el boton. 
 
 
 El circuito:
 * LED en pin 13 y negativo (ground)
 * Pulsador en pin 2 y +3,3V
 * Reistencia de 10K entre pin 2 y negativo (ground)
 
 
 creado en 2005
 por DojoDave <http://www.0j0.org>
 modificado 30 Agosto 2011
 por Tom Igoe
 modificado 27 Abril 2012
 por Robert Wessels
 
 Este código es de dominio público.

 */

// constantes que no deben cambiarse

const int buttonPin = 2;     // Pin donde se conecta el pulsador
const int ledPin =  13;      // Pin donde se conecta el LED

// variables que cambiarán:
int buttonState = 0;         // variable para leer el estado del 						  //pulsador

void setup() {
  // initializa el LED como salida:
  pinMode(ledPin, OUTPUT);      
  // initializa el pulsador como entrada:
  pinMode(buttonPin, INPUT_PULLUP);     
}

void loop(){
  // Lee el estado del pulsador:
  buttonState = digitalRead(buttonPin);

  // checkea si esta pulsado.
  // Si lo esta, El estado del pulsador es HIGH:
  if (buttonState == HIGH) {     
    // Enciende el LED:    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
    // Apaga el LED:
    digitalWrite(ledPin, LOW); 
  }
}
