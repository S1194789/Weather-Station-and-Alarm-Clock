#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

// ---------- System modes ----------
typedef enum {
    MODE_NORMAL,
    MODE_CONFIG,
    MODE_RECORDS
} system_mode_t;

// ---------- System flags ----------
typedef struct {
    uint8_t one_second : 1;   // 1-second tick flag
} system_flags_t;

// ---------- Clock ----------
typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} clock_t;

// ---------- Sensors ----------
typedef struct {
    int8_t temperature;   // current temperature
    uint8_t luminosity;   // current luminosity (0?3)
} sensors_t;

// ---------- Alarm parameters ----------
typedef struct {
    uint8_t enabled;     // alarms ON/OFF (A/a)

    // Clock alarm time
    uint8_t clk_h;
    uint8_t clk_m;
    uint8_t clk_s;

    // Thresholds
    int8_t  temp_thr;    // temperature threshold
    uint8_t lum_thr;     // luminosity threshold

    // Active alarms (for CTL indicators)
    uint8_t active_C;
    uint8_t active_T;
    uint8_t active_L;

    // PWM alarm LED
    uint8_t pwm_active;
    uint8_t pwm_timer;
} alarms_t;

// ---------- Global system structure ----------
typedef struct {
    system_mode_t mode;      // system mode
    system_flags_t flags;    // flags
    clock_t clock;           // time
    sensors_t sensors;       // sensor readings
    alarms_t alarms;         // alarm settings/state

    uint8_t records_timer;   // timeout for RECORDS mode
} system_t;

extern system_t system;

void system_init(void);

#endif
