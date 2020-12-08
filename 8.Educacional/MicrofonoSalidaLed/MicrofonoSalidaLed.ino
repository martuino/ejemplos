/*
  Experimento educativo
  
  Microfono a una entrada análogica, LED en una salida, puerto serie como salida
  
 Lee una entrada análogica, crea un resultado entre 0 y 255
 y usa el resultado para salir por el pin de salida una señal PWM.
 Mostramos el resultado por el puerto serie.
 
 EL circuito:
 * Microfono (analogico) al pin A0 y negativo.
 * LED conectado al pin 3 y negativo.
 
 creado 29 Dec. 2008
 modificado 30 Agosto 2011
 por Tom Igoe
 modificado 03 Dic 2013
 por Dung Dang 
 
 Este ejemplo es de dominio público.
 
 */


// Pines usados:
const int analogInPin = A0;  
const int analogOutPin = 03; 

int sensorValue = 0;        // valor leido en el potenciometro
int outputValue = 0;        // valor de salida PWM

void setup() {

  // inicializa comunicaciones serie a 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // Lee elalor análogico:
  sensorValue = analogRead(analogInPin);            
  // Ajusta el valor leido:
  outputValue = map(sensorValue, 0, 4096, 0, 255);  
  // Cambia el valor para la salida:
  analogWrite(analogOutPin, outputValue);           

  // muestra el resultado por el puerto serie:
  Serial.print("microfono = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t salida = ");      
  Serial.println(outputValue);   

  // Espera 10 milisegundos antes del proximo loop
 
  delay(10);                     
}
