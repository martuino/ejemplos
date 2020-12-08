/*
 Martucar
    
  Hardware Requerido:
  * Martuino
  
  Este c�digo es de dominio p�blico.
*/

#include <WiFi.h>


// Pines
int uno = 2;
int dos = 3;
int tres = 4;
int cua = 5;
// your network name also called SSID
char ssid[] = "MiSSID";
// your network password
char password[] = "Mipassword";
boolean alreadyConnected = false;

WiFiServer server(23);

  
// Aqui empieza despu�s de un reset:
void setup() {                
  // Inicializa el LED como salida.
  pinMode(uno, OUTPUT);
  pinMode(dos, OUTPUT);
  pinMode(tres, OUTPUT);
  pinMode(cua, OUTPUT);
  digitalWrite(uno, LOW);   //adelante rueda derecha
  digitalWrite(dos, LOW);   //atràs rueda derecha
  digitalWrite(tres, LOW);  //atras rueda izquierda 
  digitalWrite(cua, LOW);  //adelante rueda izquierda
  Serial.begin(19200);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
   while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }
   printWifiStatus();
   server.begin();
}

// La rutina loop() es un bucle repetitivo
void loop() {
  WiFiClient client = server.available();
  parar(); 
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();
      Serial.println("Nuevo cliente");
      client.println("Hola soy Martucar!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      if (thisChar == 'W'){
        adelante(30);
      }else if (thisChar == 'A'){
        izquierda(30);
      }else if (thisChar == 'S'){
        atras(30);
      }else if (thisChar == 'D'){
        derecha(30);
      }else{
        parar(); 
      }
    }
  }              
}

void adelante(int x){
 digitalWrite(uno, HIGH);
 digitalWrite(cua, HIGH);
 delay(x);
 digitalWrite(uno, LOW);
 digitalWrite(cua, LOW);
}
void atras(int x){
 digitalWrite(dos, HIGH);
 digitalWrite(tres, HIGH);
 delay(x);
 digitalWrite(dos, LOW);
 digitalWrite(tres, LOW);
}  
void derecha(int x){
 digitalWrite(cua, HIGH);
 digitalWrite(dos, HIGH);
 delay(x);
 digitalWrite(cua, LOW);
 digitalWrite(dos, LOW);
}
void izquierda(int x){
 digitalWrite(uno, HIGH);
 digitalWrite(tres, HIGH);
 delay(x);
 digitalWrite(uno, LOW);
 digitalWrite(tres, LOW);
}
void parar(){
  digitalWrite(uno, LOW);   //adelante rueda derecha
  digitalWrite(dos, LOW);   //atràs rueda derecha
  digitalWrite(tres, LOW);  //atras rueda izquierda 
  digitalWrite(cua, LOW);  //adelante rueda izquierda  
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

