
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7,3,POSITIVE);  // Set the LCD I2C address


void setup()
{
  
  lcd.begin(20,4);
  lcd.noBacklight();   //Apaga luz
  delay(1000);
  lcd.backlight();     //Enciende Luz
  lcd.setCursor(0,0);             //Linea 1, caracter 0 
  lcd.print("Hola, MARTUINO ");  
  lcd.setCursor (0,1);           // Linea 2, caracter 0
  lcd.print("Visita nuestra");
  lcd.setCursor(0,2);            // Linea 3, caracter 0
  lcd.print("pagina web");
  lcd.setCursor(0,3);            // Linea 4, caracter 0
  lcd.print("www.martuino.com"); 
}

void loop()
{
  // put your main code here, to run repeatedly:
  
}
