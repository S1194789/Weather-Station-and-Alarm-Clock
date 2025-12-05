#include <xc.h>
#include "alarms.h"
#include "system.h"
#include "sensors.h"

extern unsigned char temperature_value;
extern unsigned char luminosity_value;

// LED 
#define LED_L   LATAbits.LATA4   // D2 ? Luminosity
#define LED_T   LATAbits.LATA5   // D3 ? Temperature
#define LED_A   LATAbits.LATA6   // D4 ? Alarm PWM
#define LED_C   LATAbits.LATA7   // D5 ? Clock blink

#define TALA_SECONDS  3

// --------------------------------------------------
// Update all alarms and LED status
// --------------------------------------------------
void alarms_update(void)
{
    static uint8_t prev_sec = 0;

    // --------------------------------------------------
    // 1) LED_C: blink once per second
    // --------------------------------------------------
    if (system.clock.sec != prev_sec)
    {
        prev_sec = system.clock.sec;
        LED_C ^= 1;

        // PWM countdown
        if (system.alarms.pwm_timer > 0)
        {
            system.alarms.pwm_timer--;
            if (system.alarms.pwm_timer == 0)
            {
                system.alarms.pwm_active = 0;
                LED_A = 0;
            }
        }
    }

    // --------------------------------------------------
    // 2) If alarms are disabled (A = a)
    //    ? ALL alarm LED states OFF!
    // --------------------------------------------------
    if (!system.alarms.enabled)
    {
        // Turn OFF T, L, A LEDs
        LED_T = 0;
        LED_L = 0;
        LED_A = 0;

        // Reset alarm flags
        system.alarms.active_C = 0;
        system.alarms.active_T = 0;
        system.alarms.active_L = 0;

        // Reset PWM
        system.alarms.pwm_active = 0;
        system.alarms.pwm_timer  = 0;

        return;   
    }

    // --------------------------------------------------
    // 3) T/L threshold indicators (only when alarm enabled)
    // --------------------------------------------------
    uint8_t t_on = (temperature_value > system.alarms.temp_thr);
    uint8_t l_on = (luminosity_value < system.alarms.lum_thr);

    LED_T = t_on;
    LED_L = l_on;

    // --------------------------------------------------
    // 4) detect new alarm events
    // --------------------------------------------------
    uint8_t new_alarm = 0;

    // Clock alarm
    if (!system.alarms.active_C &&
        system.clock.hour == system.alarms.clk_h &&
        system.clock.min  == system.alarms.clk_m &&
        system.clock.sec  == system.alarms.clk_s)
    {
        system.alarms.active_C = 1;
        new_alarm = 1;
    }

    // Temperature alarm
    if (t_on && !system.alarms.active_T)
    {
        system.alarms.active_T = 1;
        new_alarm = 1;
    }

    // Luminosity alarm
    if (l_on && !system.alarms.active_L)
    {
        system.alarms.active_L = 1;
        new_alarm = 1;
    }

    // --------------------------------------------------
    // 5) Activate PWM alarm if needed
    // --------------------------------------------------
    if (new_alarm)
    {
        system.alarms.pwm_active = 1;
        system.alarms.pwm_timer  = TALA_SECONDS;
        LED_A = 1;
    }
}
