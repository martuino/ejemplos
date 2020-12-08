/*
  WiFi Web Servidor

 Un simple servidor web que muestra el valor de las entradas
 análogicas.
 usando el WiFi integrado.

 Este ejemplo esta escrito para redes que usando WPA2. Para WEP or WPA, cambia el Wifi.begin()

 Circuito:
 * WiFi 
 * Entradas análogicas pines A0 hasta A3 (opcional)

 creado 13 July 2010
 de dlf (Metodo2 srl)
 modificado 31 May 2012
 por Tom Igoe

 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


// El nombre de tu red (SSID)
char ssid[] = "TUSSID";
// Tu contraseña
char password[] = "TUCONTRASEÑA";
// El Keyindex de tu red (Solo para WEP)
int keyIndex = 0;
const int analogInPin1 = A0; 
const int analogInPin2 = A1; 
const int analogInPin3 = A2; 
int sensorReading = 0;


WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // Inicializa puerto serie
 

  // Esperando a conectar con tu wifi:
  Serial.print("Esperando a conectar con: ");
  // muestra el nombre de tu red(SSID);
  Serial.println(ssid); 
  // Conectando a red WPA/WPA2. Cambia esta línea si usas una red abierta o WEP :
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // muestra puntos hata que conecte
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nEstas conectado con tu wifi");
  Serial.println("Esperando una IP");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // muestra puntos hasta obtener la IP
    Serial.print(".");
    delay(300);
  }

  // Ya esta conectado , muestra el estado  
  printWifiStatus();
  
  Serial.println("Comenzando servidor en puerto 80");
  server.begin(); //Comienza servidor en puerto 80
  Serial.println("Servidor web en marcha!");

}


void loop() {
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
        //Enviamos una replica
        if (c == '\n' && currentLineIsBlank) {
          // envia la cabecera standard http 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 5");  //Refresca cada 5 seg.
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // valor de cada entrada análogica
          sensorReading = analogRead(analogInPin1); 
          client.print("Entrada analogica 0 es:");                
          client.print(sensorReading);
          client.println("<br />");
          sensorReading = analogRead(analogInPin2);
          client.print("Entrada analogica 1 es:");                
          client.print(sensorReading);
          client.println("<br />");
          sensorReading = analogRead(analogInPin3);
          client.print("Entrada analogica 2 es:");                
          client.print(sensorReading);
          client.println("<br />");  
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
    // Da tiempo al navegador para recibir los datos
    delay(200);

    // cerramos la conexion:
    client.stop();
    Serial.println("cliente desconectado");
  }
}


void printWifiStatus() {
  // envia el SSID de la red:
  Serial.print("Nombre d la red: ");
  Serial.println(WiFi.SSID());

  // envia la IP :
  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);

  // Envia el nivel de señal:
  long rssi = WiFi.RSSI();
  Serial.print("Nivel de señal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

