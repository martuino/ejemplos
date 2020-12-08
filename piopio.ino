//Pio Pio

#include <stdio.h>
#include <DS1302.h>
#include <WiFi.h>
#include <DFPlayer_Mini_Mp3.h>
#include <driverlib/timer.h>
#include <inc/hw_timer.h>
#include <driverlib/prcm.h>
#include <inc/hw_types.h>
#include <SLFS.h>

const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock
char buftime[50];
char bufrec[50];
char mese[3];
char time[] = "TIME";
int indicebuf = 0;
String diasem;
int ano = 2016;
int mes = 1;
int dia = 1;
int diasemana = 0;
int hora = 0;
int minuto = 0;
int segundo = 0;
int minutoAnterior = 0;
int Salida1 = 3;
int Salida2 = 20;
int Salida3 = 21;
int valors1 = 0;
int valors2 = 0;
int valors3 = 0;
int cancion = 0;
boolean hubocambio = false;
boolean hubocambiomus = false;
boolean alreadyConnected = false;
char wifi_name[] = "C3LYMP3";
char wifi_password[] = "28021967"; // al menos 8 caracteres
int Memo[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//******GUARDAR***************
int Memominu[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Memohora[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Memovalor1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Memovalor2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Memovalor3[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Memomp3[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//****************************
int Nummemo = 0;
int cont = 0;
unsigned long ciclo= 640 ; //Como el reloj del micro es de 80Mhz
int misminutos = 0;
int misminutos2 = 0;
int difvalor1 = 0;
int difvalor2 = 0;
int difvalor3 = 0;
int difMinuto1 = 0;
int difMinuto2 = 0;
int difMinuto3 = 0;
boolean difneg1 = false;
boolean difneg2 = false;
boolean difneg3 = false;
boolean Memocambiada = false;
int pos = 0;
int Numactual = 19;
uint8_t flash_buffer[32]; //Se guarda la hora en la flash
uint8_t flash_buffer2[32]; //Se guarda los minutos en la flash
uint8_t flash_buffer3[32]; //Se guarda los valores en la flash
uint8_t flash_buffer4[32]; //Se guarda los valores en la flash
uint8_t flash_buffer5[32]; //Se guarda los valores en la flash
uint8_t flash_buffer6[32]; //Se guarda los mp3 en la flash
uint8_t flash_buffer7[5]; //Se guarda los valores actuales en flash
int cntminu1 = 0;
int cntminu2 = 0;
int cntminu3 = 0;
int cntsel = 0;






// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);
//Crea wifi
WiFiServer server(23);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Domingo";
    case Time::kMonday: return "Lunes";
    case Time::kTuesday: return "Martes";
    case Time::kWednesday: return "Miercoles";
    case Time::kThursday: return "Jueves";
    case Time::kFriday: return "Viernes";
    case Time::kSaturday: return "Sabado";
  }
  return "(Sin dia)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();
  // Name the day of the week.
  const String day = dayAsString(t.day);
  // Format the time and date and insert into the temporary buffer.
  
  snprintf(buftime, sizeof(buftime), "%s %02d:%02d:%02d %02d-%02d-%04d ",day.c_str(),t.hr, t.min, t.sec, t.date, t.mon, t.yr);
  diasem = day.c_str();
  ano = t.yr;
  mes = t.mon;
  dia = t.date;
  hora = t.hr;
  minuto = t.min;
  segundo =t.sec; 
  // Print the formatted string to serial so we can see the time.
  //Serial.println(buf);
}

void CambiaHora(Time t){ //Formato de t(2016,12,29,11,05,00,Time::KSunday);
   rtc.time(t); 
}
void vaciabuffer(){
   for (int x=0;x < 50;x++){
      bufrec[x] ='\0';
   } 
}

void setup()
{
  Serial.begin(19200);
  //Inicializamos RTC DS1302
  rtc.writeProtect(false);
  rtc.halt(false);
  //Inicializamos MP3
  Serial1.begin(9600);
  mp3_set_serial (Serial1);	//set Serial for DFPlayer-mini mp3 module 
  mp3_set_volume (25);
  mp3_stop();
  //Inicializamos servidor wifi AP
  Serial.print("Arrancando wifi...");
  WiFi.beginNetwork(wifi_name, wifi_password);
  Serial.println("Hecho.");
  server.begin();
  //Inicializamos salidas
  pinMode(Salida1,OUTPUT);
  pinMode(Salida2,OUTPUT);
  pinMode(Salida3,OUTPUT);
  analogWrite(Salida1,0);
  analogWrite(Salida2,0);
  digitalWrite(Salida3,0);
  hubocambio = true;
  //TIMER
  MAP_PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
  MAP_PRCMPeripheralReset(PRCM_TIMERA0);
  MAP_TimerIntRegister(TIMERA0_BASE, TIMER_A, clock); // rutina donde salta
  MAP_TimerConfigure(TIMERA0_BASE, TIMER_CFG_A_PERIODIC);//Cilco periodico
  MAP_TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT);//Habilita interrupcion
  MAP_TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 0);//No usamos preescaler
  MAP_TimerLoadSet(TIMERA0_BASE, TIMER_A, ciclo); //80Mhz = 1 segundo
  MAP_TimerEnable(TIMERA0_BASE, TIMER_A);//Ponemos en marcha Timer A
  //Extrae las memorias
  SerFlash.begin();  
  //Lee de la memoria 
  SerFlash.open("horas.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer[a] != 0){
     Memohora[a] = char(flash_buffer[a]);
    }
  }
  //Lee de la memoria 
  SerFlash.open("minu.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer2[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer2[a] != 0){
     Memominu[a] = char(flash_buffer2[a]);
    }
  }
  //Lee de la memoria
  SerFlash.open("valor1.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer3[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer3[a] != 0){
     Memovalor1[a] = char(flash_buffer3[a]);
    }
  }
  SerFlash.open("valor2.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer4[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer4[a] != 0){
     Memovalor2[a] = char(flash_buffer4[a]);
    }
  }
  SerFlash.open("valor3.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer5[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer5[a] != 0){
     Memovalor3[a] = char(flash_buffer5[a]);
    }
  }
  SerFlash.open("mp3.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<20;i++){
    flash_buffer6[i] = SerFlash.read(); 
  }
  SerFlash.close();
  for (int a=0;a<20;a++){
    if (flash_buffer6[a] != 0){
     Memomp3[a] = char(flash_buffer6[a]);
    }
  }
  SerFlash.open("conf.txt", FS_MODE_OPEN_READ);
  for (int i=0; i<5;i++){
    flash_buffer7[i] = SerFlash.read(); 
  }
  SerFlash.close();
  valors1 = flash_buffer7[0];
  valors2 = flash_buffer7[1];
  valors3 = flash_buffer7[2];
  Numactual = flash_buffer7[3];
  
}

