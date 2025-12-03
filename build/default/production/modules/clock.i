# 1 "modules/clock.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "modules/clock.c" 2
# 1 "modules/clock.h" 1
# 11 "modules/clock.h"
void clock_update_1s(void);
# 2 "modules/clock.c" 2
# 1 "system/system.h" 1
# 11 "system/system.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned short uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef short intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 12 "system/system.h" 2


typedef enum {
    MODE_NORMAL,
    MODE_CONFIG,
    MODE_RECORDS,
    MODE_ALARM
} system_mode_t;


typedef struct {
    uint8_t one_second : 1;
} system_flags_t;


typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} clock_t;


typedef struct {
    int8_t temperature;
    uint8_t luminosity;
} sensors_t;

typedef struct {
    uint8_t enabled;

    uint8_t clk_h;
    uint8_t clk_m;
    uint8_t clk_s;

    int8_t temp_thr;
    uint8_t lum_thr;


    uint8_t active_C;
    uint8_t active_T;
    uint8_t active_L;


    uint8_t pwm_active;
    uint8_t pwm_timer;
} alarms_t;


typedef struct {
    system_mode_t mode;
    system_flags_t flags;
    clock_t clock;
    sensors_t sensors;
    alarms_t alarms;
} system_t;

extern system_t system;

void system_init(void);
# 3 "modules/clock.c" 2

void clock_update_1s(void)
{
    system.clock.sec++;

    if(system.clock.sec >= 60)
    {
        system.clock.sec = 0;
        system.clock.min++;
    }

    if(system.clock.min >= 60)
    {
        system.clock.min = 0;
        system.clock.hour++;
    }

    if(system.clock.hour >= 24)
    {
        system.clock.hour = 0;
    }
}
