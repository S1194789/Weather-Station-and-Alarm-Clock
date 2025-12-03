#include <xc.h>
#include "alarms.h"
#include "system.h"
#include "sensors.h"

extern unsigned char temperature_value;
extern unsigned char luminosity_value;

// LEDs RA4..RA7
#define LED_L   LATAbits.LATA4   // D2
#define LED_T   LATAbits.LATA5   // D3
#define LED_A   LATAbits.LATA6   // D4 (PWM / alarm)
#define LED_C   LATAbits.LATA7   // D5 (clock activity)

#define TALA_SECONDS  3

void alarms_update(void)
{
    static uint8_t prev_sec = 0;

    // --- 1) LED C: miganie co sekund? + TALA ---
    if (system.clock.sec != prev_sec)
    {
        prev_sec = system.clock.sec;

        // miganie LED C
        LED_C ^= 1;

        // czas trwania LED_A
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

    // --- 2) T/L ZAWSZE pokazuj? progi (status), niezale?nie od ALAF ---
    uint8_t t_on = (temperature_value > system.alarms.temp_thr);
    uint8_t l_on = (luminosity_value < system.alarms.lum_thr);

    LED_T = t_on;
    LED_L = l_on;

    // --- 3) Je?li ALAF = 0 (ma?e a) ? ZERO alarmów, zero CTL, zero D4 ---
    if (!system.alarms.enabled)
    {
        // zabijamy wszystkie ?alarmowe? flagi
        system.alarms.active_C = 0;
        system.alarms.active_T = 0;
        system.alarms.active_L = 0;

        // zabijamy PWM / LED_A
        system.alarms.pwm_active = 0;
        system.alarms.pwm_timer  = 0;
        LED_A = 0;

        return;
    }

    // --- 4) ALARMY tylko gdy ALAF = 1 (du?e A) ---
    uint8_t new_alarm = 0;

    // Clock alarm: C ? jedno zbocze
    if (!system.alarms.active_C &&
        system.clock.hour == system.alarms.clk_h &&
        system.clock.min  == system.alarms.clk_m &&
        system.clock.sec  == system.alarms.clk_s)
    {
        system.alarms.active_C = 1;
        new_alarm = 1;
    }

    // Temp alarm: T ? pierwsze przej?cie powy?ej progu
    if (t_on && !system.alarms.active_T)
    {
        system.alarms.active_T = 1;
        new_alarm = 1;
    }

    // Lum alarm: L ? pierwsze przej?cie poni?ej progu
    if (l_on && !system.alarms.active_L)
    {
        system.alarms.active_L = 1;
        new_alarm = 1;
    }

    // --- 5) Nowy alarm ? LED_A na TALA ---
    if (new_alarm)
    {
        system.alarms.pwm_active = 1;
        system.alarms.pwm_timer  = TALA_SECONDS;
        LED_A = 1;
    }
}