void loop()
{
   printTime();
   WiFiClient client = server.available();
   if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("Nuevo cliente");
      client.println("Hola");
      indicebuf = 0;
      alreadyConnected = true;
    }
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      bufrec[indicebuf] = thisChar;
      indicebuf++;
      if (thisChar == '\r'){
        for(int x=0;x<indicebuf;x++){
        //  client.println(x);
        //  client.println(bufrec[x]);
          if (bufrec[x] == 'T'){ //Lectura hora
             client.println("\r");
             client.println(buftime);
             Serial.println("Lectura hora.");
          }
          if (bufrec[x] == '?'){
            client.println("T:Muestra la hora y fecha.");  
            client.println("F:Escribe la fecha y hora(FdddmmmaaahhhmmmsssDDD).");
            client.println("S:Pon un valor en salida(Sx000 a Sx255).x=salida(1,2,3).");  
            client.println("P:Reproduce una cancion(P00-P99).");
            client.println("C:Graba una posicion de memoria(C000-C019 + hhhmmmv1v1v1v2v2v2v3v3v3mmm).vx = valor salida x. m=nº cancion.");
            client.println("L:Lectura de una memoria (L00-L19).");
            client.println("X:Lectura de todas las memorias.");
            client.println("Y:Lectura parametros de calculo");
            client.println("Z:Forzar nº de memoria a ejecutar(Z00-Z19).");
            client.println("?:Ayuda");
            Serial.println("Ayuda");
          }
          if (bufrec[x] == 'F'){//Puesta en fecha y hora FdddmmmaaahhhMMMsssDDD
            mese[0] = bufrec[x+1];
            mese[1] = bufrec[x+2];
            mese[2] = bufrec[x+3];
            dia = atoi(mese);
            mese[0] = bufrec[x+4];
            mese[1] = bufrec[x+5];
            mese[2] = bufrec[x+6];
            mes = atoi(mese);
            mese[0] = bufrec[x+7];
            mese[1] = bufrec[x+8];
            mese[2] = bufrec[x+9];
            ano = atoi(mese);
            mese[0] = bufrec[x+10];
            mese[1] = bufrec[x+11];
            mese[2] = bufrec[x+12];
            hora = atoi(mese);
            mese[0] = bufrec[x+13];
            mese[1] = bufrec[x+14];
            mese[2] = bufrec[x+15];
            minuto = atoi(mese);
            mese[0] = bufrec[x+16];
            mese[1] = bufrec[x+17];
            mese[2] = bufrec[x+18];
            segundo = atoi(mese);
            mese[0] = bufrec[x+19];
            mese[1] = bufrec[x+20];
            mese[2] = bufrec[x+21];
            diasemana = atoi(mese); // 0 = Domingo            
            switch (diasemana){
              case 0:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kSunday);
                rtc.time(t);
              }
                break;
              case 1:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kMonday);   
                rtc.time(t);
              }
                break;
              case 2:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kTuesday);
                rtc.time(t);
              }
                break;
              case 3:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kWednesday);
                rtc.time(t);}
                break;
              case 4:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kThursday);
                rtc.time(t);}
                break;
              case 5:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kFriday);
                rtc.time(t);}
                break;
              case 6:{
                Time t(2000 + ano, mes, dia, hora, minuto, segundo, Time::kSaturday);
                rtc.time(t);}
                break;
            }
              client.println("\r");
              client.println("OK");
              Serial.println("Puesta en hora.");            
          }
          if (bufrec[x] == 'S'){ //Pon un valor en una salida
            if (bufrec[x+1] == '1'){ //Salida 1
              mese[0] = bufrec[x+2];
              mese[1] = bufrec[x+3];
              mese[2] = bufrec[x+4];
              valors1 = atoi(mese);
              client.println("\r");
              client.println("OK");
              Serial.println("Nuevo valor S1");
              hubocambio = true;
            }else if (bufrec[x+1] == '2'){ //Salida 2
              mese[0] = bufrec[x+2];
              mese[1] = bufrec[x+3];
              mese[2] = bufrec[x+4];
              valors2 = atoi(mese);
              client.println("\r");
              client.println("OK");
              Serial.println("Nuevo valor S2");
              hubocambio = true;
            }else if (bufrec[x+1] == '3'){ //Salida 3
              mese[0] = bufrec[x+2];
              mese[1] = bufrec[x+3];
              mese[2] = bufrec[x+4];
              valors3 = atoi(mese);
              client.println("\r");
              client.println("OK");
              Serial.println("Nuevo valor S3");
              hubocambio = true;
            }
          }
          if (bufrec[x] == 'P'){ //Reproduce una canción
              mese[0] = bufrec[x+2];
              mese[1] = bufrec[x+3];
              cancion = atoi(mese);
              client.println("\r");
              client.print("Cancion:");
              client.print(cancion);
              client.println(" -OK");
              Serial.println("Reproduce cancion");
              hubocambio = true;
              hubocambiomus = true;
            }
          if (bufrec[x] == 'C'){ //Escribe una posición de memoria 0-19
              mese[0] = bufrec[x+1];
              mese[1] = bufrec[x+2];
              mese[2] = bufrec[x+3];
              Nummemo = atoi(mese); //NUMERO DE MEMORIA
              if (Nummemo<20){
                mese[0] = bufrec[x+4];
                mese[1] = bufrec[x+5]; //Hora
                mese[2] = bufrec[x+6];
                Memohora[Nummemo] = atoi(mese);
                mese[0] = bufrec[x+7];
                mese[1] = bufrec[x+8]; //Minuto
                mese[2] = bufrec[x+9];
                Memominu[Nummemo] = atoi(mese);
                mese[0] = bufrec[x+10];
                mese[1] = bufrec[x+11];
                mese[2] = bufrec[x+12];
                Memovalor1[Nummemo] = atoi(mese);
                mese[0] = bufrec[x+13];
                mese[1] = bufrec[x+14];
                mese[2] = bufrec[x+15];
                Memovalor2[Nummemo] = atoi(mese);
                mese[0] = bufrec[x+16];
                mese[1] = bufrec[x+17];
                mese[2] = bufrec[x+18];
                Memovalor3[Nummemo] = atoi(mese);
                mese[0] = bufrec[x+19];
                mese[1] = bufrec[x+20];
                mese[2] = bufrec[x+21];
                Memomp3[Nummemo] = atoi(mese); 
                client.println("\r");
                client.println("OK");
                Serial.println("Nuevos datos posicion memoria");
                Memocambiada = true;
                //Guardamos la hora en flash
                for (int d=0;d<20;d++){
                   flash_buffer[d] = (uint8_t)Memohora[d];  
                }
                SerFlash.open("horas.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer,20);  // x characters
                SerFlash.close();
                //Guardamos el minuto en flash
                for (int d=0;d<20;d++){
                   flash_buffer2[d] = (uint8_t)Memominu[d];  
                }
                SerFlash.open("minu.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer2,20);  // x characters
                SerFlash.close();
                //Guardamos el valor 1 en flash
                for (int d=0;d<20;d++){
                   flash_buffer3[d] = (uint8_t)Memovalor1[d];  
                }
                SerFlash.open("valor1.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer3,20);  // x characters
                SerFlash.close();
                //Guardamos el valor 2 en flash
                for (int d=0;d<20;d++){
                   flash_buffer4[d] = (uint8_t)Memovalor2[d];  
                }
                SerFlash.open("valor2.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer4,20);  // x characters
                SerFlash.close();
                //Guardamos el valor 3 en flash
                for (int d=0;d<20;d++){
                   flash_buffer5[d] = (uint8_t)Memovalor3[d];
                }
                SerFlash.open("valor3.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer5,20);  // x characters
                SerFlash.close();
                //Guardamos el mp3 en flash
                for (int d=0;d<20;d++){
                   flash_buffer6[d] = (uint8_t)Memomp3[d];
                }
                SerFlash.open("mp3.txt",FS_MODE_OPEN_CREATE(20, _FS_FILE_OPEN_FLAG_COMMIT));
                SerFlash.write(flash_buffer6,20);  // x characters
                SerFlash.close();

              }else{
                client.println("\r");
                client.println("ERROR");
              }
          }
          if (bufrec[x] == 'L'){ //Lee una posicion de memoria
              mese[0] = bufrec[x+1];
              mese[1] = bufrec[x+2];
              mese[2] = bufrec[x+3];
              Nummemo = atoi(mese);
              if (Nummemo<20){
                client.println("");
                client.print("Memoria ");
                client.print(Nummemo);
                client.print(": ");
                client.print(Memohora[Nummemo]);
                client.print(":");
                client.print(Memominu[Nummemo]);
                client.print("-");
                client.print(Memovalor1[Nummemo]);
                client.print("-");
                client.print(Memovalor2[Nummemo]);
                client.print("-");
                client.print(Memovalor3[Nummemo]);
                client.print("-");
                client.print(Memomp3[Nummemo]);
                client.println("-OK");
                Serial.println("Lectura posicion memoria");
              }else{
                client.println("ERROR");
              }
          }
          if (bufrec[x] == 'X'){ //Lee todas las memorias 0-19
            client.println("");
            for (int y=0; y<20;y++){  
                client.print("Memoria ");
                client.print(y);
                client.print(": ");
                client.print(Memohora[y]);
                client.print(":");
                client.print(Memominu[y]);
                client.print("-");
                client.print(Memovalor1[y]);
                client.print("-");
                client.print(Memovalor2[y]);
                client.print("-");
                client.print(Memovalor3[y]);
                client.print("-");
                client.print(Memomp3[y]);
                client.println("-OK");
              }
                client.print("valor 1: ");
                client.print(valors1);
                client.println("-OK");
                client.print("Valor 2: ");
                client.print(valors2);
                client.println("-OK");
                client.print("Valor 3: ");
                client.print(valors3); 
                client.println("-OK");
                Serial.println("Leidas todas las memorias");
          }
          if (bufrec[x] == 'Y'){ //Lee parametros
            client.println("");
            client.print("Numactual: ");
            client.print(Numactual);
            client.println("");
            client.print("pos: ");
            client.println(pos);
            client.print("DifMinuto1: ");
            client.println(difMinuto1);
            client.print("DifMinuto2: ");
            client.println(difMinuto2);
            client.print("DifMinuto3: ");
            client.println(difMinuto3);
            client.print("Difvalor1: ");
            client.println(difvalor1);
            client.print("Difvalor2: ");
            client.println(difvalor2);
            client.print("Difvalor3: ");
            client.println(difvalor3);
            client.print("cntminu1: ");
            client.println(cntminu1);
            client.print("cntminu2: ");
            client.println(cntminu2);
            client.print("cntminu3: ");
            client.println(cntminu3);            
            client.print("valor 1: ");
            client.println(valors1);
            client.print("Valor 2: ");
            client.println(valors2);
            client.print("Valor 3: ");
            client.println(valors3); 
            client.println("-OK");            
            Serial.println("Leido parametros");
          }
          if (bufrec[x] == 'Z'){ //Escribe Numactual de memoria
              mese[0] = bufrec[x+1];
              mese[1] = bufrec[x+2];
              mese[2] = bufrec[x+3];
              Numactual = atoi(mese);
              client.println("");
              client.println("-OK");            
          }
        }
        indicebuf = 0;
      }
      // echo the bytes back to the client:
      server.write(thisChar);
      }
      // echo the bytes to the server as well:
      //Serial.write(thisChar);
    
  }
  //cada minuto mira los valores
  if (minuto != minutoAnterior){
      minutoAnterior = minuto;
      //Comparamos cosas cada minuto
      for (int g=0; g<20;g++){
         if (Memohora[g] == hora){
          if (Memominu[g] == minuto){
            Numactual = g;
            if (valors1 != Memovalor1[g]){
             valors1 = Memovalor1[g];
             hubocambio = true;
            }
            if (valors2 != Memovalor2[g]){
             valors2 = Memovalor2[g];
             hubocambio = true;
            }
            if (valors3 != Memovalor3[g]){
             valors3 = Memovalor3[g];
             hubocambio = true;
            }
            if (cancion != Memomp3[g]){
             cancion = Memomp3[g];
             hubocambiomus = true;
            }
          }
        }
      }
      //Ahora vemos la progresión entre memorias
      misminutos = Memohora[Numactual] * 60; //Horas a minutos memoria 0
      misminutos += Memominu[Numactual];
      if (Numactual == 19){   //La última memoria 
          misminutos2 = Memohora[0] * 60; //Horas a minutos memoria 0
          misminutos2 += Memominu[0];
         //valores
         if (Memovalor1[Numactual] > Memovalor1[0]){
           difvalor1 = Memovalor1[Numactual] - Memovalor1[0];
           difneg1 = true; //descenso de valor 1
         }else{
           difvalor1 = Memovalor1[0] - Memovalor1[Numactual];
           difneg1 = false; //Ascenso valor 1
         }
         if (Memovalor2[Numactual]> Memovalor2[0]){
           difvalor2 = Memovalor2[Numactual] - Memovalor2[0];
           difneg2 = true; //Descenso valor 2
         }else{
           difvalor2 = Memovalor2[0] - Memovalor2[Numactual];
           difneg2 = false; //Ascenso valor 2
         }
         if (Memovalor3[Numactual]> Memovalor3[0]){
           difvalor3 = Memovalor3[Numactual] - Memovalor3[0];
           difneg3 = true; //Descenso valor3
         }else{
           difvalor3 = Memovalor3[0] - Memovalor3[Numactual];
           difneg3 = false; //Ascenso valor 3
         }
      }else if (Numactual > 19){
        Numactual = 0;
      }else{
          misminutos2 = Memohora[Numactual+1] * 60; //Horas a minutos memoria 0
          misminutos2 += Memominu[Numactual+1];
         //valores
         if (Memovalor1[Numactual] > Memovalor1[Numactual+1]){
           difvalor1 = Memovalor1[Numactual] - Memovalor1[Numactual+1];
           difneg1 = true; //Descenso
         }else{
           difvalor1 = Memovalor1[Numactual+1] - Memovalor1[Numactual]; 
           difneg1 = false; //Ascenso
         }
         if (Memovalor2[Numactual] > Memovalor2[Numactual+1]){
           difvalor2 = Memovalor2[Numactual] - Memovalor2[Numactual+1];
           difneg2 = true; //Descenso
         }else{
           difvalor2 = Memovalor2[Numactual+1] - Memovalor2[Numactual];
           difneg2 = false; //Ascenso
         }
         if (Memovalor3[Numactual]> Memovalor3[Numactual+1]){
           difvalor3 = Memovalor3[Numactual] - Memovalor3[Numactual+1];
           difneg3 = true; //Descenso
         }else{
           difvalor3 = Memovalor3[Numactual+1] - Memovalor3[Numactual];
           difneg3 = false; //Ascenso
         }      
      }
      //diferencia de minutos
      if (misminutos > misminutos){
        pos = misminutos - misminutos2;
      }else{
        pos = misminutos2 - misminutos;
      }
      //Cada minuto hay que realizar un cambio de x en los valores de las salidas
      if (difvalor1 > pos){
        difMinuto1 = difvalor1 / pos; //Aumenta difMinuto1 cada minuto
      }else{
        difMinuto1 = pos / difvalor1; //Aumenta 1 cada difMinuto1 minutos
        difMinuto1 = difMinuto1 * (-1); //Lo hacemos negativo
        cntminu1++;
      }
      if (difvalor2 > pos){      
        difMinuto2 = difvalor2 / pos;
      }else{
        difMinuto2 = pos / difvalor2;
        difMinuto2 = difMinuto2 * (-1); //Lo hacemos negativo
        cntminu2++;
      }
      if (difvalor3 > pos){
        difMinuto3 = difvalor3 / pos;
      }else{
        difMinuto3 = pos / difvalor3;
        difMinuto3 = difMinuto3 * (-1);//Lo hacemos negativo        
        cntminu3++;
      }
      //Cada minuto sumamos el valor diferencia a cada salida
      if (difMinuto1 >= 0 ){
        if (difneg1){
          valors1 -= difMinuto1; 
        }else{
          valors1 += difMinuto1;
        }
        hubocambio = true;
      }else{
        cntsel = difMinuto1 * (-1); //Lo hacemos positivo
        cntsel = cntminu1 % cntsel;
        if (cntsel == 0){
          cntminu1=0;
          if (difneg1){
            valors1 -= 1; 
          }else{
            valors1 += 1;
          }          
        }
      }
      if (difMinuto2 >= 0 ){
        if (difneg2){
          valors2 -= difMinuto2; 
        }else{
          valors2 += difMinuto2;
        }
        hubocambio = true;
      }else{
        cntsel = difMinuto2 * (-1); //Lo hacemos positivo
        cntsel =  cntminu2 % cntsel;
        if (cntsel == 0){
          cntminu2=0;
          if (difneg2){
            valors2 -= 1; 
          }else{
            valors2 += 1;
          }          
        }
      }
      if (difMinuto3 >= 0 ){
        if (difneg3){
          valors3 -= difMinuto3; 
        }else{
          valors3 += difMinuto3;
        }
        hubocambio = true;
      }else{
        cntsel = difMinuto3 * (-1); //Lo hacemos positivo
        cntsel = cntminu3 % cntsel;
        if (cntsel == 0){
          cntminu3 = 0;
          if (difneg3){
            valors3 -= 1; 
          }else{
            valors3 += 1;
          }          
        }      
      }
  }  
  if (hubocambio){
    hubocambio = false;
    analogWrite(Salida1,valors1);
    analogWrite(Salida2,valors2);
    if (hubocambiomus){
      hubocambiomus= false;
      if (cancion == 0){
        mp3_stop();
      }else{
        mp3_play_physical(cancion);
        delay(1000);
        mp3_play_repeat();//Repeticion de la cancion   
      }
    }
    //Guardamos los nuevos valores
    flash_buffer7[0] = valors1;
    flash_buffer7[1] = valors2;
    flash_buffer7[2] = valors3;
    flash_buffer7[3] = Numactual;
    SerFlash.open("conf.txt",FS_MODE_OPEN_CREATE(5, _FS_FILE_OPEN_FLAG_COMMIT));
    SerFlash.write(flash_buffer7,5);  // x characters
    SerFlash.close();
  }
  
}
void clock()
{ 
  // Reset interrupt flag
  HWREG(TIMERA0_BASE + TIMER_O_ICR) = 0x1;
  cont +=1;
  
  if (valors3>cont){
    digitalWrite(Salida3,HIGH);
  }else{
    digitalWrite(Salida3,LOW);
  }
  if (cont>255){
    cont=0;
  }
}
