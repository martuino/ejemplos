/* 
 Toca la melodia de cumpleaños feliz en un buzzer, demonstrando
 buzzer tune() API y generación de pitch/tone (hence music)
 
 por Dung Dang
 
 */
#include "pitches.h"
#define NOTE_C4_1 260

int buzzerPin = 03;

// notas en la melodia:
int melody[] = {
   NOTE_C4_1,NOTE_C4, NOTE_D4, NOTE_C4,NOTE_F4,NOTE_E4,
   NOTE_C4_1,NOTE_C4,NOTE_D4,NOTE_C4,NOTE_G4,NOTE_F4,
   NOTE_C4_1,NOTE_C4,NOTE_C5,NOTE_A4,NOTE_F4,NOTE_F4, NOTE_E4,NOTE_D4,
   NOTE_AS4,NOTE_AS4,NOTE_A4,NOTE_F4,NOTE_G4,NOTE_F4};
   
//duración de la nota: 4 = cuarto de nota, 8 = octavo de nota:
int noteDurations[] = {
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,2,1,
  4, 4, 2, 2,4,4,2,1, 
  4, 4, 2, 2,2,1};

void setup() 
{
pinMode(buzzerPin,OUTPUT);
}
void loop() 
{
  for (int thisNote = 0; thisNote < 26; thisNote++) {

    //Para calcular la duración de la nota, toma un segundo 
    // y dividelo por el tipo de nota
    //ej. cuarto de note = 1000 / 4, octavo de nota = 1000/8
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //retardo entre pulsos
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);                //Para la melodia
  }
}

