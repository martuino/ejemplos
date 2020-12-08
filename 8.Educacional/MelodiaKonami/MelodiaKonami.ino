/* 
 Toca la introducción Konami/Contra en un buzzer, demostrando 
 buzzer tune() API y pitch/tone/simple generador de música 
 
 por Dung Dang
 
 */
#include "pitches.h"


int buzzerPin = 03;

 
// notas en la melodia:
int melody[] = {
  NOTE_F2, NOTE_C6, NOTE_B5, NOTE_G5, NOTE_A5, NOTE_E1, NOTE_B1, NOTE_E1, NOTE_B1, NOTE_E1, NOTE_B1, 
   
   
  NOTE_G6, NOTE_F6, NOTE_DS6, NOTE_C6, NOTE_AS5, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_GS5, 
  NOTE_G5, NOTE_F5, NOTE_DS5, NOTE_F5, NOTE_AS4, NOTE_C5, NOTE_DS5, NOTE_F5,
  
  NOTE_C6, NOTE_NOTONE, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_F5,
  NOTE_C6, NOTE_NOTONE, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_GS5, NOTE_F5,

// Repeat once   
  NOTE_C6, NOTE_NOTONE, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_F5,
  NOTE_C6, NOTE_NOTONE, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_GS5, NOTE_F5,

  NOTE_C6, NOTE_NOTONE, NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_NOTONE, NOTE_DS6, NOTE_NOTONE,
  NOTE_G5, NOTE_AS5, NOTE_C6, NOTE_D6, NOTE_NOTONE, NOTE_D6, NOTE_DS6,
  
  NOTE_C6, NOTE_NOTONE, NOTE_C6, NOTE_NOTONE, NOTE_DS6, NOTE_F6
  
  
};
   
//duración nota: 4 = cuarto de nota, 8 = octavo de nota, etc.:
int noteDurations[] = {
2, 2, 2, 2,1, 2, 2, 2, 2, 2, 1,  // 11 notas



16, 16, 16, 16, 16, 16, 16, 16, 16, 16,  // 20  notas
16, 16, 16, 16, 16, 16, 16, 2,           // para ambas lineas   

16, 16, 16, 8, 16, 2, 2,                  // 7
16, 16, 16, 8, 16, 2, 2,                  // 7

// Repeat once
16, 16, 16, 8, 16, 2, 2,                  // 7
16, 16, 16, 8, 16, 2, 2,                  // 7

16, 16, 4, 4, 16, 16, 16, 8,             // 8
16, 16, 16, 16, 16, 4, 4,                  // 7

16, 16, 16, 16, 16, 4                     // 6 

};

void setup() 
{
pinMode(buzzerPin,OUTPUT);
}
void loop() 
{
  for (int thisNote = 0; thisNote < 80; thisNote++) {

    // Para calcular la duración , toma un segundo  
    // y lo divides por el tipo de nota.
    //ej. cuarto de nota = 1000 / 4, octavo de nota= 1000/8, etc.
    // tempo 100/150 
    int noteDuration = 1500/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //retardo entre pulsos
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);                //Para la melodia
  }
}

