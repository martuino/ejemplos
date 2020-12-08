/*
  test  de Multiples serial
 

  Recibe por el puerto serie de los pines 10 y 11 y lo envias al puerto serie principal (USB).
 
  El circuito: 
  * Conectar un convertidor de niveles de 3,3v a RS232 en pines 0 y 1
  * Puerto serie en USB:
 
  creado 30 Dic. 2008
  por Tom Igoe
 
  Este ejemplo es de dominio publico.
 
*/


void setup() {
  // inicializa ambos puertos serie:
  Serial.begin(9600); //USB 
  Serial1.begin(9600); //Pines 10 y 11
}

void loop() {
  // Lee del puerto  1, envia por el puerto 0:
  if (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte); 
  }
}
