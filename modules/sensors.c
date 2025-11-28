#include <xc.h>
#include "sensors.h"
#include "i2c.h"

unsigned char temperature_value = 0;

void sensors_init(void)
{
    OpenI2C();   
}

/* ---- ??? readTC74() ??? ---- */
unsigned char readTC74 (void)
{
    unsigned char value;

    do {
        IdleI2C();
        StartI2C(); IdleI2C();
        WriteI2C(0x9A | 0x00); IdleI2C();
        WriteI2C(0x01); IdleI2C();
        RestartI2C(); IdleI2C();
        WriteI2C(0x9A | 0x01); IdleI2C();
        value = ReadI2C(); IdleI2C();
        NotAckI2C(); IdleI2C();
        StopI2C();
    } while (!(value & 0x40));

    IdleI2C();
    StartI2C(); IdleI2C();
    WriteI2C(0x9A | 0x00); IdleI2C();
    WriteI2C(0x00); IdleI2C();
    RestartI2C(); IdleI2C();
    WriteI2C(0x9A | 0x01); IdleI2C();
    value = ReadI2C(); IdleI2C();
    NotAckI2C(); IdleI2C();
    StopI2C();

    return value;
}

void sensors_update(void)
{
    static unsigned char sec_count = 0;

    
    sec_count++;

    if(sec_count >= 5)   // PMON = 5?
    {
        sec_count = 0;
        temperature_value = readTC74();
    }
}
