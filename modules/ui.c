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
static uint8_t alarm_edit_step = 0;

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
    // turn off the cursor
    LCDcmd(0x0C);
}

static void ui_time_update(void)
{
    char buf[17];
    
    
    char cC = system.alarms.active_C ? 'C' : ' ';
    char cT = system.alarms.active_T ? 'T' : ' ';
    char cL = system.alarms.active_L ? 'L' : ' ';
    char cA = system.alarms.enabled  ? 'A' : 'a';
    
    sprintf(buf, "%02d:%02d:%02d %c%c%c %c ",
        system.clock.hour,
        system.clock.min,
        system.clock.sec,
        cC, cT, cL,
        cA);

    LCDpos(0, 0);
    LCDstr(buf);

// linia 2 zale?y od stanu UI
    LCDpos(1, 0);

    switch (ui_state)
    {
        case UI_NORMAL:
            // "tt C L  l" ? aktualne sensory
            sprintf(buf, "%02d C L  %d",
                    temperature_value,
                    luminosity_value);
            break;

        case UI_CFG_HOUR:
        case UI_CFG_MIN:
        case UI_CFG_SEC:
            // ustawianie czasu zegara ? podpowied?
            sprintf(buf, "Set clock time   ");
            break;

        case UI_CFG_C:
            // podgl?d czasu alarmu
            sprintf(buf, "AL %02d:%02d:%02d   ",
                    system.alarms.clk_h,
                    system.alarms.clk_m,
                    system.alarms.clk_s);
            break;

        case UI_CFG_T:
            // prog temp
            sprintf(buf, "Tthr=%02d C       ", system.alarms.temp_thr);
            break;

        case UI_CFG_L:
            // prog lum
            sprintf(buf, "Lthr=%d (0-3)     ", system.alarms.lum_thr);
            break;

        case UI_CFG_ALARM_EN:
            // stan ALAF
            if (system.alarms.enabled)
                sprintf(buf, "Alarms: ON       ");
            else
                sprintf(buf, "Alarms: OFF      ");
            break;

        case UI_CFG_RESET:
            // pó?niej: reset rekordów
            sprintf(buf, "Reset records S2 ");
            break;
    }

    LCDstr(buf);
}

void ui_next_state(void)
{
    switch(ui_state)
    {
        case UI_NORMAL:
            
            system.alarms.active_C = 0;
            system.alarms.active_T = 0;
            system.alarms.active_L = 0;
            
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
            ui_state = UI_CFG_C;
            LCDpos(0, 13);        // cursor at A/a
            break;
            
         case UI_CFG_C:
            ui_state = UI_CFG_T;
            LCDpos(0, 10);     // T na 1. linii: "hh:mm:ss CTL A"
            alarm_edit_step = 0;   // na przysz?o?? znów zaczynasz od case 0

        case UI_CFG_T:
            ui_state = UI_CFG_L;
            // kursor na literze L (kolumna 11)
            LCDpos(0, 11);
            break;
            
        case UI_CFG_L:
            ui_state = UI_CFG_ALARM_EN;
            // kursor na A/a (kolumna 13)
            LCDpos(0, 13);
            break;

        case UI_CFG_ALARM_EN:
            ui_state = UI_CFG_RESET;
            // kursor na R ? my nie rysujemy litery R na 1. linii,
            // wi?c kursor ustaw na pocz?tek drugiej linii
            LCDpos(1, 0);
            break;

        case UI_CFG_RESET:
        default:
            ui_state = UI_NORMAL;
            ui_normal();        // powrót do normal, kursor OFF
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
            
        case UI_CFG_C:
             switch (alarm_edit_step)
            {
            case 0:
                // wej?cie w edycj?: przeskocz z 'C' na godziny alarmu
                alarm_edit_step = 1;
                break;

            case 1:
                // inkrement godziny alarmu (ALAH)
                system.alarms.clk_h = (system.alarms.clk_h + 1) % 24;
                break;

            case 2:
                // inkrement minut alarmu (ALAM)
                system.alarms.clk_m = (system.alarms.clk_m + 1) % 60;
                break;

            case 3:
                // inkrement sekund alarmu (ALAS)
                system.alarms.clk_s = (system.alarms.clk_s + 1) % 60;
                break;
            }
            break;

        case UI_CFG_T:
            // prog temp ? zakres np. 0..50
            if (system.alarms.temp_thr < 50)
                system.alarms.temp_thr++;
            else
                system.alarms.temp_thr = 0;
            break;

        case UI_CFG_L:
            // prog lum ? 0..3
            system.alarms.lum_thr = (system.alarms.lum_thr + 1) % 4;
            break;

        case UI_CFG_ALARM_EN:
            // toggle A/a
            system.alarms.enabled ^= 1;
            break;

        case UI_CFG_RESET:
            // TU PÓ?NIEJ: wyzerujemy rekordy max/min, jak je zaimplementujemy
            // teraz S2 nic nie robi
            break;

        default:
            // w NORMAL na S2 prze??czysz tryb rekordów, ale to zrobimy pó?niej
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
        case UI_CFG_HOUR:      LCDpos(0, 1);  break;
        case UI_CFG_MIN:       LCDpos(0, 4);  break;
        case UI_CFG_SEC:       LCDpos(0, 7);  break;
        case UI_CFG_C:
            switch (alarm_edit_step)
            {
            case 0:            LCDpos(0, 9); break;
            case 1:            LCDpos(1, 4); break;
            case 2:            LCDpos(1, 7); break;
            case 3:            LCDpos(1, 10); break;
            }
        break;
        case UI_CFG_T:         LCDpos(0,10);  break;
        case UI_CFG_L:         LCDpos(0,11);  break;
        case UI_CFG_ALARM_EN:  LCDpos(0,13);  break;
        case UI_CFG_RESET:     LCDpos(1, 0);  break;

        case UI_NORMAL:
        default:
            LCDcmd(0x0C);      // kursor OFF
            break;
    }
}