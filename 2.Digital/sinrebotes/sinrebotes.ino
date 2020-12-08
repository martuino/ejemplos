/* 
 Sin rebotes
 
 Cada vez que una entrada pasa de alto a bajo (ej. porque se pulsa un pulsador), la salida pasa de alto a bajo y viceversa varias veces. Normalmente se pone un peque�o retardo para evitar estos rebotes en cialquier circuito(ej. ignorar el ruido).

 
 El circuito:
 * LED conectado al pin 13 y negativo(ground)
 * pulsador conectado al pin 2 y +3,3V
 
 
 
 creado el 21 Noviembre 2006
 por David A. Mellis
 modificado 30 Agosto 2011
 por Limor Fried
 modificado 27 Abril 2012
 por Robert Wessels
 
Este c�digo es de dominio p�blico.
 
 http://www.arduino.cc/en/Tutorial/Debounce
 */

// constantes que deben de cambiarse
const int buttonPin = 2;     // n�mero de pin del pulsador
const int ledPin =  13;      // n�mero de pin del LED

// Variables que cambiar�n:
int ledState = HIGH;         // estado actual del pin de salida
int buttonState;             // estado actual del pulsador
int lastButtonState = LOW;   // lectura previa del pulsador

// las siguientes variables son "long" porque el tiempo, se mide //en milisegundos, y esta cifras son muy grandes para una //variable "int"

long lastDebounceTime = 0;  // �ltima vez que ha sido cambiado
long debounceDelay = 50;    // tiempo de rebote; incrementalo si 						//la salida tiene rebotes

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);//Entrada pulsador con r. 							//interna
  pinMode(ledPin, OUTPUT); //Salida LED
}

void loop() {
  // lee estado del pulsador y mandalo a la variable:
  int reading = digitalRead(buttonPin);

  // chequea si esta pulsadp el pulsador:  

  // Si el pulsador a cambiado,comienza a contar retardo:
  if (reading != lastButtonState) {
    // reset del contador de tiempo
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // si el estado del pulsador es m�s largo que el tiempo de    	//rebote, entonces toma el valor de la entrada como valor 	//sin rebotes
    buttonState = reading;
  }
  
  // cambia el estado del LED:
  digitalWrite(ledPin, buttonState);

  // guarda la lectura. 
  lastButtonState = reading;
}

