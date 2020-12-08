/*
  Punto de acceso con servidor web
  Configuramos el WiFi com punto de acceso(AP) creando una red a    	la que tu puedes conectarte desde un PC o teléfono. Una vez     conectado abres el navegador y poner la IP 192.168.1.1 y te saldrá el servidor web imprementado en este ejemplo.
  ATENCIÓN: solo se puede conectar un cliente a la vez.
  Hardware Requerido:
  * Martuino

  Creado Noviembre 2014
  por Robert Wessels
  Modificado Octubre 2015 
  por Vicente Martinez (Se añade el servidor web)
  Este ejemplo es de dominio público.
*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


char wifi_name[] = "Martunio";
char wifi_password[] = "vicente1234"; // al menos 8 caracteres
WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  // Arranca wifi y creamos la red propia con nombre SSID        //wifi_name y con wifi_password como contraseña.
  Serial.print("Arrancando wifi...");
  WiFi.beginNetwork(wifi_name, wifi_password);
  Serial.println("Hecho.");
  server.begin();
}

void loop()
{
  // Escucha para conexiones entrantes
  WiFiClient client = server.available();
  if (client) {
    Serial.println("nuevo cliente");
    // el http request finaliza con una linea en blanco
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          // envia la cabecera standard http 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); 
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
	    client.println("<p><font size=+5>Bienvenido al servidor web Martuino</font><p>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // Comienza una nueva linea
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
           
          currentLineIsBlank = false;
        }


	 }
    }
    delay(200);
    // cerramos la conexion:
    client.stop();
    Serial.println("cliente desconectado");
  } 
}

