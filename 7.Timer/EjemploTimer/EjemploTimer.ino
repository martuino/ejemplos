/*Ejemplo de timer. 
 Utilizamos un timer para realizar un contador de segundos por interrupción y lo muestra en el puerto serie.


Creado el 04 Oct 2015
por Vicente Martinez

*/

#include <inc/hw_types.h>
#include <inc/hw_timer.h>
#include <driverlib/prcm.h>
#include <driverlib/timer.h>
int cont = 0;
unsigned long ciclo= 80000000 ; //Como el reloj del micro es de 80Mhz
					//timer = 80 Millones y el ciclo = 1 segundo

void setup()
{
  Serial.begin(9600);
  // Configuración del Timer 
  MAP_PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
  MAP_PRCMPeripheralReset(PRCM_TIMERA0);
  MAP_TimerIntRegister(TIMERA0_BASE, TIMER_A, clock); // rutina donde salta
  MAP_TimerConfigure(TIMERA0_BASE, TIMER_CFG_A_PERIODIC);//Cilco periodico
  MAP_TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT);//Habilita interrupcion
  MAP_TimerPrescaleSet(TIMERA0_BASE, TIMER_A, 0);//No usamos preescaler
  MAP_TimerLoadSet(TIMERA0_BASE, TIMER_A, ciclo); //80Mhz = 1 segundo
  MAP_TimerEnable(TIMERA0_BASE, TIMER_A);//Ponemos en marcha Timer A

}
void loop()
{

}

void clock()
{ 
  // Reset interrupt flag
  HWREG(TIMERA0_BASE + TIMER_O_ICR) = 0x1;
  cont +=1;
  Serial.print("Contador=");
  Serial.println(cont);
}
