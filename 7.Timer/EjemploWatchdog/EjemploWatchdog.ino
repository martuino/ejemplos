#include "hw_wdt.h"
#include "wdt.h"
#include "utils.h"
#include "prcm.h"
#include "interrupt.h"
#include "hw_ints.h"
 
volatile unsigned long g_ulWatchdogCycles = 0;
typedef void (*fAPPWDTDevCallbk)();
#define WD_PERIOD_MS 1000
#define SYS_CLK                             80000000
#define MILLISECONDS_TO_TICKS(ms)   ((SYS_CLK/1000) * (ms))

void setup() {
  boolean bRetcode;
  Serial.begin(115200);
  Serial.println("**************************************************");
  Serial.println("*             CC3200 Watchdog Test               *");
  Serial.println("**************************************************");

  //
  // Set up the watchdog interrupt handler.
  //
  WDT_IF_Init(WatchdogIntHandler, MILLISECONDS_TO_TICKS(WD_PERIOD_MS));

  bRetcode = MAP_WatchdogRunning(WDT_BASE);
  if (!bRetcode)
  {
    Serial.println("Watchdog failed to run. Shutting down");
    WDT_IF_DeInit();
    while (1);
  }
}

void loop() {
  Serial.print("Watchdog interrupt count: ");
  Serial.println(g_ulWatchdogCycles);
  delay(1000);
}

void WatchdogIntHandler(void)
{
  //
  // After 10 interrupts, switch On LED6 to indicate system reset
  // and don't clear watchdog interrupt which causes system reset
  //
  if (g_ulWatchdogCycles >= 10)
  {
    MAP_UtilsDelay(800000);
    return;
  }
  //
  // Clear the watchdog interrupt.
  //
  MAP_WatchdogIntClear(WDT_BASE);

  //
  // Increment our interrupt counter.
  //
  g_ulWatchdogCycles++;

}

void WDT_IF_Init(fAPPWDTDevCallbk fpAppWDTCB, unsigned int uiReloadVal)
{
  //
  // Enable the peripherals used by this example.
  //
  MAP_PRCMPeripheralClkEnable(PRCM_WDT, PRCM_RUN_MODE_CLK);

  //
  // Unlock to be able to configure the registers
  //
  MAP_WatchdogUnlock(WDT_BASE);

  if (fpAppWDTCB != NULL)
  {
    MAP_IntPrioritySet(INT_WDT, INT_PRIORITY_LVL_1);
    MAP_WatchdogIntRegister(WDT_BASE, fpAppWDTCB);
  }

  //
  // Set the watchdog timer reload value
  //
  MAP_WatchdogReloadSet(WDT_BASE, uiReloadVal);

  //
  // Start the timer. Once the timer is started, it cannot be disable.
  //
  MAP_WatchdogEnable(WDT_BASE);

}

void WDT_IF_DeInit()
{
  //
  // Unlock to be able to configure the registers
  //
  MAP_WatchdogUnlock(WDT_BASE);

  //
  // Disable stalling of the watchdog timer during debug events
  //
  MAP_WatchdogStallDisable(WDT_BASE);

  //
  // Clear the interrupt
  //
  MAP_WatchdogIntClear(WDT_BASE);

  //
  // Unregister the interrupt
  //
  MAP_WatchdogIntUnregister(WDT_BASE);
}