/* Papadeo sin retardo
 
 Enciende y apaga un LED sin el uso de la funci�n Delay(). Esto  necesita de otro c�digo que pueda funcionar a la vez sin interrumpir el c�digo del LED.
 
 El circuito:
 * LED conectado al pin 13 y negativo(ground).
 
 
 creado 2005
 por David A. Mellis
 modificado 8 Feb 2010
 por Paul Stoffregen
 modificado 27 Abril 2012
 por Robert Wessels
 
 Este c�digo es de domnio p�blico.

 
 http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
 */

// constantes que no deben de cambiarse:
const int ledPin =  13;      // El n�mero del pin del LED

// Variables que pueden cambiarse:
int ledState = LOW;             // Estado del LED
long previousMillis = 0;        // Para almacenar la �ltima vez 							//que el LED estuvo encendido

// La siguiente variable es un "long" por que tiene que almacenar //un n�mero grande para el tiempo que se mide en milisegundos.No //puede ser un "int".
long interval = 1000;           // intervalo del papadeo 								//(milisegundos)

void setup() {
  // Pon el LED como salida:
  pinMode(ledPin, OUTPUT);      
}

void loop()
{
  // C�digo que se repite.

  // checkea el tiempo de parpadeo; si es diferente entre el //actual y el anterior, cambiamos el estado del LED 
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // guarda el tiempo de parpadeo del LED 
    previousMillis = currentMillis;   

    // Si esta apagado lo enciendes y vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // Pon el LEd seg�n el estado de la variable:
    digitalWrite(ledPin, ledState);
  }
}

