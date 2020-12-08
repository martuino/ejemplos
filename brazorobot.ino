/*
 Brazo robotico

 Circuit:
 * Martuino

 creado Ago 2017
 */

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// your network name also called SSID
char ssid[] = "BRAZOROBOT";
// your network password
char password[] = "vicente66";
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();
WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously
unsigned int pos0=110; // ancho de pulso en cuentas para pocicion 0°
unsigned int pos180=470; // ancho de pulso en cuentas para la pocicion 180°
unsigned int pos90=290;
unsigned int manocerrada = 500;
unsigned int manoabierta = 200;
unsigned int mano = 4;
unsigned int muneca = 2;
unsigned int munecag = 3;
unsigned int codo = 1;
unsigned int hombro = 0;
unsigned int angulohombro = 90;
unsigned int angulocodo = 90;
unsigned int angulomuneca = 90;
unsigned int angulomunecag = 90;
unsigned int angulopinza = manocerrada; 
unsigned int angulohombroprox = 90;
unsigned int angulocodoprox = 90;
unsigned int angulomunecaprox = 90;
unsigned int angulomunecagprox = 90;
unsigned int angulopinzaprox = manocerrada; 
unsigned int tiempohombro = 1;
unsigned int tiempocodo = 1;
unsigned int tiempomunecag = 1;
unsigned int tiempomuneca = 1;
unsigned int tiempopinza = 1;
char bufrec[50];
char mese[3];
int z = 0;
int indicebuf = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(19200);
  servos.begin();
  servos.setPWMFreq(56);  // Analog servos run at ~56 Hz updates
  servos.setPWM(hombro,0,pos90);
  delay(1000);
  servos.setPWM(codo,0,pos90);
  delay(1000);
  servos.setPWM(muneca,0,pos90);
  delay(1000);
  servos.setPWM(munecag,0,pos90);
  delay(1000);
  servos.setPWM(mano,0,manocerrada);
  angulohombro = 90;
  angulocodo = 90;
  angulomuneca = 90;
  angulomunecag = 90;
  angulopinza = 0;
  //Opción 1: conexion a un router
  //Serial.print("Conectando con: ");
  //Serial.println(ssid); 
  //WiFi.beginNetwork(ssid, password);
  //while ( WiFi.status() != WL_CONNECTED) {
  //  Serial.print(".");
  //  delay(300);
  //}  
  //Serial.println("\nConectado.Esperando IP");
  //while (WiFi.localIP() == INADDR_NONE) {
  //  Serial.print(".");
  //  delay(300);
  //}
  //Serial.println("\nIP obtenida ");
  //printWifiStatus();
  //Opción 2: Como Access Point AP
  Serial.print("Arrancando wifi...");
  WiFi.beginNetwork(ssid, password);
  Serial.println("Hecho.");

  // Arracamos el servidor en el puerto 23:
  server.begin();
}


