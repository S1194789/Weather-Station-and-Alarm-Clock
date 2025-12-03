#include <xc.h>
#include "sensors.h"
#include "system.h"
#include "adcc.h"
#include "i2c.h"

unsigned char temperature_value = 0;
unsigned char luminosity_value = 0;

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
    
    // Single blocking conversion on ANA0 (potentiometer on RA0)
    uint16_t adc_val = ADCC_GetSingleConversion(channel_ANA0);  // 0..4095

    // DEBUG: if adc_val is ALWAYS 0 here, ADCC or hardware is wrong.
    // Map 12-bit ADC value into 4 levels 0..3
    // 0..1023  -> 0
    // 1024..2047 -> 1
    // 2048..3071 -> 2
    // 3072..4095 -> 3
    uint8_t level;
    if (adc_val < 256)
        level = 0;
    else if (adc_val < 512)
        level = 1;
    else if (adc_val < 768)
        level = 2;
    else
        level = 3;

    luminosity_value = level;
}