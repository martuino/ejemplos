/*
  Event puerto serie

  Cuando un dato nuevo llega por el puerto serie, este programa lo añade a un string.
  Cuando una nueva linea es recibida, el loop lo muestra y lo borra.

  Un buen testeo para probar un receptor GPS que envia protocolo 
  NMEA 0183. 

  Creatdo 9 May 2011
  por Tom Igoe
  Modificado 24 Abril 2013
  por Sean Alvarado

  Hardware Required:
  * Martuino
  
  Este ejemplo es de dominio publico.

*/

String inputString = ""; //String donde se guardara datos recibidos
boolean stringComplete = false; //a true cuando string este completo

void setup() {
  // inicializa serie:
  Serial.begin(9600);
  // reserva 200 bytes para el inputString:
  inputString.reserve(200);
}

void loop() {
  // muestra cuando una nueva linea haya llegado:
  if (stringComplete) {
    Serial.println(inputString); 
    // Vacia el String:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent ocure cuando un nuevo dato llega al pin RX del puerto serie.
*/
void serialEvent() {
  while (Serial.available()) {
    // coge el nuevo byte:
    char inChar = (char)Serial.read(); 
    // añadelo a inputString:
    inputString += inChar;
    // Si el dato entrante es una nueva linea, ponemos a true     //StringComplete:
    if (inChar == '\n') {
    stringComplete = true;
    } 
  }
}


