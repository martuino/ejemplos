/*
  Melodia
 
 Toca una melodia
 
 circuito:
 * Altavoz de 8-ohm en el pin 3 y negativo (ground)
 
 creado 21 Enero 2010
 modificado 30 Agosto 2011
 por Tom Igoe 

Este código es de dominio público.
  
 */
 #include "pitches.h"

// notas en la melodia:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// duratión de la nota: 4 = cuarto de nota, 8 = octavo de nota, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };

void setup() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // Para calcular la duración de la nota, toma un segundo 
    // y dividelo por el tipo de nota.
    //ej. cuarto de nota = 1000 / 4, octavo de nota= 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(3, melody[thisNote],noteDuration);

    //Para distinguir las notas, ponemos un mínimo de tiempo //entre ellas.
    // Con la duración de la nota + 30% funciona bien:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // Para de tocar las nota:
    noTone(3);
  }
}

void loop() {
  // no necesita repetirse la melodia.
}
