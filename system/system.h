/* 
 * File:   system.h
 * Author: HYEJOO KWON
 *
 * Created on November 22, 2025, 12:46 AM
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

// ---- Modes ----
typedef enum {
    MODE_NORMAL,
    MODE_CONFIG,
    MODE_RECORDS,
    MODE_ALARM
} system_mode_t;

// ---- Flags ----
typedef struct {
    uint8_t one_second : 1;
} system_flags_t;

// ---- Clock ----
typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} clock_t;

// ---- Sensors ----
typedef struct {
    int8_t temperature;
    uint8_t luminosity;
} sensors_t;

typedef struct {
    uint8_t enabled;    // ALAF: 1 = alarms ON, 0 = OFF

    uint8_t clk_h;      // ALAH
    uint8_t clk_m;      // ALAM
    uint8_t clk_s;      // ALAS

    int8_t  temp_thr;   // ALAT
    uint8_t lum_thr;    // ALAL

    // które alarmy s? aktywne (dla CTL + LED T/L)
    uint8_t active_C;
    uint8_t active_T;
    uint8_t active_L;

    // PWM / LED A
    uint8_t pwm_active;
    uint8_t pwm_timer;  // sekundy do ko?ca TALA
} alarms_t;

// ---- Global system structure ----
typedef struct {
    system_mode_t mode;
    system_flags_t flags;
    clock_t clock;
    sensors_t sensors;
    alarms_t alarms;
} system_t;

extern system_t system;

void system_init(void);

#endif


