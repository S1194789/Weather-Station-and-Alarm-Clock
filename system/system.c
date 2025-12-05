#include <xc.h>
#include "mcc.h"
#include "system.h"
#include "interrupt_manager.h"

system_t system;

void system_init(void)
{
    SYSTEM_Initialize();            

    INTCONbits.GIE  = 1;           
    INTCONbits.PEIE = 1;            

    /* ---- System mode ---- */
    system.mode = MODE_NORMAL;      // start in normal mode
    system.records_timer = 0;       // inactivity timer (TINA)

    /* ---- Flags ---- */
    system.flags.one_second = 0;    // 1-second tick flag

    /* ---- Clock defaults ---- */
    system.clock.hour = 0;          // CLKH
    system.clock.min  = 0;          // CLKM
    system.clock.sec  = 0;          // CLKS

    /* ---- Sensors ---- */
    system.sensors.temperature = 0; // initial temp
    system.sensors.luminosity  = 0; // initial luminosity

    /* ---- Alarms ---- */
    system.alarms.enabled  = 0;     // ALAF = alarms OFF

    // Alarm time defaults
    system.alarms.clk_h = 12;       // ALAH
    system.alarms.clk_m = 0;        // ALAM
    system.alarms.clk_s = 0;        // ALAS

    // Thresholds
    system.alarms.temp_thr = 20;    // ALAT (°C)
    system.alarms.lum_thr  = 2;     // ALAL (0?3)

    /* ---- Active alarm states ---- */
    system.alarms.active_C = 0;     // clock alarm
    system.alarms.active_T = 0;     // temperature alarm
    system.alarms.active_L = 0;     // luminosity alarm

    /* ---- PWM alarm output ---- */
    system.alarms.pwm_active = 0;   // PWM off
    system.alarms.pwm_timer  = 0;   // remaining TALA time
}