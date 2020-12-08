/*
 Entrada a Serial
 
 Este ejemplo demuestra el uso de pinMode(INPUT_PULLUP). Esto  lee un entrada digital y envia su estado por el puerto serie.
 
 El circuito: 
 * Pulsador en pin 2 y negetivo(ground) 
 * LED en pin 13 y negativo(ground)
 
 Utilizando pinMode(INPUT), no llevará resistencia a vcc interna.   Si utilizamos pinMode(INPUT_PULLUP) el pin asignado a esa entrada llevará un resistencia a +3,3v internamente, haciendo que cuendo esa entrada este sin conexión el estado leido sea "1" (HIGH). 
 
 creado 14 Marzo 2012
 por Scott Fitzgerald
 modificado 27 Abril 2012
 por Robert Wessels
 
 
 Este código es de dominio público
 
 */
const int PUSH2 = 2;
const int RED_LED = 13;

void setup(){
  //Configura el puerto serie a 9600 baudios
  Serial.begin(9600);
  //Configura el pulsador como entrada y con resistencia a Vcc y         //LED como salida
  pinMode(PUSH2, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT); 

}

void loop(){
  //Lee el valor de la entrada del pulsador a una variable
  int sensorVal = digitalRead(PUSH2);
  //Envia el valor al puerto serie
  Serial.println(sensorVal);
  
  // Acuerdate que la lógica del pulsador es invertida
  // Cuando el pulsador este pulsado la entrada se leerá como   //nivel bajo (LOW) y si no esta pulsado a nivel alto (HIGH)
  
  if (sensorVal == HIGH) {
    digitalWrite(RED_LED, LOW);
  } 
  else {
    digitalWrite(RED_LED, HIGH);
  }
}



