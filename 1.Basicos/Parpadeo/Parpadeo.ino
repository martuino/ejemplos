/*
  Parpadeo
  Enciende el LED 1 segundo y apagalo otro segundo, 	repetitivamente.
    
  Hardware Requerido:
  * Martuino
  
  Este código es de dominio público.
*/

// Pin del LED
int LED = 13;

  
// Aqui empieza después de un reset:
void setup() {                
  // Inicializa el LED como salida.
  pinMode(LED, OUTPUT);     
}

// La rutina loop() es un bucle repetitivo
void loop() {
  digitalWrite(LED, HIGH);   // Enciende el LED(HIGH = 1)
  delay(1000);               // Espera 1 segundo
  digitalWrite(LED, LOW);    // Apaga LED (LOW=0)
  delay(1000);               // Espera 1 segundo
}
