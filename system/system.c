#include <xc.h>
#include "mcc.h"
#include "system.h"
#include "interrupt_manager.h"

// ?? system ??? ??
system_t system;

void system_init(void)
{
    // MCC? ?? ??? ??? ??
    SYSTEM_Initialize();

    // ???? ???
    INTCONbits.GIE  = 1;
    INTCONbits.PEIE = 1;

    // ?? ??? ??
    system.mode = MODE_NORMAL;

    // ??? ???
    system.flags.one_second = 0;

    // ?? ???
    system.clock.hour = 0;
    system.clock.min  = 0;
    system.clock.sec  = 0;

    // ?? ??? (??? 0?? ?)
    system.sensors.temperature = 0;
    system.sensors.luminosity  = 0;
}
