// I2C lectura PCF8591 ADC + DAC
// por Vicente Martínez y Javier Martínez

// Creado 16 Agosto 2015

// Este ejemplo es de dominio público.


#include <Wire.h>
#define PCF8591 (0x90 >> 1) // I2C bus address
int ADC0;
int incr;
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  incr = 0;  //Inicializamos DAC
}

void loop()
{
  int registro = 0x40; //utilizaremos 0x41=ADC1, 0x42=ADC2, 0X43=ADC3
  Wire.beginTransmission(PCF8591);
  Wire.write(registro);
  Wire.write(incr);   //Salida dac
  Wire.endTransmission();
  Wire.requestFrom(PCF8591, 1);    // request 2 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    ADC0 = Wire.read(); // recibe primer byte
    Serial.print("Dato leido ADC:");
    float var = ADC0 * 0.0128; // VCC/256 = 0.0128 voltios por bit
    Serial.println(var);
  }
  
  incr+=4;   //geneamos una rampa en DAC
  if (incr>255) 
  {
    incr = 0;
  }
  
  delay(500);
}
