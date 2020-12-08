
/*
  Web cliente

 Conectamos a martuino.com mediante wifi

 este ejemplo esta escrito para una red que use WPA encryption. Para WEP or WPA, cambia el Wifi.begin().

 Circuit:

 creado 13 July 2010
 por dlf (Metodo2 srl)
 modificado 31 May 2012
 por Tom Igoe
 */


#include <WiFi.h>

// El nombre de tu red (SSID)
char ssid[] = "TUSSID";
// La contraseña de tu red
char password[] = "TUCONTRASEÑA";

// Si tu no quieres usar DNS
// usa la IP con números en el nombre del servidor:
//IPAddress server(217,76,132,138);  // IP de martuino
char server[] = "www.martuino.com";    // dirección web para martuino.com (usando DNS)


// la conexión se realiza por el puerto 80 por defecto en HTTP):
WiFiClient client;

void setup() {
  //Inicializa puerto serie:
  Serial.begin(9600);

  // conecta con tu red Wifi:
  Serial.print("Conectando con red llamada: ");
  // muestra tu nombre de red (SSID);
  Serial.println(ssid); 
  // Conecta a WPA/WPA2. Cambia esta líneathis si usas WEP:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // muestra puntos hasta que se conecte
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nTu estas conectado");
  Serial.println("Esperando la IP");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // muestra puntos hasta que tengamos una IP asignada
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP obtenida");
  printWifiStatus();

  Serial.println("\nComenzando conexión al servidor...");
  // si conectamos lo mostramos por el puerto serie:
  if (client.connect(server, 80)) {
    Serial.println("conectado al servidor");
    // Hacemos petición HTTP:
    client.println("GET /hola.html HTTP/1.1");
    client.println("Host: martuino.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
  // Si recibimos algo del servidor, leelo y muestralo:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // Si el servidor desconecta, para el cliente:
  if (!client.connected()) {
    Serial.println();
    Serial.println("desconectando del servidor.");
    client.stop();

    // bucle infinito:
    while (true);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}





