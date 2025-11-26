#include "clock.h"
#include "system.h"

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