void loop() {
  // wait for a new client:
  WiFiClient client = server.available();
  // when the client sends the first byte, say hello:
  if (client) {  
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();
      Serial.println("NUEVO CLIENTE CONECTADO");
      alreadyConnected = true;
    }
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      bufrec[indicebuf] = thisChar;
      indicebuf++;
      if (thisChar == '\r'){
        for(int x=0;x<indicebuf;x++){
          if (bufrec[x] == 'H'){//Movimiento hombro
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            angulohombroprox = atoi(mese);
            mese[0] = bufrec[x+5];
            mese[1] = bufrec[x+6];
            mese[2] = bufrec[x+7];
            tiempohombro = atoi(mese);
            client.println("");
            client.print("H");
            client.println(angulohombroprox);
            actuarhombro(tiempohombro);
          }
          if (bufrec[x] == 'C'){//Movimiento codo
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            angulocodoprox = atoi(mese);
            mese[0] = bufrec[x+5];
            mese[1] = bufrec[x+6];
            mese[2] = bufrec[x+7];
            tiempocodo = atoi(mese);
            client.println("");
            client.print("C");
            client.println(angulocodoprox);
            actuarcodo(tiempocodo);
          }
          if (bufrec[x] == 'M'){//Movimiento muñeca giro
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            angulomunecagprox = atoi(mese);
            mese[0] = bufrec[x+5];
            mese[1] = bufrec[x+6];
            mese[2] = bufrec[x+7];
            tiempomunecag = atoi(mese);
            client.println("");
            client.print("M");
            client.println(angulomunecagprox);
            actuargiromuneca(tiempomunecag);
          }
          if (bufrec[x] == 'A'){//Movimiento muñeca arriba-abajo
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            angulomunecaprox = atoi(mese);
            mese[0] = bufrec[x+5];
            mese[1] = bufrec[x+6];
            mese[2] = bufrec[x+7];
            tiempomuneca = atoi(mese);
            client.println("");
            client.print("A");
            client.println(angulomunecaprox);
            actuarmuneca(tiempomuneca);
          }
          if (bufrec[x] == 'P'){//Movimiento pinza
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            angulopinzaprox = atoi(mese);
            mese[0] = bufrec[x+5];
            mese[1] = bufrec[x+6];
            mese[2] = bufrec[x+7];
            tiempopinza = atoi(mese);
            client.println("");
            client.print("P");
            client.println(angulopinzaprox);
            actuarpinza(tiempopinza);
          }   
          if (bufrec[x] == '?'){//angulos
            client.println("");
            client.print("H");
            client.println(angulohombroprox);        
            client.print("C");
            client.println(angulocodoprox);
            client.print("M");
            client.println(angulomunecagprox);
            client.print("A");
            client.println(angulomunecaprox);
            client.print("P");
            client.println(angulopinzaprox);
          }
          if (bufrec[x] == 'R'){//Reset
            resetservos();
            client.println("");
            client.print("H");
            client.println(angulohombroprox);        
            client.print("C");
            client.println(angulocodoprox);
            client.print("M");
            client.println(angulomunecagprox);
            client.print("A");
            client.println(angulomunecaprox);
            client.print("P");
            client.println(angulopinzaprox);
          }
          if (bufrec[x] == 'D'){//Delay
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            delay(atoi(mese));
            client.println("");
            client.print("D");
            client.println(atoi(mese));    
          }
        }
        indicebuf = 0;
      }     
    }
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Nivel de señal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void actuarmuneca(int tiempo){
  if (angulomunecaprox > 180){
     angulomunecaprox = 180; 
  }
  if (angulomunecaprox<0){
     angulomunecaprox = 0; 
  }
  if (angulomunecaprox>angulomuneca){
    for (int y=angulomuneca;y<angulomunecaprox;y++){
      z = (y * 2)+ pos0;
      servos.setPWM(muneca,0,z);
      delay(tiempo);
    }
  }else{
    while (angulomunecaprox<angulomuneca){
      z = (angulomuneca * 2)+ pos0;
      servos.setPWM(muneca,0,z);
      delay(tiempo);
      angulomuneca--;
    }    
  }
  angulomuneca = angulomunecaprox;
}
void actuarhombro(int tiempo){
if (angulohombroprox > 180){
     angulohombroprox = 180; 
  }
  if (angulohombroprox<0){
     angulohombroprox = 0; 
  }
  if (angulohombroprox > angulohombro){
    for (int y=angulohombro;y<angulohombroprox;y++){
      z = (y * 2)+ pos0;
      servos.setPWM(hombro,0,z);
      delay(tiempo);
    }
  }else{
    while (angulohombroprox<angulohombro){
      z = (angulohombro * 2)+ pos0;
      servos.setPWM(hombro,0,z);
      delay(tiempo);
      angulohombro--;
    }    
  }
  angulohombro = angulohombroprox;
}
void actuargiromuneca(int tiempo){
  if (angulomunecagprox > 180){
     angulomunecagprox = 180; 
  }
  if (angulomunecagprox<0){
     angulomunecagprox = 0; 
  }
  if (angulomunecagprox > angulomunecag){
    for (int y=angulomunecag;y<angulomunecagprox;y++){
      z = (y * 2)+ pos0;
      servos.setPWM(munecag,0,z);
      delay(tiempo);
    }
  }else{
    while (angulomunecagprox<angulomunecag){
      z = (angulomunecag * 2)+ pos0;
      servos.setPWM(munecag,0,z);
      delay(tiempo);
      angulomunecag--;
    }    
  }
  angulomunecag = angulomunecagprox;
}
void actuarcodo(int tiempo){
  if (angulocodoprox > 180){
     angulocodoprox = 180; 
  }
  if (angulocodoprox<0){
     angulocodoprox = 0; 
  }
  if (angulocodoprox > angulocodo){
    for (int y=angulocodo;y<angulocodoprox;y++){
      z = (y * 2)+ pos0;
      servos.setPWM(codo,0,z);
      delay(tiempo);
    }
  }else{
    while (angulocodoprox<angulocodo){
      z = (angulocodo * 2)+ pos0;
      servos.setPWM(codo,0,z);
      delay(tiempo);
      angulocodo--;
    }    
  }
  angulocodo = angulocodoprox;
}
void resetservos(){
  angulohombro = 90;
  angulocodo = 90;
  angulomuneca = 90;
  angulomunecag = 90;
  angulopinza = 0;
  
  servos.setPWM(hombro,0,pos90);
  delay(250);
  servos.setPWM(codo,0,pos90);
  delay(250);
  servos.setPWM(muneca,0,pos90);
  delay(250);
  servos.setPWM(munecag,0,pos90);
  delay(250);
  servos.setPWM(mano,0,manocerrada);
}
void actuarpinza(int tiempo){
if (angulopinzaprox > 180){
     angulopinzaprox = 180; 
  }
  if (angulopinzaprox<0){
     angulopinzaprox = 0; 
  }
  if (angulopinzaprox == 0){
      servos.setPWM(mano,0,manocerrada);
  }else{
      servos.setPWM(mano,0,manoabierta);
  }
  angulopinza = angulopinzaprox;
}


