
#include <WiFi.h>

void setup()
{
  Serial.begin(9600);

  Serial.println("Comenzando WiFi SmartConfig");
  WiFi.startSmartConfig();

  Serial.print("Conectado a SSID: ");
  Serial.println(WiFi.SSID());

  while (WiFi.localIP() == INADDR_NONE) {
    // printa puntos hasta obtener una ip
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP obtenida");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // Nada que hacer
}
