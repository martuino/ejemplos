#include <WiFi.h>

// Pines
int uno = 2;
int dos = 3;
int tres = 4;
int cua = 5;
char ssid[] = "MiSSID";
char password[] = "Micontraseña";
boolean alreadyConnected = false;
WiFiServer server(23);

// Aqui empieza despues de un reset:
void setup() {
 // Inicializa los pines como salidas
 pinMode(uno, OUTPUT);
 pinMode(dos, OUTPUT);
 pinMode(tres, OUTPUT);
 pinMode(cua, OUTPUT);
 digitalWrite(uno, LOW); //adelante rueda derecha
 digitalWrite(dos, LOW); //atrÃ s rueda derecha
 digitalWrite(tres, LOW); //atras rueda izquierda
 digitalWrite(cua, LOW); //adelante rueda izquierda 

 WiFi.begin(ssid, password);
 while ( WiFi.status() != WL_CONNECTED) {
 // Espera a conectar
 delay(300);
 }
 while (WiFi.localIP() == INADDR_NONE) {
 // Espera a que tengamos una IP
 delay(300);
 }
 server.begin();
}

// La rutina loop() es un bucle repetitivo
void loop() {
 WiFiClient client = server.available();
 parar();
 if (client) {
  if (!alreadyConnected) {
    client.flush();
    Serial.println("Nuevo cliente");
    client.println("Hola soy Martucar!");
    alreadyConnected = true;
  }
  if (client.available() > 0) {
    // Lee los caracteres del cliente:
    char thisChar = client.read();
    // Haz eco de lo recibido:
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
7
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
 digitalWrite(uno, LOW); //adelante rueda derecha
 digitalWrite(dos, LOW); //atrÃ s rueda derecha
 digitalWrite(tres, LOW); //atras rueda izquierda
 digitalWrite(cua, LOW); //adelante rueda izquierda
} 
