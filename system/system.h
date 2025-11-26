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

// ---- Global system structure ----
typedef struct {
    system_mode_t mode;
    system_flags_t flags;
    clock_t clock;
    sensors_t sensors;
} system_t;

extern system_t system;

void system_init(void);

#endif


