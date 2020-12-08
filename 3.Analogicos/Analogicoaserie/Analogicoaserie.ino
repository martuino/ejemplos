/*
  Entrada Analogica, Salida analogica, Salida puerto serie
 
 Lee la entrada analogica, convierte el rango enre 0 y 255
  y utiliza el resultado para crear un PWM en la saldia.
 Tambien lo muestra por el puerto serie.
 
 EL circuito:
* potenciometro conectado al pin A0
   El pin del centro del potenciometro va a la entrada analogica.
   Los extremos del potenciometro a negativo y a +1,8v
 * El LED conectado al pin 3 y negativo (ground)
 
 creado 29 Dic. 2008
 modificado 30 Ago 2011
 por Tom Igoe
 
 Este ejemplo es de dominio público.
 
 */

// Estas constantes no deben de cambiarse:
const int analogInPin = 16;  // Entrada analogica 0
const int analogOutPin = 3; // Salida analogica -> LED

int sensorValue = 0;        // valor leido del potenciometro
int outputValue = 0;        // valor salidas PWM

void setup() {
  // inicializa el puerto serie 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // Lee el valor analogico:
  sensorValue = analogRead(analogInPin);            
  // convierte el valor analogico a valores 0-255:
  outputValue = map(sensorValue, 0, 1023, 0, 255);  
  // cambia el valor en la salida:
  analogWrite(analogOutPin, outputValue);           

  // Muestra el valor por el puerto serie:
  Serial.print("sensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t Salida = ");      
  Serial.println(outputValue);   

  // espera 10 ms antes del proximo loop
  // para que el convertidor analogico-digital pueda hacer
  // otra lectura.
  delay(10);                     
}
