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
    
    // ---- Alarms defaults ----
    system.alarms.enabled  = 0;   // a

    // i changed values below for easier testing
    system.alarms.clk_h    = 0;  // ALAH should be 12
    system.alarms.clk_m    = 5;   // ALAM should be 0
    system.alarms.clk_s    = 5;   // ALAS should be 0

    system.alarms.temp_thr = 22;  // ALAT (°C) should be 20
    system.alarms.lum_thr  = 2;   // ALAL (level 0..3) 

    system.alarms.active_C = 0;
    system.alarms.active_T = 0;
    system.alarms.active_L = 0;

    system.alarms.pwm_active = 0;
    system.alarms.pwm_timer  = 0;
}
