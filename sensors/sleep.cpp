#include <avr/sleep.h>
#include <RF24.h>

#include "sleep.h" 


volatile int sleepCyleToGo;
uint8_t sleepCyclesPerTransmission;

void sleepSetup(uint8_t sleepTimer, uint8_t sleepCyclesBetweenTransmit)
{
    sleepCyleToGo = sleepCyclesBetweenTransmit;
    sleepCyclesPerTransmission = sleepCyclesBetweenTransmit;

    uint8_t prescalar = min(9,(uint8_t)sleepTimer);
    uint8_t wdtcsr = prescalar & 7;
    if (prescalar & 8) {
        wdtcsr |= _BV(WDP3);
    }

    MCUSR &= ~_BV(WDRF);
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDCE) | wdtcsr | _BV(WDIE);
}

void goToSleep()
{
    while (sleepCyleToGo--){
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }
    printf_P(PSTR("waking up\n"));
    sleepCyleToGo = sleepCyclesPerTransmission;
}


ISR( WDT_vect ) {
  /* dummy */
}