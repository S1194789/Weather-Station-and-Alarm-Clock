#include "ui.h"
#include "system.h"
#include "lcd.h"
#include <stdio.h>
#include "i2c.h"
#include "sensors.h"

#define _XTAL_FREQ 4000000
extern unsigned char temperature_value;   // ? ???
extern unsigned char luminosity_value;     // ? ??? ? ?

static void ui_normal(void);
static void ui_time_update(void);

ui_state_t ui_state = UI_NORMAL;
static uint8_t blink = 0;

void ui_init(void)
{
    OpenI2C(); 
    LCDinit();
    __delay_ms(100);

    LCDcmd(0x01);
    LCDpos(0, 0);
    LCDstr("Loading...");
    __delay_ms(700);

    ui_normal();
}

static void ui_normal(void)
{
    LCDcmd(0x01);

    LCDpos(0, 0);
    LCDstr("00:00:00   A");

    LCDpos(1, 0);
    LCDstr("00 C");


    // turn off the cursor
    LCDcmd(0x0C);
}

static void ui_time_update(void)
{
    char buf[17];
    sprintf(buf, "%02d:%02d:%02d   A",
        system.clock.hour,
        system.clock.min,
        system.clock.sec);

    LCDpos(0, 0);
    LCDstr(buf);

    // ---- NORMAL ????? ??? ?? ?? ----
    if (ui_state == UI_NORMAL)
    {
        LCDpos(1, 0);
        sprintf(buf, "%02d  C L  0", temperature_value);
        LCDstr(buf);
    }
}

void ui_next_state(void)
{
    switch(ui_state)
    {
        case UI_NORMAL:
            ui_state = UI_CFG_HOUR;
            LCDcmd(0x0F);      // BLOCK CURSOR BLINK ON
            LCDpos(0,1);       // cursor at HH
            break;

        case UI_CFG_HOUR:
            ui_state = UI_CFG_MIN;
            LCDpos(0,4);       // cursor at MM
            break;

        case UI_CFG_MIN:
            ui_state = UI_CFG_SEC;
            LCDpos(0,7);       // cursor at SS
            break;

        case UI_CFG_SEC:
        default:
            ui_state = UI_NORMAL;
            ui_normal();        // cursor turned OFF
            break;
    }
}

void ui_select(void)
{
    switch(ui_state)
    {
        case UI_CFG_HOUR:
            system.clock.hour = (system.clock.hour + 1) % 24;
            break;

        case UI_CFG_MIN:
            system.clock.min = (system.clock.min + 1) % 60;
            break;

        case UI_CFG_SEC:
            system.clock.sec = (system.clock.sec + 1) % 60;
            break;

        default:
            break;
    }
}

void ui_update(void)
{
    if (ui_state == UI_NORMAL)
    {
        ui_time_update();
        return;
    }

    // Config Mode
    ui_time_update();

    // cursor position must be maintained
    switch(ui_state)
    {
        case UI_CFG_HOUR: LCDpos(0,1); break;
        case UI_CFG_MIN:  LCDpos(0,4); break;
        case UI_CFG_SEC:  LCDpos(0,7); break;
    }
}
