/*
  Leyendo un ASCII-encoded por el puerto serie.

  En serial monitor selecciona Newline cada vez que pulsas send
  Pon un numero entre 0-255 y pulsa send
  Este ejemplo es de dominio publico.
*/

// pin para led:
const int redPin = 3;


void setup() {
 // inicializa serie:
 Serial.begin(9600);
 // pon los pines de salida:
 pinMode(redPin, OUTPUT); 
 Serial.println("Intro numero:");

}

void loop() {
// si el puerto serie esta habilitado, leelo:
while (Serial.available() > 0) {

  // mira si es un numero valido:
  int red = Serial.parseInt(); 


  // mira si es nueva linea (retorno de carro):
  if (Serial.read() == '\n') {
  // contiene un valor entre 0 - 255 
  // Según este valor iluminamos el LED
  red = constrain(red, 0, 255);
  

  // variamos la luminosidad del LEDLED: 
  analogWrite(redPin, red);
 

  // Muestra el numero en Hexadecimal por el serie:
  Serial.print(red, HEX);
  Serial.println("");
 
  }
 }
}
